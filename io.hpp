#ifndef IO_H
#define IO_H

#include <stdint.h>

/**
 *
 * @brief Обеспечивает переносимое управление ножками в стиле arduino
 *
 */

class IO
{

public:

    IO();

    enum PIN
    {
        DO,
        D1,
        D2,
        D3,
        D4,
        D5,
        D6,
        D7,
        D8,
        D9,
        D10,
        // ...
        UNUSED = 1024
    };

    static void use(PIN pin);
    static void unuse(PIN pin);
    static void high(PIN pin);
    static void low(PIN pin);
    static bool read(PIN pin);

    static void in(PIN pin);
    static void out(PIN pin);

private:

    struct PIN_DESCRIPTION
    {
        uint32_t port;
        uint32_t pin;
        bool     used;
    };

    // static const PIN_DESCRIPTION dpinToHardwarePin[1];

};

#endif // IO_H