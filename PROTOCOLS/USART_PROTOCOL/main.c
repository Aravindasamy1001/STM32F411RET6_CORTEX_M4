#include <stdint.h>

//clock enable
#define RCC_AHB1ENR (*(volatile uint32_t*)0x40023830)
#define RCC_APB2ENR (*(volatile uint32_t*)0x40023844)


#define GPIOA_MODER (*(volatile uint32_t*)0x40020000)
#define GPIOA_AFRH  (*(volatile uint32_t*)0x40020024)


#define USART1_SR   (*(volatile uint32_t*)0x40011000)
#define USART1_DR   (*(volatile uint32_t*)0x40011004)
#define USART1_BRR  (*(volatile uint32_t*)0x40011008)
#define USART1_CR1  (*(volatile uint32_t*)0x4001100c)

void GPIO_Init(){
	// Enable GPIOA clock
	RCC_AHB1ENR |= (1<<0);
// Clear PA9
	GPIOA_MODER &= ~(3<<20);  // Clear PA10
	GPIOA_MODER |= (2<<18);   // Set PA9 to AF (10)
	GPIOA_MODER |= (2<<20);   // Set PA10 to AF (10)

	// Set Alternate Function to USART1 (AF7)
	// AFRH[7:4] for PA9, AFRH[11:8] for PA10
	GPIOA_AFRH &= ~(15<<4);   // Clear PA9 AF bits
	GPIOA_AFRH &= ~(15<<8);   // Clear PA10 AF bits
	GPIOA_AFRH |= (7<<4);     // Set PA9 to AF7
	GPIOA_AFRH |= (7<<8);     // Set PA10 to AF7
}

void USART1_Init(){
	// Enable USART1 clock on APB2
	RCC_APB2ENR |= (1<<4);


	USART1_BRR = 0x008B;

	// Enable TX
	USART1_CR1 |= (1<<3);

	// Enable RX
	USART1_CR1 |= (1<<2);

	// Enable USART1
	USART1_CR1 |= (1<<13);
}

void USART1_SendChar(char ch)
{
	// Wait until TX buffer is empty (TXE flag)
	while(!(USART1_SR & (1<<7)));

	// Send character
	USART1_DR = ch;
}

void USART1_SendString(char *str)
{
	while(*str)
	{
		USART1_SendChar(*str++);
	}
}

void delay_ms(uint32_t ms)
{
	for(uint32_t i = 0; i < ms; i++)
		for(uint32_t j = 0; j < 5600; j++);  // Approximate delay for 84MHz clock
}

int main(void)
{
	GPIO_Init();
	USART1_Init();

	// Add small delay to ensure USART is ready
	delay_ms(50);

	while(1){
		USART1_SendString("aravind samy1001\n");
		delay_ms(10);  // Send every 1 second
	}

	return 0;
}
