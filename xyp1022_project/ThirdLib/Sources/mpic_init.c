/*------- general description ------- 
  All rights reserved by C*Core Corporation
  date:   2012.12.21
  author: swang
 
------ ------- ------- ------- */


#include "environment.h"
#include "mpic.h"
#include "gpio.h"
#define MPIC_GPIO_INT_NUM 50
#define MPIC_IIVPR_GPIO MPIC_IIVPR50

void mpic_unmask_Num(int VectorNum)
{
	unsigned int i, u;
	
	for(i=MPIC_IILR0;i<=MPIC_IILR70;i=i+0x20) {
	  *(volatile unsigned int*)(i) = MPIC_IILR_inttgt_int;
	  u = *(volatile unsigned int*)(i);
	}

	i = MPIC_IIVPR0+VectorNum*0x20;
	u = *(volatile unsigned int*)(i);
	*(volatile unsigned int*)(i) = u & 0x7fffffff;
}


#if 0
void mpic_init(void)
{
	unsigned unsigned int i,j,u;

/*
//mask on
	for(i=MPIC_EIVPR0;i<=MPIC_EIVPR11;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u|0x80000000;
	}

	for(i=MPIC_IIVPR0;i<=MPIC_IIVPR127;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u|0x80000000;
	}

	for(i=MPIC_IPIVPR0;i<=MPIC_IPIVPR3;i=i+0x10) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u|0x80000000;
	}

	for(i=MPIC_GTVPRA0;i<=MPIC_GTVPRA3;i=i+0x40) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u|0x80000000;
	}

	for(i=MPIC_GTVPRB0;i<=MPIC_GTVPRB3;i=i+0x40) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u|0x80000000;
	}

	for(i=MPIC_MIVPRA0;i<=MPIC_MIVPRA3;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u|0x80000000;
	}

	for(i=MPIC_MIVPRB0;i<=MPIC_MIVPRB3;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u|0x80000000;
	}

	for(i=MPIC_MSIVPRA0;i<=MPIC_MSIVPRA3;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u|0x80000000;
	}

	for(i=MPIC_MSIVPRB0;i<=MPIC_MSIVPRB3;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u|0x80000000;
	}

	for(i=MPIC_MSIVPRC0;i<=MPIC_MSIVPRC3;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u|0x80000000;
	}
*/

// set 128 IRQs, whose priority=10, vector=0~127
	j = MPIC_IIVPR_PRI_5;
	for(i=MPIC_IIVPR0;i<=MPIC_IIVPR31;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|u;
		//*(volatile unsigned int*)(i) = j;
		j++;
	}

	j = MPIC_IIVPR_PRI_4 + 32;
	for(i=MPIC_IIVPR32;i<=MPIC_IIVPR63;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|u;
		//*(volatile unsigned int*)(i) = j;
		j++;
	}

	j = MPIC_IIVPR_PRI_3 + 32 + 32;
	for(i=MPIC_IIVPR64;i<=MPIC_IIVPR95;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|u;
		//*(volatile unsigned int*)(i) = j;
		j++;
	}

	j = MPIC_IIVPR_PRI_2 + 32 + 32 + 32;
	for(i=MPIC_IIVPR96;i<=MPIC_IIVPR127;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|u;
		//*(volatile unsigned int*)(i) = j;
		j++;
	}

// set 12 EIRQs, whose priority=9, level sense & active high, vector=128~139
	j = MPIC_EIVPR_PRI_6 | MPIC_EIVPR_POR_HIGH | MPIC_EIVPR_SEN_LEVEL + 128;
	for(i=MPIC_EIVPR0;i<=MPIC_EIVPR11;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|u;
		//*(volatile unsigned int*)(i) = j;
		j++;
	}

// set ipi IRQs, whose priority=6, vector=140~143
	j = MPIC_IPIVPR_PRI_9 + 140;
	for(i=MPIC_IPIVPR0;i<=MPIC_IPIVPR3;i=i+0x10) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|u;
		//*(volatile unsigned int*)(i) = j;
		j++;
	}

// set internal timers IRQs groupA, whose priority=7, vector=144~147
	j = MPIC_GTVPRA_PRI_8 + 144;
	for(i=MPIC_GTVPRA0;i<=MPIC_GTVPRA3;i=i+0x40) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|u;
		//*(volatile unsigned int*)(i) = j;
		j++;
	}

// set internal timers IRQs groupB, whose priority=8, vector=148~151
	j = MPIC_GTVPRB_PRI_7 + 148;
	for(i=MPIC_GTVPRB0;i<=MPIC_GTVPRB3;i=i+0x40) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|u;
		//*(volatile unsigned int*)(i) = j;
		j++;
	}

// set msg IRQs groupA, whose priority=1, vector=152~155
	j = MPIC_MIVPRA_PRI_14 + 152;
	for(i=MPIC_MIVPRA0;i<=MPIC_MIVPRA3;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|u;
		//*(volatile unsigned int*)(i) = j;
		j++;
	}

// set msg IRQs groupB, whose priority=2, vector=156~159
	j = MPIC_MIVPRB_PRI_13 + 156;
	for(i=MPIC_MIVPRB0;i<=MPIC_MIVPRB3;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|u;
		//*(volatile unsigned int*)(i) = j;
		j++;
	}

// set msg share IRQs groupA, whose priority=3, vector=160~163
	j = MPIC_MSIVPRA_PRI_12 + 160;
	for(i=MPIC_MSIVPRA0;i<=MPIC_MSIVPRA3;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|u;
		//*(volatile unsigned int*)(i) = j;
		j++;
	}

// set msg share IRQs groupB, whose priority=4, vector=164~167
	j = MPIC_MSIVPRB_PRI_11 + 164;
	for(i=MPIC_MSIVPRB0;i<=MPIC_MSIVPRB3;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|u;
		//*(volatile unsigned int*)(i) = j;
		j++;
	}

// set msg share IRQs groupC, whose priority=5, vector=168~171
	j = MPIC_MSIVPRC_PRI_10 + 168;
	for(i=MPIC_MSIVPRC0;i<=MPIC_MSIVPRC3;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|u;
		//*(volatile unsigned int*)(i) = j;
		j++;
	}



// set CPU0 intr priority threshold to 0, priority 1~15 can trigger intr.  
	*(volatile unsigned int*)(MPIC_CTPR0) = MPIC_CTPR_PRI_0;  // clear MPIC_CTPR

	*(volatile unsigned int*)(MPIC_GCR) = MPIC_GCR_MM;    // enable MPIC mix_mode


// unmask
/*
	for(i=MPIC_EIVPR0;i<=MPIC_EIVPR11;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u & 0x7fffffff;
	}

	for(i=MPIC_IIVPR0;i<=MPIC_IIVPR127;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u & 0x7fffffff;
	}

	for(i=MPIC_IPIVPR0;i<=MPIC_IPIVPR3;i=i+0x10) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u & 0x7fffffff;
	}

	for(i=MPIC_GTVPRA0;i<=MPIC_GTVPRA3;i=i+0x40) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u & 0x7fffffff;
	}

	for(i=MPIC_GTVPRB0;i<=MPIC_GTVPRB3;i=i+0x40) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u & 0x7fffffff;
	}

	for(i=MPIC_MIVPRA0;i<=MPIC_MIVPRA3;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u & 0x7fffffff;
	}

	for(i=MPIC_MIVPRB0;i<=MPIC_MIVPRB3;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u & 0x7fffffff;
	}

	for(i=MPIC_MSIVPRA0;i<=MPIC_MSIVPRA3;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u & 0x7fffffff;
	}

	for(i=MPIC_MSIVPRB0;i<=MPIC_MSIVPRB3;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u & 0x7fffffff;
	}

	for(i=MPIC_MSIVPRC0;i<=MPIC_MSIVPRC3;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u & 0x7fffffff;
	}
*/
}
#endif

volatile unsigned long long   q;
volatile unsigned long long   q1;
volatile unsigned long long   p;
volatile unsigned long long   cnt;

volatile unsigned int sources_order1[15] = {0};
volatile unsigned int int_stack1[15] = {0};
volatile unsigned int handle_order1[30] = {0};
volatile unsigned int handle_order2[30] = {0};
volatile unsigned long long  msx_addr1[16]= {MPIC_MSIIRA,MPIC_MSIIRA,MPIC_MSIIRA,MPIC_MSIIRA,MPIC_MSIIRB,MPIC_MSIIRB,MPIC_MSIIRB,MPIC_MSIIRB,MPIC_MSGRA0,MPIC_MSGRA1,MPIC_MSGRA2,MPIC_MSGRA3,MPIC_MSGRB0,MPIC_MSGRB1,MPIC_MSGRB2,MPIC_MSGRB3};
volatile unsigned long long msx_trigger1[16]= {0,0x21000000,0x42000000,0x63000000,0,0x21000000,0x42000000,0x63000000,MPIC_MSGRA0,MPIC_MSGRA1,MPIC_MSGRA2,MPIC_MSGRA3,MPIC_MSGRB0,MPIC_MSGRB1,MPIC_MSGRB2,MPIC_MSGRB3};


void mpic_reset()
{
	unsigned int i,j,u=0x1234;
	*(volatile unsigned int*)(MPIC_GCR) = MPIC_GCR_RST;
	u = *(volatile unsigned int*)(MPIC_GCR);
	while((u&MPIC_GCR_RST)==MPIC_GCR_RST){
		u = *(volatile unsigned int*)(MPIC_GCR);
	}
}

