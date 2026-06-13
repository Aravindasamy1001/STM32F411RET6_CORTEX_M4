#include <stdint.h>

#define RCC_AHB1ENR (*(volatile uint32_t*)0x40023830)

#define GPIOA_MODER (*(volatile uint32_t*)0x40020000)
#define GPIOA_ODR   (*(volatile uint32_t*)0x40020014)

#define STK_CTRL    (*(volatile uint32_t*)0xE000E010)
#define STK_LOAD    (*(volatile uint32_t*)0xE000E014)
#define STK_VAL     (*(volatile uint32_t*)0xE000E018)

void GPIO_INIT(void)
{
    RCC_AHB1ENR |= (1<<0);

    GPIOA_MODER &= ~(3<<10);
    GPIOA_MODER |=  (1<<10);
}

void delay_ms(uint32_t ms)
{
    for(uint32_t i=0; i<ms; i++)
    {
        STK_LOAD = 16000 - 1;     // 1 ms at 16 MHz
        STK_VAL  = 0;             // clear current count

        STK_CTRL = (1<<0) | (1<<2); // ENABLE + CPU CLOCK

        while(!(STK_CTRL & (1<<16)));
    }
}

int main(void)
{
    GPIO_INIT();

    while(1)
    {
        GPIOA_ODR ^= (1<<5);

        delay_ms(1000);
    }
}
