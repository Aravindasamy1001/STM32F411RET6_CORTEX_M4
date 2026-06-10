#include <stdint.h>

#define RCC_AHB1ENR   (*(volatile uint32_t*)0x40023830)
#define RCC_APB2ENR   (*(volatile uint32_t*)0x40023844)

#define GPIOA_MODER   (*(volatile uint32_t*)0x40020000)
#define GPIOA_AFRH    (*(volatile uint32_t*)0x40020024)

#define USART1_SR     (*(volatile uint32_t*)0x40011000)
#define USART1_DR     (*(volatile uint32_t*)0x40011004)
#define USART1_BRR    (*(volatile uint32_t*)0x40011008)
#define USART1_CR1    (*(volatile uint32_t*)0x4001100C)

void delay()
{
    for(int i=0;i<500000;i++);
}

void GPIO_Init()
{
    // Enable GPIOA clock
    RCC_AHB1ENR |= (1<<0);

    // PA9  -> TX
    // PA10 -> RX

    GPIOA_MODER &= ~((3<<18) | (3<<20));
    GPIOA_MODER |=  ((2<<18) | (2<<20));

    // AF7 for USART1

    GPIOA_AFRH &= ~((15<<4) | (15<<8));
    GPIOA_AFRH |=  ((7<<4) | (7<<8));
}

void USART1_Init()
{
    // Enable USART1 clock
    RCC_APB2ENR |= (1<<4);

    // 9600 baud @16MHz
    USART1_BRR = 0x0683;

    // TE = transmitter enable
    USART1_CR1 |= (1<<3);

    // RE = receiver enable
    USART1_CR1 |= (1<<2);

    // UE = USART enable
    USART1_CR1 |= (1<<13);
}

void USART1_SendChar(char ch)
{
    // Wait until TXE = 1
    while(!(USART1_SR & (1<<7)));

    USART1_DR = ch;
}

void USART1_SendString(char *str)
{
    while(*str)
    {
        USART1_SendChar(*str++);
    }
}

int main()
{
    GPIO_Init();

    USART1_Init();

    while(1)
    {
        USART1_SendString("HELLO 8051\r\n");

        delay();
    }
}
