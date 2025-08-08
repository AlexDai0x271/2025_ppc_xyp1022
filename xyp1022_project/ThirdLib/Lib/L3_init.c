#include "cpc.h"

void L3_init() {
volatile unsigned int rddata;
unsigned int mask_v;


// set CPCSR0[CPCFI] CPCCSR0[CPCFC] using the same write operation
mask_v = (1<<BIT_CPCLFC) | (1<<BIT_CPCFI);
*(volatile unsigned int*)(CPCCSR0) = mask_v;
rddata = 1;

while( (rddata & mask_v) != 0) {
    rddata = *(volatile unsigned int*)(CPCCSR0);
}

#ifdef L3_SRAM
*(volatile unsigned int*)(CPCSRCR1) = 0;
*(volatile unsigned int*)(CPCSRCR0) = (0<<BIT_CPCSRCR0_SRBARL) | (0<<BIT_CPCSRCR0_INTLVEN) | (0<<BIT_CPCSRCR0_SRAMSZ) | (1<<BIT_CPCSRCR0_SRAMEN);
asm("isync");
#endif

//Enable L3 Cache
    rddata = *(volatile unsigned int*)(CPCCSR0);
    *(volatile unsigned int*)(CPCCSR0) = rddata | (1<<BIT_CPCE);

}
