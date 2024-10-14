#include "testmr25h40.hpp"

#include <iostream>
#include <cstring>
#include <filesystem>

testMR25H40::~testMR25H40()
{
    if(ram)
    {
        delete [] ram;
    }
}

testMR25H40::testMR25H40(std::string dumpFilename)
{

    if( dumpFilename == "" )
    {
        ram = new uint8_t[MEMORY_SIZE_IN_BYTES];

        return;
    }

    if( !std::filesystem::exists(dumpFilename) )
    {

        dumpFile.open(dumpFilename, std::ios::out | std::ios::in | std::ios::binary | std::ios::app );

        dumpFile.close();

    }

    dumpFile.open(dumpFilename, std::ios::out | std::ios::in | std::ios::binary );

    if( !dumpFile.is_open() )
    {
        std::cout << "Error on open file!" << std::endl;
        std::cerr << "Error: " << strerror(errno);

    }

}

int testMR25H40::read( void* buffer, uint32_t numberOfBytes, uint32_t address )
{

    if( ( buffer == 0 ) || (address > MEMORY_SIZE_IN_BYTES) || (sleepMode) || (!isInitialized) )
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

        dumpFile.seekg(address);

        dumpFile.read((char*)buffer, numberOfBytes);

    }

    return numberOfBytes;

}

int testMR25H40::write( void* buffer, uint32_t numberOfBytes, uint32_t address )
{

    if( ( buffer == 0 )                                        ||
        ( address > MEMORY_SIZE_IN_BYTES )                     ||
        ( ( address + numberOfBytes ) > MEMORY_SIZE_IN_BYTES ) ||
        ( sleepMode )                                          ||
        ( !isInitialized )                                     ||
        ( !writeEnabled  )
       )
    {
        return -1;
    }

    if( currentProtectMode == PROTECT_MODE_ALL )
    {
        return -1;
    }

    if( (currentProtectMode == PROTECT_MODE_UPPER_QUARTER) && ( ( address + numberOfBytes ) >= ( MEMORY_SIZE_IN_BYTES - ( MEMORY_SIZE_IN_BYTES / 4 ) ) ) )
    {
        return -1;
    }

    if( ( currentProtectMode == PROTECT_MODE_UPPER_HALF ) && ( ( address + numberOfBytes ) >= (MEMORY_SIZE_IN_BYTES/2) ) )
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

int testMR25H40::fill(uint8_t value, uint32_t address, uint32_t numberOfBytes )
{

    if(
        ( address > MEMORY_SIZE_IN_BYTES )                     ||
        ( ( address + numberOfBytes ) > MEMORY_SIZE_IN_BYTES ) ||
        ( sleepMode )                                          ||
        ( !isInitialized )                                     ||
        ( !writeEnabled  )
       )
    {
        return -1;
    }

    if( currentProtectMode == PROTECT_MODE_ALL )
    {
        return -1;
    }

    if( (currentProtectMode == PROTECT_MODE_UPPER_QUARTER) && ( ( address + numberOfBytes ) >= ( MEMORY_SIZE_IN_BYTES - ( MEMORY_SIZE_IN_BYTES / 4 ) ) ) )
    {
        return -1;
    }

    if( ( currentProtectMode == PROTECT_MODE_UPPER_HALF ) && ( ( address + numberOfBytes ) >= (MEMORY_SIZE_IN_BYTES/2) ) )
    {
        return -1;
    }

    if(ram)
    {
        for( uint32_t i = 0 ; i < numberOfBytes; i++ )
        {
            ram[address+i] = value;
        }
    }
    else
    {

        uint8_t* buffer = new uint8_t[numberOfBytes];

        for(uint32_t i = 0; i < numberOfBytes;i++)
        {
            buffer[i] = value;
        }

        dumpFile.seekp(address);

        dumpFile.write((char*)buffer, numberOfBytes);

        dumpFile.flush();

        delete [] buffer;

    }

    return -1;

}

/**
 * @brief testMR25H40::protect
 * @param mode
 * @return
 */

int testMR25H40::protect( PROTECT_MODES* mode )
{
    *mode = currentProtectMode;
    return -1;
}

int testMR25H40::setProtect(PROTECT_MODES mode, bool srwd)
{
    if(!isInitialized)
    {
        return -1;
    }
    currentProtectMode = mode;
    return 0;
}

int testMR25H40::sleep()
{
    if(!isInitialized)
    {
        return -1;
    }
    sleepMode = false;
    return 0;
}

int testMR25H40::wake()
{
    if(!isInitialized)
    {
        return -1;
    }
    sleepMode = true;
    return 0;
}

int testMR25H40::hold()
{
    if( !isInitialized )
    {
        return -1;
    }
    return 0;
}

int testMR25H40::unhold()
{
    if( !isInitialized )
    {
        return -1;
    }
    return 0;
}

void testMR25H40::init()
{
    isInitialized = true;
}

void testMR25H40::deinit()
{
    isInitialized = false;
}

int testMR25H40::writeEnable()
{
    writeEnabled = true;
    return -1;
}

int testMR25H40::writeDisable()
{
    writeEnabled = false;
    return -1;
}
