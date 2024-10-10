#ifndef TEST_H
#define TEST_H

#include "abstractmr25h40.hpp"

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

int test_memory(AbstractMR25H40* memory);

#endif
