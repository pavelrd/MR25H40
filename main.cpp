#include <iostream>

#include "io.hpp"
#include "spi.hpp"

#include "mr25h40.hpp"

#include "tests.hpp"

/// Чтобы выбрать отдельные тесты передать следующий параметр
///  при запуске программы
///
/// --gtest_filter=[fill_numbers*|fill_structs*|random_access*|control_methods*]
///
///  Пример:
///
///  --gtest_filter=fill_structs*
///

int main(int argc, char **argv)
{

    ::testing::InitGoogleTest(&argc, argv);

    if ( 0 == RUN_ALL_TESTS() )
    {
        std::cout << "ALL test completed!" << std::endl;
    }
    else
    {
        std::cout << "Tests FAILED" << std::endl;
    }

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

