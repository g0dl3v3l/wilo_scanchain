#include <iostream>
#include "cppspi/cppspi.h"
#include <string.h>
#include "driver/gpio.h"

using namespace std;

namespace CPPSPI
{
    esp_err_t Spi::transferByte(const uint8_t reg_addr, const uint8_t data, const uint8_t command)
    {
        _spi_transaction.flags = SPI_TRANS_USE_RXDATA | SPI_TRANS_USE_TXDATA;
        _spi_transaction.cmd = command;
        _spi_transaction.length = 8;
        _spi_transaction.addr = reg_addr;
        _spi_transaction.tx_data[0] = data;

        return spi_device_transmit(_handle, &_spi_transaction);
    }

    esp_err_t Spi::transferMultiplesBytes( uint8_t* tx_buf, size_t data_length)
    {
        //spi_transaction_t spi_transaction_multibyte;        // spi_transaction_t to use the tx and rx buffers
         
        memset(&_spi_transaction, 0, sizeof(_spi_transaction));

        if (data_length < 1) { data_length = 1; }

        //spi_transaction_multibyte.flags = 0;
        _spi_transaction.length = data_length;
        //spi_transaction_multibyte.rxlength = 0;
        //spi_transaction_multibyte.cmd = command;
        //spi_transaction_multibyte.addr = reg_addr;
        _spi_transaction.tx_buffer = tx_buf;
        //spi_transaction_multibyte.rx_buffer = rx_buf;        

       //return spi_device_transmit(_handle, &spi_transaction_multibyte);
       printf("transfer done\n");
        return spi_device_polling_transmit(_handle, &_spi_transaction);
    }

    esp_err_t Spi::Init(const spi_host_device_t spi_peripheral, const int pin_miso, const int pin_mosi, const int pin_sclk)
    {
        esp_err_t status = ESP_OK;

        _spi_peripheral = spi_peripheral;

        // _spi_transaction.tx_buffer = nullptr;
        // _spi_transaction.rx_buffer = nullptr;

        _spi_bus_cfg.mosi_io_num = pin_mosi;
        _spi_bus_cfg.miso_io_num = pin_miso;
        _spi_bus_cfg.sclk_io_num = pin_sclk;
        _spi_bus_cfg.quadwp_io_num = -1;
        _spi_bus_cfg.quadhd_io_num = -1;

        status = spi_bus_initialize(_spi_peripheral, &_spi_bus_cfg, SPI_DMA_CH_AUTO);

        return status;
    } 

    esp_err_t Spi::RegisterDevice(const uint8_t mode, const int ss, const int addr_length, const int command_length, const int bus_speed)
    {
        //gpio_set_level((gpio_num_t)ss, 1);
        esp_err_t status = ESP_OK;

        _spi_interface_cfg = {};
        _spi_interface_cfg.command_bits = command_length; 
        _spi_interface_cfg.address_bits = addr_length;
         _spi_interface_cfg.dummy_bits = 0;
        _spi_interface_cfg.clock_speed_hz = bus_speed;
         _spi_interface_cfg.duty_cycle_pos = 0;
        _spi_interface_cfg.mode = mode;
        _spi_interface_cfg.spics_io_num = ss; // IO pin 5 is my chip select
        _spi_interface_cfg.queue_size = 3;

        status |= spi_bus_add_device(_spi_peripheral, &_spi_interface_cfg, &_handle);

        return status;
    }

    uint8_t Spi::ReadRegister(const uint8_t reg_addr, const uint8_t command)
    {
        transferByte(reg_addr, 0, command);

        return _spi_transaction.rx_data[0];
    }

    esp_err_t Spi::WriteRegister(const uint8_t reg_addr, const uint8_t reg_data, const uint8_t command)
    {
        esp_err_t status{ESP_OK};

        status |= transferByte(reg_addr, reg_data, command);

        return status;
    }

    esp_err_t Spi::WriteRegisterMultipleBytes(uint8_t* reg_data_buffer, const uint8_t byte_count)
    {
        return transferMultiplesBytes(reg_data_buffer, byte_count);
    }

