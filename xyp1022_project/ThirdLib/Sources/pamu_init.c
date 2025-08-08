

#include "environment.h"

void pamu_bypass(void) {

  *(volatile unsigned int*)(CIR0_MISC0_ADDR) = 1<<31;
  *(volatile unsigned int*)(CIR1_MISC0_ADDR) = 1<<31;
  *(volatile unsigned int*)(CIP0_MISC0_ADDR) = 1<<31;
  *(volatile unsigned int*)(CIP1_MISC0_ADDR) = 1<<31;

}