void mpic_init(void)
{
	 unsigned int i,j,u;

// set 12 EIRQs, whose priority=9, level sense & active high, vector=0~11
	j = (MPIC_EIVPR_PRI_6 | MPIC_EIVPR_POR_HIGH | MPIC_EIVPR_SEN_LEVEL)+128;
	//j = MPIC_EIVPR_PRI_6 + 128;
	for(i=MPIC_EIVPR0;i<=MPIC_EIVPR11;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|0x80000000;
		//*(volatile unsigned int*)(i) = j|u;
		j++;
	}

// set 128 IRQs, whose priority=10, vector=12~139
	j = MPIC_IIVPR_PRI_5 ;
	for(i=MPIC_IIVPR0;i<=MPIC_IIVPR31;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|0x80000000;
		//*(volatile unsigned int*)(i) = j|u;
		j++;
	}

	j = MPIC_IIVPR_PRI_4  + 32;
	for(i=MPIC_IIVPR32;i<=MPIC_IIVPR63;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|0x80000000;
		//*(volatile unsigned int*)(i) = j|u;
		j++;
	}

	j = MPIC_IIVPR_PRI_3  + 32 + 32;
	for(i=MPIC_IIVPR64;i<=MPIC_IIVPR95;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|0x80000000;
		//*(volatile unsigned int*)(i) = j|u;
		j++;
	}

	j = MPIC_IIVPR_PRI_2  + 32 + 32 + 32;
	for(i=MPIC_IIVPR96;i<=MPIC_IIVPR127;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|0x80000000;
		//*(volatile unsigned int*)(i) = j|u;
		j++;
	}

// set ipi IRQs, whose priority=6, vector=140~143
	j = MPIC_IPIVPR_PRI_9 + 140;
	for(i=MPIC_IPIVPR0;i<=MPIC_IPIVPR3;i=i+0x10) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|0x80000000;
		//*(volatile unsigned int*)(i) = j|u;
		j++;
	}

// set internal timers IRQs groupA, whose priority=7, vector=144~147
	j = MPIC_GTVPRA_PRI_8 + 144;
	for(i=MPIC_GTVPRA0;i<=MPIC_GTVPRA3;i=i+0x40) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|0x80000000;
		//*(volatile unsigned int*)(i) = j|u;
		j++;
	}

// set internal timers IRQs groupB, whose priority=8, vector=148~151
	j = MPIC_GTVPRB_PRI_7 + 148;
	for(i=MPIC_GTVPRB0;i<=MPIC_GTVPRB3;i=i+0x40) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|0x80000000;
		//*(volatile unsigned int*)(i) = j|u;
		j++;
	}

// set msg IRQs groupA, whose priority=1, vector=152~155
	j = MPIC_MIVPRA_PRI_14 + 152;
	for(i=MPIC_MIVPRA0;i<=MPIC_MIVPRA3;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|0x80000000;
		//*(volatile unsigned int*)(i) = j|u;
		j++;
	}

// set msg IRQs groupB, whose priority=2, vector=156~159
	j = MPIC_MIVPRB_PRI_13 + 156;
	for(i=MPIC_MIVPRB0;i<=MPIC_MIVPRB3;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|0x80000000;
		//*(volatile unsigned int*)(i) = j|u;
		j++;
	}

// set msg share IRQs groupA, whose priority=3, vector=160~163
	j = MPIC_MSIVPRA_PRI_12 + 160;
	for(i=MPIC_MSIVPRA0;i<=MPIC_MSIVPRA3;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|0x80000000;
		//*(volatile unsigned int*)(i) = j|u;
		j++;
	}

// set msg share IRQs groupB, whose priority=4, vector=164~167
	j = MPIC_MSIVPRB_PRI_11 + 164;
	for(i=MPIC_MSIVPRB0;i<=MPIC_MSIVPRB3;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|0x80000000;
		//*(volatile unsigned int*)(i) = j|u;
		j++;
	}

// set msg share IRQs groupC, whose priority=5, vector=168~171
	j = MPIC_MSIVPRC_PRI_10 + 168;
	for(i=MPIC_MSIVPRC0;i<=MPIC_MSIVPRC3;i=i+0x20) {
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = j|0x80000000;
		//*(volatile unsigned int*)(i) = j|u;
		j++;
	}



// set CPU0 intr priority threshold to 0, priority 1~15 can trigger intr.  
	*(volatile unsigned int*)(MPIC_CTPR0) = MPIC_CTPR_PRI_0;  // clear MPIC_CTPR
	*(volatile unsigned int*)(MPIC_CTPR1) = MPIC_CTPR_PRI_0;  // clear MPIC_CTPR
	*(volatile unsigned int*)(MPIC_CTPR2) = MPIC_CTPR_PRI_0;  // clear MPIC_CTPR
	*(volatile unsigned int*)(MPIC_CTPR3) = MPIC_CTPR_PRI_0;  // clear MPIC_CTPR

	*(volatile unsigned int*)(MPIC_GCR) = MPIC_GCR_MM;    // enable MPIC mix_mode


}

void mpic_unmask(void)
{
	 unsigned int i,j,u;

	 
	for(i=MPIC_IILR0;i<=MPIC_IILR70;i=i+0x20) {
		*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_int;
		u = *(volatile unsigned int*)(i);
	}
	for(i=MPIC_IILR74;i<=MPIC_IILR127;i=i+0x20) {
		*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_cint;
		u = *(volatile unsigned int*)(i);
	}
	for(i=MPIC_EILR0;i<=MPIC_EILR11;i=i+0x20) {
		*(volatile unsigned int*)(i) = MPIC_EILR_inttgt_cint;
		u = *(volatile unsigned int*)(i);
	}
	
	// set 128 IRQs, whose priority=10, vector=12~139
	 for(i=MPIC_IIVPR0;i<=MPIC_IIVPR127;i=i+0x20) {       //umask iic int
	  	u = *(volatile unsigned int*)(i);
	  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
	 }
	
	 // set 12 EIRQs, whose priority=9, level sense & active high, vector=0~11
	 for(i=MPIC_IIVPR0;i<=MPIC_EIVPR11;i=i+0x20) {      
	  	u = *(volatile unsigned int*)(i);
	  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
	 }

	 // set ipi IRQs, whose priority=6, vector=140~143
	 for(i=MPIC_IPIVPR0;i<=MPIC_IPIVPR3;i=i+0x10) {      
	  	u = *(volatile unsigned int*)(i);
	  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
	 }

	 // set internal timers IRQs groupA, whose priority=7, vector=144~147
	 for(i=MPIC_GTVPRA0;i<=MPIC_GTVPRA3;i=i+0x40) {      
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u & 0x7fffffff;
	 }

	 // set internal timers IRQs groupB, whose priority=8, vector=148~151
	 for(i=MPIC_GTVPRB0;i<=MPIC_GTVPRB3;i=i+0x40) {      
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u & 0x7fffffff;
	 }
	 
	 // set msg IRQs groupA, whose priority=1, vector=152~155
	 for(i=MPIC_MIVPRA0;i<=MPIC_MIVPRA3;i=i+0x20) {      
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u & 0x7fffffff;
	 }
	 
	 // set msg IRQs groupB, whose priority=2, vector=156~159
	 for(i=MPIC_MIVPRB0;i<=MPIC_MIVPRB3;i=i+0x20) {      
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u & 0x7fffffff;
	 }

	 // set msg share IRQs groupA, whose priority=3, vector=160~163
	 for(i=MPIC_MSIVPRA0;i<=MPIC_MSIVPRA3;i=i+0x20) {      
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u & 0x7fffffff;
	 }

	 // set msg share IRQs groupB, whose priority=4, vector=164~167
	 for(i=MPIC_MSIVPRB0;i<=MPIC_MSIVPRB3;i=i+0x20) {      
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u & 0x7fffffff;
	 }

	 // set msg share IRQs groupC, whose priority=5, vector=168~171
	 for(i=MPIC_MSIVPRC0;i<=MPIC_MSIVPRC3;i=i+0x20) {      
		u = *(volatile unsigned int*)(i);
		*(volatile unsigned int*)(i) = u & 0x7fffffff;
	 }
}


static void eirq0_routine()
{
  q=q+1;

 // while(1);
  //failed();
}
volatile unsigned int   gpio_cint_flag;
static void gpio_cint_isr(void)
{
	unsigned int temp = 0;


	gpio_cint_flag = gpio_cint_flag+1;
	unsigned int i,u;
	  for(i=MPIC_IIVPR_GPIO;i<=MPIC_IIVPR_GPIO;i=i+0x20) {       //irq
	  	u = *(volatile unsigned int*)(i);
	  	*(volatile unsigned int*)(i) = u | 0x80000000;
	  }

	uart_printf_buf("m gpio  cint inter pass!\n");

}

void mpic_cint (void)
  /* main program */
{
	unsigned int i,j,u;
	//InstallInterruptHandler0( 192 ,spi_modf_int_isr);

 // InstallInterruptHandler0( 192 ,eirq0_routine);
//  InstallInterruptHandler0( 1 ,eirq1_routine);


  q=0;


  //---- mpic initial config ----//
  //mpic_init();

  //---- change config ---------//
  // enable MPIC pass-through mode	
  //*(volatile unsigned int*)(MPIC_GCR) = MPIC_GCR_PTM;    

  
  // set target for cint
  for(i=MPIC_IILR0;i<=MPIC_IILR70;i=i+0x20) {
  	*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_cint;
	u = *(volatile unsigned int*)(i);
  }
  for(i=MPIC_IILR74;i<=MPIC_IILR127;i=i+0x20) {
  	*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_cint;
	u = *(volatile unsigned int*)(i);
  }
  for(i=MPIC_EILR0;i<=MPIC_EILR11;i=i+0x20) {
  	*(volatile unsigned int*)(i) = MPIC_EILR_inttgt_cint;
	u = *(volatile unsigned int*)(i);
  }

  //tmpa = *(volatile unsigned int*)(MPIC_IRQSIESR0); //before trig
  //if (tmpa!=0)  failed();
  //tmpb = *(volatile unsigned int*)(MPIC_IRQSIESR1); //before trig
  //if (tmpb!=0)  failed();

  // unmask
/*
  for(i=MPIC_IIVPR0;i<=MPIC_IIVPR0;i=i+0x20) {       //irq
  	u = *(volatile unsigned int*)(i);
  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
  }
  */
  InstallInterruptHandler0( 192 ,gpio_cint_isr);
  gpio_cint_flag = 0;
  for(i=MPIC_IIVPR_GPIO;i<=MPIC_IIVPR_GPIO;i=i+0x20) {       //irq
  	u = *(volatile unsigned int*)(i);
  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
  }

	while(gpio_cint_flag==0);
  gpio_cint_flag = 0;
  //while(1);
  //spi_modf_cint_test();

  
}

static void ipi_irq0_routine()
{
	unsigned int i,j,u;
	q=q+1;
  	//int mask
	for(i=MPIC_IPIVPR0;i<=MPIC_IPIVPR0;i=i+0x10) {
		u = *(volatile unsigned int*)(i);
  		*(volatile unsigned int*)(i) = u | 0x80000000;
	}

   	//passed();
}

