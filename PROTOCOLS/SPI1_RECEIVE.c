#include <stdint.h>

#define RCC_AHB1ENR   (*(volatile uint32_t*)0x40023830)
#define RCC_APB2ENR   (*(volatile uint32_t*)0x40023844)

#define GPIOA_MODER   (*(volatile uint32_t*)0x40020000)
#define GPIOA_AFRL    (*(volatile uint32_t*)0x40020020)

#define SPI1_CR1      (*(volatile uint32_t*)0x40013000)
#define SPI1_SR       (*(volatile uint32_t*)0x40013008)
#define SPI1_DR       (*(volatile uint32_t*)0x4001300C)

volatile uint8_t received_data = 0;

void SPI1_Init(void)
{
    /* GPIOA Clock */
    RCC_AHB1ENR |= (1<<0);

    /* SPI1 Clock */
    RCC_APB2ENR |= (1<<12);

    /*
      PA4 NSS
      PA5 SCK
      PA6 MISO
      PA7 MOSI
    */

    GPIOA_MODER &= ~(0xFF << 8);
    GPIOA_MODER |=  (0xAA << 8);

    GPIOA_AFRL &= ~(0xFFFF << 16);
    GPIOA_AFRL |=  (0x5555 << 16);

    /* Slave Mode (MSTR = 0) */
    SPI1_CR1 &= ~(1<<2);

    /* CPOL = 0 */
    SPI1_CR1 &= ~(1<<1);

    /* CPHA = 0 */
    SPI1_CR1 &= ~(1<<0);

    /* 8-bit */
    SPI1_CR1 &= ~(1<<11);

    /* Enable SPI */
    SPI1_CR1 |= (1<<6);
}

int main(void)
{
    SPI1_Init();

    while(1)
    {
        if(SPI1_SR & (1<<0))      // RXNE
        {
            received_data = SPI1_DR;
        }
    }
}
