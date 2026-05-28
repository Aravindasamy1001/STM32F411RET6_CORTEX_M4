#include<stdint.h>
int main(void){

	int a = 11;
	int b = 22;
	int c = 33;
	int result;

	__asm volatile(

	"ADD %0,%1,%2\n"
	"ADD %0,%0,%3\n"
			: "=r"(result)
			: "r"(a), "r"(b), "r"(c)

	);
	while(1);
}
