#include<stdint.h>
int main(void){

	int a;
	int b = 23;

	__asm volatile(

	"STR %1,[%0]"
        :
		:"r"(&a),
		 "r"(b)
        :"memory"
	);

	while(1);
}
