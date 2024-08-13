#include "rcc.hpp"
#include "systick.hpp"
#include "gpio.hpp"
#include "serial.hpp"
#include "tim.hpp"
#include "soft_i2c.hpp"
#include "crc.hpp"

#include "main.h"
#include "clock.h"
#include <stdio.h>
#include "oled.h"

#include "formatter/utils.hpp"

#include "serial.hpp"

extern "C" int __io_putchar(int ch)
{
    while (!serial1.flag.txe.is_set());
    serial1.write_dr(ch);
    while (!serial1.flag.tc.is_set());
    return ch;
}

uint32_t buffer[256];
uint8_t receive[10];

int main(void)
{
    clock_setup(RCC_CFGR_PLLMULL9);
    Rcc::update_clocks();
    syst.reload(Rcc::get_clocks().sysclk / 1000);
    syst.clear_count();
    syst.interrupt.tickint.enable();
    syst.count(true);
    MODIFY_REG(AFIO->MAPR, AFIO_MAPR_SWJ_CFG, AFIO_MAPR_SWJ_CFG_JTAGDISABLE);

    pa<>.clock(true);
    pa<9>.into_output().alternate_push_pull();
    pa<10>.into_input().floating();
    serial1.clock(true);
    serial1.defult();
    serial1.request_transmitter_from_dma(true);
    serial1.enable();
    serial1.tx_dma_channel.clock(true);
    serial1.tx_dma_channel.memory_size_8bits();
    serial1.tx_dma_channel.periph_size_8bits();
    serial1.tx_dma_channel.memory_increment(true);
    serial1.tx_dma_channel.periph_increment(false);
    serial1.tx_dma_channel.circular(false);
    serial1.tx_dma_channel.data_from_memory();
    serial1.tx_dma_channel.memory_address(buffer);
    serial1.tx_dma_channel.periph_address(&serial1.instance->DR);
    crc.clock(true);

    for (uint32_t i = 0; i < 256;)
    {
        buffer[i] = crc.calculate((uint32_t *)&i, 1);
        serial1.println("{} {}", i, buffer[i]);
        while (!serial1.flag.rxne.is_set());
        if (serial1.read_dr() == '\n')
        {
            i++;
        }
    }

    while (true)
    {
        // while (!serial1.flag.idle.is_set());
        // serial1.flag.idle.clear();
        // serial1.tx_dma_channel.disable();
        // uint32_t value = crc.calculate((uint32_t *)buffer, (sizeof(buffer) - serial1.tx_dma_channel.get_length()) / sizeof(uint32_t));
    }
}