static void ipi_irq1_routine()
{
	unsigned int i,j,u;
	q=q+1;
  	//int mask
	for(i=MPIC_IPIVPR1;i<=MPIC_IPIVPR1;i=i+0x10) {
		u = *(volatile unsigned int*)(i);
  		*(volatile unsigned int*)(i) = u | 0x80000000;
	}

   	//passed();
}

static void ipi_irq2_routine()
{
	unsigned int i,j,u;
	q=q+1;
  	//int mask
	for(i=MPIC_IPIVPR2;i<=MPIC_IPIVPR2;i=i+0x10) {
		u = *(volatile unsigned int*)(i);
  		*(volatile unsigned int*)(i) = u | 0x80000000;
	}

   	//passed();
}

static void ipi_irq3_routine()
{
	unsigned int i,j,u;
	q=q+1;
  	//int mask
	for(i=MPIC_IPIVPR3;i<=MPIC_IPIVPR3;i=i+0x10) {
		u = *(volatile unsigned int*)(i);
  		*(volatile unsigned int*)(i) = u | 0x80000000;
	}


	if (q!=0x4)  while(1);
}

void mpic_ipi ()
  /* main program */
{
	unsigned int i,j,u;
	//InstallInterruptHandler0( 0 ,ipi_irq0_routine);
	InstallInterruptHandler0( 140 ,ipi_irq0_routine);
	InstallInterruptHandler0( 141 ,ipi_irq1_routine);
	InstallInterruptHandler0( 142 ,ipi_irq2_routine);
	InstallInterruptHandler0( 143 ,ipi_irq3_routine);

	q=0;

  //---- mpic initial config ----//
  //mpic_init();

  //---- change config ---------//

  // int unmask
  for(i=MPIC_IPIVPR0;i<=MPIC_IPIVPR3;i=i+0x10) {
  	u = *(volatile unsigned int*)(i);
  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
  }

  //int en
  for(i=MPIC_CPU0_IPIDR0;i<=MPIC_CPU0_IPIDR3;i=i+0x10) {        
 // 	u = *(volatile unsigned int*)(i);

  	*(volatile unsigned int*)(i) = 0x1; 

  	
  }
  while(q!=4);


}
static void gt_irq0_routine()
{
	unsigned int i,j,u;
	q=q+1;
  	// int mask,A0
  	for(i=MPIC_GTVPRA0;i<=MPIC_GTVPRA0;i=i+0x40) {        
  		u = *(volatile unsigned int*)(i);
  		*(volatile unsigned int*)(i) = u | 0x80000000;
  	}

   	//passed();
}

static void gt_irq1_routine()
{
	unsigned int i,j,u;
	q=q+1;
  	// int mask,A1
  	for(i=MPIC_GTVPRA1;i<=MPIC_GTVPRA1;i=i+0x40) {        
  		u = *(volatile unsigned int*)(i);
  		*(volatile unsigned int*)(i) = u | 0x80000000;
  	}

   	//passed();
}

static void gt_irq2_routine()
{
	unsigned int i,j,u;
	q=q+1;
  	// int mask,A2
  	for(i=MPIC_GTVPRA2;i<=MPIC_GTVPRA2;i=i+0x40) {        
  		u = *(volatile unsigned int*)(i);
  		*(volatile unsigned int*)(i) = u | 0x80000000;
  	}

   	//passed();
}

static void gt_irq3_routine()
{
	unsigned int i,j,u;
	q=q+1;
  	// int mask,A3
  	for(i=MPIC_GTVPRA3;i<=MPIC_GTVPRA3;i=i+0x40) {        
  		u = *(volatile unsigned int*)(i);
  		*(volatile unsigned int*)(i) = u | 0x80000000;
  	}

   	//passed();
}


static void gt_irq4_routine()
{
	unsigned int i,j,u;
	q=q+1;
  	// int mask,B0
  	for(i=MPIC_GTVPRB0;i<=MPIC_GTVPRB0;i=i+0x40) {        
  		u = *(volatile unsigned int*)(i);
  		*(volatile unsigned int*)(i) = u | 0x80000000;
  	}


	
   	//passed();
}

static void gt_irq5_routine()
{
	unsigned int i,j,u;
	q=q+1;
  	// int mask,B1
  	for(i=MPIC_GTVPRB1;i<=MPIC_GTVPRB1;i=i+0x40) {        
  		u = *(volatile unsigned int*)(i);
  		*(volatile unsigned int*)(i) = u | 0x80000000;
  	}



   	//passed();
}

static void gt_irq6_routine()
{
	unsigned int i,j,u;
	q=q+1;
  	// int mask,B2
  	for(i=MPIC_GTVPRB2;i<=MPIC_GTVPRB2;i=i+0x40) {        
  		u = *(volatile unsigned int*)(i);
  		*(volatile unsigned int*)(i) = u | 0x80000000;
  	}


	
   	//passed();
}

static void gt_irq7_routine()
{
	unsigned int i,j,u;
	q=q+1;
  	// int mask,B3
  	for(i=MPIC_GTVPRB3;i<=MPIC_GTVPRB3;i=i+0x40) {        
  		u = *(volatile unsigned int*)(i);
  		*(volatile unsigned int*)(i) = u | 0x80000000;
  	}



   	//passed();
}

void mpic_gt()
  /* main program */
{
	unsigned int i,j,u;

	InstallInterruptHandler0( 144 ,gt_irq0_routine);
	InstallInterruptHandler0( 145 ,gt_irq1_routine);
	InstallInterruptHandler0( 146 ,gt_irq2_routine);
	InstallInterruptHandler0( 147 ,gt_irq3_routine);
	InstallInterruptHandler0( 148 ,gt_irq4_routine);
	InstallInterruptHandler0( 149 ,gt_irq5_routine);
	InstallInterruptHandler0( 150 ,gt_irq6_routine);
	InstallInterruptHandler0( 151 ,gt_irq7_routine);

	q=0;

  //---- mpic initial config ----//
  //mpic_init();

  //---- change config ---------//
  //timers not cascaded , reload with base cnt
  *(volatile unsigned int*)(MPIC_TCRA) = 0x0;    
  u = *(volatile unsigned int*)(MPIC_TCRA);
  *(volatile unsigned int*)(MPIC_TCRB) = 0x0;    
  u = *(volatile unsigned int*)(MPIC_TCRB);

  // int unmask,A/B
  for(i=MPIC_GTVPRA0;i<=MPIC_GTVPRA3;i=i+0x40) {        
  	u = *(volatile unsigned int*)(i);
  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
  }
  for(i=MPIC_GTVPRB0;i<=MPIC_GTVPRB3;i=i+0x40) {        
  	u = *(volatile unsigned int*)(i);
  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
  }

  //base cnt, and cnt en
  for(i=MPIC_GTBCRA0;i<=MPIC_GTBCRA3;i=i+0x40) {        
  	u = *(volatile unsigned int*)(i);
  	*(volatile unsigned int*)(i) = u | 0x00000900; //base cnt is 0x900
  }
  for(i=MPIC_GTBCRB0;i<=MPIC_GTBCRB3;i=i+0x40) {        
  	u = *(volatile unsigned int*)(i);
  	*(volatile unsigned int*)(i) = u | 0x00000900; //base cnt is 0x900
  }


  for(i=MPIC_GTBCRA0;i<=MPIC_GTBCRA3;i=i+0x40) {       
	  q1 = q;
  	u = *(volatile unsigned int*)(i);
  	*(volatile unsigned int*)(i) = u & 0x7fffffff; //cnt en
  	while(q1 == q);

  }
  
  for(i=MPIC_GTBCRB0;i<=MPIC_GTBCRB3;i=i+0x40) {   
	  q1 = q;
  	u = *(volatile unsigned int*)(i);
  	*(volatile unsigned int*)(i) = u & 0x7fffffff; //cnt en
  	while(q1 == q);

  }
  while(q!=8);


}

static void msg_irq0_routine()
{
	unsigned int i,j,u;
	q=q+1;
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSRA);
	if ((u&0x1)!=0x1)  while(1);

	*(volatile unsigned int*)(MPIC_MSRA) = 0x1;

   	//passed();

}

static void msg_irq0_routine1()
{
	unsigned int i,j,u;
	
	handle_order1[q] = 8;
	q = q + 1;
	q1 = q1 +1;
	
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSRA);
	*(volatile unsigned int*)(MPIC_MSRA) = u;
	
	if(q1 < 15)
	{
		*(volatile unsigned int*)msx_addr1[sources_order1[q1]] = msx_trigger1[sources_order1[q1]];
		delay(100);
	}

	handle_order1[q] = 0x18;
	q = q + 1;
   	//passed();
}
static void msg_irq1_routine1()
{
	unsigned int i,j,u;
	
	handle_order1[q] = 9;
	q = q + 1;
	q1 = q1 +1;
	
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSRA);
	*(volatile unsigned int*)(MPIC_MSRA) = u;
	
	if(q1 < 15)
	{
		*(volatile unsigned int*)msx_addr1[sources_order1[q1]] = msx_trigger1[sources_order1[q1]];
		delay(100);
	}

	handle_order1[q] = 0x19;
	q = q + 1;
   	//passed();
}
static void msg_irq2_routine1()
{
	unsigned int i,j,u;
	
	handle_order1[q] = 0xA;
	q = q + 1;
	q1 = q1 +1;
	
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSRA);
	*(volatile unsigned int*)(MPIC_MSRA) = u;
	
	if(q1 < 15)
	{
		*(volatile unsigned int*)msx_addr1[sources_order1[q1]] = msx_trigger1[sources_order1[q1]];
		delay(100);
	}

	handle_order1[q] = 0x1A;
	q = q + 1;
   	//passed();
}
static void msg_irq3_routine1()
{
	unsigned int i,j,u;
	
	handle_order1[q] = 0xB;
	q = q + 1;
	q1 = q1 +1;
	
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSRA);
	*(volatile unsigned int*)(MPIC_MSRA) = u;
	
	if(q1 < 15)
	{
		*(volatile unsigned int*)msx_addr1[sources_order1[q1]] = msx_trigger1[sources_order1[q1]];
		delay(100);
	}

	handle_order1[q] = 0x1B;
	q = q + 1;
   	//passed();
}
static void msg_irq4_routine1()
{
	unsigned int i,j,u;
	
	handle_order1[q] = 0xC;
	q = q + 1;
	q1 = q1 +1;
	
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSRB);
	*(volatile unsigned int*)(MPIC_MSRB) = u;
	
	if(q1 < 15)
	{
		*(volatile unsigned int*)msx_addr1[sources_order1[q1]] = msx_trigger1[sources_order1[q1]];
		delay(100);
	}

	handle_order1[q] = 0x1C;
	q = q + 1;
   	//passed();
}
static void msg_irq5_routine1()
{
	unsigned int i,j,u;
	
	handle_order1[q] = 0xD;
	q = q + 1;
	q1 = q1 +1;
	
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSRB);
	*(volatile unsigned int*)(MPIC_MSRB) = u;
	
	if(q1 < 15)
	{
		*(volatile unsigned int*)msx_addr1[sources_order1[q1]] = msx_trigger1[sources_order1[q1]];
		delay(100);
	}

	handle_order1[q] = 0x1D;
	q = q + 1;
   	//passed();
}
static void msg_irq6_routine1()
{
	unsigned int i,j,u;
	
	handle_order1[q] = 0xE;
	q = q + 1;
	q1 = q1 +1;
	
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSRB);
	*(volatile unsigned int*)(MPIC_MSRB) = u;
	
	if(q1 < 15)
	{
		*(volatile unsigned int*)msx_addr1[sources_order1[q1]] = msx_trigger1[sources_order1[q1]];
		delay(100);
	}

	handle_order1[q] = 0x1E;
	q = q + 1;
   	//passed();
}
static void msg_irq7_routine1()
{
	unsigned int i,j,u;
	
	handle_order1[q] = 0xF;
	q = q + 1;
	q1 = q1 +1;
	
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSRB);
	*(volatile unsigned int*)(MPIC_MSRB) = u;
	
	if(q1 < 15)
	{
		*(volatile unsigned int*)msx_addr1[sources_order1[q1]] = msx_trigger1[sources_order1[q1]];
		delay(100);
	}

	handle_order1[q] = 0x1F;
	q = q + 1;
   	//passed();
}
static void msg_irq1_routine()
{
	unsigned int i,j,u;
	q=q+1;
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSRA);
	if ((u&0x2)!=0x2)  while(1);

	*(volatile unsigned int*)(MPIC_MSRA) = 0x2;

   	//passed();
}

