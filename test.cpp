#include "test.hpp"
#include "checksum.hpp"

#include <iostream>

struct Bureau
{
#if defined(PC_PLATFORM)
    uint32_t prog_qty; // Размер задается в зависимости от разрядности целевого контроллера.
                       //  иначе при тестировании на ПК эта переменная будет размером 64 бита, а на
                       //  целевом контроллере размер будет другой, например 32 бита.
#else
    size_t   prog_qty;
#endif
    uint32_t math_qty;
    uint8_t  head_qty;
    float    salary_sum;
};

int test_memory_fill_uint32_numbers(AbstractMR25H40* memory)
{

    uint32_t counter = 0;

    for(uint32_t i = 0 ; i < AbstractMR25H40::MEMORY_SIZE_IN_BYTES; i+=4)
    {
        if( 0 > memory->write( &counter, 4 , i ) )
        {
            return -1;
        }
        counter++;
    }

    counter = 0;

    for(uint32_t i = 0 ; i < AbstractMR25H40::MEMORY_SIZE_IN_BYTES; i+=4)
    {
        uint32_t readCounter = 0;
        if( 0 > memory->read( &readCounter, 4 , i ) )
        {
            return -1;
        }
        if( counter != readCounter )
        {
            return -1;
        }
        counter++;
    }

    return 0;

}

/**
 * @brief test_memory
 * @param memory
 * @return
 */

int test_memory_fill_structures(AbstractMR25H40* memory)
{

    uint32_t writedStructs = 0;

    while(1)
    {

        Bureau bure = { writedStructs, writedStructs + 1, (uint8_t) ( writedStructs + 2 ), (float) ( writedStructs + 3 ) };

        uint32_t currentWritePosition = writedStructs * (sizeof(Bureau) + sizeof(checksumType));

        if ( 0 > memory->write( &bure, sizeof(Bureau), currentWritePosition ) )
        {
            break;
        }

        checksumType checksum = getChecksum( 0, &bure, sizeof(Bureau) );

        if ( 0 > memory->write( &checksum, sizeof(checksumType), currentWritePosition + sizeof(Bureau)) )
        {
            break;
        }

        writedStructs += 1;

    }

    std::cout << "Write completed. Press enter for continue " << std::endl;

    char a;

    std::cin >> a;

    for( uint32_t i = 0 ; i < writedStructs; i++ )
    {

        Bureau bure = { 0, 0, 0, 0 };

        if( 0 > memory->read( &bure, sizeof(Bureau), 0 ) )
        {
            return -1;
        }

        checksumType checksum = 0;

        if ( 0 > memory->read( &checksum, sizeof(checksumType), 4 ) )
        {
            return -1;
        }

        if( getChecksum( 0, &bure, sizeof(Bureau) ) != checksum )
        {

        }

    }

    return 0;

}
