#include<stdint.h>

#define  RCC_AHB1ENR (*(volatile uint32_t*)0x40023830)
#define  RCC_APB1ENR (*(volatile uint32_t*)0x40023840)

#define  GPIOA_MODER (*(volatile uint32_t*)0x40020000)
#define  GPIOA_ODR (*(volatile uint32_t*)0x40020014)

#define  GPIOB_MODER (*(volatile uint32_t*)0x40020400)
#define  GPIOB_AFRL (*(volatile uint32_t*)0x40020420)

#define  SPI3_CR1 (*(volatile uint32_t*)0x40003C00)
#define  SPI3_SR (*(volatile uint32_t*)0x40003C08)
#define  SPI3_DR (*(volatile uint32_t*)0x40003C0C)

#define TFT_CS_LOW()    (GPIOA_ODR &= ~(1<<0))
#define TFT_CS_HIGH()   (GPIOA_ODR |=  (1<<0))

#define TFT_DC_LOW()    (GPIOA_ODR &= ~(1<<1))
#define TFT_DC_HIGH()   (GPIOA_ODR |=  (1<<1))

#define TFT_RST_LOW()   (GPIOA_ODR &= ~(1<<2))
#define TFT_RST_HIGH()  (GPIOA_ODR |=  (1<<2))

volatile uint32_t test;

void delay(void){

	for(volatile uint32_t i=0;i<5000000;i++);

}
void LINT(void){

	    RCC_AHB1ENR |= (1<<0);

		GPIOA_MODER &= ~(3<<10);
		GPIOA_MODER |= (1<<10);

}
void LED(void){


	GPIOA_ODR |= (1<<5);

	delay();

	GPIOA_ODR &= ~(1<<5);

	delay();

}


void GPIO_INI(void){


	RCC_AHB1ENR |= (1<<1);
	RCC_APB1ENR |= (1<<15);

	GPIOB_MODER &= ~(0x3F<<6);
	GPIOB_MODER |= (0x2A<<6);

	GPIOB_AFRL &= ~(0xFFF<<12);
	GPIOB_AFRL |= (0x666<<12);



}
void SPI3_REG(void)
{
    RCC_APB1ENR |= (1<<15);

    SPI3_CR1 &= ~(1<<2);
    SPI3_CR1 |=  (1<<2);    // Master

    SPI3_CR1 &= ~(7<<3);
    SPI3_CR1 |=  (3<<3);    // Baud Rate

    SPI3_CR1 |= (1<<9);     // SSM
    SPI3_CR1 |= (1<<8);     // SSI

    SPI3_CR1 &= ~(1<<1);    // CPOL=0
    SPI3_CR1 &= ~(1<<0);    // CPHA=0

    SPI3_CR1 &= ~(1<<11);   // 8-bit

    SPI3_CR1 |= (1<<6);     // SPE

    test = SPI3_CR1;
}

void TFT_GPIO_INIT(void)
{
    RCC_AHB1ENR |= (1<<0);

    GPIOA_MODER &= ~(0x3F);
    GPIOA_MODER |=  (0x15);

    TFT_CS_HIGH();
    TFT_DC_HIGH();
    TFT_RST_HIGH();
}

void SPI3_WRITE(uint8_t data)
{
    while(!(SPI3_SR & (1<<1)));   // TXE

    *((volatile uint8_t*)&SPI3_DR) = data;

    while(!(SPI3_SR & (1<<0)));   // RXNE

    volatile uint8_t dummy = *((volatile uint8_t*)&SPI3_DR);
    (void)dummy;

    while(SPI3_SR & (1<<7));      // BSY
}

void LCD_RESET(void)
{
    TFT_RST_HIGH();
    delay();

    TFT_RST_LOW();
    delay();

    TFT_RST_HIGH();
    delay();
}


void LCD_CMD(uint8_t cmd)
{
    TFT_DC_LOW();

    TFT_CS_LOW();

    SPI3_WRITE(cmd);

    TFT_CS_HIGH();
}

void LCD_DATA(uint8_t data)
{
    TFT_DC_HIGH();

    TFT_CS_LOW();

    SPI3_WRITE(data);

    TFT_CS_HIGH();
}

void LCD_INIT(void)
{
    LCD_RESET();

    LCD_CMD(0x01);
    delay();

    LCD_CMD(0x11);
    delay();

    LCD_CMD(0x3A);
    LCD_DATA(0x55);

    LCD_CMD(0x36);
    LCD_DATA(0x48);

    LCD_CMD(0x29);

    delay();
}
void LCD_PIXEL(uint16_t x,uint16_t y,uint16_t color)
{
    LCD_CMD(0x2A);

    LCD_DATA(x >> 8);
    LCD_DATA(x);

    LCD_DATA(x >> 8);
    LCD_DATA(x);

    LCD_CMD(0x2B);

    LCD_DATA(y >> 8);
    LCD_DATA(y);

    LCD_DATA(y >> 8);
    LCD_DATA(y);

    LCD_CMD(0x2C);

    LCD_DATA(color >> 8);
    LCD_DATA(color);
}

int main(void)
{
    GPIO_INI();

    TFT_GPIO_INIT();

    SPI3_REG();

    LCD_INIT();

    LCD_PIXEL(159,160,0xF800);

    while(1)
    {


    }
}
