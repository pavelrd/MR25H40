#include "tests.hpp"

/**
 * @brief Заполнение памяти последовательно с начала до конца в цикле
 *         сначала блоками MIN_WRITE_BLOCK_IN_TEST затем с шагом BLOCK_STEP_IN_TEST до MAX_WRITE_BLOCK_LENGTH_IN_TEST
 *         числа в блоках повторяются и идут последовательно
 */

TEST( fill_numbers, sequence_straight )
{

    testMR25H40 memory(DUMP_FILENAME); // ""

    memory.init();

    memory.writeEnable();

    for( uint32_t i = MIN_WRITE_BLOCK_IN_TEST; i < MAX_WRITE_BLOCK_LENGTH_IN_TEST ; i += BLOCK_STEP_IN_TEST )
    {

        ASSERT_EQ( 0, test_memory_fill(&memory, i ) );

    }

}

/**
 * @brief Заполнение памяти последовательно с конца до начала в цикле
 *         сначала блоками MIN_WRITE_BLOCK_IN_TEST затем с шагом BLOCK_STEP_IN_TEST до MAX_WRITE_BLOCK_LENGTH_IN_TEST
 *         числа в блоках повторяются и идут последовательно
 */

TEST( fill_numbers, sequence_reverse )
{

    testMR25H40 memory(DUMP_FILENAME); // ""

    memory.init();

    memory.writeEnable();

    for( uint32_t i = MIN_WRITE_BLOCK_IN_TEST; i < MAX_WRITE_BLOCK_LENGTH_IN_TEST ; i += BLOCK_STEP_IN_TEST )
    {

        ASSERT_EQ( 0, test_memory_fill(&memory, i, false, true ) );

    }

}

/**
 * @brief Заполнение памяти последовательно с начала до конца в цикле
 *         сначала блоками MIN_WRITE_BLOCK_IN_TEST затем с шагом BLOCK_STEP_IN_TEST до MAX_WRITE_BLOCK_LENGTH_IN_TEST
 *         числа в блоках псевдослучайны
 */

TEST( fill_numbers, random_straight )
{

    testMR25H40 memory(DUMP_FILENAME); // ""

    memory.init();

    memory.writeEnable();

    for( uint32_t i = MIN_WRITE_BLOCK_IN_TEST; i < MAX_WRITE_BLOCK_LENGTH_IN_TEST ; i += BLOCK_STEP_IN_TEST )
    {

        ASSERT_EQ( 0, test_memory_fill(&memory, i, true, false ) );

    }

}

/**
 * @brief Заполнение памяти последовательно с конца до начала в цикле
 *         сначала блоками MIN_WRITE_BLOCK_IN_TEST затем с шагом BLOCK_STEP_IN_TEST до MAX_WRITE_BLOCK_LENGTH_IN_TEST
 *         числа в блоках псевдослучайны
 */

TEST( fill_numbers, random_reverse )
{

    testMR25H40 memory(DUMP_FILENAME); // ""

    memory.init();

    memory.writeEnable();

    for( uint32_t i = MIN_WRITE_BLOCK_IN_TEST; i < MAX_WRITE_BLOCK_LENGTH_IN_TEST ; i += BLOCK_STEP_IN_TEST )
    {

        ASSERT_EQ( 0, test_memory_fill(&memory, i, true, true ) );

    }

}

/**
 * @brief Заполнение памяти структурами Bureau с контрольными суммами.
 *         Поля структуры заполняются числами, которые идут последовательно
 */

TEST( fill_structs, sequence_numbers )
{

    testMR25H40 memory(DUMP_FILENAME); // ""

    memory.init();

    memory.writeEnable();

    ASSERT_EQ( 0, test_memory_fill_structures(&memory, false ) );

}


/**
 * @brief Заполнение памяти структурами Bureau с контрольными суммами.
 *         Поля структур заполняются псевдослучайными числами
 *
 */

TEST( fill_structs, random_numbers )
{

    testMR25H40 memory(DUMP_FILENAME);

    memory.init();

    memory.writeEnable();

    ASSERT_EQ( 0, test_memory_fill_structures(&memory, true ) );

}

/**
 * @brief Заполнение памяти блоками произвольного размера от MIN_WRITE_BLOCK_IN_TEST до MAX_WRITE_BLOCK_IN_TEST
 *         Блоки заполняются псевдослучайными числами и размещаются в случайном порядке
 */

TEST( random_access, fill_sequence )
{

    testMR25H40 memory(DUMP_FILENAME);

    memory.init();

    memory.writeEnable();

    ASSERT_EQ( 0, memory_test_random_access_fill( memory, MIN_WRITE_BLOCK_IN_TEST, MAX_WRITE_BLOCK_IN_TEST ) );

}

/**
 * @brief Проверка установки режима защиты памяти
 */

TEST(control_methods, protect_set)
{

    SPI8 spi( IO::D1, IO::D2, IO::D3, IO::D4 );  //  SPI spi(SPI::HARDWARE_MODE, IO::D4); SPI spi(SPI::HARDWARE_MODE, SPI::HARDWARE_CS0);

    spi.init(1000000);

    AbstractMR25H40 memory2( &spi, IO::D5, IO::D6 ); // &spi

    memory2.init();

    memory2.writeEnable();

    AbstractMR25H40::PROTECT_MODES mode[] =
    {
            AbstractMR25H40::PROTECT_MODE_NONE,
            AbstractMR25H40::PROTECT_MODE_UPPER_QUARTER,
            AbstractMR25H40::PROTECT_MODE_UPPER_HALF,
            AbstractMR25H40::PROTECT_MODE_ALL
    };

    for( auto i = 0 ; i < 4; i++ )
    {

        memory2.setProtect(mode[i], false);

        AbstractMR25H40::PROTECT_MODES readMode = AbstractMR25H40::PROTECT_MODE_NONE;

        ASSERT_EQ( 0, memory2.protect(&readMode) );

        ASSERT_EQ( mode[i], readMode );

    }

    memory2.writeDisable();

}

/**
 * @brief Проверка работоспособности режима защиты памяти
 */

TEST(control_methods, protect_work)
{

}