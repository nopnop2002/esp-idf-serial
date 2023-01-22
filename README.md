# esp-idf-serial
User-level UART I/O library for ESP-IDF.

# Background
The Arduino environment has several libraries for handling UART devices such as MP3 players, GPS receivers, GAS sensor.   
ESP-IDF provides a highly functional UART driver.   
But we can't treat it like STDIN/STDOUT.   
I created this to port applications and libraries that use UART from the Arduino environment to the ESP-IDF environment.   


# Software requirements
ESP-IDF V4.4/V5.0.   

# Installation

```Shell
git clone https://github.com/nopnop2002/esp-idf-serial
cd esp-idf-serial
idf.py flash
```

# Wirering
|ESP32||TTL-USB Converter|
|:-:|:-:|:-:|
|GPIO4|--|TX|
|GPIO5|--|RX|
|GND|--|GND|

Plug the TTL-USB Converter into your host and open a serial terminal on your host.   
I use GtkTerm on linux.   

# Arduno code
```
#include <SoftwareSerial.h>

SoftwareSerial _serial(4, 5); // RX, TX

unsigned long lastMillis;

void setup() {
  Serial.begin(115200);
  _serial.begin(115200);
  lastMillis = 0;
}

void loop() {
  int len = Serial.available();
  if (len > 0) {
    for (int i=0;i<len;i++) {
      char ch = Serial.read();
      _serial.println(ch, HEX);
    }
  }

  long nowMillis = millis();
  if (nowMillis - lastMillis > 1000) {
    lastMillis = nowMillis;
    char buffer[64];
    sprintf(buffer, "Hello World %d",nowMillis);  
    Serial.println(buffer);
  }
}
```


}

# ESP-IDF code using this
```
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

void app_main(void)
{
    serial_begin(112500, TXD_PIN, RXD_PIN);
    TickType_t lastTick = xTaskGetTickCount();
    while(1) {
        int len = serial_available();
        if (len > 0) {
            ESP_LOGI(TAG, "serial_available=%d", len);
            for (int i=0;i<len;i++) {
                uint8_t ch = serial_read();
                ESP_LOGI(TAG, "serial_read=%x", ch);
            }
        }

        TickType_t nowTick = xTaskGetTickCount();
        if (nowTick - lastTick > 100) {
            lastTick = xTaskGetTickCount();
            char buffer[64];
            sprintf(buffer, "Hello World %"PRIu32, nowTick);
            ESP_LOGI(TAG, "buffer=[%s]", buffer);
            serial_println(buffer);
        }
        vTaskDelay(1);
    }
}
```
