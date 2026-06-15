#include<stdint.h>
#define RCC_AHB1ENR   (*(volatile uint32_t*)0x40023830)
#define RCC_APB1ENR   (*(volatile uint32_t*)0x40023840)

#define GPIOA_MODER   (*(volatile uint32_t*)0x40020000)
#define GPIOA_ODR     (*(volatile uint32_t*)0x40020014)

#define GPIOC_MODER   (*(volatile uint32_t*)0x40020800)
#define GPIOC_IDR     (*(volatile uint32_t*)0x40020810)

#define TIM2_CR1      (*(volatile uint32_t*)0x40000000)
#define TIM2_DIER     (*(volatile uint32_t*)0x4000000C)
#define TIM2_SR       (*(volatile uint32_t*)0x40000010)
#define TIM2_PSC      (*(volatile uint32_t*)0x40000028)
#define TIM2_ARR      (*(volatile uint32_t*)0x4000002C)
#define TIM2_EGR      (*(volatile uint32_t*)0x40000014)

#define NVIC_ISER0    (*(volatile uint32_t*)0xE000E100)

void GPIO_INIT(void)
{
    RCC_AHB1ENR |= (1<<0); // GPIOA
    RCC_AHB1ENR |= (1<<2); // GPIOC

    /* PA5 Output */

    GPIOA_MODER &= ~(3<<10);
    GPIOA_MODER |=  (1<<10);

    /* PC13 Input */

    GPIOC_MODER &= ~(3<<26);
}

void TIM2_INIT(void)
{
    RCC_APB1ENR |= (1<<0);

    TIM2_PSC = 15999;
    TIM2_ARR = 999;

    TIM2_EGR = 1;

    TIM2_SR = 0;

    TIM2_DIER |= (1<<0);   // UIE

    NVIC_ISER0 |= (1<<28); // TIM2 IRQ

    TIM2_CR1 |= (1<<0);
}

void TIM2_IRQHandler(void)
{
    if(TIM2_SR & 1)
    {
        TIM2_SR &= ~1;

        GPIOA_ODR ^= (1<<5);
    }
}

int main(void)
{
    GPIO_INIT();

    while(1)
    {
        if(!(GPIOC_IDR & (1<<13)))
        {
            TIM2_INIT();

            while(!(GPIOC_IDR & (1<<13)));
        }
    }
}
