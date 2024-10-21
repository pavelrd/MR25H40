#include "tests.hpp"
#include <random>

struct randTest
{
    uint32_t addr;
    uint16_t len;
};

static randTest randTestTable[RANDOM_TABLE_SIZE] = {0};

uint8_t block[BLOCK_SIZE] = {0};

/**
 *
 * @todo Пока еще проходит не всегда, требует доработки
 *
 * @brief Тестирование произвольного доступа к памяти
 *         Заполняет RANDOM_TABLE_SIZE ячеек в памяти в произвольных местах
 *
 * @param memory
 * @param size
 *
 */

int memory_test_random_access_fill( AbstractMR25H40* memory, int minSize, int maxSize )
{

    std::random_device seed;
    std::mt19937 gen{seed()}; // seed the generator
    std::uniform_int_distribution<> dist{minSize, maxSize}; // set min and max

    // Заполнение randTestTable

    for( randTest& element : randTestTable )
    {

        element.addr = std::rand() % AbstractMR25H40::MEMORY_SIZE_IN_BYTES;
        element.len = dist(gen);

    }

    uint8_t counter = 0;

    for( randTest& element : randTestTable )
    {

        for( uint32_t i = 0; i < element.len; i++ )
        {
            memory->fill(counter, element.addr + i, 1);
            counter += 1;
        }

    }

    counter = 0;

    // -------

    for( randTest& element : randTestTable )
    {

        for( uint32_t i = 0; i < element.len; i++ )
        {

            uint8_t readValue = 0;

            memory->read(&readValue, 1, element.addr+i);

            if( readValue != counter )
            {
                return -1;
            }

            counter += 1;

        }

    }

    return 0;

}

/**
 *
 * @brief Заполняет память элементами указанной длины
 *         значения в элементах перечисляются последовательно
 *         пока не случается переполнения, затем снова.
 *         После полного заполнения считывает память и проверяет было
 *         ли прочитано, то что ранее записано.
 *
 *         Если size == 1 то: 00 01 02 03 FF ...
 *
 *         Если size 2 то: 00 00 01 01 02 02 ... FF FF
 *           и.т.д
 *
 * @param memory        - память
 * @param size          - размер кусков которыми будет заполняться память, максиум BLOCK_SIZE
 * @param randomNumbers - true - заполнять случайными числами, false - заполнять последовательными числами
 *
 * @return 0 - тест пройден успешно, иначе -1
 *
 */

int test_memory_fill( AbstractMR25H40* memory, uint32_t size, bool randomNumbers, bool reverse )
{

    time_t timestamp = 0;

    if( randomNumbers )
    {
        timestamp = std::time(nullptr);
        std::srand(timestamp);
    }

    uint8_t counter = 0;

    uint32_t addressMaximum = (( AbstractMR25H40::MEMORY_SIZE_IN_BYTES / size ) * size);

    for( uint32_t addressCounter = 0 ; addressCounter < addressMaximum; addressCounter+= size )
    {

        if(randomNumbers)
        {
            for( uint32_t blockCounter = 0 ; blockCounter < size; blockCounter++ )
            {
                // Не оптимально, так как каждый раз теряется по 3 случайных байта!
                block[blockCounter] = std::rand() % 256;
            }
        }
        else
        {
            for( uint32_t blockCounter = 0 ; blockCounter < size; blockCounter++ )
            {
                block[blockCounter] = counter;
            }
        }

        if( reverse )
        {
            if( 0 > memory->write( block, size , AbstractMR25H40::MEMORY_SIZE_IN_BYTES - addressCounter - size) )
            {
                return -1;
            }
        }
        else
        {
            if( 0 > memory->write( block, size , addressCounter ) )
            {
                return -1;
            }
        }

        counter++;

    }

    counter = 0;

    if( randomNumbers )
    {
        std::srand(timestamp);
    }

    std::memset(block, 0, size);

    for( uint32_t addressCounter = 0 ; addressCounter < addressMaximum; addressCounter+= size )
    {

        if( reverse )
        {
            if( (int)size != memory->read( block, size , AbstractMR25H40::MEMORY_SIZE_IN_BYTES - addressCounter - size ) )
            {
                return -1;
            }
        }
        else
        {
            if( (int)size != memory->read( block, size , addressCounter ) )
            {
                return -1;
            }
        }

        if(randomNumbers)
        {
            for( uint32_t blockCounter = 0 ; blockCounter < size; blockCounter++ )
            {
                if( block[blockCounter] != std::rand() % 256 )
                {
                    return -1;
                }
            }
        }
        else
        {
            for( uint32_t blockCounter = 0 ; blockCounter < size; blockCounter++ )
            {
                if ( block[blockCounter] != counter )
                {
                    return -1;
                }
            }
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

    if( randomNumbers )
    {
        std::srand(timestamp);
    }

    uint32_t currentStructIndex = 0;

    Bureau bure = { 0, 0, 0, 0 };

    for(  ; currentStructIndex < writedStructs; currentStructIndex++ )
    {

        bure.head_qty = 0;
        bure.math_qty = 0;
        bure.prog_qty = 0;
        bure.salary_sum = 0;

        uint32_t currentReadPosition = currentStructIndex * (sizeof(Bureau) + sizeof(checksumType));

        if( sizeof(Bureau) != memory->read( &bure, sizeof(Bureau), currentReadPosition ) )
        {
            return -1;
        }

        checksumType checksum = 0;

        if ( sizeof(checksumType) != memory->read( &checksum, sizeof(checksumType), currentReadPosition + sizeof(Bureau) ) )
        {
            return -1;
        }

        if( getChecksum( 0, &bure, sizeof(Bureau) ) != checksum )
        {
            return -1;
        }
        else if( randomNumbers )
        {

            if ( bure.head_qty               != std::rand()  % 256     ) return -1;
            if ( bure.math_qty               != (uint32_t) std::rand() ) return -1;
            if ( bure.prog_qty               != (uint32_t) std::rand() ) return -1;
            if ( ((uint32_t)bure.salary_sum) != (uint32_t)std::rand()  ) return -1;

        }
        else
        {

            if ( bure.prog_qty   != currentStructIndex                 ) return -1;
            if ( bure.math_qty   != currentStructIndex + 1             ) return -1;
            if ( bure.head_qty   != ( currentStructIndex + 2 ) % 256   ) return -1;
            if ( ((uint32_t)bure.salary_sum) != currentStructIndex + 3 )
            {
                return -1; /// @todo fabs... > EPSILON
            }

        }

    }

    return 0;

}
