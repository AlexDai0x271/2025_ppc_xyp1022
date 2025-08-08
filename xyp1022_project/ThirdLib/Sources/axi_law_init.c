#include "environment.h"

void axi_law_init(void) {
	 
   unsigned int plb6_value;
////////////////////////////////////////
    //pcie0
    plb6_value = 0x0;
    STW(plb6_value, AXI_LAW_BAR_BASE + 0*4);
    
    plb6_value = 0x06;
    STW(plb6_value, AXI_LAW_BARH_BASE + 0*4);
    //8GB
    plb6_value = 1<<31 | 0x18;
    STW(plb6_value, AXI_LAW_AR_BASE + 0*4);

//    plb6_value = 1;
//    STW(plb6_value, AXI_LAW_UPDATE + 0*4);
////////////////////////////////////////
    //pcie1
    plb6_value = 0x0;
    STW(plb6_value, AXI_LAW_BAR_BASE + 1*4);
    
    plb6_value = 0x08;
    STW(plb6_value, AXI_LAW_BARH_BASE + 1*4);
    //8GB
    plb6_value = 1<<31 | 0x18;
    STW(plb6_value, AXI_LAW_AR_BASE + 1*4);

//    plb6_value = 1;
//    STW(plb6_value, AXI_LAW_UPDATE + 1*4);
////////////////////////////////////////
    //pcie2
    plb6_value = 0x0;
    STW(plb6_value, AXI_LAW_BAR_BASE + 2*4);
    
    plb6_value = 0x0A;
    STW(plb6_value, AXI_LAW_BARH_BASE + 2*4);
    //8GB
    plb6_value = 1<<31 | 0x18;
    STW(plb6_value, AXI_LAW_AR_BASE + 2*4);

//    plb6_value = 1;
//    STW(plb6_value, AXI_LAW_UPDATE + 2*4);
////////////////////////////////////////
    //pcie3
    plb6_value = 0x0;
    STW(plb6_value, AXI_LAW_BAR_BASE + 3*4);
    
    plb6_value = 0x0C;
    STW(plb6_value, AXI_LAW_BARH_BASE + 3*4);
    //8GB
    plb6_value = 1<<31 | 0x18;
    STW(plb6_value, AXI_LAW_AR_BASE + 3*4);

//    plb6_value = 1;
//    STW(plb6_value, AXI_LAW_UPDATE + 3*4);
////////////////////////////////////////
    //aurora
    plb6_value = 0x0;
    STW(plb6_value, AXI_LAW_BAR_BASE + 4*4);
    
    plb6_value = 0x0E;
    STW(plb6_value, AXI_LAW_BARH_BASE + 4*4);
    //4GB
    plb6_value = 1<<31 | 0x17;
    STW(plb6_value, AXI_LAW_AR_BASE + 4*4);

    plb6_value = 1;
    STW(plb6_value, AXI_LAW_UPDATE + 4*4);
////////////////////////////////////////
    //ddr
    plb6_value = 0x0;
    STW(plb6_value, AXI_LAW_BAR_BASE + 5*4);
    
    plb6_value = 0x0;
    STW(plb6_value, AXI_LAW_BARH_BASE + 5*4);
    //16GB
    plb6_value = 1<<31 | 0x19;
    STW(plb6_value, AXI_LAW_AR_BASE + 5*4);

//    plb6_value = 1;
//    STW(plb6_value, AXI_LAW_UPDATE + 5*4);
////////////////////////////////////////
    //cit0
    plb6_value = 0xFFFE0000;
    STW(plb6_value, AXI_LAW_BAR_BASE + 6*4);
    
    plb6_value = 0x4;
    STW(plb6_value, AXI_LAW_BARH_BASE + 6*4);
    //128KB
    plb6_value = 1<<31 | 0x8;
    STW(plb6_value, AXI_LAW_AR_BASE + 6*4);


    //cit1
    plb6_value = 0xE0000000;
    STW(plb6_value, AXI_LAW_BAR_BASE + 7*4);
    
    plb6_value = 0x4;
    STW(plb6_value, AXI_LAW_BARH_BASE + 7*4);
    //256MB
    plb6_value = 1<<31 | 0x13;
    STW(plb6_value, AXI_LAW_AR_BASE + 7*4);

//    plb6_value = 1;
//    STW(plb6_value, AXI_LAW_UPDATE + 7*4);
////////////////////////////////////////
   //cit2
    plb6_value = 0xF0000000;
    STW(plb6_value, AXI_LAW_BAR_BASE + 8*4);
    
    plb6_value = 0x4;
    STW(plb6_value, AXI_LAW_BARH_BASE + 8*4);
    //128MB
    plb6_value = 1<<31 | 0x12;
    STW(plb6_value, AXI_LAW_AR_BASE + 8*4);

//    plb6_value = 1;
//    STW(plb6_value, AXI_LAW_UPDATE + 8*4);
////////////////////////////////////////
    //cit3
    //dummy
    NOP10;
    ISYNC;
    plb6_value = 1;
    STW(plb6_value, AXI_LAW_UPDATE + 7*0);
    NOP10;
    ISYNC;
}
