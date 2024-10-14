#include <iostream>

#include "io.hpp"
#include "spi.hpp"

#include "mr25h40.hpp"
#include "testmr25h40.hpp"

#include "test.hpp"


int main()
{

    std::cout << "Memory test started!" << std::endl;

    testMR25H40 memory("mr25h40_test_dump.bin"); // ""

    memory.init();

    memory.writeEnable();

    //memory.setProtect(AbstractMR25H40::PROTECT_MODE_UPPER_HALF);

// -------

    for( uint32_t i = 4; i < 16 ; i += 1 )
    {
        std::cout << std::endl << "\nStart memory test(fill uint32_t numbers)..." << std::endl;
        if( 0 == test_memory_fill(&memory, i ) )
        {
            std::cout << "Memory test(fill uint32_t numbers) completed success" << std::endl;
        }
        else
        {
            std::cout << "Memory test(fill uint32_t numbers) FAILED!!!" << std::endl;
        }
    }

// -------

    std::cout << std::endl << "Start memory test(fill structures)..." << std::endl;

    if( 0 == test_memory_fill_structures(&memory, false) )
    {
        std::cout << "Memory test(fill structures) completed success" << std::endl;
    }
    else
    {
        std::cout << "Memory test FAILED!!!" << std::endl;
    }

    std::cout << std::endl << "Start memory test(fill random numbers)..." << std::endl;

    if( 0 == test_memory_fill_structures(&memory, true) )
    {
        std::cout << "Memory test(fill random numbers) completed success" << std::endl;
    }
    else
    {
        std::cout << "Memory test FAILED!!!" << std::endl;
    }

    // void memory_test_random_access_fill( AbstractMR25H40* memory, uint32_t size )


    /* Для проверки на "железе"

    SPI8 spi( IO::D1, IO::D2, IO::D3, IO::D4 );  //  SPI spi(SPI::HARDWARE_MODE, IO::D4); SPI spi(SPI::HARDWARE_MODE, SPI::HARDWARE_CS0);

    spi->init(1000000);

    MR25H40 memory2( &spi, IO::D5, IO::D6 ); // &spi

    memory2.init();

    memory2.writeEnable();

    uint32_t numberOfSuccessTests    = 0;

    uint32_t numberOfRequiredTests   = 100000; // примерно 24 часа при скоростях SPI порядка десятков мегагерц

    std::cout << "Start tests" << std::endl;

    while(1)
    {

        if( 0 > test_memory_fill_uint32_numbers(&memory, 4, false) )
        {
            std::cout << "Memory test(fill uint32_t numbers) FAILED!!!" << std::endl;
            std::cout << "Success tests counter: " << numberOfSuccessTests << std::endl;
            while(1)
            {
                vTaskDelay(1000); // Обычно я использую обертку Thread::delay() которая под PC использует std::thread
            }
        }

        if( 0 > test_memory_fill_structures(&memory, false) )
        {
            std::cout << "Memory test(fill structures) FAILED!!!" << std::endl;
            std::cout << "Success tests counter: " << numberOfSuccessTests << std::endl;
            while(1)
            {
                vTaskDelay(1000);
            }
        }

        if( 0 > test_memory_fill_structures(&memory, true) )
        {
            std::cout << "Memory test FAILED!!!" << std::endl;
            std::cout << "Success tests counter: " << numberOfSuccessTests << std::endl;
            while(1)
            {
                vTaskDelay(1000);
            }
        }

        numberOfSuccessTests += 1;

        if( (numberOfSuccessTests % 10) == 0 ) // Чтобы сообщение показывалось примерно раз в 5-100 секунд, зависит от скорости SPI
        {
            std::cout << "Success tests counter: " << numberOfSuccessTests << std::endl;

            if( numberOfSuccessTests > numberOfRequiredTests )
            {
                break;
            }

        }

    }

    std::cout << "ALL tests completed!" << std::endl;

    while(1)
    {
        vTaskDelay(1000); // Обычно я использую обертку Thread::delay() которая под PC использует std::thread
    }


    */

    return 0;

}

