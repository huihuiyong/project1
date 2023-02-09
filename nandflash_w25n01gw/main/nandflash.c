

#include "nandflash.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

/******************************************************************************nandflash_spi配置********************************************************************************/
spi_device_handle_t spi_handle;//需修改

void Flash_SPIInit()
{
    spi_bus_config_t spi2_bus_config = {
    .mosi_io_num         = SPI_MOSI_IO_NUM,
    .miso_io_num         = SPI_MISO_IO_NUM,
    .sclk_io_num         = SPI_CLK_IO_NUM,
    .quadwp_io_num       = -1,
    .quadhd_io_num       = -1,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &spi2_bus_config, SPI_DMA_DISABLED));

    spi_device_interface_config_t device_config = {
    .command_bits   = 8,
    .address_bits   = 16,
    .dummy_bits     = 8,
    .mode           = 1,
    .spics_io_num   = SPI_CS_IO_NUM,
    .flags          = SPI_DEVICE_HALFDUPLEX | SPI_DEVICE_POSITIVE_CS,
    };
    //ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, spi_device_interface_config_t &device_config, &spi_handle));
    // err = spi_bus_add_device(ctx->cfg.host, &devcfg, &ctx->spi);
    // if  (err != ESP_OK) {
    //     goto cleanup;
    // }

}

void SPI_Flash_WriteByte(uint8_t data)
{
    esp_err_t err;
    err = spi_device_acquire_bus(spi_handle, portMAX_DELAY);
    if (err != ESP_OK) return;

    spi_transaction_t t = {
        .length = 8,
        .flags = SPI_TRANS_USE_TXDATA,
        .tx_data = {data},
    };
    err = spi_device_polling_transmit(spi_handle, &t);

    spi_device_release_bus(spi_handle);
    return;
}

uint8_t SPI_Flash_ReadByte()
{
    uint8_t data = 0;
    spi_transaction_t t = {
        .rxlength = 8,
        .flags = SPI_TRANS_USE_RXDATA,
        .tx_data = {data},
    };
    spi_device_polling_transmit(spi_handle, &t);

    return data;
}

void delay_ms(uint16_t time)        //需修改
{
    while (time--)
    {
        
    }
    
}

/*******************************************************************************nandflash API*******************************************************************************/
void (*Flash_Delay)(uint16_t time) = &delay_ms;    //
void (*Flash_SendByte)(uint8_t) = &SPI_Flash_WriteByte;   //
uint8_t (*Flash_ReciByte)() = &SPI_Flash_ReadByte;   //

Flash_InitTypeDef flashinit;//初始化结构体

void Flash_Enable(uint8_t flag)
{
    if (flag == 0)
    {
        gpio_set_level(flashinit.flash_CS, 0);
    }
    if (flag == 1)
    {
        gpio_set_level(flashinit.flash_CS, 1);
    }
}

void Flash_Reset()
{
    Flash_Enable(0);
    Flash_SendByte(FLASH_DEVICE_RESET);
    Flash_Delay(1);
    Flash_Enable(1);
}

/*读状态寄存器*/
uint8_t Flash_ReadStatusRegister(uint8_t addr)
{
    uint8_t data;
    Flash_Enable(0);
    Flash_SendByte(FLASH_READ_STATUS_REGISTER);
    Flash_SendByte(addr);
    Flash_SendByte(0x00);
    data = Flash_ReciByte();
    Flash_Enable(1);
    return data;
}

/*写状态寄存器*/
void Flash_WriteStatusRegister(uint8_t addr, uint8_t value)
{
    Flash_Enable(0);
    Flash_SendByte(FLASH_WRITE_STATUS_REGISTER);
    Flash_SendByte(addr);
    Flash_SendByte(value);
    Flash_Enable(1);
}

/*读状态寄存器BUSY位，等待任务结束*/
uint8_t Flash_WaitBusy(uint16_t timeout)
{
	while(timeout--)
	{
		if((Flash_ReadStatusRegister(FLASH_STATUS_REG3_ADDR) & FLASH_SR_BUSY) != FLASH_SR_BUSY)
		{
            printf("已完成等待！");
			return 0;//等待结束
		}
		Flash_Delay(1);
	}
    printf("等待已超时！");
	return 1;//等待超时
}