static void msg_irq2_routine()
{
	unsigned int i,j,u;
	q=q+1;
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSRA);
	if ((u&0x4)!=0x4)  while(1);

	*(volatile unsigned int*)(MPIC_MSRA) = 0x4;

   	//passed();
}

static void msg_irq3_routine()
{
	unsigned int i,j,u;
	q=q+1;
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSRA);
	if ((u&0x8)!=0x8)  while(1);

	*(volatile unsigned int*)(MPIC_MSRA) = 0x8;
	
   	//passed();
}

static void msg_irq4_routine()
{
	unsigned int i,j,u;
	q=q+1;
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSRB);
	if ((u&0x1)!=0x1)  while(1);

	*(volatile unsigned int*)(MPIC_MSRB) = 0x1;

   	//passed();
}

static void msg_irq5_routine()
{
	unsigned int i,j,u;
	q=q+1;
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSRB);
	if ((u&0x2)!=0x2)  while(1);

	*(volatile unsigned int*)(MPIC_MSRB) = 0x2;

   	//passed();
}

static void msg_irq6_routine()
{
	unsigned int i,j,u;
	q=q+1;
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSRB);
	if ((u&0x4)!=0x4)  while(1);

	*(volatile unsigned int*)(MPIC_MSRB) = 0x4;

   	//passed();
}

static void msg_irq7_routine()
{
	unsigned int i,j,u;
	q=q+1;
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSRB);
	if ((u&0x8)!=0x8)  while(1);

	*(volatile unsigned int*)(MPIC_MSRB) = 0x8;


}

void mpic_msg()
  /* main program */
{
	unsigned int i,j,u;

	InstallInterruptHandler0( 152 ,msg_irq0_routine);
	InstallInterruptHandler0( 153 ,msg_irq1_routine);
	InstallInterruptHandler0( 154 ,msg_irq2_routine);
	InstallInterruptHandler0( 155 ,msg_irq3_routine);
	InstallInterruptHandler0( 156 ,msg_irq4_routine);
	InstallInterruptHandler0( 157 ,msg_irq5_routine);
	InstallInterruptHandler0( 158 ,msg_irq6_routine);
	InstallInterruptHandler0( 159 ,msg_irq7_routine);

	q=0;

  //---- mpic initial config ----//
  //mpic_init();

  //---- change config ---------//

  // int unmask
  for(i=MPIC_MIVPRA0;i<=MPIC_MIVPRA3;i=i+0x20) {
  	u = *(volatile unsigned int*)(i);
  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
  }
  for(i=MPIC_MIVPRB0;i<=MPIC_MIVPRB3;i=i+0x20) {
  	u = *(volatile unsigned int*)(i);
  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
  }

  //int en
  *(volatile unsigned int*)(MPIC_MERA) = 0xf;
  *(volatile unsigned int*)(MPIC_MERB) = 0xf;

  for(i=MPIC_MSGRA0;i<=MPIC_MSGRA3;i=i+0x10) {
	  q1 = q;
  	*(volatile unsigned int*)(i) = i;
  	while(q1 == q);

  }
  
  for(i=MPIC_MSGRB0;i<=MPIC_MSGRB3;i=i+0x10) {
	  
	  q1 = q;
  	*(volatile unsigned int*)(i) = i;
 	while(q1 == q);

  }
  

  while(q!=8);

}

//__declspec (msi_irq0_routine1())
//__InterruptHandler__(void) ;

static void msi_irq0_routine1()
{
	unsigned int i,j,u;
	
	handle_order1[q] = 0;
	q = q + 1;
	q1 = q1 +1;
	
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSIRA0);
		
	if(q1 < 15)
	{
		*(volatile unsigned int*)msx_addr1[sources_order1[q1]] = msx_trigger1[sources_order1[q1]];
		delay(100);
	}

	handle_order1[q] = 0x10;
	q = q + 1;
   	//passed();
}
static void msi_irq1_routine1()
{
	unsigned int i,j,u;
	
	handle_order1[q] = 1;
	q = q + 1;
	q1 = q1 +1;
	
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSIRA1);
		
	if(q1 < 15)
	{
		*(volatile unsigned int*)msx_addr1[sources_order1[q1]] = msx_trigger1[sources_order1[q1]];
		delay(100);
	}

	handle_order1[q] = 0x11;
	q = q + 1;
   	//passed();
}
static void msi_irq2_routine1()
{
	unsigned int i,j,u;
	
	handle_order1[q] = 2;
	q = q + 1;
	q1 = q1 +1;
	
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSIRA2);
		
	if(q1 < 15)
	{
		*(volatile unsigned int*)msx_addr1[sources_order1[q1]] = msx_trigger1[sources_order1[q1]];
		delay(100);
	}

	handle_order1[q] = 0x12;
	q = q + 1;
   	//passed();
}
static void msi_irq3_routine1()
{
	unsigned int i,j,u;
	
	handle_order1[q] = 3;
	q = q + 1;
	q1 = q1 +1;
	
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSIRA3);
		
	if(q1 < 15)
	{
		*(volatile unsigned int*)msx_addr1[sources_order1[q1]] = msx_trigger1[sources_order1[q1]];
		delay(100);
	}

	handle_order1[q] = 0x13;
	q = q + 1;
   	//passed();
}
static void msi_irq4_routine1()
{
	unsigned int i,j,u;
	
	handle_order1[q] = 4;
	q = q + 1;
	q1 = q1 +1;
	
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSIRB0);
		
	if(q1 < 15)
	{
		*(volatile unsigned int*)msx_addr1[sources_order1[q1]] = msx_trigger1[sources_order1[q1]];
		delay(100);
	}

	handle_order1[q] = 0x14;
	q = q + 1;
   	//passed();
}
static void msi_irq5_routine1()
{
	unsigned int i,j,u;
	
	handle_order1[q] = 5;
	q = q + 1;
	q1 = q1 +1;
	
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSIRB1);
		
	if(q1 < 15)
	{
		*(volatile unsigned int*)msx_addr1[sources_order1[q1]] = msx_trigger1[sources_order1[q1]];
		delay(100);
	}

	handle_order1[q] = 0x15;
	q = q + 1;
   	//passed();
}
static void msi_irq6_routine1()
{
	unsigned int i,j,u;
	
	handle_order1[q] = 6;
	q = q + 1;
	q1 = q1 +1;
	
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSIRB2);
		
	if(q1 < 15)
	{
		*(volatile unsigned int*)msx_addr1[sources_order1[q1]] = msx_trigger1[sources_order1[q1]];
		delay(100);
	}

	handle_order1[q] = 0x16;
	q = q + 1;
   	//passed();
}
static void msi_irq7_routine1()
{
	unsigned int i,j,u;
	
	handle_order1[q] = 7;
	q = q + 1;
	q1 = q1 +1;
	
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSIRB3);
		
	if(q1 < 15)
	{
		*(volatile unsigned int*)msx_addr1[sources_order1[q1]] = msx_trigger1[sources_order1[q1]];
		delay(100);
	}

	handle_order1[q] = 0x17;
	q = q + 1;
   	//passed();
}
static void msi_irq0_routine()
{	unsigned int i,j,u;
	q=q+1;
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSIRA0);

   	//passed();
}

static void msi_irq1_routine()
{	unsigned int i,j,u;
	q=q+1;
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSIRA1);

   	//passed();
}

static void msi_irq2_routine()
{	unsigned int i,j,u;
	q=q+1;
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSIRA2);

   	//passed();
}

static void msi_irq3_routine()
{	unsigned int i,j,u;
	q=q+1;
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSIRA3);

   	//passed();
}

static void msi_irq4_routine()
{	unsigned int i,j,u;
	q=q+1;
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSIRB0);

   	//passed();
}

static void msi_irq5_routine()
{	unsigned int i,j,u;
	q=q+1;
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSIRB1);

   	//passed();
}

static void msi_irq6_routine()
{	unsigned int i,j,u;
	q=q+1;
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSIRB2);

   	//passed();
}

static void msi_irq7_routine()
{	unsigned int i,j,u;
	q=q+1;
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSIRB3);

   	//passed();
}

static void msi_irq8_routine()
{	unsigned int i,j,u;
	q=q+1;
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSIRC0);

   	//passed();
}

