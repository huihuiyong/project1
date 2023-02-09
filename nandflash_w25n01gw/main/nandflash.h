#include "driver/gpio.h"

//SPI
#define SPI_MOSI_IO_NUM         23
#define SPI_MISO_IO_NUM         19
#define SPI_CLK_IO_NUM          19
#define SPI_CS_IO_NUM           19

//设备ID
#define FLASH_ID_W25N01GW                   0xEFBA21

//寄存器
#define FLASH_SR_BUSY                       0x01
#define FLASH_SR_WEL                        0x02
#define FLASH_SR_ERASE_FAIL                 0x03
#define FLASH_SR_WRITE_FAIL                 0x04
#define FLASH_SR_ECC0                       0x10
#define FLASH_SR_ECC1                       0x20
#define FLASH_SR_BBM_LUT_FULL               0x40


//指令
#define FLASH_DEVICE_RESET                  0xFF
#define FLASH_READ_JEDEC_ID                 0x9F
#define FLASH_READ_STATUS_REGISTER          0x0F
#define FLASH_WRITE_STATUS_REGISTER         0x1F
#define FLASH_STATUS_REG1_ADDR              0xA0 
#define FLASH_STATUS_REG2_ADDR              0xB0 
#define FLASH_STATUS_REG3_ADDR              0xC0
#define FLASH_WRITE_ENABLE                  0x06
#define FLASH_WRITE_DISABLE                 0x04
#define FLASH_BBM                           0xA1      
#define FLASH_LAST_ECC_FAILURE_PAGE_ADDR    0xA9
#define FLASH_BLOCK_ERASE                   0xD8
#define FLASH_READ_BBM_LUT                  0xA5
#define FLASH_READ_PAGE_DATA                0x13
#define FLASH_READ_DATA                     0x03
#define FLASH_LOAD_PROGRAM_DATA             0x02
#define FLASH_PROGRAM_EXECUTE               0x10

typedef struct 
{
    gpio_num_t flash_CS;
    
}Flash_InitTypeDef;
