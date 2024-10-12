#include "testmr25h40.hpp"

#include <iostream>
#include <cstring>

testMR25H40::testMR25H40()
{
    ram = new uint8_t[MEMORY_SIZE_IN_BYTES];
}

testMR25H40::~testMR25H40()
{
    if(ram)
    {
        delete [] ram;
    }
}

testMR25H40::testMR25H40(std::string dumpFilename)
{

    dumpFile.open(dumpFilename, std::ios::out | std::ios::in | std::ios::binary );

    if( !dumpFile.is_open() )
    {
        std::cout << "Error on open file!" << std::endl;
        std::cerr << "Error: " << strerror(errno);

    }

}

int testMR25H40::read( void* buffer, uint32_t numberOfBytes, uint32_t address )
{

    if( ( buffer == 0 ) || (address > MEMORY_SIZE_IN_BYTES) )
    {
        return -1;
    }

    if(ram)
    {
        for( uint32_t i = 0 ; i < numberOfBytes; i++ )
        {
            ((uint8_t*)buffer)[i] = ram[address+i];
        }
    }
    else
    {

        dumpFile.seekp(address);

        dumpFile.read((char*)buffer, numberOfBytes);

    }

    return 0;

}

int testMR25H40::write( void* buffer, uint32_t numberOfBytes, uint32_t address )
{

    if( ( buffer == 0 )                                        ||
        ( address > MEMORY_SIZE_IN_BYTES )                     ||
        ( ( address + numberOfBytes ) > MEMORY_SIZE_IN_BYTES )
       )
    {
        return -1;
    }

    if(ram)
    {

        for( uint32_t i = 0 ; i < numberOfBytes; i++ )
        {
            ram[address+i] = ((uint8_t*)buffer)[i];
        }

    }
    else
    {

        dumpFile.seekp(address, std::ios::beg);

        dumpFile.write((char*)buffer, numberOfBytes);

        dumpFile.flush();

    }

    return 0;

}

int testMR25H40::setProtect(PROTECT_MODES mode)
{
    currentProtectMode = mode;
    return 0;
}

int testMR25H40::sleep()
{
    isWake = false;
    return 0;
}

int testMR25H40::wake()
{
    isWake = true;
    return 0;
}

int testMR25H40::hold()
{
    return 0;
}

void testMR25H40::init()
{

}

void testMR25H40::deinit()
{

}