static void msi_irq9_routine()
{	unsigned int i,j,u;
	q=q+1;
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSIRC1);

   	//passed();
}

static void msi_irq10_routine()
{	unsigned int i,j,u;
	q=q+1;
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSIRC2);

   	//passed();
}

static void msi_irq11_routine()
{	unsigned int i,j,u;
	q=q+1;
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSIRC3);



}
void mpic_msi()
  /* main program */
{
	unsigned int i,j,u;
	InstallInterruptHandler0( 160 ,msi_irq0_routine); //A
	InstallInterruptHandler0( 161 ,msi_irq1_routine);
	InstallInterruptHandler0( 162 ,msi_irq2_routine);
	InstallInterruptHandler0( 163 ,msi_irq3_routine);

	InstallInterruptHandler0( 164 ,msi_irq4_routine); //B
	InstallInterruptHandler0( 165 ,msi_irq5_routine);
	InstallInterruptHandler0( 166 ,msi_irq6_routine);
	InstallInterruptHandler0( 167 ,msi_irq7_routine);

	InstallInterruptHandler0( 168 ,msi_irq8_routine); //C
	InstallInterruptHandler0( 169 ,msi_irq9_routine);
	InstallInterruptHandler0( 170 ,msi_irq10_routine);
	InstallInterruptHandler0( 171 ,msi_irq11_routine);

	q=0;

  //---- mpic initial config ----//
  //mpic_init();

  //---- change config ---------//

  // int unmask
  for(i=MPIC_MSIVPRA0;i<=MPIC_MSIVPRA3;i=i+0x20) {
  	u = *(volatile unsigned int*)(i);
  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
  }
  for(i=MPIC_MSIVPRB0;i<=MPIC_MSIVPRB3;i=i+0x20) {
  	u = *(volatile unsigned int*)(i);
  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
  }
  for(i=MPIC_MSIVPRC0;i<=MPIC_MSIVPRC3;i=i+0x20) {
  	u = *(volatile unsigned int*)(i);
  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
  }

  //int en
  for (i=0; i<4; i++) {
  	//j = rand()%32;
  	j = i*9;
	  q1 = q;
  	*(volatile unsigned int*)(MPIC_MSIIRA) = i*0x20000000+j*0x1000000;
  	while(q1 == q);
  }
  
  for (i=0; i<4; i++) {
  	//j = rand()%32;
  	j = i*9;
	  q1 = q;
  	*(volatile unsigned int*)(MPIC_MSIIRB) = i*0x20000000+j*0x1000000;
  	while(q1 == q);
  }
  
  for (i=0; i<4; i++) {
  	//j = rand()%32;
  	j = i*9;
	  q1 = q;
  	*(volatile unsigned int*)(MPIC_MSIIRC) = i*0x20000000+j*0x1000000;
  	while(q1 == q);
  }
  while(q!= 0xc);

}

volatile unsigned int   gpio_mcp_flag;
static void gpio_mcp_isr(void)
{
	unsigned int temp = 0;


	gpio_mcp_flag = gpio_mcp_flag+1;
	unsigned int i,u;
	  for(i=MPIC_IIVPR_GPIO;i<=MPIC_IIVPR_GPIO;i=i+0x20) {       //irq
	  	u = *(volatile unsigned int*)(i);
	  	*(volatile unsigned int*)(i) = u | 0x80000000;
	  }

	uart_printf_buf("m gpio mcp  pass!\n");

}

void mpic_mcp()
  /* main program */
{
	 unsigned int i,j,u;


  //InstallInterruptHandler0( 200 ,eirq0_routine);
//  InstallInterruptHandler0( 1 ,eirq1_routine);


  q=0;


  //---- mpic initial config ----//
  //mpic_init();

  //---- change config ---------//
  // enable MPIC pass-through mode	
  //*(volatile unsigned int*)(MPIC_GCR) = MPIC_GCR_PTM;    

  // set target for mcp
  for(i=MPIC_IILR0;i<=MPIC_IILR70;i=i+0x20) {
  	*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_mcp;
	u = *(volatile unsigned int*)(i);
  }
  for(i=MPIC_IILR74;i<=MPIC_IILR127;i=i+0x20) {
  	*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_mcp;
	u = *(volatile unsigned int*)(i);
  }
  for(i=MPIC_EILR0;i<=MPIC_EILR11;i=i+0x20) {
  	*(volatile unsigned int*)(i) = MPIC_EILR_inttgt_mcp;
	u = *(volatile unsigned int*)(i);
  }

  //tmpa = *(volatile unsigned int*)(MPIC_IRQSIESR0); //before trig
  //if (tmpa!=0)  failed();
  //tmpb = *(volatile unsigned int*)(MPIC_IRQSIESR1); //before trig
  //if (tmpb!=0)  failed();

  // unmask

  InstallInterruptHandler0( 193 ,gpio_mcp_isr);
  gpio_mcp_flag = 0;
  for(i=MPIC_IIVPR_GPIO;i<=MPIC_IIVPR_GPIO;i=i+0x20) {       //irq
  	u = *(volatile unsigned int*)(i);
  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
  }

	while(gpio_mcp_flag==0);
  gpio_mcp_flag = 0;


}

volatile unsigned int   gpio_p_thr_flag;
static void gpio_p_thr_isr(void)
{
	unsigned int temp = 0;


	gpio_p_thr_flag = gpio_p_thr_flag+1;
	unsigned int i,u;
	  for(i=MPIC_IIVPR_GPIO;i<=MPIC_IIVPR_GPIO;i=i+0x20) {       //irq
	  	u = *(volatile unsigned int*)(i);
	  	*(volatile unsigned int*)(i) = u | 0x80000000;
	  }

	uart_printf_buf("m gpio  p_thr inter pass!\n");

}
void mpic_int_pass_through()
  /* main program */
{
	 unsigned int i,j,u;

  q=0;

    u = *(volatile unsigned int*)(MPIC_IRQSIESR4);
  *(volatile unsigned int*)(MPIC_GCR) = MPIC_GCR_PTM;    
  u = *(volatile unsigned int*)(MPIC_GCR);
  // set target for mcp
  for(i=MPIC_IILR0;i<=MPIC_IILR70;i=i+0x20) {
  	*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_IO;
	u = *(volatile unsigned int*)(i);
  }
  for(i=MPIC_IILR71;i<=MPIC_IILR127;i=i+0x20) {
  	*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_IO;
	u = *(volatile unsigned int*)(i);
  }

  u = *(volatile unsigned int*)(MPIC_GCR);
  // unmask

  for(i=MPIC_IIVPR_GPIO;i<=MPIC_IIVPR_GPIO;i=i+0x20) {       //irq
  	u = *(volatile unsigned int*)(i);
  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
  }
  InstallInterruptHandler0( MPIC_GPIO_INT_NUM ,gpio_p_thr_isr);
  //spi_modf_p_thr_test();
  u = *(volatile unsigned int*)(MPIC_IRQSIESR4);
  while((u|0x80000000)==0)
    {	
      u = *(volatile unsigned int*)(MPIC_IRQSIESR4);
    }
}

void mpic_int_proxy()
  /* main program */
{
	 unsigned int i,j,u;

  q=0;

  
  __asm__ (
  	    "li	 6,_NCINTR_PREHANDLER@l \n"
  	    "mtspr	 0x194,6 \n"		
  );
  
  *(volatile unsigned int*)(MPIC_GCR) = MPIC_GCR_EPM;    
  u = *(volatile unsigned int*)(MPIC_GCR);
  // set target for mcp
  for(i=MPIC_IILR0;i<=MPIC_IILR70;i=i+0x20) {
  	*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_int;
	u = *(volatile unsigned int*)(i);
  }
  for(i=MPIC_IILR71;i<=MPIC_IILR127;i=i+0x20) {
  	*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_int;
	u = *(volatile unsigned int*)(i);
  }
  for(i=MPIC_EILR0;i<=MPIC_EILR11;i=i+0x20) {
  	*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_int;
	u = *(volatile unsigned int*)(i);
  }
  // unmask

  for(i=MPIC_IIVPR_GPIO;i<=MPIC_IIVPR_GPIO;i=i+0x20) {       //irq
  	u = *(volatile unsigned int*)(i);
  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
  }
//  InstallInterruptHandler0( MPIC_GPIO_INT_NUM ,gpio_int_isr);
  //gpio_int_isr
 // spi_modf_int_test();
  __asm__ (
 	    "li	 6,_NCINTR_PREHANDLER@l \n"
  	    "mtspr	 0x194,6 \n"
  );


  
}
volatile unsigned int   gpio_int_flag;
static void gpio_int_isr(void)
{
	unsigned int temp = 0;


	gpio_int_flag = gpio_int_flag+1;
	unsigned int i,u;
	  for(i=MPIC_IIVPR_GPIO;i<=MPIC_IIVPR_GPIO;i=i+0x20) {       //irq
	  	u = *(volatile unsigned int*)(i);
	  	*(volatile unsigned int*)(i) = u | 0x80000000;
	  }

	uart_printf_buf("m gpio inter pass!\n");

}


void mpic_int()
  /* main program */
{
	 unsigned int i,j,u;

  q=0;

  // set target for mcp
  for(i=MPIC_IILR0;i<=MPIC_IILR70;i=i+0x20) {
  	*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_int;
	u = *(volatile unsigned int*)(i);
  }
  for(i=MPIC_IILR71;i<=MPIC_IILR127;i=i+0x20) {
  	*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_int;
	u = *(volatile unsigned int*)(i);
  }
  for(i=MPIC_EILR0;i<=MPIC_EILR11;i=i+0x20) {
  	*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_int;
	u = *(volatile unsigned int*)(i);
  }
  // unmask
  InstallInterruptHandler0( MPIC_GPIO_INT_NUM ,gpio_int_isr);
  gpio_int_flag = 0;
  for(i=MPIC_IIVPR_GPIO;i<=MPIC_IIVPR_GPIO;i=i+0x20) {       //irq
  	u = *(volatile unsigned int*)(i);
  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
  }

	while(gpio_int_flag==0);
  gpio_int_flag = 0;
  //while(1);



  
}
#define  FIRST_LAB_IN  0
#define  FIRST_LAB_OUT  1
#define  SECOND_LAB_IN  2
#define  SECOND_LAB_OUT  3
int handle_order[4] = {0,0,0,0};
unsigned long long msg_addr[8]= {MPIC_MSGRA0,MPIC_MSGRA1,MPIC_MSGRA2,MPIC_MSGRA3,MPIC_MSGRB0,MPIC_MSGRB1,MPIC_MSGRB2,MPIC_MSGRB3};


