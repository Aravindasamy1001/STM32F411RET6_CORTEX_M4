#include<stdint.h>

#define  RCC_AHB1ENR (*(volatile uint32_t*)0x40023830)
#define  RCC_APB2ENR (*(volatile uint32_t*)0x40023844)

#define  GPIOA_MODER (*(volatile uint32_t*)0x40020000)
#define  GPIOA_AFRL (*(volatile uint32_t*)0x40020020)

#define  SPI1_CR1 (*(volatile uint32_t*)0x40013000)
#define  SPI1_SR (*(volatile uint32_t*)0x40013008)
#define  SPI1_DR (*(volatile uint32_t*)0x4001300C)


void ledblini(void){

	RCC_AHB1ENR |= (1<<0);

}
void gpio_int(void){

	RCC_AHB1ENR |= (1<<0);
	RCC_APB2ENR |= (1<<12);


	GPIOA_MODER &= ~(3<<8);
	GPIOA_MODER &= ~(3<<10);
	GPIOA_MODER &= ~(3<<12);
	GPIOA_MODER &= ~(3<<14);

	GPIOA_MODER |= (2<<8);
	GPIOA_MODER |= (2<<10);
	GPIOA_MODER |= (2<<12);
	GPIOA_MODER |= (2<<14);

	GPIOA_AFRL &= ~(0xFFF<<20);
	GPIOA_AFRL |= (0X555<<20);
}
	//SPI registers
void spi1_reg(void){


	SPI1_CR1 |= (1<<2);
	SPI1_CR1 &= ~(7<<3);
	SPI1_CR1 |= (3<<3);


	SPI1_CR1 &= ~(1<<1);
	SPI1_CR1 &= ~(1<<0);

	SPI1_CR1 &= ~(1<<11);

	SPI1_CR1 |= (1<<9);
	SPI1_CR1 |= (1<<8);

	SPI1_CR1 |= (1<<6);

}

void delay(void){

	for(volatile int i=0;i<500000;i++);
}

/*void led_blink(void){

	RCC_AHB1ENR |= (1<<0);

	GPIOA_ODR |= (1<<5);

	delay();

	GPIOA_ODR &= ~(1<<5);

}*/

void spi1_data(char d){

	while(!(SPI1_SR & (1<<1)));

	SPI1_DR = d;

    while(SPI1_SR & (1<<7));
}

int main(void){


	gpio_int();
	spi1_reg();

	while(1){

		spi1_data('A');

        delay();
	}



}
