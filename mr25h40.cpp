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
 * @brief MR25H40::writeEnable
 */

void MR25H40::writeEnable()
{
   spi->enable();
   spi->transfer(C_WRITE_ENABLE);
   spi->disable();
}

/**
 * @brief MR25H40::writeDisable
 */

void MR25H40::writeDisable()
{
    spi->enable();
    spi->transfer(C_WRITE_DISABLE);
    spi->disable();
}

/**
 *
 * @brief Задать режим защиты памяти
 * @param mode
 *
 */

void MR25H40::setProtect(PROTECT_MODES mode)
{

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

    if( ( buffer == 0 ) || (address > MEMORY_SIZE_IN_BYTES) )
    {
        return -1;
    }

    spi->transfer(C_READ_DATA_BYTES);

    spi->transfer( (address >> 16) & 0xFF );
    spi->transfer( (address >> 8) & 0xFF  );
    spi->transfer( address & 0xFF );

    uint32_t bytesForRead = ( address + numberOfBytes ) > MEMORY_SIZE_IN_BYTES ? MEMORY_SIZE_IN_BYTES - address : numberOfBytes;

    spi->read(buffer, bytesForRead);

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
        ( ( address + numberOfBytes ) > MEMORY_SIZE_IN_BYTES )
       )
    {
        return -1;
    }

    writeEnable();

    spi->transfer(C_WRITE_DATA_BYTES);

    spi->transfer( (address >> 16) & 0xFF );
    spi->transfer( (address >> 8) & 0xFF  );
    spi->transfer( address & 0xFF );

    uint8_t* pBuffer = (uint8_t*) buffer;

    for( uint32_t i = 0 ; i < numberOfBytes; i++ )
    {
        spi->transfer(*pBuffer);
        pBuffer++;
    }

    writeDisable();

    return numberOfBytes;

}

/**
 * @brief MR25H40::sleep
 */

void MR25H40::sleep()
{

}

/**
 * @brief MR25H40::wake
 */

void MR25H40::wake()
{

}

/**
 * @brief MR25H40::hold
 */

void MR25H40::hold()
{

}

/**
 * @brief MR25H40::writeStatus
 */

void MR25H40::writeStatus()
{

}
