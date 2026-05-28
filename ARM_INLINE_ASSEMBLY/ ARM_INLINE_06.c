#include<stdint.h>
int main(void)
{
	int a = 100;
	int b = 99;

	__asm volatile(

	   "LDR %0,[%1]"
			: "=r"(a)
			: "r"(&b)  //ADDRESS OF B AS INPUT
	);

	while(1);
}
