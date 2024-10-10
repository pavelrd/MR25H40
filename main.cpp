#include <iostream>

#include "io.hpp"
#include "spi.hpp"

#include "mr25h40.hpp"
#include "testmr25h40.hpp"

#include "test.hpp"

int main()
{

    std::cout << "Memory test started!" << std::endl;

    SPI8 spi( IO::D1, IO::D2, IO::D3, IO::D4 );

    // SPI spi(SPI::HARDWARE_MODE, SPI::HARDWARE_CS0);

    // SPI spi(SPI::HARDWARE_MODE, IO::D4);

    testMR25H40 memory; // &spi

    if( 0 == test_memory(&memory) )
    {
        std::cout << "Memory test completed success" << std::endl;
    }
    else
    {
        std::cout << "Memory test FAILED!" << std::endl;
    }

    MR25H40 memory2( &spi, IO::D5, IO::D6 ); // &spi

    if( 0 == test_memory(&memory2) )
    {
        std::cout << "Memory test completed success" << std::endl;
    }
    else
    {
        std::cout << "Memory test FAILED!" << std::endl;
    }

    return 0;

}

