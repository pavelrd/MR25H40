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

    testMR25H40(std::string dumpFilename);

    int read( void* buffer, uint32_t numberOfBytes, uint32_t address ) override;

    int write( void* buffer, uint32_t numberOfBytes, uint32_t address ) override;

    void setProtect(PROTECT_MODES mode) override;

    void sleep() override;
    void wake() override;

    void hold() override;

    void writeStatus() override;
private:
    std::fstream dumpFile;
};

#endif // TESTMR25H40_H
