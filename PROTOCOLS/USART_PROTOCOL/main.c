
#include <stdint.h>



#define RCC_AHB1ENR     (*(volatile uint32_t*) 0X40023830)
#define RCC_APB1ENR     (*(volatile uint32_t*) 0X40023840)

#define GPIOA_MODER     (*(volatile uint32_t*) 0x40020000)
#define GPIOA_AFRL      (*(volatile uint32_t*) 0x40020020)

#define GPIOD_MODER     (*(volatile uint32_t*) 0x40020C00)
#define GPIOD_ODR       (*(volatile uint32_t*) 0x40020C14)

#define USART2_SR       (*(volatile uint32_t*) 0x40004400)
#define USART2_DR       (*(volatile uint32_t*) 0x40004404)
#define USART2_BRR      (*(volatile uint32_t*) 0x40004408)
#define USART2_CR1      (*(volatile uint32_t*) 0x4000440C)

#define NVIC_ISER1      (*(volatile uint32_t*) 0xE000E104)

void GPIO_Init(void)
{
    RCC_AHB1ENR |= (1<<0);
    RCC_AHB1ENR |= (1<<3);

    GPIOD_MODER |= (1<<24);
    GPIOD_MODER &= ~(1<<25);

    GPIOA_MODER |= (1<<5) | (1<<7);
    GPIOA_MODER &= ~((1<<4) | (1<<6));

    GPIOA_AFRL |= (7<<8);
    GPIOA_AFRL |= (7<<12);
}

void USART2_Init(void)
{
    RCC_APB1ENR |= (1<<17);
    USART2_BRR = 0x0683;

    USART2_CR1 |= (1<<3);
    USART2_CR1 |= (1<<2);
    USART2_CR1 |= (1<<5);

    USART2_CR1 |= (1<<13);

    NVIC_ISER1 |= (1<<6);
}



void USART2_SendString(char *str)
{
    while(*str)
    {
    	 while(!(USART2_SR & (1<<7)));
    	    USART2_DR = *str++ ;
    }
}

void USART2_IRQHandler(void)
{
    if(USART2_SR & (1<<5))
    {
        char data=USART2_DR;

        if(data == '1')
        {
            GPIOD_ODR |= (1<<12);
            USART2_SendString("on");
        }
        else if(data == '0')
        {
            GPIOD_ODR &= ~(1<<12);
            USART2_SendString("off");
        }
        else{

        	 USART2_SendString(&data);
        }
    }
}
int main(void)
{
    GPIO_Init();
    USART2_Init();

    USART2_SendString("ready\n");


}
