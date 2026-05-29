#include<stdint.h>
int main(void){

	int a = 54;
	int b;

	__asm volatile(

	   "LDR %0,[%1]\n"
	   "ADD %0,%0,#1\n"
	   "STR  %0,[%2]\n"
	   : "=r"(b)
	   : "r"(&a),"r"(&b)
	   : "memory"


	);

	while(1);
}
