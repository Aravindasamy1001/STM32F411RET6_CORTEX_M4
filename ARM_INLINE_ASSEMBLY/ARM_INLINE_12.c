#include<stdint.h>
int main(void){

	uint32_t control_reg;

	__asm volatile(

	 "MRS %0, CONTROL\n"
			: "=r"(control_reg)
			:
			:

	);

	while(1);
}
