#include "spi.hpp"

#include "asf.h" // берется из состава atmel software framework
                 //  программы atmel/microchip studio
#include "sam4e16c.h"

#include "FreeRTOS.h"
#include "semphr.h"

static SemaphoreHandle_t spiSemaphore;

/**
 *
 * @brief Настройка аппаратного модуля SPI
 *
 */

void SPI8::_init()
{

    // Настройка выводов, подача тактирования вручную, включение альтернативной функции

    // (Pio*)IOPORT_PIO[A-F]->PIO_OER = 1<<[0-32] | 1<<[0-32] | 1<<[0-32];

    // (Pio*)IOPORT_PIO[A-F]->PIO_PER = 1<<[0-32] | 1<<[0-32] | 1<<[0-32];

    // (Pio*)IOPORT_PIO[A-F]->PIO_ABCDSR |= ( 3<<[0-32] | 3<<[0-32] | 3<<[0-32] );
    //

    // SPI --- 0x40088000U адрес регистов контроллера SPI
    //  можно сделать напрямую задавая значения регистров
    //
    //   Spi *p_spi = (Spi*) SPI; (описание структуры в файле spi.h из состава ASF)
    //
    //   SPI->... = 0x....;

    spiSemaphore = xSemaphoreCreateBinary();

    NVIC_DisableIRQ(SPI_IRQn);
    NVIC_ClearPendingIRQ(SPI_IRQn);
    NVIC_SetPriority(SPI_IRQn, 0);
    NVIC_EnableIRQ(SPI_IRQn);

    spi_enable_clock(SPI);

    spi_disable(SPI);
    spi_reset(SPI);
    spi_set_lastxfer(SPI);
    spi_set_master_mode(SPI);
    spi_disable_mode_fault_detect(SPI);
    spi_set_peripheral_chip_select_value(SPI, SPI_CHIP_PCS);
    spi_set_clock_polarity(SPI, SPI_CHIP_SEL, 0);
    spi_set_clock_phase(SPI, SPI_CHIP_SEL, 0);
    spi_set_bits_per_transfer(SPI, SPI_CHIP_SEL, SPI_CSR_BITS_8_BIT);

    spi_set_baudrate_div(SPI, SPI_CHIP_SEL,(sysclk_get_peripheral_hz()/ gs_ul_spi_clock));
    spi_set_transfer_delay(SPI, SPI_CHIP_SEL, SPI_DLYBS, SPI_DLYBCT);

    spi_enable(SPI);


}

/**
 *
 * @brief Деконфигурация SPI
 *
 */

void SPI8::_deinit()
{

    spi_disable(SPI);
    spi_disable_clock(SPI);

    NVIC_DisableIRQ(SPI_IRQn);
    NVIC_ClearPendingIRQ(SPI_IRQn);

    // Выключение ножек, отключение альтернативной функции, переключение на вход
    // ...
    //

    // (Pio*)IOPORT_PIO[A-F]->PIO_ODR = 1<<[0-32] | 1<<[0-32] | 1<<[0-32];

    // (Pio*)IOPORT_PIO[A-F]->PIO_PDR = 1<<[0-32] | 1<<[0-32] | 1<<[0-32];

    // (Pio*)IOPORT_PIO[A-F]->PIO_ABCDSR &= ~( 3<<[0-32] | 3<<[0-32] | 3<<[0-32] );


}

/**
 *
 * Контроль передачи в методах transfer, read, write производится самым простым путем, путем опроса. Что ресурсозатратно.
 *
 * Менее плохой способ. Записывать байт в буфер отправки(или оправлять много байт через DMA) и
 *  затем в цикле опроса флага завершения передачи вызывать vTaskDelay(1), чтобы не тратить впустую время процессора.
 *
 * Хорошие способы:
 *
 *  Задействовать DMA контроллер. Передать ему буфер данных настроить его на передачу
 *  этого буфера по SPI, настроить чтобы по окончании передачи буфера было прерывание. Также нужно создать семафор,
 *  брать его перед началом передачи. Освобождение семафора сделать в обработчике прерывания.
 *
 *  Можно тоже самое но без DMA, отдельное прерывание для каждого байта.
 *
 */

/**
 * @brief Реализация под SAM4E передачи и приема одного байта
 * @param data байт для передачи
 * @return принятый байт
*/

int SPI8::_transfer(uint8_t data, uint8_t* out)
{

   if( SPI_OK != spi_write(SPI_MASTER_BASE, &data, 0, 0) )
   {
       return 0;
   }

   /* Wait transfer done. */

   // Вместо этого можно: xSemaphoreTake( spiSemaphore, portMAX_DELAY ); вместо portMAX_DELAY сделать таймаут и
   //                      если достигли таймаута сбросить контроллер SPI и запустить его заново, вернуть -1

   while ((spi_read_status(SPI_MASTER_BASE) & SPI_SR_RDRF) == 0);

   uint8_t outData = 0;

   if( SPI_OK == spi_read(SPI_MASTER_BASE, &outData, &uc_pcs) )
   {

       if ( out != 0 )
       {
           *out = outData;
       }
       return 0;
   }

   return 0;

}

/**
 *
 * @brief Реализация под SAM4E чтения данных с spi
 *
 * @param buffer данные
 * @param size размер данных для чтения в байтах
 *
 * @return 0 - данные прочитаны успешно, иначе -1
 *
 */

int SPI8::_read(void* buffer, uint32_t size)
{

    /// \todo Можно/нужно переделать на DMA

    uint8_t *p_buffer = buffer;

    for (uint32_t i = 0; i < size; i++)
    {
        spi_write(SPI_MASTER_BASE, p_buffer[i], 0, 0);
        /* Wait transfer done. */

        // xSemaphoreTake( spiSemaphore, portMAX_DELAY );
        while ((spi_read_status(SPI_MASTER_BASE) & SPI_SR_RDRF) == 0); // Вместо этого можно: xSemaphoreTake( spiSemaphore, portMAX_DELAY );

        uint8_t uc_pcs;

        uint16_t data;

        spi_read(SPI_MASTER_BASE, &data, &uc_pcs);

        p_buffer[i] = data;

    }

    return 0;

}

/**
 *
 * @brief Реализация под SAM4E записи данных по spi
 *
 * @param buffer данные
 * @param size размер данных для записи в байтах
 *
 * @return 0 - данные записаны успешно, иначе -1
 *
 */

int SPI8::_write( void* buffer, uint32_t size )
{

    /// \todo Можно/нужно переделать на DMA

    uint8_t *p_buffer = buffer;

    for (uint32_t i = 0; i < size; i++)
    {

        spi_write(SPI_MASTER_BASE, p_buffer[i], 0, 0);

        // xSemaphoreTake( spiSemaphore, portMAX_DELAY );
        while ((spi_read_status(SPI_MASTER_BASE) & SPI_SR_RDRF) == 0);

        uint8_t uc_pcs;

        uint16_t data;

        spi_read(SPI_MASTER_BASE, &data, &uc_pcs);

    }

    return 0;
}

/**
 *
 * @brief Обработчик прерываний SPI контроллера
 *
 */

extern "C" void SPI_Handler(void)
{

    // Проверяем флаги, отдаем семафор

    // ...

    xHigherPriorityTaskWoken = pdFALSE;

    if (spi_read_status(SPI_SLAVE_BASE) & SPI_SR_RDRF)
    {
        xSemaphoreGiveFromISR( spiSemaphore, &xHigherPriorityTaskWoken );
    }

}