static void msg_irq_first()
{
	 unsigned int i,j,u;
	
	handle_order[q] = FIRST_LAB_IN;
	q=q+1;
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSRA);
	*(volatile unsigned int*)(MPIC_MSRA) = u;
	
	u = *(volatile unsigned int*)(MPIC_MSRB);
	*(volatile unsigned int*)(MPIC_MSRB) = u;
	*(volatile unsigned int*)(msg_addr[q1]) = msg_addr[q1];//set second int
	u = *(volatile unsigned int*)(MPIC_MSRB);
delay(100);
	handle_order[q] = FIRST_LAB_OUT;
	q=q+1;
   	//passed();

}
static void msg_irq_second()
{
	unsigned int i,j,u;
	
	handle_order[q] = SECOND_LAB_IN;
	q=q+1;
  	//int dis
	u = *(volatile unsigned int*)(MPIC_MSRA);
	*(volatile unsigned int*)(MPIC_MSRA) = u;
	
	u = *(volatile unsigned int*)(MPIC_MSRB);
	*(volatile unsigned int*)(MPIC_MSRB) = u;

	//*(volatile unsigned int*)(msg_addr[q1]) = msg_addr[q1];
	

	handle_order[q] = SECOND_LAB_OUT;
	q=q+1;
   	//passed();

}

void mpic_int_order_the_same_Priority()
  /* main program */
{
	unsigned int i,j,u;
	__asm__ (

	    "lis		0, (0x00000000)@ha \n"
	    "ori		0, 0, (0x00000000)@l \n"
	    "mtmsr	0 \n"
	    );
	q=0;
  //---- change config ---------//

	// set msg IRQs groupA, whose priority=1, vector=152~155
		j = MPIC_MIVPRA_PRI_14 + 152;
		for(i=MPIC_MIVPRA0;i<=MPIC_MIVPRA3;i=i+0x20) {
			u = *(volatile unsigned int*)(i);
			*(volatile unsigned int*)(i) = j|0x80000000;
			//*(volatile unsigned int*)(i) = j|u;
			j++;
		}
  // int unmask
	  for(i=MPIC_MIVPRA0;i<=MPIC_MIVPRA3;i=i+0x20) {
	  	u = *(volatile unsigned int*)(i);
	  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
	  }


  
    //int en
    *(volatile unsigned int*)(MPIC_MERA) = 0xf;

    *(volatile unsigned int*)(msg_addr[3]) = msg_addr[3];// vector 155
    *(volatile unsigned int*)(msg_addr[2]) = msg_addr[2];// vector 154
    *(volatile unsigned int*)(msg_addr[1]) = msg_addr[1];// vector 153
    *(volatile unsigned int*)(msg_addr[0]) = msg_addr[0]; // vector 152
    delay(200);
    u = *(volatile unsigned int*)(0x200A0 + MPIC_BASE_ADDR);//IACK
    delay(200);
    while(u != 152);//MSGA0
  
    mpic_reset();
	__asm__ (

	    "lis		0, (0x80029000)@ha \n"
	    "ori		0, 0, (0x80029000)@l \n"
	    "mtmsr	0 \n"

	    );
	//mpic_reset();
}

void mpic_nest()
  /* main program */
{
	unsigned int i,j,u;

	q=0;
  //---- change config ---------//

	/*
  // int unmask
	  for(i=MPIC_MIVPRA0;i<=MPIC_MIVPRA3;i=i+0x20) {
	  	u = *(volatile unsigned int*)(i);
	  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
	  }
	  for(i=MPIC_MIVPRB0;i<=MPIC_MIVPRB3;i=i+0x20) {
	  	u = *(volatile unsigned int*)(i);
	  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
	  }
	  */
	//8个中断源，优先级14到7
  *(volatile unsigned int*)(MPIC_MIVPRA0) = 0x000E0098;
  *(volatile unsigned int*)(MPIC_MIVPRA1) = 0x000D0099;
  *(volatile unsigned int*)(MPIC_MIVPRA2) = 0x000C009A;
  *(volatile unsigned int*)(MPIC_MIVPRA3) = 0x000B009B;
  *(volatile unsigned int*)(MPIC_MIVPRB0) = 0x000A009C;
  *(volatile unsigned int*)(MPIC_MIVPRB1) = 0x0009009D;
  *(volatile unsigned int*)(MPIC_MIVPRB2) = 0x0008009E;
  *(volatile unsigned int*)(MPIC_MIVPRB3) = 0x0007009F;
  
  //int en
  *(volatile unsigned int*)(MPIC_MERA) = 0xf;
  *(volatile unsigned int*)(MPIC_MERB) = 0xf;


  for(i=0;i<=7;i=i+1) {//first int source
	  for(j=0;j<=7;j=j+1) {//second int source
		  if(i == j)   continue;
		  InstallInterruptHandler0( 152+i ,msg_irq_first);
		  InstallInterruptHandler0( 152+j ,msg_irq_second);
		  q=0;
		  handle_order[0]=0;
		  handle_order[1]=0;
		  handle_order[2]=0;
		  handle_order[3]=0;
		  
          q1 = j;
	      *(volatile unsigned int*)(msg_addr[i]) = msg_addr[i];//set first int
	   	  while(4 != q);
	  	 
		  if(i < j){//未打断
			  if((handle_order[0]!=FIRST_LAB_IN)||(handle_order[1]!=FIRST_LAB_OUT)||(handle_order[2]!=SECOND_LAB_IN)||(handle_order[3]!=SECOND_LAB_OUT))
				  while(1);
		  }
		  else{//打断
			  if((handle_order[0]!=FIRST_LAB_IN)||(handle_order[1]!=SECOND_LAB_IN)||(handle_order[2]!=SECOND_LAB_OUT)||(handle_order[3]!=FIRST_LAB_OUT))
			 	  while(1);
		  }
	  }

  }
  
  COMMENT("mpic nest test is ok");

}


void headle_order_software()
{
	int i,j,m,n;
	j = 0;
	m = 0;
	for(i=0;i<=14;i=i+1)
	{
		if(j == 0)//无嵌套，直接入栈
		{
			int_stack1[j] = sources_order1[i];
			handle_order2[m] = sources_order1[i];
			j++;
			m++;
		}
		else{
			if(sources_order1[i] > int_stack1[j-1])//优先级高，直接入栈
			{
				int_stack1[j] = sources_order1[i];
				handle_order2[m] = sources_order1[i];
				j++;
				m++;
			}
			else{//优先级低，先出栈再入栈
				while((j > 0)&&(sources_order1[i] < int_stack1[j-1]))
				{
					handle_order2[m] = int_stack1[j-1] + 0x10;
					j--;
					m++;
				}
				
				int_stack1[j] = sources_order1[i];
				handle_order2[m] = sources_order1[i];
				j++;
				m++;
			}
		}
	}
	for(i=j-1;i>=0;i=i-1)//剩下的 ，出栈
	{
		handle_order2[m] = int_stack1[i] + 0x10;
		m++;
	}
}

