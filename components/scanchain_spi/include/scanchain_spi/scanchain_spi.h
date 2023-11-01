

// configuring the spi scanchain devise is done here.

#pragma once
#include "cppspi/cppspi.h"

namespace SCANCHAINSPI
{
    constexpr static uint8_t MODE = 2;
    constexpr static uint8_t ADDR_BITS = 0;
    constexpr static uint8_t CMD_BITS = 0;

    constexpr static uint8_t SPI_WRITE = 1;
    constexpr static uint8_t SPI_READ = 0;


    class scanchainDevice{
        private:
            CPPSPI::Spi *spi; // creating an instance of the spi bus , to which the devise will be registered

        protected: 
            //esp_err_t writeByteData(const uint8_t reg, const uint8_t value);
            //int readByteData(const uint8_t reg);
            //int readWordData(const uint8_t reg); // this is important
            //esp_err_t readBlockData(const uint8_t reg, uint8_t *buf, const int length);


        public:
            esp_err_t InitSpi(CPPSPI::Spi *i_spi, const int ss,const int bus_speed); // this function resgisters the SPI devise to the spi bus

                     
    
    
    
    };

}