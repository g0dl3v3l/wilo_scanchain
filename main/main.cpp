#include "cppspi/cppspi.h"
#include <iostream>
#include "scanchain_spi/scanchain_spi.h"
#include "string.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

constexpr static int SPI_3_MISO = 34;
constexpr static int SPI_3_MOSI = 35;
constexpr static int SPI_3_SCLK = 36;
constexpr static int SS_PIN = 47;
#ifdef CONFIG_IDF_TARGET_ESP32
#define SENDER_HOST HSPI_HOST
#else
#define SENDER_HOST SPI2_HOST
#endif
CPPSPI::Spi spi3;

extern "C" void app_main(void)
{  
    esp_err_t ret;
    SCANCHAINSPI::scanchainDevice scanchainSpi;

     // initialise the spi bus
    ret = spi3.Init(SENDER_HOST, SPI_3_MISO, SPI_3_MOSI, SPI_3_SCLK);
   assert(ret == ESP_OK);

   // registered the device .
   ret = scanchainSpi.InitSpi(&spi3 ,SS_PIN,1000000);
   assert(ret == ESP_OK);

   while(1){

        //std::string send_data = "0b01010101"; // 12-bit data to send
        int send_data[] = {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1};
        uint8_t nbits = 26; // Number of bits to send

        spi3.sendNBitData(send_data, nbits);

        vTaskDelay(pdMS_TO_TICKS(100));  // Adjust delay between transmissions as needed



   }



}
