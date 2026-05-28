#include<stdio.h>
int main(void){

	int a = 25;

	__asm volatile(

			"ADD %0,%0,#1\n"
			: "+r"(a)

	);

	while(1);
}
