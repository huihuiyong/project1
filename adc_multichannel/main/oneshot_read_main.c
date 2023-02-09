/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

const static char *TAG = "EXAMPLE";

/*---------------------------------------------------------------
        ADC General Macros
---------------------------------------------------------------*/
//ADC1 Channels
#define ADC1_CHAN0          ADC_CHANNEL_0
#define ADC1_CHAN3          ADC_CHANNEL_3
#define ADC1_CHAN4          ADC_CHANNEL_4
#define ADC1_CHAN5          ADC_CHANNEL_5
#define ADC1_CHAN6          ADC_CHANNEL_6
#define ADC1_CHAN7          ADC_CHANNEL_7

//ADC2 Channels
#define ADC2_CHAN0          ADC_CHANNEL_0
#define ADC2_CHAN2          ADC_CHANNEL_2
#define ADC2_CHAN3          ADC_CHANNEL_3
#define ADC2_CHAN4          ADC_CHANNEL_4
#define ADC2_CHAN5          ADC_CHANNEL_5
#define ADC2_CHAN6          ADC_CHANNEL_6
#define ADC2_CHAN7          ADC_CHANNEL_7
#define ADC2_CHAN8          ADC_CHANNEL_8
#define ADC2_CHAN9          ADC_CHANNEL_9

static int adc_raw[2][10];

void app_main(void)
{
    //-------------ADC1 Init---------------//
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    //-------------ADC1 Config---------------//
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_11,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC1_CHAN0, &config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC1_CHAN3, &config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC1_CHAN4, &config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC1_CHAN5, &config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC1_CHAN6, &config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC1_CHAN7, &config));
   
    //-------------ADC2 Init---------------//
    adc_oneshot_unit_handle_t adc2_handle;
    adc_oneshot_unit_init_cfg_t init_config2 = {
        .unit_id = ADC_UNIT_2,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config2, &adc2_handle));

    //-------------ADC2 Config---------------//
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc2_handle, ADC2_CHAN0, &config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc2_handle, ADC2_CHAN2, &config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc2_handle, ADC2_CHAN3, &config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc2_handle, ADC2_CHAN4, &config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc2_handle, ADC2_CHAN5, &config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc2_handle, ADC2_CHAN6, &config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc2_handle, ADC2_CHAN7, &config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc2_handle, ADC2_CHAN8, &config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc2_handle, ADC2_CHAN9, &config));

    while (1) {
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC1_CHAN0, &adc_raw[0][0]));
        ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, ADC1_CHAN0, adc_raw[0][0]);
        vTaskDelay(pdMS_TO_TICKS(1000));

        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC1_CHAN3, &adc_raw[0][1]));
        ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, ADC1_CHAN3, adc_raw[0][1]);
        vTaskDelay(pdMS_TO_TICKS(1000));

        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC1_CHAN4, &adc_raw[0][2]));
        ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, ADC1_CHAN4, adc_raw[0][2]);
        vTaskDelay(pdMS_TO_TICKS(1000));

        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC1_CHAN5, &adc_raw[0][3]));
        ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, ADC1_CHAN5, adc_raw[0][3]);
        vTaskDelay(pdMS_TO_TICKS(1000));

        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC1_CHAN6, &adc_raw[0][4]));
        ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, ADC1_CHAN6, adc_raw[0][4]);
        vTaskDelay(pdMS_TO_TICKS(1000));

        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC1_CHAN7, &adc_raw[0][5]));
        ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, ADC1_CHAN7, adc_raw[0][5]);
        vTaskDelay(pdMS_TO_TICKS(1000));


        ESP_ERROR_CHECK(adc_oneshot_read(adc2_handle, ADC2_CHAN0, &adc_raw[1][0]));
        ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_2 + 1, ADC2_CHAN0, adc_raw[1][0]);
        vTaskDelay(pdMS_TO_TICKS(1000));

        ESP_ERROR_CHECK(adc_oneshot_read(adc2_handle, ADC2_CHAN2, &adc_raw[1][1]));
        ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_2 + 1, ADC2_CHAN2, adc_raw[1][1]);
        vTaskDelay(pdMS_TO_TICKS(1000));

        ESP_ERROR_CHECK(adc_oneshot_read(adc2_handle, ADC2_CHAN3, &adc_raw[1][2]));
        ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_2 + 1, ADC2_CHAN3, adc_raw[1][2]);
        vTaskDelay(pdMS_TO_TICKS(1000));

        ESP_ERROR_CHECK(adc_oneshot_read(adc2_handle, ADC2_CHAN4, &adc_raw[1][3]));
        ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_2 + 1, ADC2_CHAN4, adc_raw[1][3]);
        vTaskDelay(pdMS_TO_TICKS(1000));

        ESP_ERROR_CHECK(adc_oneshot_read(adc2_handle, ADC2_CHAN5, &adc_raw[1][4]));
        ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_2 + 1, ADC2_CHAN5, adc_raw[1][4]);
        vTaskDelay(pdMS_TO_TICKS(1000));

        ESP_ERROR_CHECK(adc_oneshot_read(adc2_handle, ADC2_CHAN6, &adc_raw[1][5]));
        ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_2 + 1, ADC2_CHAN6, adc_raw[1][5]);
        vTaskDelay(pdMS_TO_TICKS(1000));

        ESP_ERROR_CHECK(adc_oneshot_read(adc2_handle, ADC2_CHAN7, &adc_raw[1][6]));
        ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_2 + 1, ADC2_CHAN7, adc_raw[1][6]);
        vTaskDelay(pdMS_TO_TICKS(1000));

        ESP_ERROR_CHECK(adc_oneshot_read(adc2_handle, ADC2_CHAN8, &adc_raw[1][7]));
        ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_2 + 1, ADC2_CHAN8, adc_raw[1][7]);
        vTaskDelay(pdMS_TO_TICKS(1000));

        ESP_ERROR_CHECK(adc_oneshot_read(adc2_handle, ADC2_CHAN9, &adc_raw[1][8]));
        ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_2 + 1, ADC2_CHAN9, adc_raw[1][8]);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    //Tear Down
    ESP_ERROR_CHECK(adc_oneshot_del_unit(adc1_handle));
    ESP_ERROR_CHECK(adc_oneshot_del_unit(adc2_handle));
}

