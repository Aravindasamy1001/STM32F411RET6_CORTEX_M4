#include<stdint.h>

#define  RCC_AHB1ENR (*(volatile uint32_t*)0x40023830)
#define  RCC_APB1ENR (*(volatile uint32_t*)0x40023840)

#define  GPIOA_MODER (*(volatile uint32_t*)0x40020000)
#define  GPIOA_ODR   (*(volatile uint32_t*)0x40020014)
#define  GPIOA_AFRL   (*(volatile uint32_t*)0x40020020)

#define  TIM2_CR1 (*(volatile uint32_t*)0x40000000)
#define  TIM2_SR  (*(volatile uint32_t*)0x40000010)
#define  TIM2_PSC (*(volatile uint32_t*)0x40000028)
#define  TIM2_ARR (*(volatile uint32_t*)0x4000002C)
#define  TIM2_EGR (*(volatile uint32_t*)0x40000014)
#define  TIM2_CNT (*(volatile uint32_t*)0x40000024)

#define  TIM2_CCR1 (*(volatile uint32_t*)0x40000034)
#define  TIM2_CCMR1 (*(volatile uint32_t*)0x40000018)
#define  TIM2_CCER (*(volatile uint32_t*)0x40000020)
void delay(void){

	for(int i=0;i<500000;i++){

	}

}
void GPIO_INIT(void)
{
	RCC_AHB1ENR |=(1<<0);
	RCC_APB1ENR |=(1<<0);

	GPIOA_MODER &= ~(3<<10);
	GPIOA_MODER |= (2<<10);

	GPIOA_AFRL &=~(0xF<<20);
	GPIOA_AFRL |= (1<<20);

}

/*void IN_LED(void){

	GPIOA_ODR |= (1<<5);
	delay();
	GPIOA_ODR &= ~(1<<5);
	delay();

}
*/

void TIM(void){
	TIM2_PSC   = 15;
	TIM2_ARR   = 999;

	TIM2_CCMR1 |= (1<<3);
	TIM2_CCMR1 |= (6<<4);

	TIM2_CCER |= (1<<0);

	TIM2_EGR = 1;

	TIM2_CR1 |= (1<<0);
}
int main(void){

	GPIO_INIT();
	TIM();

	while(1){

		TIM2_CCR1  = 250;
		delay();
		TIM2_CCR1  = 1000;
		delay();
		TIM2_CCR1  = 000;
		delay();
	}

}


