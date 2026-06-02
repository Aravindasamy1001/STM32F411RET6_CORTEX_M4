#include <stdint.h>

/* RCC */

#define RCC_AHB1ENR (*(volatile uint32_t*)0x40023830)
#define RCC_APB1ENR (*(volatile uint32_t*)0x40023840)

/* GPIOA */

#define GPIOA_MODER (*(volatile uint32_t*)0x40020000)
#define GPIOA_ODR   (*(volatile uint32_t*)0x40020014)

/* GPIOB */

#define GPIOB_MODER   (*(volatile uint32_t*)0x40020400)
#define GPIOB_OTYPER  (*(volatile uint32_t*)0x40020404)
#define GPIOB_PUPDR   (*(volatile uint32_t*)0x4002040C)
#define GPIOB_AFRH    (*(volatile uint32_t*)0x40020424)

/* I2C1 */

#define I2C1_CR1    (*(volatile uint32_t*)0x40005400)
#define I2C1_CR2    (*(volatile uint32_t*)0x40005404)
#define I2C1_DR     (*(volatile uint32_t*)0x40005410)
#define I2C1_SR1    (*(volatile uint32_t*)0x40005414)
#define I2C1_SR2    (*(volatile uint32_t*)0x40005418)
#define I2C1_CCR    (*(volatile uint32_t*)0x4000541C)
#define I2C1_TRISE  (*(volatile uint32_t*)0x40005420)


void delay(void)
{
    for(uint32_t i=0;i<500000;i++);
}


/* GPIO INIT */

void gpio_init(void)
{
    RCC_AHB1ENR |= (1<<0);
    RCC_AHB1ENR |= (1<<1);

    /* PA5 OUTPUT */

    GPIOA_MODER &= ~(3<<10);
    GPIOA_MODER |=  (1<<10);

    /* PB8 PB9 AF MODE */

    GPIOB_MODER &= ~(3<<16);
    GPIOB_MODER |=  (2<<16);

    GPIOB_MODER &= ~(3<<18);
    GPIOB_MODER |=  (2<<18);

    /* OPEN DRAIN */

    GPIOB_OTYPER |= (1<<8);
    GPIOB_OTYPER |= (1<<9);

    /* PULL UP */

    GPIOB_PUPDR &= ~(3<<16);
    GPIOB_PUPDR |=  (1<<16);

    GPIOB_PUPDR &= ~(3<<18);
    GPIOB_PUPDR |=  (1<<18);

    /* AF4 */

    GPIOB_AFRH &= ~(0xFF);

    GPIOB_AFRH |= (4<<0);
    GPIOB_AFRH |= (4<<4);
}


/* I2C INIT */

void i2c_init(void)
{
    RCC_APB1ENR |= (1<<21);

    I2C1_CR1 &= ~(1<<0);

    I2C1_CR2 = 16;

    I2C1_CCR = 80;

    I2C1_TRISE = 17;

    I2C1_CR1 |= (1<<0);
}


/* START */

void i2c_start(void)
{
    I2C1_CR1 |= (1<<8);

    while(!(I2C1_SR1 & (1<<0)));
}


/* ADDRESS */

void i2c_address(uint8_t address)
{
    I2C1_DR = (address << 1);

    while(!(I2C1_SR1 & (1<<1)));

    volatile uint32_t temp;

    temp = I2C1_SR1;
    temp = I2C1_SR2;
}


/* WRITE */

void i2c_write(uint8_t data)
{
    while(!(I2C1_SR1 & (1<<7)));

    I2C1_DR = data;

    while(!(I2C1_SR1 & (1<<2)));
}


/* STOP */

void i2c_stop(void)
{
    I2C1_CR1 |= (1<<9);
}


/* LED */

void led_blink(void)
{
    GPIOA_ODR |= (1<<5);

    delay();

    GPIOA_ODR &= ~(1<<5);

    delay();
}


int main(void)
{
    gpio_init();

    i2c_init();

    while(1)
    {
        led_blink();

        i2c_start();

        /* USE REAL 7-BIT SLAVE ADDRESS */

        i2c_address(0x27);

        /* SEND ONE BYTE */

        i2c_write(0x55);

        i2c_stop();

        delay();
    }
}
