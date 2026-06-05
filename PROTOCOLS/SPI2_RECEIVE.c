#include <stdint.h>

/* RCC Registers */

#define RCC_AHB1ENR   (*(volatile uint32_t*)0x40023830)
#define RCC_APB1ENR   (*(volatile uint32_t*)0x40023840)

/* GPIOA Registers (LED) */

#define GPIOA_MODER   (*(volatile uint32_t*)0x40020000)
#define GPIOA_ODR     (*(volatile uint32_t*)0x40020014)

/* GPIOB Registers (SPI2) */

#define GPIOB_MODER   (*(volatile uint32_t*)0x40020400)
#define GPIOB_AFRH    (*(volatile uint32_t*)0x40020424)

/* SPI2 Registers */

#define SPI2_CR1      (*(volatile uint32_t*)0x40003800)
#define SPI2_SR       (*(volatile uint32_t*)0x40003808)
#define SPI2_DR       (*(volatile uint32_t*)0x4000380C)

volatile uint8_t received_data;
volatile uint32_t spi2_cr1_value;

/* Delay */

void delay(void)
{
    for(volatile uint32_t i = 0; i < 100000; i++);
}


/* LED Init (PA1) */

void LED_Init(void)
{
    RCC_AHB1ENR |= (1<<0);

    /* PA1 Output Mode */

    GPIOA_MODER &= ~(3<<2);
    GPIOA_MODER |=  (1<<2);
}


/* SPI2 Init */

void SPI2_Init(void)
{
    /* GPIOB Clock Enable */

    RCC_AHB1ENR |= (1<<1);

    /* SPI2 Clock Enable */

    RCC_APB1ENR |= (1<<14);

    /*
       PB12 -> NSS
       PB13 -> SCK
       PB14 -> MISO
       PB15 -> MOSI
    */

    GPIOB_MODER &= ~(0xFF << 24);
    GPIOB_MODER |=  (0xAA << 24);

    /* AF5 */

    GPIOB_AFRH &= ~(0xFFFF << 16);
    GPIOB_AFRH |=  (0x5555 << 16);

    /* Slave Mode */

    SPI2_CR1 &= ~(1<<2);

    /* CPOL = 0 */

    SPI2_CR1 &= ~(1<<1);

    /* CPHA = 0 */

    SPI2_CR1 &= ~(1<<0);

    /* 8-bit Mode */

    SPI2_CR1 &= ~(1<<11);

    /* Enable SPI */
    SPI2_CR1 &= ~(1<<6);
    SPI2_CR1 |= (1<<6);
}


/* Main */

int main(void)
{
    LED_Init();

    SPI2_Init();
    spi2_cr1_value = SPI2_CR1;

    while(1)
    {
        /* RXNE = Bit 0 */


      while(!(SPI2_SR & (1<<0)))  {

        	received_data = SPI2_DR;

            /* LED ON */

            GPIOA_ODR |= (1<<1);

            delay();

            /* LED OFF */

            GPIOA_ODR &= ~(1<<1);
        }
    }
}
