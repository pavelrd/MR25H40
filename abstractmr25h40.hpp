#ifndef ABSTRACTMR25H40_H
#define ABSTRACTMR25H40_H

#include <stdint.h>

/**
 *
 * @brief The AbstractMR25H40 class
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
