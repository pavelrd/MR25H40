#ifndef SPI_H
#define SPI_H

#include "io.hpp"
#include <stdint.h>

/**
 * @brief Обертка для медленного программного SPI или работы с быстрыми аппаратными
 *         SPI модулями
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

    void deinit();

    int transfer(uint8_t data, uint8_t *out);

    int read(void* buffer, uint32_t size);

    int write( void* buffer, uint32_t size );

    void enable();

    void disable();

    bool isEnabled();

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

    uint32_t speed = 0;

    bool     isInitialized = false;

// Платформо-зависимые функции

    void _init();
    void _deinit();

    int _transfer(uint8_t data, uint8_t* out);

    int _read(void* buffer, uint32_t size);

    int _write( void* buffer, uint32_t size );

};

#endif // SPI_H