void mpic_nest_16_sources()
  /* main program */
{
	int i,j,k,u;

	q=0;
	q1=0;
	
  //---- change config ---------//
	InstallInterruptHandler0( 160 ,msi_irq0_routine1); //A
	InstallInterruptHandler0( 161 ,msi_irq1_routine1);
	InstallInterruptHandler0( 162 ,msi_irq2_routine1);
	InstallInterruptHandler0( 163 ,msi_irq3_routine1);

	InstallInterruptHandler0( 164 ,msi_irq4_routine1); //B
	InstallInterruptHandler0( 165 ,msi_irq5_routine1);
	InstallInterruptHandler0( 166 ,msi_irq6_routine1);
	InstallInterruptHandler0( 167 ,msi_irq7_routine1);
	
	InstallInterruptHandler0( 152 ,msg_irq0_routine1);
	InstallInterruptHandler0( 153 ,msg_irq1_routine1);
	InstallInterruptHandler0( 154 ,msg_irq2_routine1);
	InstallInterruptHandler0( 155 ,msg_irq3_routine1);
	InstallInterruptHandler0( 156 ,msg_irq4_routine1);
	InstallInterruptHandler0( 157 ,msg_irq5_routine1);
	InstallInterruptHandler0( 158 ,msg_irq6_routine1);
	InstallInterruptHandler0( 159 ,msg_irq7_routine1);
	
	//16个中断源，优先级0到15
	
	  *(volatile unsigned int*)(MPIC_MSIVPRA0) = 0x000000A0;
	  *(volatile unsigned int*)(MPIC_MSIVPRA1) = 0x000100A1;
	  *(volatile unsigned int*)(MPIC_MSIVPRA2) = 0x000200A2;
	  *(volatile unsigned int*)(MPIC_MSIVPRA3) = 0x000300A3;
	  *(volatile unsigned int*)(MPIC_MSIVPRB0) = 0x000400A4;
	  *(volatile unsigned int*)(MPIC_MSIVPRB1) = 0x000500A5;
	  *(volatile unsigned int*)(MPIC_MSIVPRB2) = 0x000600A6;
	  *(volatile unsigned int*)(MPIC_MSIVPRB3) = 0x000700A7;
	
  *(volatile unsigned int*)(MPIC_MIVPRA0) = 0x00080098;
  *(volatile unsigned int*)(MPIC_MIVPRA1) = 0x00090099;
  *(volatile unsigned int*)(MPIC_MIVPRA2) = 0x000A009A;
  *(volatile unsigned int*)(MPIC_MIVPRA3) = 0x000B009B;
  *(volatile unsigned int*)(MPIC_MIVPRB0) = 0x000C009C;
  *(volatile unsigned int*)(MPIC_MIVPRB1) = 0x000D009D;
  *(volatile unsigned int*)(MPIC_MIVPRB2) = 0x000E009E;
  *(volatile unsigned int*)(MPIC_MIVPRB3) = 0x000F009F;
    
  //int en
  *(volatile unsigned int*)(MPIC_MERA) = 0xf;
  *(volatile unsigned int*)(MPIC_MERB) = 0xf;

  for(i=0;i<=14;i=i+1) {
	  
	  sources_order1[i] = (i + 1);
  }
  
  
  for(i=0;i<=100;i=i+1)//random order
  {
	  j = rand()%15;
	  k = rand()%15;
	  
	  u = sources_order1[j];
	  sources_order1[j] = sources_order1[k];
	  sources_order1[k] = u;
  }
  
  *(volatile unsigned int*)msx_addr1[sources_order1[q1]] = msx_trigger1[sources_order1[q1]];//trigger first int
  
  while(q != 30);//wait trigger 15 int
  headle_order_software();
  
  for(i=0;i<=14;i=i+1) {
	  if(handle_order1[i] != handle_order2[i])
          while(1);
  }

}
void mpic_nest_16_sources_proxy()
  /* main program */
{
	int i,j,k,u;

	q=0;
	q1=0;
	
//	  __asm__ (
//
//	  		
//	  	    "li	 6,NCI_PREHANDLER@l \n"
//	  	    "mtspr	 0x194,6 \n"
//
//	  		
//	  );
	  __asm__ (
	  	    "li	 6,_NCINTR_PREHANDLER@l \n"
	  	    "mtspr	 0x194,6 \n"		
	  );
	  *(volatile unsigned int*)(MPIC_GCR) = MPIC_GCR_EPM;    
	  u = *(volatile unsigned int*)(MPIC_GCR);
  //---- change config ---------//
	InstallInterruptHandler0( 160 ,msi_irq0_routine1); //A
	InstallInterruptHandler0( 161 ,msi_irq1_routine1);
	InstallInterruptHandler0( 162 ,msi_irq2_routine1);
	InstallInterruptHandler0( 163 ,msi_irq3_routine1);

	InstallInterruptHandler0( 164 ,msi_irq4_routine1); //B
	InstallInterruptHandler0( 165 ,msi_irq5_routine1);
	InstallInterruptHandler0( 166 ,msi_irq6_routine1);
	InstallInterruptHandler0( 167 ,msi_irq7_routine1);
	
	InstallInterruptHandler0( 152 ,msg_irq0_routine1);
	InstallInterruptHandler0( 153 ,msg_irq1_routine1);
	InstallInterruptHandler0( 154 ,msg_irq2_routine1);
	InstallInterruptHandler0( 155 ,msg_irq3_routine1);
	InstallInterruptHandler0( 156 ,msg_irq4_routine1);
	InstallInterruptHandler0( 157 ,msg_irq5_routine1);
	InstallInterruptHandler0( 158 ,msg_irq6_routine1);
	InstallInterruptHandler0( 159 ,msg_irq7_routine1);
	
	//16个中断源，优先级0到15
	
	  *(volatile unsigned int*)(MPIC_MSIVPRA0) = 0x000000A0;
	  *(volatile unsigned int*)(MPIC_MSIVPRA1) = 0x000100A1;
	  *(volatile unsigned int*)(MPIC_MSIVPRA2) = 0x000200A2;
	  *(volatile unsigned int*)(MPIC_MSIVPRA3) = 0x000300A3;
	  *(volatile unsigned int*)(MPIC_MSIVPRB0) = 0x000400A4;
	  *(volatile unsigned int*)(MPIC_MSIVPRB1) = 0x000500A5;
	  *(volatile unsigned int*)(MPIC_MSIVPRB2) = 0x000600A6;
	  *(volatile unsigned int*)(MPIC_MSIVPRB3) = 0x000700A7;
	
  *(volatile unsigned int*)(MPIC_MIVPRA0) = 0x00080098;
  *(volatile unsigned int*)(MPIC_MIVPRA1) = 0x00090099;
  *(volatile unsigned int*)(MPIC_MIVPRA2) = 0x000A009A;
  *(volatile unsigned int*)(MPIC_MIVPRA3) = 0x000B009B;
  *(volatile unsigned int*)(MPIC_MIVPRB0) = 0x000C009C;
  *(volatile unsigned int*)(MPIC_MIVPRB1) = 0x000D009D;
  *(volatile unsigned int*)(MPIC_MIVPRB2) = 0x000E009E;
  *(volatile unsigned int*)(MPIC_MIVPRB3) = 0x000F009F;
    
  //int en
  *(volatile unsigned int*)(MPIC_MERA) = 0xf;
  *(volatile unsigned int*)(MPIC_MERB) = 0xf;

  for(i=0;i<=14;i=i+1) {
	  
	  sources_order1[i] = (i + 1);
  }
  
  /*
  for(i=0;i<=100;i=i+1)//random order
  {
	  j = rand()%15;
	  k = rand()%15;
	  
	  u = sources_order1[j];
	  sources_order1[j] = sources_order1[k];
	  sources_order1[k] = u;
  }
  */
  *(volatile unsigned int*)msx_addr1[sources_order1[q1]] = msx_trigger1[sources_order1[q1]];//trigger first int
  
  while(q != 30);//wait trigger 15 int
  headle_order_software();
  
  for(i=0;i<=14;i=i+1) {
	  if(handle_order1[i] != handle_order2[i])
          while(1);
  }
  __asm__ (
 	    "li	 6,_NCINTR_PREHANDLER@l \n"
  	    "mtspr	 0x194,6 \n"
  );
}
void mpic_check_source()
{
	 unsigned int i,j,u;

     q=0;

  // set target for mcp
  for(i=MPIC_IILR0;i<=MPIC_IILR70;i=i+0x20) {
  	  *(volatile unsigned int*)(i) = MPIC_IILR_inttgt_int;
	  u = *(volatile unsigned int*)(i);
  }
  for(i=MPIC_IILR71;i<=MPIC_IILR127;i=i+0x20) {
  	  *(volatile unsigned int*)(i) = MPIC_IILR_inttgt_int;
	  u = *(volatile unsigned int*)(i);
  }
  for(i=MPIC_EILR0;i<=MPIC_EILR11;i=i+0x20) {
  	  *(volatile unsigned int*)(i) = MPIC_IILR_inttgt_int;
	  u = *(volatile unsigned int*)(i);
  }
  
 // *(volatile unsigned int*)(MPIC_CTPR0) = MPIC_CTPR_PRI_15;
  // unmask

  for(i=MPIC_EIVPR0;i<=MPIC_EIVPR11;i=i+0x20) {        //eirq
      if((i == MPIC_EIVPR1)||(i == MPIC_EIVPR3)||(i == MPIC_EIVPR5)||(i == MPIC_EIVPR7)||(i == MPIC_EIVPR9)||(i == MPIC_EIVPR11)) continue;
  	  u = *(volatile unsigned int*)(i);
  	  *(volatile unsigned int*)(i) = u & 0x7fffffff;
  }
  for(i=MPIC_IIVPR0;i<=MPIC_IIVPR70;i=i+0x20) {        //irq
	  if((i == MPIC_IIVPR24)||(i == MPIC_IIVPR26)||(i == MPIC_IIVPR28)||(i == MPIC_IIVPR30)) continue;
	  
	  if((i == MPIC_IIVPR32)||(i == MPIC_IIVPR34)||(i == MPIC_IIVPR36)||(i == MPIC_IIVPR38)) continue;
      u = *(volatile unsigned int*)(i);
  	  *(volatile unsigned int*)(i) = u & 0x7fffffff;
  }
  for(i=MPIC_IIVPR71;i<=MPIC_IIVPR127;i=i+0x20) {      //irq
  	  u = *(volatile unsigned int*)(i);
  	  *(volatile unsigned int*)(i) = u & 0x7fffffff;
  }  
/*
  u = *(volatile unsigned int*)(MPIC_FRR); 
  while((u&MPIC_FRR_NIRQ_S) != 0)
  {
	  u = *(volatile unsigned int*)(MPIC_IACK0);
	  *(volatile unsigned int*)(MPIC_EOI0) = 0;
	  u = *(volatile unsigned int*)(MPIC_FRR); 
  }

  *(volatile unsigned int*)(MPIC_CTPR0) = MPIC_CTPR_PRI_0;

  */
  while(1);
}
void nmi_irq_routine()
{
	int u;
	q = q + 1 ;
	*(volatile unsigned int*)(MPIC_PNMIR) = 0;
	u = *(volatile unsigned int*)(MPIC_PNMIR);
}
void mpic_nmi()
{
    q = 0;
	InstallInterruptHandler0( 193 ,nmi_irq_routine);
	*(volatile unsigned int*)(MPIC_PNMIR) = 1;
	
	while(q != 1);
}

void mpic_reset_test()
{ 
	int u;
	__asm__ (

	    "lis		0, (0x00000000)@ha \n"
	    "ori		0, 0, (0x00000000)@l \n"
	    "mtmsr	0 \n"
	    );
	*(volatile unsigned int*)(MPIC_PIR) = 0x80;
	*(volatile unsigned int*)(MPIC_TFRRA) = 0xFFFFFFFF;
	*(volatile unsigned int*)(MPIC_TCRA) = 0xFFFFFFFF;
	*(volatile unsigned int*)(MPIC_MERA) = 0xFFFFFFFF;
	*(volatile unsigned int*)(MPIC_MSRA) = 0xFFFFFFFF;
	*(volatile unsigned int*)(MPIC_MSGRA0) = 0xFFFFFFFF;
	*(volatile unsigned int*)(MPIC_GTDRA0) = 0xFFFFFFFF;
	*(volatile unsigned int*)(MPIC_GTBCRA0) = 0xFFFFFFFF;
	*(volatile unsigned int*)(MPIC_CTPR) = 0x0;
	*(volatile unsigned int*)(MPIC_SVR) = 0x0;
	*(volatile unsigned int*)(MPIC_MSIVPRA0) = 0x0;
	*(volatile unsigned int*)(MPIC_MIVPRA0) = 0x0;
	*(volatile unsigned int*)(MPIC_GTVPRA0) = 0x0;
	*(volatile unsigned int*)(MPIC_IPIVPR0) = 0x0;
	mpic_reset();
    u = *(volatile unsigned int*)(MPIC_PIR);
    if(u != 0) while(1);
    u = *(volatile unsigned int*)(MPIC_TFRRA);
    if(u != 0) while(1);
    u = *(volatile unsigned int*)(MPIC_TCRA);
    if(u != 0) while(1);
    u = *(volatile unsigned int*)(MPIC_MERA);
    if(u != 0) while(1);
    u = *(volatile unsigned int*)(MPIC_MSRA);
    if(u != 0) while(1);
    u = *(volatile unsigned int*)(MPIC_MSGRA0);
    if(u != 0) while(1);
    u = *(volatile unsigned int*)(MPIC_GTDRA0);
    if(u != 0x1) while(1);
    u = *(volatile unsigned int*)(MPIC_GTBCRA0);
    if(u != 0x80000000) while(1);
    u = *(volatile unsigned int*)(MPIC_CTPR);
    if(u != 0xF) while(1);
    u = *(volatile unsigned int*)(MPIC_SVR);
    if(u != 0xFFFF) while(1);
    u = *(volatile unsigned int*)(MPIC_MSIVPRA0);
    if(u != 0x80000000) while(1);
    u = *(volatile unsigned int*)(MPIC_MIVPRA0);
    if(u != 0x80000000) while(1);
    u = *(volatile unsigned int*)(MPIC_GTVPRA0);
    if(u != 0x80000000) while(1);
    u = *(volatile unsigned int*)(MPIC_IPIVPR0);
    if(u != 0x80000000) while(1);

	__asm__ (

	    "lis		0, (0x80029000)@ha \n"
	    "ori		0, 0, (0x80029000)@l \n"
	    "mtmsr	0 \n"

	    );
}
void mpic_hreset()
{
	*(volatile unsigned int*)(MPIC_PIR) = 8;
	while(1);
}
void pic_sv1()
{
	int u;
	*(volatile unsigned int*)(MPIC_SVR) = 210;  
	u = *(volatile unsigned int*)(MPIC_SVR);
	//no int trig,check IACK0
	u = 0;
	u = *(volatile unsigned int*)(MPIC_IACK0);
	if(u != 210)  while(1);
}

