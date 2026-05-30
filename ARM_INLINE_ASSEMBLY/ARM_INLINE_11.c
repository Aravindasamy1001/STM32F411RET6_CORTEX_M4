#include<stdint.h>
int main(void){


	int a = 10;
	int b = 0;
	int counter;

	__asm volatile(

			"MOV %0, %2\n"
			"loop_start:\n"
			"ADD %1, %1,%0\n"
			"SUB %0, %0,#1\n"
			"BNE loop_start\n"
			: "=r"(counter), "+r"(b)
			: "r"(a)
		    : "cc"


	);
	while(1);

}
