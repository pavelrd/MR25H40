#ifndef MR25H40_H
#define MR25H40_H

#include "spi.hpp"
#include "abstractmr25h40.hpp"

/**
 *
 * @brief Реализация под "железо" класса AbstractMR25H40
 *
 *         Обеспечивает возможность записи/чтения памяти, задания режима защиты памяти и входа/выхода
 *         из режима сна, включение/выключение режима удержания(hold).
 *         Также обеспечивается защита от: непроинициализированного использования, записи во время того
 *         как запись запрещена(бит WEL == 0 в регистре Status Register), записи в недопустимые области памяти,
 *         посылки любых команд кроме wake во время режима сна.
 *
 *         Как пользоваться:
 *
 *         1) В начале надо настроить и проинициализировать объект типа SPI,
 *         при помощи него MR25H40 будет обращаться к "железу".
 *
 *         2) В конструкторе MR25H40 передать объект типа spi и задать ножки writeProtect и hold.
 *
 *         3) Вызвать метод init()
 *
 *         4) Пользоваться остальными методами по-необходимости
 *
 *         Пример:
 *
 *          SPI8 spi( IO::D1, IO::D2, IO::D3, IO::D4 );
 *
 *          spi->init(1000000);
 *
 *          MR25H40 memory( &spi, IO::D5, IO::D6 );
 *
 *          memory.init();
 *
 *          char writeBuf[3] = {1,2,3};
 *          char readBuf[3]  = {0};
 *
 *          memory.write(writeBuf, 3, 0);
 *
 *          memory.read(readBuf, 3, 0);
 *
 *
 */

class MR25H40 : public AbstractMR25H40
{

public:

    MR25H40( SPI8* _spi, IO::PIN _writeProtect, IO::PIN _hold );

    void init();

    void deinit();

    int read( void* buffer, uint32_t numberOfBytes, uint32_t address ) override;

    int write( void* buffer, uint32_t numberOfBytes, uint32_t address ) override;

    int fill(uint8_t value, uint32_t address, uint32_t numberOfBytes ) override;

    int writeEnable() override;

    int writeDisable() override;

    int setProtect(PROTECT_MODES mode) override;

    int sleep() override;
    int wake() override;

    int hold() override;
    int unhold() override;

private:

    IO::PIN writeProtectLine = IO::UNUSED;
    IO::PIN holdLine         = IO::UNUSED;

    SPI8* spi           = 0;
    bool  sleepMode     = false;
    bool  isInitialized = false;
    bool  writeEnabled  = false;

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

};

#endif // MR25H40_H
