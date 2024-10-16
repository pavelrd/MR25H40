#ifndef TESTS_H
#define TESTS_H

#include "tests.hpp"
#include "checksum.hpp"

#include <iostream>
#include <ctime>
#include <cstring>

#include "io.hpp"
#include "spi.hpp"

#include "mr25h40.hpp"
#include "testmr25h40.hpp"

#include "abstractmr25h40.hpp"

#include <googletest/googletest/include/gtest/gtest.h>

#define TEST_REPORT_FILENAME "test_report.txt"

#define DUMP_FILENAME "" // "mr25h40_test_dump.bin"

#define MIN_WRITE_BLOCK_IN_TEST   1
#define MAX_WRITE_BLOCK_IN_TEST 128
#define BLOCK_STEP_IN_TEST        1

#define RANDOM_TABLE_SIZE 16

#define BLOCK_SIZE MAX_WRITE_BLOCK_IN_TEST

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

int memory_test_random_access_fill(AbstractMR25H40* memory, int minSize, int maxSize );
int test_memory_fill( AbstractMR25H40* memory, uint32_t size, bool randomNumbers=false, bool reverse=false );
int test_memory_fill_structures(AbstractMR25H40* memory, bool randomNumbers );

#endif
