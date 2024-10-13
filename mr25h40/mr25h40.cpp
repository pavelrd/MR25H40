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

    isInitialized = true;

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

    isInitialized = false;

}

/**
 * @brief Разрешить запись в незащищенные с помощью метода setProtect() области памяти
 */

int MR25H40::writeEnable()
{

    if( ( sleepMode ) || ( !isInitialized ) )
    {
        return -1;
    }

   spi->enable();

   if( -1 == spi->transfer(C_WRITE_ENABLE, 0) )
   {
       spi->disable();
       return -1;
   }

   spi->disable();

   writeEnabled = true;

   return 0;

}

/**
 * @brief Запретить запись во все области памяти
 */

int MR25H40::writeDisable()
{

    if( ( sleepMode ) || ( !isInitialized ) )
    {
        return -1;
    }

    spi->enable();
    if( -1 == spi->transfer(C_WRITE_DISABLE, 0) )
    {
        spi->disable();
        return -1;
    }
    spi->disable();

    writeEnabled = false;

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

    if( ( sleepMode ) || ( !isInitialized ) || (!writeEnabled) )
    {
        return -1;
    }

    IO::high(writeProtectLine);

    uint8_t status = 0;

    if( -1 == spi->transfer( C_READ_STATUS_REGISTER, 0 ) )
    {
        IO::low(writeProtectLine);
        return -1;
    }

    if( -1 == spi->transfer( 0, &status ) )
    {
        IO::low(writeProtectLine);
        return -1;
    }

    status &= 0xF3;

    switch(mode)
    {
        case PROTECT_MODE_NONE          : status |= 0 << 2; break;
        case PROTECT_MODE_UPPER_QUARTER : status |= 1 << 2; break;
        case PROTECT_MODE_UPPER_HALF    : status |= 2 << 2; break;
        case PROTECT_MODE_ALL           : status |= 3 << 2; break;
    }


    if( -1 == spi->transfer( C_WRITE_STATUS_REGISTER, 0 ) )
    {
        IO::low(writeProtectLine);
        return -1;
    }

    if( -1 == spi->transfer( status, 0 ) )
    {
        IO::low(writeProtectLine);
        return -1;
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

    if( ( buffer == 0 ) || (address > MEMORY_SIZE_IN_BYTES) || (sleepMode) || (!isInitialized) )
    {
        return -1;
    }

    spi->enable();

    uint8_t requestBuffer[4] =
    {
        (uint8_t)(address >> 16),
        (uint8_t)(address >> 8),
        (uint8_t) address,
        C_READ_DATA_BYTES
    };

    if ( -1 == spi->write(requestBuffer, 4 ) )
    {
        spi->disable();
        return -1;
    }

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
        ( sleepMode )                                          ||
        ( !isInitialized )                                     ||
        ( !writeEnabled )
       )
    {
        return -1;
    }

    spi->enable();

    uint8_t requestBuffer[4] =
    {
        (uint8_t)(address >> 16),
        (uint8_t)(address >> 8),
        (uint8_t) address,
        C_WRITE_DATA_BYTES
    };

    if ( -1 == spi->write(requestBuffer, 4 ) )
    {
        spi->disable();
        return -1;
    }

    spi->write(buffer, numberOfBytes);

    spi->disable();

    return numberOfBytes;

}

/**
 *
 * @brief Заполнение памяти заданными значениями
 *
 * @param value         - значение которым заполнять память
 * @param startAddress  - начальный адрес заполнения
 * @param numberOfBytes - количество байт
 *
 * @return 0 - успешно заполнено, -1 - ошибка
 *
 */

int MR25H40::fill( uint8_t value, uint32_t address, uint32_t numberOfBytes )
{

    if(
        ( address > MEMORY_SIZE_IN_BYTES )                     ||
        ( ( address + numberOfBytes ) > MEMORY_SIZE_IN_BYTES ) ||
        ( sleepMode )                                          ||
        ( !isInitialized )
       )
    {
        return -1;
    }

    spi->enable();

    uint8_t requestBuffer[4] =
    {
        (uint8_t)(address >> 16),
        (uint8_t)(address >> 8),
        (uint8_t) address,
        C_WRITE_DATA_BYTES
    };

    if ( -1 == spi->write(requestBuffer, 4 ) )
    {
        spi->disable();
        return -1;
    }

    /// \todo Медленная реализация, нужно переписать на блочную запись

    for( uint32_t i = 0; i < numberOfBytes; i++ )
    {
        if( -1 == spi->transfer(value, 0) )
        {
            spi->disable();
            return -1;
        }
    }

    spi->disable();

    return -1;

}

/**
 * @brief Перейти в режим сна. В режиме сна
 *        блокируется выполнение всех методов, кроме wake()!
 *        По причине: "The only valid command following SLEEP mode entry is a WAKE command"
 */

int MR25H40::sleep()
{

    if( ( !isInitialized ) || ( sleepMode ) )
    {
        return -1;
    }

    spi->enable();
    if( -1 == spi->transfer(C_ENTER_SLEEP_MODE, 0) )
    {
        spi->disable();
        return -1;
    }
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

    if( !isInitialized )
    {
        return -1;
    }

    spi->enable();
    if( -1 == spi->transfer(C_EXIT_SLEEP_MODE, 0) )
    {
        spi->disable();
        return -1;
    }
    spi->disable();

    // Ожидание Trdp > 400 us
    //
    // Обертка Thread::delay(1) которая под Freertos использует vTaskDelay(1) а под PC std::thread и std::chrono
    //

    sleepMode = false;

    return 0;

}

/**
 * @brief Приостановить работу. Использовать только
 *         когда выбран чип.
 *
 *        Не совсем понятно как использовать данную функцию микросхемы.
 *
 *        Держать массив объектов MR25H40 и в обработчике соответствующего прерывания
 *        вызывать для всех неиспользуемых микросхем hold()...
 *
 */

int MR25H40::hold()
{
    if( ( isInitialized ) && ( spi->isEnabled() ) )
    {
        IO::low(holdLine);
    }
    else
    {
        return -1;
    }
    return 0;
}

/**
 * @brief Возобновить работу. Использовать только
 *         когда выбран чип!
 */

int MR25H40::unhold()
{
    if( ( isInitialized ) && ( spi->isEnabled() ) )
    {
        IO::high(holdLine);
    }
    else
    {
        return -1;
    }
    return 0;
}
