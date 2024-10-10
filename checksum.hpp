#ifndef CHECKSUM_HPP
#define CHECKSUM_HPP

#include <stdint.h>

typedef uint32_t checksumType;

checksumType getChecksum( uint32_t crc, void* buffer, uint32_t len );

#endif
