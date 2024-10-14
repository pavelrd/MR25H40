#ifndef ABSTRACTMR25H40_H
#define ABSTRACTMR25H40_H

#include <stdint.h>

/**
 *
 * @brief Для работы с микросхемой SPI MRAM памяти модели MR25H40 фирмы EVERSPIN
 *
 *         MR25H40 Rev. 5, 11/2011
 *
 *         включает в себя:
 *           MR25H40     - содержит реализацию под железо
 *           testMR25H40 - для тестирования
 *
 */

class AbstractMR25H40
{

public:

    enum SIZES
    {
        MEMORY_SIZE_IN_BYTES = 524288
    };

    enum PROTECT_MODES
    {
        PROTECT_MODE_NONE          = 0,
        PROTECT_MODE_UPPER_QUARTER = 1,
        PROTECT_MODE_UPPER_HALF    = 2,
        PROTECT_MODE_ALL           = 3
    };

    virtual void init() = 0;

    virtual int read( void* buffer, uint32_t numberOfBytes, uint32_t address ) = 0;

    virtual int write( void* buffer, uint32_t numberOfBytes, uint32_t address ) = 0;

    virtual int fill( uint8_t value, uint32_t startAddress, uint32_t numberOfBytes ) = 0;

    virtual int writeEnable() = 0;

    virtual int writeDisable() = 0;

    virtual int protect( PROTECT_MODES* mode ) = 0;

    virtual int setProtect(PROTECT_MODES mode, bool srwd = false) = 0;

    virtual int sleep() = 0;
    virtual int wake() = 0;

    virtual int hold() = 0;
    virtual int unhold() = 0;

};
#endif // ABSTRACTMR25H40_H
