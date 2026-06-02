#include <stdint.h>
/* RCC Registers */
#define RCC_AHB1ENR   (*(volatile uint32_t*)0x40023830)
#define RCC_APB1ENR   (*(volatile uint32_t*)0x40023840)
/* GPIOB Registers */
#define GPIOB_MODER    (*(volatile uint32_t*)0x40020400)
#define GPIOB_OTYPER   (*(volatile uint32_t*)0x40020404)
#define GPIOB_OSPEEDR  (*(volatile uint32_t*)0x40020408)
#define GPIOB_PUPDR    (*(volatile uint32_t*)0x4002040C)
#define GPIOB_AFRH     (*(volatile uint32_t*)0x40020424)
/* I2C1 Registers */
#define I2C1_CR1    (*(volatile uint32_t*)0x40005400)
#define I2C1_CR2    (*(volatile uint32_t*)0x40005404)
#define I2C1_DR     (*(volatile uint32_t*)0x40005410)
#define I2C1_SR1    (*(volatile uint32_t*)0x40005414)
#define I2C1_SR2    (*(volatile uint32_t*)0x40005418)
#define I2C1_CCR    (*(volatile uint32_t*)0x4000541C)
#define I2C1_TRISE  (*(volatile uint32_t*)0x40005420)

void I2C1_Init(void)
{
    RCC_AHB1ENR |= (1 << 1);
    RCC_APB1ENR |= (1 << 21);
    GPIOB_MODER &= ~(0xF << 16);
    GPIOB_MODER |=  (0xA << 16);
    GPIOB_OTYPER |= (1 << 8);
    GPIOB_OTYPER |= (1 << 9);
    GPIOB_OSPEEDR |= (0xF << 16);
    GPIOB_PUPDR &= ~(0xF << 16);
    GPIOB_PUPDR |=  (0x5 << 16);
    GPIOB_AFRH &= ~(0xFF);
    GPIOB_AFRH |=  (0x44);
    I2C1_CR1 |= (1 << 15);
    I2C1_CR1 &= ~(1 << 15);
    I2C1_CR2 = 16;
    I2C1_CCR = 80;
    I2C1_TRISE = 17;
    I2C1_CR1 |= (1 << 0);
}
void I2C1_Start(void)
{
    I2C1_CR1 |= (1 << 8);
    while(!(I2C1_SR1 & (1 << 0)));
}
void I2C1_Address(uint8_t address)
{
    I2C1_DR = address;
    while(!(I2C1_SR1 & (1 << 1)));
    volatile uint32_t temp;
    temp = I2C1_SR1;
    temp = I2C1_SR2;
}
void I2C1_Data(uint8_t data)
{
    while(!(I2C1_SR1 & (1 << 7)));
    I2C1_DR = data;
}
void I2C1_Stop(void)
{
    I2C1_CR1 |= (1 << 9);
}
void I2C1_SendString(char *str)
{
    while(*str)
    {
        I2C1_Data(*str++);
    }
    while(!(I2C1_SR1 & (1 << 2)));
}
int main(void)
{
    char msg[] = "Aravind samy";
    I2C1_Init();
    I2C1_Start();
    I2C1_Address(0x4E);
    I2C1_SendString(msg);
    I2C1_Stop();
    while(1);
}
