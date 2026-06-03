#include <stdint.h>



#define RCC_AHB1ENR (*(volatile uint32_t*)0x40023830)
#define RCC_APB1ENR (*(volatile uint32_t*)0x40023840)



#define GPIOA_MODER (*(volatile uint32_t*)0x40020000)
#define GPIOA_ODR   (*(volatile uint32_t*)0x40020014)

#define GPIOB_MODER (*(volatile uint32_t*)0x40020400)
#define GPIOB_AFRH  (*(volatile uint32_t*)0x40020424)
#define GPIOB_OTYPER (*(volatile uint32_t*)0x40020404)
#define GPIOB_PUPDR  (*(volatile uint32_t*)0x4002040C)



#define I2C_CR1   (*(volatile uint32_t*)0x40005400)
#define I2C_CR2   (*(volatile uint32_t*)0x40005404)
#define I2C_DR    (*(volatile uint32_t*)0x40005410)
#define I2C_SR1   (*(volatile uint32_t*)0x40005414)
#define I2C_SR2   (*(volatile uint32_t*)0x40005418)
#define I2C_CCR   (*(volatile uint32_t*)0x4000541C)
#define I2C_TRISE (*(volatile uint32_t*)0x40005420)

uint8_t received;

void delay(void)
{
    for(int i=0;i<500000;i++);
}

void gpio_init(void)
{
    RCC_AHB1ENR |= (1<<0);
    RCC_AHB1ENR |= (1<<1);

    /* LED */

    GPIOA_MODER &= ~(3<<10);
    GPIOA_MODER |=  (1<<10);

    /* PB8 PB9 AF */

    GPIOB_MODER &= ~(3<<16);
    GPIOB_MODER |=  (2<<16);

    GPIOB_MODER &= ~(3<<18);
    GPIOB_MODER |=  (2<<18);

    GPIOB_AFRH &= ~(0xFF);

    GPIOB_AFRH |= (4<<0);
    GPIOB_AFRH |= (4<<4);

    GPIOB_OTYPER |= (1<<8);
    GPIOB_OTYPER |= (1<<9);

    GPIOB_PUPDR &= ~(3<<16);
    GPIOB_PUPDR |=  (1<<16);

    GPIOB_PUPDR &= ~(3<<18);
    GPIOB_PUPDR |=  (1<<18);
}

void i2c_init(void)
{
    RCC_APB1ENR |= (1<<21);

    I2C_CR1 &= ~(1<<0);

    I2C_CR2 = 16;

    I2C_CCR = 80;

    I2C_TRISE = 17;

    I2C_CR1 |= (1<<0);
}

void start(void)
{
    I2C_CR1 |= (1<<8);

    while(!(I2C_SR1 & (1<<0)));
}

void stop(void)
{
    I2C_CR1 |= (1<<9);
}

void address_write(uint8_t addr)
{
    I2C_DR = (addr << 1);

    while(!(I2C_SR1 & (1<<1)));

    volatile uint32_t temp;

    temp = I2C_SR1;
    temp = I2C_SR2;
}

void address_read(uint8_t addr)
{
    I2C_DR = (addr << 1) | 1;

    while(!(I2C_SR1 & (1<<1)));

    volatile uint32_t temp;

    temp = I2C_SR1;
    temp = I2C_SR2;
}

void write_data(uint8_t data)
{
    while(!(I2C_SR1 & (1<<7)));

    I2C_DR = data;

    while(!(I2C_SR1 & (1<<2)));
}

void write_string(char *str)
{
    while(*str)
    {
        write_data(*str);

        str++;
    }
}

uint8_t read_data(void)
{
    I2C_CR1 |= (1<<10);

    while(!(I2C_SR1 & (1<<6)));

    return I2C_DR;
}

void ledblink(void)
{
    GPIOA_ODR ^= (1<<5);

    delay();
}

int main(void)
{
    char msg[] = "AS";

    gpio_init();

    i2c_init();

    while(1)
    {

//write
        start();

        address_write(0x27);

        write_string(msg);

        stop();

        delay();

 //read

        start();

        address_read(0x27);

        received = read_data();

        stop();

        if(received)
        {
            ledblink();
        }

        delay();
    }
}
