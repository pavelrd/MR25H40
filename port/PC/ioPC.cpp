#include "io.hpp"

#include <iostream>

/**
 * @brief Заглушка
 * @param pin
 */

void IO::use(PIN pin)
{
    std::cout << "IO::use(" << (int) pin << ")" << std::endl;
    usedPins[pin] = true;
}

/**
 * @brief Заглушка
 * @param pin
 */

void IO::unuse(PIN pin)
{
    std::cout << "IO::unuse(" << (int) pin << ")" << std::endl;
    usedPins[pin] = false;
}

/**
 * @brief Заглушка
 * @param pin
 */

void IO::high(PIN pin)
{
    if( usedPins[pin] )
    {
        std::cout << "IO::high(" << (int) pin << ")" << std::endl;
    }
    else
    {
        std::cout << "IO::high(" << (int) pin << ") FAILED! reason: pin unused." << std::endl;
    }
}

/**
 * @brief Заглушка
 * @param pin
 */

void IO::low(PIN pin)
{
    if( usedPins[pin] )
    {
        std::cout << "IO::low(" << (int) pin << ")" << std::endl;
    }
    else
    {
        std::cout << "IO::low(" << (int) pin << ") FAILED! reason: pin unused." << std::endl;
    }
}

/**
 * @brief Заглушка
 * @param pin
 * @return
 */

bool IO::read(PIN pin)
{
    if( usedPins[pin] )
    {
        std::cout << "IO::read(" << (int) pin << ")" << std::endl;
    }
    else
    {
        std::cout << "IO::read(" << (int) pin << ") FAILED! reason: pin unused." << std::endl;
    }
    return 0;
}

/**
 * @brief Заглушка
 * @param pin
 */

void IO::in(PIN pin)
{
    if( usedPins[pin] )
    {
        std::cout << "IO::in(" << (int) pin << ")" << std::endl;
    }
    else
    {
        std::cout << "IO::in(" << (int) pin << ") FAILED! reason: pin unused." << std::endl;
    }
}

/**
 * @brief Заглушка
 * @param pin
 */

void IO::out(PIN pin)
{
    if( usedPins[pin] )
    {
        std::cout << "IO::out(" << (int) pin << ")" << std::endl;
    }
    else
    {
        std::cout << "IO::out(" << (int) pin << ") FAILED! reason: pin unused." << std::endl;
    }
}

bool IO::usedPins[255] = {0};

const IO::PIN_DESCRIPTION IO::dpinToHardwarePin[255] = {0};
