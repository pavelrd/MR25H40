#ifndef TEST_H
#define TEST_H

#include "abstractmr25h40.hpp"

int test_memory_fill(AbstractMR25H40* memory, uint32_t startSize = 4, uint32_t endSize = 4, bool randomNumbers = false );

int test_memory_fill_structures(AbstractMR25H40* memory, bool randomNumbers = false );

#endif
