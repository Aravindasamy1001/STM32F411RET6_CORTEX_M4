#include<stdint.h>

#define RCC_AHB1ENR (*(volatile uint32_t*)0x40023830)
#define GPIOA_MODER (*(volatile uint32_t*)0x40020000)
#define GPIOA_ODR   (*(volatile uint32_t*)0x40020014)

void gpio_init(void)
{
    RCC_AHB1ENR |= (1<<0);

    GPIOA_MODER &= ~(3<<10);
    GPIOA_MODER |=  (1<<10);
}

int main(void)
{
    gpio_init();

    while(1)
    {
        // LED ON
        __asm volatile(

            "LDR R0,=0x40020014\n"
            "LDR R1,[R0]\n"
            "ORR R1,R1,#(1<<5)\n"
            "STR R1,[R0]"

            :
            :
            : "r0","r1","memory"
        );

        // Delay
        for(volatile int i=0;i<10000000;i++);

        // LED OFF
        __asm volatile(

            "LDR R0,=0x40020014\n"
            "LDR R1,[R0]\n"
            "BIC R1,R1,#(1<<5)\n"
            "STR R1,[R0]"

            :
            :
            : "r0","r1","memory"
        );

        // Delay
        for(volatile int i=0;i<10000000;i++);
    }
}
