#include<stdint.h>

#define  RCC_AHB1ENR (*(volatile uint32_t*)0x40023830)
#define  RCC_APB1ENR (*(volatile uint32_t*)0x40023840)

#define  GPIOA_MODER (*(volatile uint32_t*)0x40020000)
#define  GPIOA_ODR   (*(volatile uint32_t*)0x40020014)

#define  TIM2_CR1 (*(volatile uint32_t*)0x40000000)
#define  TIM2_SR  (*(volatile uint32_t*)0x40000010)
#define  TIM2_PSC (*(volatile uint32_t*)0x40000028)
#define  TIM2_ARR (*(volatile uint32_t*)0x4000002C)
#define  TIM2_EGR (*(volatile uint32_t*)0x40000014)
#define  TIM2_CNT (*(volatile uint32_t*)0x40000024)



void LED(void){

	RCC_AHB1ENR |= (1<<0);
	RCC_APB1ENR |= (1<<0);

	GPIOA_MODER &= ~(3<<10);
	GPIOA_MODER |= (1<<10);

}
void TIME(void){

	TIM2_PSC = 15999;
	TIM2_ARR = 999;

	TIM2_EGR =1;

	TIM2_SR =0;

	TIM2_CR1 =1;

}

int main(void){

	LED();
    TIME();

    while(1)
    {
        while(!(TIM2_SR & (1<<0)));

        TIM2_SR &= ~(1<<0);

        GPIOA_ODR ^= (1<<5);
    }
 }
