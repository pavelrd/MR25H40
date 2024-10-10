#ifndef TESTMR25H40_H
#define TESTMR25H40_H

#include "abstractmr25h40.hpp"

/**
 *
 * @brief The testMR25H40 class
 *
 */

class testMR25H40 : public AbstractMR25H40
{
public:

    testMR25H40();

    int read( void* buffer, uint32_t numberOfBytes, uint32_t address ) override;

    int write( void* buffer, uint32_t numberOfBytes, uint32_t address ) override;

    void setProtect(PROTECT_MODES mode) override;

    void sleep() override;
    void wake() override;

    void hold() override;

    void writeStatus() override;

};

#endif // TESTMR25H40_H
