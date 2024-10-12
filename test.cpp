#include "test.hpp"
#include "checksum.hpp"

#include <iostream>
#include <ctime>

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

struct randTest
{
    uint32_t addr;
    uint16_t len;
};

randTest randTestTable[32] = {0};

void rt( AbstractMR25H40* memory, uint32_t startSize, uint32_t endSize )
{

    //std::srand(timestamp);

    // Заполнение randTestTable

    for( ;; )
    {

        // i.addr = std::rand() % AbstractMR25H40::MEMORY_SIZE_IN_BYTES;

        uint32_t size = std::rand();

        if(size < startSize)
        {
            size += startSize;
        }

        if( size > endSize )
        {
            size %= endSize;
        }

        // i.len  = size;

    }

    // -------

    for( ;; )
    {

    }

}

/**
 *
 * @brief Заполняет память элементами указанной длины
 *         значения в элементах перечисляются последовательно
 *         пока не случается переполнения, затем снова.
 *         После полного заполнения считывает память и проверяет было
 *         ли прочитано, то что ранее записано.
 *
 *         Если startSize == endSize == 1 то: 00 01 02 03 FF ...
 *
 *         Если startSize == endSize == 2 то: 00 01 00 02 00 03 ... FF FF ....
 *           и.т.д
 *
 *         Если startSize != endSize, то память будет заполняться кусками
 *          случайного размера от начального до конечного, например:
 *
 * @param memory    - память
 * @param startSize - начальный размер кусков которыми будет заполняться память
 * @param endSize   - конечный размер кусков которыми будет заполняться память
 * @param randomNumbers - true - заполнять случайными числами, false - заполнять последовательными числами
 *
 * @return 0 - тест пройден успешно, иначе -1
 *
 */

int test_memory_fill(AbstractMR25H40* memory, uint32_t startSize, uint32_t endSize, bool randomNumbers )
{

    time_t timestamp = 0;

    if( randomNumbers )
    {
        timestamp = std::time(nullptr);
        std::srand(timestamp);
    }

    uint64_t counter = 0;

    for( auto i = 0 ; i < AbstractMR25H40::MEMORY_SIZE_IN_BYTES; i+= startSize )
    {

        if(randomNumbers)
        {
            counter = std::rand();
        }

        if( 0 > memory->write( &counter, startSize , i ) )
        {
            return -1;
        }

        counter++;

    }

    counter = 0;

    if( randomNumbers )
    {
        std::srand(timestamp);
    }

    for( auto i = 0 ; i < AbstractMR25H40::MEMORY_SIZE_IN_BYTES; i+= startSize )
    {

        uint64_t readCounter = 0;

        if( 0 > memory->read( &readCounter, startSize , i ) )
        {
            return -1;
        }

        if(randomNumbers)
        {
            counter = std::rand();
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
 * @brief Тестирует память, полностью заполняет её структурами типа Bureau
 *         также добавляет после каждой структуры контрольную сумму. Затем считывает
 *         память и проверяет контрольные суммы
 *
 * @param memory        - память
 * @param randomNumbers - true - заполнять структуры псевдослучайными числами
 *                        false - заполнять структуры последовательно идущими числами
 *
 * @return 0 - тест пройден успешно, -1 - ошибка
 *
 */


int test_memory_fill_structures(AbstractMR25H40* memory, bool randomNumbers )
{

    time_t timestamp = 0;

    if( randomNumbers )
    {
        timestamp = std::time(nullptr);
        std::srand(timestamp);
    }

    uint32_t writedStructs = 0;

    while(1)
    {

        Bureau bure = { writedStructs, writedStructs + 1, (uint8_t) ( writedStructs + 2 ), (float) ( writedStructs + 3 ) };

        if( randomNumbers )
        {
            bure.head_qty   = std::rand();
            bure.math_qty   = std::rand();
            bure.prog_qty   = std::rand();
            bure.salary_sum = std::rand();
        }

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

    std::srand(timestamp);

    for( uint32_t i = 0 ; i < writedStructs; i++ )
    {

        Bureau bure = { 0, 0, 0, 0 };

        uint32_t currentReadPosition = writedStructs * (sizeof(Bureau) + sizeof(checksumType));

        if( 0 > memory->read( &bure, sizeof(Bureau), currentReadPosition ) )
        {
            return -1;
        }

        checksumType checksum = 0;

        if ( 0 > memory->read( &checksum, sizeof(checksumType), currentReadPosition + sizeof(Bureau) ) )
        {
            return -1;
        }

        if( getChecksum( 0, &bure, sizeof(Bureau) ) != checksum )
        {
            return -1;
        }
        else if( randomNumbers )
        {

            if ( bure.head_qty   != std::rand() ) return -1;
            if ( bure.math_qty   != (uint32_t) std::rand() ) return -1;
            if ( bure.prog_qty   != (uint32_t) std::rand() ) return -1;
            // if ( bure.salary_sum != std::rand() ) return -1;

        }
        else
        {

            if ( bure.head_qty   != i     ) return -1;
            if ( bure.math_qty   != i + 1 ) return -1;
            if ( bure.prog_qty   != i + 2 ) return -1;
            // if ( bure.salary_sum != i + 3 ) return -1;

        }

    }

    return 0;

}
