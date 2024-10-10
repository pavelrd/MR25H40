#include "spi.hpp"

#include <iostream>

void SPI8::_init()
{
    std::cout << "SPI8::_init()" << std::endl;
}

/**
 * @brief SPI::transfer
 * @param data
 * @return
*/

uint8_t SPI8::_transfer(uint8_t data)
{

    std::cout << "SPI8::transfer(" << data << ")" << std::endl;

    return 0;

}

/**
 * @brief read
 * @param buffer
 * @param size
 * @return
 */

int SPI8::_read(void* buffer, uint32_t size)
{

    std::cout << "SPI8::read(" << (uint64_t) buffer << "," << size << ")" << std::endl;

    for(uint32_t i = 0 ; i < size; i++ )
    {
        ((uint8_t*)buffer)[i] = 0x00;
    }

    return 0;

}

/**
 * @brief write
 * @param buffer
 * @param size
 * @return
 */

int SPI8::_write( void* buffer, uint32_t size )
{

    std::cout << "SPI8::write(" << (uint64_t) buffer << "," << size << ")" << std::endl;

    return 0;

}
