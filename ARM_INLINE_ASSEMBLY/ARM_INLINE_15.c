#include<stdint.h>
#include<stdio.h>
int main(void){

	int a=10;
	int b=13;
	int t;
	int rr;
	__asm volatile(

	"ADD %0, %1, %2\n"
	"STR %0, [%3]\n"
		: "=r"(rr)
		: "r"(a), "r"(b), "r"(&t)
	    : "memory"
	);

	printf("%d\n",t);
}
