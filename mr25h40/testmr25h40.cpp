#include "testmr25h40.hpp"

testMR25H40::testMR25H40(std::string dumpFilename)
{
    dumpFile.open(dumpFilename, std::ios::binary | std::ios::in | std::ios::out );
}

int testMR25H40::read( void* buffer, uint32_t numberOfBytes, uint32_t address )
{

    if( ( buffer == 0 ) || (address > MEMORY_SIZE_IN_BYTES) )
    {
        return -1;
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

    return 0;
}

void testMR25H40::setProtect(PROTECT_MODES mode)
{

}

void testMR25H40::sleep()
{

}

void testMR25H40::wake()
{

}

void testMR25H40::hold()
{

}

void testMR25H40::writeStatus()
{

}
