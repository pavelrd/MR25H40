#ifndef SPI_H
#define SPI_H

#include "io.hpp"
#include <stdint.h>

/**
 * @brief The SPI8 class
 */

class SPI8
{

public:

    enum MODULE
    {
        MODULE0,
        MODULE1,
        UNSELECTED_MODULE
    };

    enum CHIP_SELECT
    {
        HARDWARE_CS0,
        HARDWARE_CS1,
        HARDWARE_CS2,
        UNSELECTED_HARDWARE_CS
    };

    SPI8( MODULE _module, CHIP_SELECT _hardwareCS );

    SPI8( MODULE _module, IO::PIN _cs );

    SPI8( IO::PIN _miso, IO::PIN _mosi, IO::PIN _sck, IO::PIN _cs );

    void init(uint32_t _speed);

    uint8_t transfer(uint8_t data);

    int read(void* buffer, uint32_t size);

    int write( void* buffer, uint32_t size );

    void enable();

    void disable();

private:

    IO::PIN miso = IO::UNUSED;
    IO::PIN mosi = IO::UNUSED;
    IO::PIN sck  = IO::UNUSED;
    IO::PIN cs   = IO::UNUSED;

    enum MODE
    {
        SOFTWARE_MODE,
        HARDWARE_MODE,
        UNSELECTED_MODE
    };

    MODE mode = UNSELECTED_MODE;

    MODULE module = UNSELECTED_MODULE;

    CHIP_SELECT hardwareCS = UNSELECTED_HARDWARE_CS;

    uint32_t speed;

// Платформо-зависимые функции

    void _init();

    uint8_t _transfer(uint8_t data);

    int _read(void* buffer, uint32_t size);

    int _write( void* buffer, uint32_t size );

};

#endif // SPI_H