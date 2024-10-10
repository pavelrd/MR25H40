#include "io.hpp"

#include <iostream>

void IO::use(PIN pin)
{
    std::cout << "IO::use(" << (int) pin << ")" << std::endl;
}

void IO::unuse(PIN pin)
{
    std::cout << "IO::unuse(" << (int) pin << ")" << std::endl;
}

void IO::high(PIN pin)
{
    std::cout << "IO::high(" << (int) pin << ")" << std::endl;
}

void IO::low(PIN pin)
{
    std::cout << "IO::low(" << (int) pin << ")" << std::endl;
}

bool IO::read(PIN pin)
{
    std::cout << "IO::read(" << (int) pin << ")" << std::endl;
    return 0;
}

void IO::in(PIN pin)
{
    std::cout << "IO::in(" << (int) pin << ")" << std::endl;
}

void IO::out(PIN pin)
{
    std::cout << "IO::out(" << (int) pin << ")" << std::endl;
}

/*
IO::PIN_DESCRIPTION IO::dpinToHardwarePin[1] =
{
    { 0, 0, 0 }
// Не используется под ПК
};
*/
