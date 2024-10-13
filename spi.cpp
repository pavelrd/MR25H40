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
            IO::high(cs);
            IO::out(cs);
        }

        _init();

    }

    isInitialized = true;

}

/**
 *
 * @brief Деконфигурация ножек, настроенных ранее методом init()
 *
 */

void SPI8::deinit()
{

    if( mode == SOFTWARE_MODE )
    {

        IO::in(mosi);
        IO::in(sck);
        IO::in(cs);

        IO::unuse(miso);
        IO::unuse(mosi);
        IO::unuse(sck);
        IO::unuse(cs);

    }
    else
    {
        _deinit();
    }

    isInitialized = false;

}

/**
 * @brief Передать и принять один байт
 * @param data байт для передачи
 * @return принятый байт
*/

int SPI8::transfer( uint8_t data, uint8_t* out )
{

    if( !isInitialized )
    {
        return -1;
    }

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

        if( out != 0 )
        {
            *out = outData;
        }

        return 0;

    }
    else
    {
        return _transfer(data, out);
    }

    return 0;

}

/**
 *
 * @brief Прочитать данные с spi
 *
 * @param buffer данные
 * @param size размер данных для чтения в байтах
 *
 * @return 0 - данные прочитаны успешно, иначе -1
 *
 */

int SPI8::read(void* buffer, uint32_t size)
{

    if( !isInitialized )
    {
        return -1;
    }

    if( mode == SOFTWARE_MODE )
    {
        for(uint32_t i = 0 ; i < size; i++)
        {
             if( -1 == SPI8::transfer( 0x00, ((uint8_t*)buffer) + i ) )
             {
                 return -1;
             }
        }
    }
    else
    {
        return _read(buffer, size);
    }

    return 0;

}

/**
 *
 * @brief Записать данные по spi
 *
 * @param buffer данные
 * @param size размер данных для записи в байтах
 *
 * @return 0 - данные записаны успешно, иначе -1
 *
 */

int SPI8::write( void* buffer, uint32_t size )
{

    if( !isInitialized )
    {
        return -1;
    }

    if( mode == SOFTWARE_MODE )
    {
        for(uint32_t i = 0 ; i < size; i++)
        {
            if( -1 == SPI8::transfer( ((uint8_t*)buffer)[i], 0 ) )
            {
                return -1;
            }
        }
    }
    else
    {
        return _write(buffer, size);
    }

    return 0;

}

/**
 *
 * @brief Включение чипа, вызывается перед transfer, read, write
 *
 */

void SPI8::enable()
{
    if( ( hardwareCS != UNSELECTED_HARDWARE_CS ) && ( cs != IO::UNUSED) )
    {
        IO::low(cs);
    }
}

/**
 *
 * @brief Выключеине чипа, вызывается после transfer, read, write
 *
 */

void SPI8::disable()
{
    if( ( hardwareCS != UNSELECTED_HARDWARE_CS ) && ( cs != IO::UNUSED) )
    {
        IO::high(cs);
    }
}

/**
 *
 * @brief Выбран(включен ли сейчас чип)
 *
 * @return true - выбран(линия cs в низком уровне), false - не выбран(линия cs в высоком уровне)
 *
 */

bool SPI8::isEnabled()
{
    return IO::read(cs);
}
