#include "environment.h"

void cc_law_init(void) {
	 
   unsigned int wrdata;
   unsigned int rddata;
//-------------------law0 config before L3 Cache enable, for ddr1 rank0------------
   wrdata = 0x00000000;
   STW(wrdata,LAWBARL0); 

   wrdata = 0x00000000;
   STW(wrdata,LAWBARH0); 

   wrdata = 0x80000000 | (0x10 << 20) | CC_LAW_16G; 
   STW(wrdata,LAWAR0); 

//-------------------law1 ebc monitor 
   wrdata = 0xf6000000;
   STW(wrdata,LAWBARL1); 

   wrdata = 0x4;
   STW(wrdata,LAWBARH1); 

   wrdata = 0x80000000 | (0x1f << 20) | CC_LAW_16M; 
   STW(wrdata,LAWAR1); 

//-------------------law2 config for PCIE0------------
   wrdata = 0x00000000;
   STW(wrdata,LAWBARL2); 

   wrdata = 0x00000006;
   STW(wrdata,LAWBARH2); 

   wrdata = 0x80000000 | (0x00 << 20) | CC_LAW_8G; 
   STW(wrdata,LAWAR2); 

//-------------------law3 config for PCIE1------------
   wrdata = 0x00000000;
   STW(wrdata,LAWBARL3); 

   wrdata = 0x00000008;
   STW(wrdata,LAWBARH3); 

   wrdata = 0x80000000 | (0x00 << 20) | CC_LAW_8G; 
   STW(wrdata,LAWAR3);

//-------------------law4 config for PCIE2------------
   wrdata = 0x00000000;
   STW(wrdata,LAWBARL4); 

   wrdata = 0x0000000A;
   STW(wrdata,LAWBARH4); 

   wrdata = 0x80000000 | (24 << 20) | CC_LAW_8G; 
   STW(wrdata,LAWAR4);

//-------------------law5 config for PCIE3------------
   wrdata = 0x00000000;
   STW(wrdata,LAWBARL5); 

   wrdata = 0x0000000C;
   STW(wrdata,LAWBARH5); 

   wrdata = 0x80000000 | (24 << 20) | CC_LAW_8G; 
   STW(wrdata,LAWAR5);

//-------------------law6 config for aurora------------
   wrdata = 0x00000000;
   STW(wrdata,LAWBARL6); 

   wrdata = 0x0000000E;
   STW(wrdata,LAWBARH6); 

   wrdata = 0x80000000 | (24 << 20) | CC_LAW_4G; 
   STW(wrdata,LAWAR6);

//-------------------law10 ebc bank2 
   wrdata = 0xf7000000;
   STW(wrdata,LAWBARL10); 

   wrdata = 0x4;
   STW(wrdata,LAWBARH10); 

   wrdata = 0x80000000 | (0x1f << 20) | CC_LAW_16M; 
   STW(wrdata,LAWAR10);

//-------------------law11 ebc bank3 
   wrdata = 0xf5000000;
   STW(wrdata,LAWBARL11); 

   wrdata = 0x4;
   STW(wrdata,LAWBARH11); 

   wrdata = 0x80000000 | (0x1f << 20) | CC_LAW_16M; 
   STW(wrdata,LAWAR11);
   
//-------------------law12 ebc bank4 
   wrdata = 0xf4000000;
   STW(wrdata,LAWBARL12); 

   wrdata = 0x4;
   STW(wrdata,LAWBARH12); 

   wrdata = 0x80000000 | (0x1f << 20) | CC_LAW_16M; 
   STW(wrdata,LAWAR12);

//-------------------law13 ebc bank5 
   wrdata = 0xf3000000;
   STW(wrdata,LAWBARL13); 

   wrdata = 0x4;
   STW(wrdata,LAWBARH13); 

   wrdata = 0x80000000 | (0x1f << 20) | CC_LAW_16M; 
   STW(wrdata,LAWAR13);

//-------------------law14 ebc bank6 
   wrdata = 0xf2000000;
   STW(wrdata,LAWBARL14); 

   wrdata = 0x4;
   STW(wrdata,LAWBARH14); 

   wrdata = 0x80000000 | (0x1f << 20) | CC_LAW_16M; 
   STW(wrdata,LAWAR14);

//-------------------law15 ebc bank7 
   wrdata = 0xf1000000;
   STW(wrdata,LAWBARL15); 

   wrdata = 0x4;
   STW(wrdata,LAWBARH15); 

   wrdata = 0x80000000 | (0x1f << 20) | CC_LAW_16M; 
   STW(wrdata,LAWAR15);

//-------------------law16 sram
   wrdata = 0xe0000000;
   STW(wrdata,LAWBARL16); 

   wrdata = 0x4;
   STW(wrdata,LAWBARH16); 

   wrdata = 0x80000000 | (0x1f << 20) | CC_LAW_16M; 
   STW(wrdata,LAWAR16);
   
   
   
   
 //-------------------law17 ebc bank0 
      wrdata = 0xf8000000;
      STW(wrdata,LAWBARL17); 

      wrdata = 0x4;
      STW(wrdata,LAWBARH17); 

      wrdata = 0x80000000 | (0x1f << 20) | CC_LAW_32M; 
      STW(wrdata,LAWAR17);

  
   NOP10;
   ISYNC;
}
