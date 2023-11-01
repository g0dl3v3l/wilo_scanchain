#include "scanchain_spi/scanchain_spi.h"


namespace SCANCHAINSPI{
    
    // writing single bit 
    // esp_err_t scanchainDevice::writeByteData(const uint8_t reg, const uint8_t value)
    // {
    //     return spi->WriteRegister(reg, value, SPI_WRITE);
    // }

    //read single bit
    // int scanchainDevice::readByteData(const uint8_t reg)
    // {
    //     return spi->ReadRegister(reg, SPI_READ);
    // }

    // this reads buffer data from the spi device
    // int scanchainDevice::readWordData(const uint8_t reg)
    // {
    //     uint8_t buff[2];
    //     spi->ReadRegisterMultipleBytes(reg, buff, 2, SPI_READ);
    //     return buff[1] << 8 | buff[0];
    // }

    // writes buffer data to the spi device
    // esp_err_t scanchainDevice::readBlockData(const uint8_t reg, uint8_t *buf, const int length)
    // {
    //     return spi->ReadRegisterMultipleBytes(reg, buf, length, SPI_READ);
    // }


    //registers the device to the spi bus
    esp_err_t scanchainDevice::InitSpi(CPPSPI::Spi *i_spi, const int ss,const int bus_speed)
    {
        spi = i_spi;

        return spi->RegisterDevice(MODE, ss, ADDR_BITS, CMD_BITS, bus_speed);
    }

}