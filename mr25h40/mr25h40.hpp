#ifndef MR25H40_H
#define MR25H40_H

#include "spi.hpp"
#include "abstractmr25h40.hpp"

/**
 *
 * @brief Реализация под "железо" класса AbstractMR25H40
 *
 */

class MR25H40 : public AbstractMR25H40
{

public:

    MR25H40( SPI8* _spi, IO::PIN _writeProtect, IO::PIN _hold );

    int read( void* buffer, uint32_t numberOfBytes, uint32_t address ) override;

    int write( void* buffer, uint32_t numberOfBytes, uint32_t address ) override;

    void setProtect(PROTECT_MODES mode) override;

    void sleep() override;
    void wake() override;

    void hold() override;

    void writeStatus() override;

private:

    IO::PIN writeProtectLine = IO::UNUSED;
    IO::PIN holdLine         = IO::UNUSED;

    SPI8* spi = 0;

    enum COMMANDS
    {
        C_WRITE_ENABLE          = 0x06,
        C_WRITE_DISABLE         = 0x04,
        C_READ_STATUS_REGISTER  = 0x05,
        C_WRITE_STATUS_REGISTER = 0x01,
        C_READ_DATA_BYTES       = 0x03,
        C_WRITE_DATA_BYTES      = 0x02,
        C_ENTER_SLEEP_MODE      = 0xB9,
        C_EXIT_SLEEP_MODE       = 0xAB
    };

    void writeEnable();
    void writeDisable();

};

#endif // MR25H40_H