uint8_t Flash_DeviceInit()
{
    int id = 0;
    Flash_Enable(0);
    Flash_SendByte(FLASH_READ_JEDEC_ID);
    Flash_SendByte(0x00);
    id |= Flash_ReciByte() << 24;
    id |= Flash_ReciByte() << 8;
    id |= Flash_ReciByte();
	Flash_Enable(1);
	
	if((id & FLASH_ID_W25N01GW) != FLASH_ID_W25N01GW)
	{
        printf("芯片型号错误，或芯片已损坏！");
		return 1;
	}

	if(Flash_ReadStatusRegister(FLASH_STATUS_REG3_ADDR) & FLASH_SR_BBM_LUT_FULL)
	{
        printf("坏块映射表已满，继续使用不能保证数据的正确性！");
		return 2;
	}

	//芯片设置
	id = Flash_ReadStatusRegister(FLASH_STATUS_REG1_ADDR);
	id = 0;//禁用所有写保护
	Flash_WriteStatusRegister(FLASH_STATUS_REG1_ADDR, id & 0x000000FF);

	id = Flash_ReadStatusRegister(FLASH_STATUS_REG2_ADDR);
	id |= (0x10 | 0x80);//开启ECC，禁用读取缓冲区
	Flash_WriteStatusRegister(FLASH_STATUS_REG2_ADDR, id & 0x000000FF);

	return 0;
    
}

/*写使能*/
void Flash_Write_Enable()
{
    Flash_Enable(0);
    Flash_SendByte(FLASH_WRITE_ENABLE);
    Flash_Enable(1);
}

/*写失能*/
void Flash_Write_Disable()
{
    Flash_Enable(0);
    Flash_SendByte(FLASH_WRITE_DISABLE);
    Flash_Enable(1);
}

/*读取上次ECC失败地址（如果有多页，则为最后一页）*/
uint16_t Flash_LastECCFailurePageAddress()
{
    uint16_t pageaddr = 0;
    Flash_Enable(0);
    Flash_SendByte(FLASH_LAST_ECC_FAILURE_PAGE_ADDR);
    Flash_SendByte(0x00);
    pageaddr |= Flash_ReciByte() << 8;
    pageaddr |= Flash_ReciByte();
    Flash_Enable(1);

    return pageaddr;
}

/*块擦除（64页，128k*/
void Flash_BlockErase(uint16_t page_addr)
{
    Flash_Write_Enable();
    Flash_Enable(0);
    Flash_SendByte(FLASH_BLOCK_ERASE);
    Flash_SendByte(0x00);
    Flash_SendByte((uint8_t)page_addr >> 8);
    Flash_SendByte((uint8_t)(page_addr & 0x00FF));
    Flash_Enable(1);
    Flash_Delay(10);//tBE max=10ms
}

uint8_t Flash_WriteData(uint16_t page_addr, uint8_t *data, uint32_t len)
{
    Flash_Write_Enable();
    Flash_Enable(0);
    Flash_SendByte(FLASH_LOAD_PROGRAM_DATA);
    Flash_SendByte(0x00);
    Flash_SendByte(0x00);
    for (int i = 0; i < len; i++)
    {
        Flash_SendByte(data[i]);
    }
    Flash_Enable(1);

    Flash_Enable(0);
    Flash_SendByte(FLASH_PROGRAM_EXECUTE);
    Flash_SendByte(0x00);
    Flash_SendByte((uint8_t)page_addr >> 8);
    Flash_SendByte((uint8_t)(page_addr & 0x00FF));
    Flash_Enable(1);

    Flash_WaitBusy(100);

    if (Flash_ReadStatusRegister(FLASH_STATUS_REG3_ADDR) & FLASH_SR_WRITE_FAIL )
    {
        return 1;
    }
    
    return 0;
}

