#include<stdint.h>
int main(void)
{
    uint32_t a = 0x02;

     __asm volatile(

         "MSR CONTROL, %0\n"
    		 :
    		 : "r"(a)


     );

while(1);
}
