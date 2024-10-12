#include "mr25h40.hpp"

/**
 *
 * @brief MR25H40::MR25H40
 *
 * @param _spi          объект с помощью методов которого MR25H40 будет передавать данные
 * @param _writeProtect ножка контроллера, к которой подключен вывод WP микросхемы MR25H40
 * @param _hold         ножка контроллера, к которой подключен вывод HOLD микросхемы MR25H40
 *
 */

MR25H40::MR25H40(SPI8* _spi, IO::PIN _writeProtect, IO::PIN _hold ) //
{
    spi              = _spi;
    writeProtectLine = _writeProtect;
    holdLine         = _hold;
}

/**
 * @brief Настройка ножек специфичных для данной микросхемы
 */

void MR25H40::init()
{

    IO::use(writeProtectLine);
    IO::low(writeProtectLine);
    IO::out(writeProtectLine);

    IO::use(holdLine);
    IO::high(holdLine);
    IO::out(holdLine);

}

/**
 *
 * @brief Деконфигурация ножек, настроенных ранее методом init()
 *
 */

void MR25H40::deinit()
{

    IO::in(writeProtectLine);
    IO::unuse(writeProtectLine);

    IO::in(holdLine);
    IO::unuse(holdLine);

}

/**
 * @brief Разрешить запись в незащищенные с помощью метода setProtect() области памяти
 */

int MR25H40::writeEnable()
{

    if( sleepMode )
    {
        return -1;
    }

   spi->enable();
   spi->transfer(C_WRITE_ENABLE);
   spi->disable();

   return 0;

}

/**
 * @brief Запретить запись во все области памяти
 */

int MR25H40::writeDisable()
{

    if( sleepMode )
    {
        return -1;
    }

    spi->enable();
    spi->transfer(C_WRITE_DISABLE);
    spi->disable();

    return 0;

}

/**
 *
 * @brief Задать режим защиты памяти
 * @param mode
 *
 */

int MR25H40::setProtect(PROTECT_MODES mode)
{

    if( sleepMode )
    {
        return -1;
    }

    IO::high(writeProtectLine);

    uint8_t status = 0;

    switch(mode)
    {
        case PROTECT_MODE_NONE          : status |= 0 << 2; break;
        case PROTECT_MODE_UPPER_QUARTER : status |= 1 << 2; break;
        case PROTECT_MODE_UPPER_HALF    : status |= 2 << 2; break;
        case PROTECT_MODE_ALL           : status |= 3 << 2; break;
    }

    IO::low(writeProtectLine);

    return 0;

}

/**
 *
 * @brief Считывает из памяти запрошенное количество байт
 *         Если количество байт превысит размер памяти, будет считано
 *         столько байт, сколько осталось до конца памяти.
 *
 * @param buffer буфер для считываемых данных
 * @param numberOfBytes количество байт для чтения.
 * @param address адрес начала чтения данных
 *
 * @return -1 - ошибка, иначе количество считанных байт.
 *
 */

int MR25H40::read( void* buffer, uint32_t numberOfBytes, uint32_t address )
{

    if( ( buffer == 0 ) || (address > MEMORY_SIZE_IN_BYTES) || (sleepMode) )
    {
        return -1;
    }

    spi->enable();

    spi->transfer(C_READ_DATA_BYTES);

    spi->transfer( (address >> 16) & 0xFF );
    spi->transfer( (address >> 8) & 0xFF  );
    spi->transfer( address & 0xFF );

    uint32_t bytesForRead = ( address + numberOfBytes ) > MEMORY_SIZE_IN_BYTES ? MEMORY_SIZE_IN_BYTES - address : numberOfBytes;

    spi->read(buffer, bytesForRead);

    spi->disable();

    return bytesForRead;

}

/**
 *
 * @brief Записывает в память указанное количество байт
 *         Вернет ошибку при попытке записать данные за пределы
 *         доступной памяти
 *
 * @param buffer буфер из которого будут браться записываемые данные
 * @param numberOfBytes количество в байт для записи
 * @param address адрес начала записи данных
 *
 * @return -1 - ошибка, иначе количество считанных байт.
 *
 */

int MR25H40::write( void* buffer, uint32_t numberOfBytes, uint32_t address )
{

    if( ( buffer == 0 )                                        ||
        ( address > MEMORY_SIZE_IN_BYTES )                     ||
        ( ( address + numberOfBytes ) > MEMORY_SIZE_IN_BYTES ) ||
        ( sleepMode )
       )
    {
        return -1;
    }

    writeEnable();

    spi->enable();

    spi->transfer(C_WRITE_DATA_BYTES);

    spi->transfer( (address >> 16) & 0xFF );
    spi->transfer( (address >> 8) & 0xFF  );
    spi->transfer( address & 0xFF );

    spi->write(buffer, numberOfBytes);

    spi->disable();

    writeDisable();

    return numberOfBytes;

}

/**
 * @brief Перейти в режим сна. В режиме сна
 *        блокируется выполнение всех методов, кроме wake()!
 *        По причине: "The only valid command following SLEEP mode entry is a WAKE command"
 */

int MR25H40::sleep()
{

    spi->enable();
    spi->transfer(C_ENTER_SLEEP_MODE);
    spi->disable();

    // через 3 мкс энергопотребление должно упать до примерно 15 микроампер

    sleepMode = true;

    return 0;

}

/**
 * @brief Выйти из режима сна
 */

int MR25H40::wake()
{

    spi->enable();
    spi->transfer(C_EXIT_SLEEP_MODE);
    spi->disable();

    // Ожидание Trdp > 400 us
    //
    // Обертка Thread::delay(1) которая под Freertos использует vTaskDelay(1) а под PC std::thread и std::chrono
    //

    sleepMode = false;

    return 0;

}

/**
 * @brief MR25H40::hold
 */

int MR25H40::hold()
{
    return 0;
}