uint8_t Flash_ReadData(uint16_t page_addr, uint8_t *data, uint32_t len)
{
    Flash_Enable(0);
    Flash_SendByte(FLASH_READ_PAGE_DATA);
    Flash_SendByte(0x00);
    Flash_SendByte((uint8_t)page_addr >> 8);
    Flash_SendByte((uint8_t)(page_addr & 0x00FF));
    Flash_Enable(1);

    Flash_Enable(0);
    Flash_SendByte(FLASH_READ_DATA);
    Flash_SendByte(0x00);
    Flash_SendByte(0x00);
    Flash_SendByte(0x00);
    for (int i = 0; i < len; i++)
    {
        data[i] = Flash_ReciByte();
    }
    Flash_Enable(1);

    return Flash_ReadStatusRegister(FLASH_STATUS_REG3_ADDR) & 0x30;
}

/*读BBM查找表*/
/*20个逻辑-物理内存块链接*/
/*格式是LBA0,PBA0,LBA1,PBA1......LBA19,PBA19*/
void Flash_ReadBBMLookUpTable(uint16_t *BBMLUT)
{
    Flash_Enable(0);
    Flash_SendByte(FLASH_READ_BBM_LUT);
    Flash_SendByte(0x00);
    for (int i = 0; i < 40; i++)
    {
    *BBMLUT |= Flash_ReciByte() << 8;
    *BBMLUT |= Flash_ReciByte() ;        
    BBMLUT++;
    }
    Flash_Enable(1);
}

/*坏块管理*/
/*坏块将被好块所取代，并添加链路到LUT中*/
uint8_t Flash_BadBlockMapping(uint16_t LBA, uint16_t PBA)
{
    if(Flash_ReadStatusRegister(FLASH_STATUS_REG3_ADDR) & FLASH_SR_BBM_LUT_FULL)
	{
        printf("LUT中链路已满，无法建立。");
		return 1;
	}

    Flash_Write_Enable();
    Flash_Enable(0);
    Flash_SendByte(FLASH_BBM);
    Flash_SendByte(LBA >> 8);
    Flash_SendByte(LBA & 0x00FF);
    Flash_SendByte(PBA >> 8);
    Flash_SendByte(PBA & 0x00FF);
    Flash_Enable(1);

    Flash_WaitBusy(1);
    return 0;

}

uint8_t Flash_BadBlockscan(uint16_t blocknum)
{
    blocknum <<= 6;
    
    Flash_Enable(0);
    Flash_SendByte(FLASH_READ_PAGE_DATA);
    Flash_SendByte(0x00);
    Flash_SendByte((uint8_t)blocknum >> 8);
    Flash_SendByte((uint8_t)(blocknum & 0x00FF));
    Flash_Enable(1);

    Flash_Delay(1);

    Flash_Enable(0);
    Flash_SendByte(FLASH_READ_DATA);
    Flash_SendByte(0x00);
    Flash_SendByte(0x00);
    Flash_SendByte(0x00);
    if (Flash_ReciByte() !=0xFF)
    {
        Flash_Enable(1);
        return 1;
    }
    Flash_Enable(1);
    return 0;
}

void Flash_BadBlockManagement()
{
    uint16_t temp[40] = {0};

    printf("坏块扫描开始\r\n");
    for (uint16_t i = 0; i < 1024; i++)
    {
        printf("正在扫描第%d块\r\n", i);
        if (Flash_BadBlockscan(i))
        {
            printf("第%d块为坏块，需替换\r\n", i); 

            if(Flash_ReadStatusRegister(FLASH_STATUS_REG3_ADDR) & FLASH_SR_BBM_LUT_FULL)
            {
                printf("LUT中链路已满，无法替换。");
            }
            else
            {
                Flash_ReadBBMLookUpTable(temp);
                for (uint16_t j = 0; j < 20; j++)
                {
                    if (temp[j * 2]  == 0x0000 && temp[j * 2 + 1]  == 0x0000)
                    {
                        j += 1000;
                        Flash_BadBlockMapping(i, j);
                        printf("第 %d 块被映射为第 %d 块 \r\n", i, j);
                        break;
                    }
                    
                }
                
            }
        }
    }
    printf("坏块扫描结束\r\n");
}