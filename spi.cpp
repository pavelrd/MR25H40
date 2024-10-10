#include "spi.hpp"

/**
 *
 * @brief Настройка аппаратного режима работы, аппаратный выбор чипа
 *
 * @param _module     модуль SPI в составе контроллера
 * @param _hardwareCS аппаратный вывод выбора чипа
 *
 */

SPI8::SPI8(MODULE _module, CHIP_SELECT _hardwareCS)
{
    module     = _module;
    hardwareCS = _hardwareCS;
}

/**
 *
 * @brief Настройка аппаратного режима работы, программный выбор чипа
 *
 * @param _module модуль SPI в составе контроллера
 * @param _cs     ножка выбора чипа, управляется программно

 */

SPI8::SPI8( MODULE _module, IO::PIN _cs )
{
    module = _module;
    cs     = _cs;
}

/**
 * @brief Программный режим работы, программный выбор чипа
 * @param _miso данные к master
 * @param _mosi данные к slave
 * @param _sck  ножка синхронизации
 * @param _cs   ножка выбора чипа, управляется программно
 */

SPI8::SPI8( IO::PIN _miso, IO::PIN _mosi, IO::PIN _sck, IO::PIN _cs )
{
    miso = _miso;
    mosi = _mosi;
    sck  = _sck;
    cs   = _cs;
    mode = SOFTWARE_MODE;
}

/**
 *
 * @brief Инициализация выводов, настойка аппаратного модуля если требуется
 *
 * @param _speed скорость работы по SPI, актуально только для аппаратного режима
 *                в программном режиме скорость не используется(возможно использование в будущем)
 *
 */

void SPI8::init(uint32_t _speed)
{

    speed = _speed;

    if( mode == SOFTWARE_MODE )
    {

        IO::use(miso);
        IO::use(mosi);
        IO::use(sck);
        IO::use(cs);

        IO::low(mosi);
        IO::low(sck);
        IO::high(cs);

        IO::in(miso);
        IO::out(mosi);
        IO::out(sck);
        IO::out(cs);

    }
    else
    {

        if( ( hardwareCS != UNSELECTED_HARDWARE_CS ) && ( cs != IO::UNUSED) )
        {
            IO::use(cs);
            IO::high(cs);
            IO::out(cs);
        }

        _init();

    }

}

/**
 * @brief Передать(сдвинуть регистр) один байт
 *
 * @param data передаваемый байт данных
 *
 * @return возвращаемый байт данных
 *
 */

uint8_t SPI8::transfer(uint8_t data)
{

    if( mode == SOFTWARE_MODE )
    {

        uint8_t outData = 0;

        for( uint8_t i = 0 ; i < 8 ; i++ )
        {

            if( data & 0x80 ) { IO::high( mosi ); } else { IO::low( mosi ); }

            data <<= 1;

            IO::low(sck);

            outData <<= 1;

            if( IO::read(miso) ) { outData |= 0x01; }

            IO::high(sck);

        }

        return outData;

    }
    else
    {
        return _transfer(data);
    }

    return 0;

}

/**
 * @brief read
 * @param buffer
 * @param size
 * @return
 */

int SPI8::read(void* buffer, uint32_t size)
{

    if( mode == SOFTWARE_MODE )
    {
        for(uint32_t i = 0 ; i < size; i++)
        {
            ((uint8_t*)buffer)[i] = SPI8::transfer(0x00);
        }
    }
    else
    {
        return _read(buffer, size);
    }

    return 0;

}

/**
 * @brief write
 * @param buffer
 * @param size
 * @return
 */

int SPI8::write( void* buffer, uint32_t size )
{

    if( mode == SOFTWARE_MODE )
    {
        for(uint32_t i = 0 ; i < size; i++)
        {
            SPI8::transfer(((uint8_t*)buffer)[i]);
        }
    }
    else
    {
        return _write(buffer, size);
    }

    return 0;

}

/**
 * @brief SPI8::enable
 */

void SPI8::enable()
{
    if( ( hardwareCS != UNSELECTED_HARDWARE_CS ) && ( cs != IO::UNUSED) )
    {
        IO::low(cs);
    }
}

/**
 * @brief SPI8::disable
 */

void SPI8::disable()
{
    if( ( hardwareCS != UNSELECTED_HARDWARE_CS ) && ( cs != IO::UNUSED) )
    {
        IO::high(cs);
    }
}

/*
 *
 *     int read(void* buffer, uint32_t size);
    uint32_t read();
    uint8_t* pBuffer = (uint8_t*)buffer;

    for( uint32_t i = 0 ; i < bytesForRead; i++ )
    {
        *pBuffer = spi->transfer(0x00);
        pBuffer++;
    }
    */