    // esp_err_t Spi::ReadRegisterMultipleBytes(const uint8_t reg_addr, uint8_t* reg_data_buffer, const uint8_t byte_count, const uint8_t command)
    // {   
    //     return transferMultiplesBytes(reg_addr, nullptr, reg_data_buffer, byte_count, command);
    // }

    spi_device_handle_t Spi::GetHandle(void)
    {
        return _handle;
    }

    std::bitset<8> Spi::binary(unsigned long long int value) {
    // Ensure that the value is within the 8-bit range
    value &= 0xFFull;
    return std::bitset<8>(value);
}

//     void Spi::sendNBitData(std::string data, uint8_t n){
//     uint8_t numBytes = (n + 7) / 8; // Calculate the number of bytes required
//     int extraBits = n%8; //counts number of extra bits , to which 8-extrabits 0s need to be padded to make it divisible by eight
//     uint8_t sendBuf[numBytes] = {0}; 

//     if(extraBits != 0){
//         //pad with extra zeros
//         for (int i = 0 ; i< extraBits;i++){
//             data = data+"0";
//         }

//         for(int i = 0;i<numBytes;i++){
//             string byteStr = data.substr(i*8 ,8);
//             unsigned long long int decimalValue = stoull(byteStr, nullptr, 2);
//            sendBuf[i] = static_cast<uint8_t>(binary(decimalValue).to_ulong());
//         }
//         sendBuf[numBytes-1] = sendBuf[numBytes-1]>>(8-extraBits);
//     }else{
//         for(int i = 0;i<numBytes;i++){
//             string byteStr = data.substr(i*8 ,8);
//             unsigned long long int decimalValue = stoull(data, nullptr, 2);
//             sendBuf[i] = static_cast<uint8_t>(binary(decimalValue).to_ulong());
            
//         }
       
//     }

    
//     gpio_set_level((gpio_num_t)_spi_interface_cfg.spics_io_num, 0);
    
//     esp_err_t ret = WriteRegisterMultipleBytes(sendBuf , n);
//     assert(ret == ESP_OK);
//     gpio_set_level((gpio_num_t)_spi_interface_cfg.spics_io_num, 0);

//     }


    void Spi::sendNBitData(int data[], uint8_t n) {
        // Calculate the number of bytes required to store 'n' bits.
        uint8_t numBytes = (n + 7) / 8;

        // Calculate the number of extra bits that don't fill a full byte.
        int extraBits = n % 8;

        // Create an array 'sendBuf' to store the bits, initialized to all zeros.
        uint8_t sendBuf[numBytes] = {0};
       // printf("the length of the buffer %d\n",numBytes);
        if (extraBits != 0) {
            // Pad 'data' with extra zeros to make it divisible by eight.
            for (int i = 0; i < (8 - extraBits); i++) {
                data[n + i] = 0;
            }

            // Loop through each byte in 'sendBuf'.
            for (int i = 0; i < numBytes; i++) {
                // Loop through each bit in the byte.
                uint8_t byteValue = 0;
                for (int j = 0; j < 8; j++) {
                    
                    byteValue |= (data[i * 8 + j] & 1) << (7 - j);
                }
                sendBuf[i] = byteValue;
               // printf("The bit getting added in  the sendBuf %d \n", sendBuf[i]);

            }

            // Right-shift the last byte in 'sendBuf' to remove zero-padding (not used).
            // sendBuf[numBytes - 1] >>= (8 - extraBits);
        } else {
            // If there are no extra bits, simply copy the bits from 'data' to 'sendBuf'.
            for (int i = 0; i < numBytes; i++) {
                uint8_t byteValue = 0;
                for (int j = 0; j < 8; j++) {
                byteValue |= (data[i * 8 + j] & 1) << (7 - j);
                }
                sendBuf[i] = byteValue;
                printf("The bit getting added in  the sendBuf %d \n", sendBuf[i]);
            }
        }

        gpio_set_level((gpio_num_t)_spi_interface_cfg.spics_io_num, 0);
        esp_err_t ret = WriteRegisterMultipleBytes(sendBuf , n);
        assert(ret == ESP_OK);
        gpio_set_level((gpio_num_t)_spi_interface_cfg.spics_io_num, 1);
    }

}





 // namespace CPPSPI
