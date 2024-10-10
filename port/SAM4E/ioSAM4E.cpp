#include "io.hpp"

#include "asf.h" // берется из состава atmel software framework
 // либо напрямую через регистры

IO::IO()
{

}

/**
 * @brief Найти порт соответствующий выбранной ножке и подать на него тактовую частоту
 * @param pin номер ножки
 */

void IO::use(PIN pin)
{
    uint8_t port = pinToHardware[pinNumber].port;

    if ( port == IOPORT_PIOA )
    {
        sysclk_enable_peripheral_clock(ID_PIOA); // либо SCB->
    }
    else if( port == IOPORT_PIOB )
    {
        sysclk_enable_peripheral_clock(ID_PIOB);
    }
    else if( port == IOPORT_PIOC )
    {
        sysclk_enable_peripheral_clock(ID_PIOC);
    }
    else if( port == IOPORT_PIOD )
    {
        sysclk_enable_peripheral_clock(ID_PIOD);
    }
    else if( port == IOPORT_PIOE )
    {
        sysclk_enable_peripheral_clock(ID_PIOE);
    }
}

/**
 * @brief Найти порт соответствующий выбранной ножке и отключить подачу тактовой частоты
 *         если не осталось больше используемых ножек на данном порту. Программа ведет учет используемых
 *         ножек в массиве. Когда используются аппаратные модули нужно обязательно вызывать use для
 *         ножек задействованных аппаратным модулем.
 * @param pin номер ножки
 */

void IO::unuse(PIN pin)
{

}

/**
 * @brief IO::high
 * @param pin
 */

void IO::high(PIN pin)
{

}

/**
 * @brief IO::low
 * @param pin
 */

void IO::low(PIN pin)
{

}

/**
 * @brief IO::read
 * @param pin
 */

void IO::read(PIN pin)
{

}

/**
 * @brief IO::in
 * @param pin
 */

void IO::in(PIN pin)
{

}

/**
 * @brief IO::out
 * @param pin
 */

void IO::out(PIN pin)
{

}

// Данная таблица уже была, взята из моего проекта. Также еще требуется таблица задействованных ножек для фукнции use/unuse.

