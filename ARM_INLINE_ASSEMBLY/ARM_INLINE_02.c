#include <stdint.h>
int main(void){

	int c = 10;
	int e = 0;

	__asm volatile(

	     "MOV %0,%1"
			: "=r"(e)
			: "r"(c)
);

while(1);
}