void ipi_sv_routine()
{
	unsigned int i,j,u;
	q=q+4;
  	//int mask
	for(i=MPIC_IPIVPR0;i<=MPIC_IPIVPR0;i=i+0x10) {
		u = *(volatile unsigned int*)(i);
  		*(volatile unsigned int*)(i) = u | 0x80000000;
	}

   	//passed();
}
void mpic_sv2()
{
	unsigned int i,j,u;
	int sv_int_num = 210;
//	  __asm__ (
//	 	    "li	 6,_NCINTR_SV_PREHANDLER@l \n"
//	  	    "mtspr	 0x194,6 \n"
//	  );
	  __asm__ (
	  	    "li	 6,_NCINTR_PREHANDLER@l \n"
	  	    "mtspr	 0x194,6 \n"		
	  );
	InstallInterruptHandler0( 140 ,ipi_irq0_routine);
	InstallInterruptHandler0( sv_int_num ,ipi_sv_routine);

	q=0;

	*(volatile unsigned int*)(MPIC_SVR) = sv_int_num;  
	u = *(volatile unsigned int*)(MPIC_SVR);

  //---- change config ---------//

  // int unmask
  for(i=MPIC_IPIVPR0;i<=MPIC_IPIVPR3;i=i+0x10) {
  	u = *(volatile unsigned int*)(i);
  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
  }
  //int en

  	*(volatile unsigned int*)(MPIC_CPU0_IPIDR0) = 0x1; 
    while(q != 4);
    __asm__ (
   	    "li	 6,_NCINTR_PREHANDLER@l \n"
    	    "mtspr	 0x194,6 \n"
    );
  

}

static void gpio_internal_routine()
{
	unsigned int i,j,u;
	q=q+1;
  	//int mask
	  for(i=MPIC_IIVPR49;i<=MPIC_IIVPR64;i=i+0x20) {       //irq
		u = *(volatile unsigned int*)(i);
	  	*(volatile unsigned int*)(i) = u | 0x80000000;
	 
	  }

   	//passed();
}
static void eport_internal_routine()
{
	unsigned int i,j,u;
	q=q+1;
	EPORT0_EPFR=0xff;
  	//int mask
	  for(i=MPIC_IIVPR49;i<=MPIC_IIVPR64;i=i+0x20) {       //irq
		u = *(volatile unsigned int*)(i);
	  	*(volatile unsigned int*)(i) = u | 0x80000000;
	 
	  }

   	//passed();
}
static void gpio_external_routine()
{
	unsigned int i,j,u;
	q=q+1;
  	//int mask
	  for(i=MPIC_EIVPR0;i<=MPIC_EIVPR11;i=i+0x20) {       //irq
		u = *(volatile unsigned int*)(i);
	  	*(volatile unsigned int*)(i) = u | 0x80000000;
	 
	  }

   	//passed();
}
static void eport_external_routine()
{
	unsigned int i,j,u;
	q=q+1;
	EPORT0_EPFR=0xff;
  	//int mask
	  for(i=MPIC_EIVPR0;i<=MPIC_EIVPR11;i=i+0x20) {       //irq
		u = *(volatile unsigned int*)(i);
	  	*(volatile unsigned int*)(i) = u | 0x80000000;
	 
	  }

   	//passed();
}
void mpic_gpio_external()
{
        int gpio_int_num;
		unsigned int i,j,u;
	    for(gpio_int_num = 128;gpio_int_num <= 128+11;gpio_int_num++)
	    {
	    	InstallInterruptHandler0(gpio_int_num,gpio_external_routine);
	    }
	  q=0;

	  // set target for mcp
	  for(i=MPIC_IILR0;i<=MPIC_IILR70;i=i+0x20) {
	  	*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_int;
		u = *(volatile unsigned int*)(i);
	  }
	  for(i=MPIC_IILR71;i<=MPIC_IILR127;i=i+0x20) {
	  	*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_int;
		u = *(volatile unsigned int*)(i);
	  }
	  for(i=MPIC_EILR0;i<=MPIC_EILR11;i=i+0x20) {
	  	*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_int;
		u = *(volatile unsigned int*)(i);
	  }
	  // unmask
///////////////////////////	  gpio_int_test();
  
	  for(i=MPIC_EIVPR4;i<=MPIC_EIVPR7;i=i+0x20) {        //eirq
		  q1 = q;
	  	u = *(volatile unsigned int*)(i);
	  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
	  	while(q1 == q);
	  }
while(q != 4);
	 // spi_modf_int_test();


}
void mpic_gpio_internal()
{
        int gpio_int_num;
		unsigned int i,j,u;
	    for(gpio_int_num = 49;gpio_int_num <= 64;gpio_int_num++)
	    {
	    	InstallInterruptHandler0(gpio_int_num,gpio_internal_routine);
	    }
	  q=0;

	  // set target for mcp
	  for(i=MPIC_IILR0;i<=MPIC_IILR70;i=i+0x20) {
	  	*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_int;
		u = *(volatile unsigned int*)(i);
	  }
	  for(i=MPIC_IILR71;i<=MPIC_IILR127;i=i+0x20) {
	  	*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_int;
		u = *(volatile unsigned int*)(i);
	  }
	  for(i=MPIC_EILR0;i<=MPIC_EILR11;i=i+0x20) {
	  	*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_int;
		u = *(volatile unsigned int*)(i);
	  }
	  // unmask
//////////////////	  gpio_int_test();
	  for(i=MPIC_IIVPR53;i<=MPIC_IIVPR56;i=i+0x20) {       //test GPIOM4 --M7
		  q1 = q;
	  	u = *(volatile unsigned int*)(i);
	  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
	  	while(q1 == q);
	  }
while(q != 4);
	
}

void mpic_eport_internal()
{
        int gpio_int_num;
		unsigned int i,j,u;
	    for(gpio_int_num = 49;gpio_int_num <= 64;gpio_int_num++)
	    {
	    	InstallInterruptHandler0(gpio_int_num,eport_internal_routine);
	    }
	  q=0;

	  // set target for mcp
	  for(i=MPIC_IILR0;i<=MPIC_IILR70;i=i+0x20) {
	  	*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_int;
		u = *(volatile unsigned int*)(i);
	  }
	  for(i=MPIC_IILR71;i<=MPIC_IILR127;i=i+0x20) {
	  	*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_int;
		u = *(volatile unsigned int*)(i);
	  }
	  for(i=MPIC_EILR0;i<=MPIC_EILR11;i=i+0x20) {
	  	*(volatile unsigned int*)(i) = MPIC_IILR_inttgt_int;
		u = *(volatile unsigned int*)(i);
	  }
	  // unmask
//////////////////	  gpio_int_test();
	  for(i=MPIC_IIVPR53;i<=MPIC_IIVPR56;i=i+0x20) {       //test EPORT4 --7
		  q1 = q;
	  	u = *(volatile unsigned int*)(i);
	  	*(volatile unsigned int*)(i) = u & 0x7fffffff;
	  	while(q1 == q);
	  }
while(q != 4);
	
}
void mpic_test(void)
{
	__asm__ (

	    "lis		0, (0x80029000)@ha \n"
	    "ori		0, 0, (0x80029000)@l \n"
	    "mtmsr	0 \n"

	    );
	
	//128in + 12 ex
	//mpic_check_source();
	//while(1){
	//    mpic_nest_16_sources();
	//}
	//mpic输入源
	//mpic_gpio_external();
	mpic_reset();
	mpic_init();
	
	//gpio_output_high(1);
    mpic_msi();
	mpic_msg();
	mpic_gt();
    mpic_ipi();
	
    //mpic输出
	mpic_cint();
	mpic_mcp();
	mpic_int();
	mpic_nmi();
	//mpic_hreset();//------err!!!
	mpic_reset();
	mpic_init();
	//中断嵌套,优先级
	mpic_nest();
	mpic_reset();
	mpic_init();

	
	//假中断向量测试
	mpic_sv2();
	
	
	mpic_reset();
	mpic_init();
	
	//同一优先级下，默认顺序
	mpic_int_order_the_same_Priority();
	//mpic复位
	mpic_reset_test();
	
	//16个优先级，随机中断嵌套  
	mpic_reset();
	mpic_init();
	mpic_nest_16_sources();
	mpic_reset();
	mpic_init();
	
    //proxy模式下，16个优先级，随机中断嵌套
	mpic_nest_16_sources_proxy();
	mpic_reset();
	mpic_init();
	
	//其他模式
	mpic_int_pass_through();
	
}


unsigned  mpic_get(void)
{
	unsigned val;
	val = *(volatile unsigned int*)(MPIC_CTPR0);
	return val;
}

void mpic_set(unsigned int val)
{
	*(volatile unsigned int*)(MPIC_CTPR0) |= val;
}
