#include <stdint.h>
int main(void){

	int a = 6;
	int b ;

	__asm volatile(

			"MOV R1,%1\n"
			"MOV R3,R1\n"
			"MOV %0,R3\n"
			: "=r"(b)
			: "r"(a)
            : "r1","r3"
	);
	while(1);
}
