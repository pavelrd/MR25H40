#ifndef TEST_H
#define TEST_H

#include "abstractmr25h40.hpp"

void memory_test_random_access_fill(AbstractMR25H40* memory, uint32_t size = 4 );

int test_memory_fill( AbstractMR25H40* memory, uint32_t size = 4, bool randomNumbers = false , bool reverse = false );

int test_memory_fill_structures(AbstractMR25H40* memory, bool randomNumbers = false );

#endif
