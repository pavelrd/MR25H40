#ifndef TESTMR25H40_H
#define TESTMR25H40_H

#include "abstractmr25h40.hpp"

#include <string>
#include <fstream>

/**
 *
 * @brief The testMR25H40 class
 *
 */

class testMR25H40 : public AbstractMR25H40
{
public:

    testMR25H40();

    ~testMR25H40();

    testMR25H40( std::string dumpFilename );

    void init();

    void deinit();

    int read( void* buffer, uint32_t numberOfBytes, uint32_t address ) override;

    int write( void* buffer, uint32_t numberOfBytes, uint32_t address ) override;

    int setProtect(PROTECT_MODES mode) override;

    int sleep() override;
    int wake() override;

    int hold() override;

private:
    std::fstream  dumpFile           = 0;
    uint8_t*      ram                = 0;
    PROTECT_MODES currentProtectMode = PROTECT_MODE_NONE;
    bool          wpState            = false;
    bool          isWake             = true;
};

#endif // TESTMR25H40_H
