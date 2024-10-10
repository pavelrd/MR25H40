#include "test.hpp"
#include "checksum.hpp"

#include <iostream>

/**
 * @brief test_memory
 * @param memory
 * @return
 */

int test_memory(AbstractMR25H40* memory)
{

    uint32_t writedStructs = 0;

    while(1)
    {

        Bureau bure = { writedStructs, writedStructs + 1, (uint8_t) ( writedStructs + 2 ), (float) ( writedStructs + 3 ) };

        uint32_t currentWritePosition = writedStructs * (sizeof(Bureau) + sizeof(checksumType));

        if ( 0 > memory->write( &bure, sizeof(Bureau), currentWritePosition ) )
        {
            return -1;
        }

        checksumType checksum = getChecksum( 0, &bure, sizeof(Bureau) );

        if ( 0 > memory->write( &checksum, sizeof(checksumType), currentWritePosition + sizeof(Bureau)) )
        {
            return -1;
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