IO::PIN_DESCRIPTION IO::dpinToHardwarePin[] =
{
    { 0, 0 }, // добавлен только для выравнивания. чтобы при обращении к элементу 1 выбирался действительно первый элемент а не нулевой
    { IOPORT_PIOD, 0 },  // 1
    { IOPORT_PIOD, 31 }, // 2
    { 0, 0 },            // 3  - VDDOUT
    { IOPORT_PIOE, 0 },  // 4
    { 0, 0 },            // 5  - VDDIN
    { IOPORT_PIOE, 1 },  // 6
    { IOPORT_PIOE, 2 },  // 7
    { 0, 0 },            // 8  - GND
    { 0, 0 },            // 9  - ADVREFP
    { IOPORT_PIOE,  3 }, // 10
    { IOPORT_PIOC,  0 }, // 11
    { IOPORT_PIOC, 27 }, // 12
    { IOPORT_PIOC, 26 }, // 13
    { IOPORT_PIOC, 31 }, // 14
    { IOPORT_PIOC, 30 }, // 15
    { IOPORT_PIOC, 29 }, // 16
    { IOPORT_PIOC, 12 }, // 17
    { IOPORT_PIOC, 15 }, // 18
    { IOPORT_PIOC, 13 }, // 19
    { IOPORT_PIOB,  1 }, // 20
    { IOPORT_PIOB,  0 }, // 21
    { IOPORT_PIOA, 20 }, // 22
    { IOPORT_PIOA, 19 }, // 23
    { IOPORT_PIOA, 18 }, // 24
    { IOPORT_PIOA, 17 }, // 25
    { IOPORT_PIOB,  2 }, // 26
    { IOPORT_PIOE,  4 }, // 27
    { IOPORT_PIOE,  5 }, // 28
    { 0, 0 },            // 29 - VDDCORE
    { 0, 0 },            // 30 - VDDIO
    { IOPORT_PIOB,   3 }, // 31
    { IOPORT_PIOA,  21 }, // 32
    { 0, 0 },            // 33 - VDDCORE
    { IOPORT_PIOD, 30 }, // 34
    { IOPORT_PIOA,  7 }, // 35
    { IOPORT_PIOA,  8 }, // 36
    { IOPORT_PIOA, 22 }, // 37
    { IOPORT_PIOC,  1 }, // 38
    { IOPORT_PIOC,  2 }, // 39
    { IOPORT_PIOC,  3 }, // 40
    { IOPORT_PIOC,  4 }, // 41
    { IOPORT_PIOA, 13 }, // 42
    { 0, 0 },            // 43 - VDDIO
    { 0, 0 },            // 44 - GND
    { IOPORT_PIOA, 16 }, // 45
    { IOPORT_PIOA, 23 }, // 46
    { IOPORT_PIOD, 27 }, // 47
    { IOPORT_PIOC,  7 }, // 48
    { IOPORT_PIOA, 15 }, // 49
    { 0, 0 },            // 50 - VDDCORE
    { IOPORT_PIOA, 14 }, // 51
    { IOPORT_PIOD, 25 }, // 52
    { IOPORT_PIOD, 26 }, // 53
    { IOPORT_PIOC,  6 }, // 54
    { IOPORT_PIOD, 24 }, // 55
    { IOPORT_PIOA, 24 }, // 56
    { IOPORT_PIOD, 23 }, // 57
    { IOPORT_PIOC,  5 }, // 58
    { IOPORT_PIOA, 25 }, // 59
    { IOPORT_PIOD, 22 }, // 60
    { 0, 0 },            // 61 - GND
    { IOPORT_PIOA, 26 }, // 62
    { IOPORT_PIOD, 21 }, // 63
    { IOPORT_PIOA, 11 }, // 64
    { IOPORT_PIOD, 20 }, // 65
    { IOPORT_PIOA, 10 }, // 66
    { IOPORT_PIOD, 19 }, // 67
    { IOPORT_PIOA, 12 }, // 68
    { IOPORT_PIOD, 18 }, // 69
    { IOPORT_PIOA, 27 }, // 70
    { IOPORT_PIOD, 28 }, // 71
    { 0, 0 },            // 72 - VDDIO
    { IOPORT_PIOA,  5 }, // 73
    { IOPORT_PIOD, 17 }, // 74
    { IOPORT_PIOA,  9 }, // 75
    { IOPORT_PIOC, 28 }, // 76
    { IOPORT_PIOA,  4 }, // 77
    { IOPORT_PIOD, 16 }, // 78
    { IOPORT_PIOB,  6 }, // 79
    { 0, 0 },            // 80 - VDDIO
    { 0, 0 },            // 81 - VDDCORE
    { IOPORT_PIOC,  8 }, // 82
    { 0, 0 },            // 83 - NRST
    { IOPORT_PIOD, 14 }, // 84
    { 0, 0 },            // 85 - TEST
    { IOPORT_PIOC,  9 }, // 86
    { IOPORT_PIOB, 12 }, // 87
    { IOPORT_PIOD, 13 }, // 88
    { IOPORT_PIOB,  7 }, // 89
    { IOPORT_PIOC, 10 }, // 90
    { IOPORT_PIOA,  3 }, // 91
    { IOPORT_PIOD, 12 }, // 92
    { IOPORT_PIOA,  2 }, // 93
    { IOPORT_PIOC, 11 }, // 94
    { 0, 0 },            // 95 - GND
    { 0, 0 },            // 96 - VDDIO
    { IOPORT_PIOC, 14 }, // 97
    { IOPORT_PIOD, 11 }, // 98
    { IOPORT_PIOA,  1 }, // 99
    { IOPORT_PIOC, 16 }, // 100
    { IOPORT_PIOD, 10 }, // 101
    { IOPORT_PIOA,  0 }, // 102
    { IOPORT_PIOC, 17 }, // 103
    { 0, 0 },            // 104 - JTAGSEL
    { IOPORT_PIOB,  4 }, // 105
    { IOPORT_PIOD, 15 }, // 106
    { 0, 0 },            // 107 - VDDCORE
    { IOPORT_PIOD, 29 }, // 108
    { IOPORT_PIOB,  5 }, // 109
    { IOPORT_PIOD,  9 }, // 110
    { IOPORT_PIOC, 18 }, // 111
    { IOPORT_PIOA, 28 }, // 112
    { IOPORT_PIOD,  8 }, // 113
    { IOPORT_PIOA,  6 }, // 114
    { 0, 0 },            // 115 - GND
    { IOPORT_PIOA, 30 }, // 116
    { IOPORT_PIOC, 19 }, // 117
    { IOPORT_PIOA, 31 }, // 118
    { IOPORT_PIOD,  7 }, // 119
    { IOPORT_PIOC, 20 }, // 120
    { IOPORT_PIOD,  6 }, // 121
    { IOPORT_PIOC, 21 }, // 122
    { 0, 0 },            // 123 - VDDCORE
    { IOPORT_PIOC, 22 }, // 124
    { IOPORT_PIOD,  5 }, // 125
    { IOPORT_PIOD,  4 }, // 126
    { IOPORT_PIOC, 23 }, // 127
    { IOPORT_PIOD,  3 }, // 128
    { IOPORT_PIOA, 29 }, // 129
    { IOPORT_PIOC, 24 }, // 130
    { IOPORT_PIOD,  2 }, // 131
    { IOPORT_PIOD,  1 }, // 132
    { IOPORT_PIOC, 25 }, // 133
    { 0, 0 },            // 134 - VDDIO
    { 0, 0 },            // 135 - GND
    { IOPORT_PIOB, 10 }, // 136
    { IOPORT_PIOB, 11 }, // 137
    { 0, 0 },            // 138 - GND
    { 0, 0 },            // 139 - VDDDPLL
    { IOPORT_PIOB, 14 }, // 140
    { IOPORT_PIOB,  8 }, // 141
    { IOPORT_PIOB,  9 }, // 142
    { 0, 0 },            // 143 - VDDIO
    { IOPORT_PIOB, 13 }  // 144
};
