#ifndef ABSTRACTMR25H40_H
#define ABSTRACTMR25H40_H

#include <stdint.h>

/**
 *
 * @brief Для работы с микросхемой SPI MRAM памяти модели MR25H40 фирмы EVERSPIN
 *         включает в себя:
 *           MR25H40     - содержит реализацию под железо
 *           testMR25H40 - для тестирования
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
        NONE,
        UPPER_QUARTER,
        UPPER_HALF,
        ALL
    };

    virtual int read( void* buffer, uint32_t numberOfBytes, uint32_t address ) = 0;

    virtual int write( void* buffer, uint32_t numberOfBytes, uint32_t address ) = 0;

    virtual void setProtect(PROTECT_MODES mode) = 0;

    virtual void sleep() = 0;
    virtual void wake() = 0;

    virtual void hold() = 0;

    virtual void writeStatus() = 0;

};
#endif // ABSTRACTMR25H40_H
