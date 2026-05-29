#include<stdint.h>
int main(void){

	int a = 20;
	int b = 30;
	int result;

	__asm volatile(

	    "CMP %1, %2\n"
		"ITE LT \n"
		"MOVLT %0, #1\n"
		"MOVGE %0, #0\n"
			: "=r"(result)
			: "r"(a), "r"(b)
			: "cc"

	);
	while(1);

}
