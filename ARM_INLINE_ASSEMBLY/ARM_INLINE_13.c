#include<stdint.h>
int main(void){

	uint32_t main_stack;
	uint32_t process_stack;

	__asm volatile(

	 "MRS %0, MSP\n"
	 "MRS %1, PSP\n"
			: "=r"(main_stack),"=r"(process_stack)
			:


	);
while(1);
}
