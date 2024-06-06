/* UART asynchronous example, that uses separate RX and TX tasks

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/


#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/gpio.h"

#include "serial.h"

#define TAG "MAIN"

#define TXD_PIN (GPIO_NUM_4)
#define RXD_PIN (GPIO_NUM_5)

#define READ_WRITE 1

void app_main(void)
{
	serial_begin(112500, TXD_PIN, RXD_PIN);
#if READ_WRITE
	TickType_t lastTick = xTaskGetTickCount();
#endif
	while(1) {
		int len = serial_available();
		if (len > 0) {
			ESP_LOGI(TAG, "serial_available=%d", len);
			for (int i=0;i<len;i++) {
				uint8_t ch = serial_read();
				if (ch >= 20 && ch < 127) {
					ESP_LOGI(TAG, "serial_read=%x[%c]", ch, ch);
				} else {
					ESP_LOGI(TAG, "serial_read=%x", ch);
				}
			}
		}

#if READ_WRITE
		TickType_t nowTick = xTaskGetTickCount();
		if (nowTick - lastTick > 100) {
			lastTick = xTaskGetTickCount();
			char buffer[64];
			sprintf(buffer, "Hello World %"PRIu32, nowTick);
			ESP_LOGI(TAG, "buffer=[%s]", buffer);
			serial_println(buffer);
		}
#endif
		vTaskDelay(1);
	}	
}
