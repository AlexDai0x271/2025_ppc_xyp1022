#include "environment.h"
static void   MPIC_Int(void);
static void   Critical_Int(void);                        
static void   MachCheck(void);                 
static void   Data_Abort(void);                   
static void   Prefetch_Abort(void);               
static void   Noncrtical_Int(void);             
static void   Unalign_Access(void);                   
static void   Illegal_Instrn(void);                        
static void   Floating_Point_Unavail(void);                  
static void   System_Call(void);                  
static void   Aux_Proc_Unavail(void);                  
static void   Decrement_Int(void);                  
static void   Fixed_Timer_Int(void);                  
static void   Watchdog_Timer_Int(void);                  
static void   Data_TLB_Err(void);                  
static void   Instr_TLB_Err(void);                  
static void   Debug_Int(void);                  

// the interrupt service vector jump table 
// ------- CPU0 vec_table ------- {{{
void (* volatile vec_table0[])(void) =	/* offset: exception vector 	*/
{
  MPIC_Int,//0:no use. 1~184:mpic. 185:spurious.
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,			//20
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,			//40
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,			//60
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,			//80
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,			//100
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,			//120
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,			//140
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,			//160
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,			//180
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,			//185
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,			//191

  Critical_Int,                 /* Critical input            */
  MachCheck,                 	/* Machine Check             */
  Data_Abort,                   /* Data Storage              */
  Prefetch_Abort,               /* Instruction Storage       */
  Noncrtical_Int,               /* External input            */
  Unalign_Access,               /* Alignment                 */
  Illegal_Instrn,               /* Program         	     */
  Floating_Point_Unavail,       /* FP unavailable            */
  System_Call,                  /* System call               */
  Aux_Proc_Unavail,             /* AP unavailable            */
  Decrement_Int,                /* Decrementer               */
  Fixed_Timer_Int,              /* FIT                       */
  Watchdog_Timer_Int,           /* Watchdog timer            */
  Data_TLB_Err,                 /* DTLB miss                  */
  Instr_TLB_Err,                /* ITLB miss                  */
  Debug_Int,                    /* Debug                 */
};
// ------- ------- ------- }}}

// ------- CPU1 vec_table ------- {{{
void (* volatile vec_table1[])(void) =	/* offset: exception vector 	*/
{
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  Critical_Int,                 /* Critical input            */
  MachCheck,                 	/* Machine Check             */
  Data_Abort,                   /* Data Storage              */
  Prefetch_Abort,               /* Instruction Storage       */
  Noncrtical_Int,               /* External input            */
  Unalign_Access,               /* Alignment                 */
  Illegal_Instrn,               /* Program         	     */
  Floating_Point_Unavail,       /* FP unavailable            */
  System_Call,                  /* System call               */
  Aux_Proc_Unavail,             /* AP unavailable            */
  Decrement_Int,                /* Decrementer               */
  Fixed_Timer_Int,              /* FIT                       */
  Watchdog_Timer_Int,           /* Watchdog timer            */
  Data_TLB_Err,                 /* DTLB miss                  */
  Instr_TLB_Err,                /* ITLB miss                  */
  Debug_Int,                    /* Debug                 */
};
// ------- ------- ------- }}}

// ------- CPU2 vec_table ------- {{{
void (* volatile vec_table2[])(void) =	// offset: exception vector 
{
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  Critical_Int,                 /* Critical input            */
  MachCheck,                 	/* Machine Check             */
  Data_Abort,                   /* Data Storage              */
  Prefetch_Abort,               /* Instruction Storage       */
  Noncrtical_Int,               /* External input            */
  Unalign_Access,               /* Alignment                 */
  Illegal_Instrn,               /* Program         	     */
  Floating_Point_Unavail,       /* FP unavailable            */
  System_Call,                  /* System call               */
  Aux_Proc_Unavail,             /* AP unavailable            */
  Decrement_Int,                /* Decrementer               */
  Fixed_Timer_Int,              /* FIT                       */
  Watchdog_Timer_Int,           /* Watchdog timer            */
  Data_TLB_Err,                 /* DTLB miss                  */
  Instr_TLB_Err,                /* ITLB miss                  */
  Debug_Int,                    /* Debug                 */
};
// ------- ------- ------- }}}

// ------- CPU3 vec_table ------- {{{
void (* volatile vec_table3[])(void) =	// offset: exception vector
{
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  MPIC_Int,
  Critical_Int,                 /* Critical input            */
  MachCheck,                 	/* Machine Check             */
  Data_Abort,                   /* Data Storage              */
  Prefetch_Abort,               /* Instruction Storage       */
  Noncrtical_Int,               /* External input            */
  Unalign_Access,               /* Alignment                 */
  Illegal_Instrn,               /* Program         	     */
  Floating_Point_Unavail,       /* FP unavailable            */
  System_Call,                  /* System call               */
  Aux_Proc_Unavail,             /* AP unavailable            */
  Decrement_Int,                /* Decrementer               */
  Fixed_Timer_Int,              /* FIT                       */
  Watchdog_Timer_Int,           /* Watchdog timer            */
  Data_TLB_Err,                 /* DTLB miss                  */
  Instr_TLB_Err,                /* ITLB miss                  */
  Debug_Int,                    /* Debug                 */
}; 
// ------- ------- ------- }}}


static void   MPIC_Int(void) {
	unsigned int i;
	MFSPR(i,SPRG6);
COMMENT("WARNING: Default MPIC handler entered, vector is:0x\n");
//CPRINTW(i);
	LONG_JUMP2FAILED;
} 

static void   Critical_Int(void) {
COMMENT("WARNING: Default Critical Interrupt handler entered \n");
	LONG_JUMP2FAILED;
}
                        
static void   MachCheck(void) {
COMMENT("WARNING: Default Machine Check Interrupt handler entered \n");
//	unsigned int i,j;
//	MFSPR(i,MCSR);
//	EXTRACT(j,i,5,5);	// check MCSR[IC]
//	if (j) 
//COMMENT("WARNING: CPU I-bus response or parity error\n");
//	EXTRACT(j,i,6,6);	// check MCSR[DC]
//	if (j) 
//COMMENT("WARNING: CPU D-bus response or parity error\n");
//	EXTRACT(j,i,10,10);	// check MCSR[L2]
//	if (j) 
//COMMENT("WARNING: External Machine Check from MPIC\n");
//	EXTRACT(j,i,11,11);	// check MCSR[DCR]
//	if (j) 
//COMMENT("WARNING: DCR time out\n");
//	EXTRACT(j,i,4,4);	// check MCSR[TLB]
//	if (j) 
//COMMENT("WARNING: CPU UTLB parity error\n");
//	MFSPR(i,MCSRR0);
//COMMENT("Exception Point's PC:");
//CPRINTW(i);
	LONG_JUMP2FAILED;
}
                 
static void   Data_Abort(void) {
	unsigned int i;
COMMENT("WARNING: Ld/St data with wrong read, write or privilege permission\n");
	MFSPR(i,SRR0);
COMMENT("Exception Point's PC:");
CPRINTW(i);
	MFSPR(i,DEAR);
COMMENT("Ld/St target addr is:");
CPRINTW(i);
	LONG_JUMP2FAILED;
}
                   
static void   Prefetch_Abort(void) {
	unsigned int i;
COMMENT("WARNING: Instruction fetch with wrong exe or privilege permission\n");
	MFSPR(i,SRR0);
COMMENT("Exception Point's PC:");
CPRINTW(i);
	LONG_JUMP2FAILED;
}   
            
static void   Noncrtical_Int(void) {
COMMENT("WARNING: Default Noncritical interrupt handler entered \n");
	LONG_JUMP2FAILED;
}
             
static void   Unalign_Access(void) {
	unsigned int i;
COMMENT("WARNING: Ld/St unaligned\n");
	MFSPR(i,SRR0);
COMMENT("Exception Point's PC:");
CPRINTW(i);
	MFSPR(i,DEAR);
COMMENT("Ld/St target addr is:");
CPRINTW(i);
	LONG_JUMP2FAILED;
}                   

static void   Illegal_Instrn(void) {
	unsigned int i;
COMMENT("WARNING: Illegal instruction, instruction with no privilege permission, or trap happens\n");
	MFSPR(i,SRR0);
COMMENT("Exception Point's PC:");
CPRINTW(i);
	LONG_JUMP2FAILED;
}                 
       
static void   Floating_Point_Unavail(void) {
	unsigned int i;
COMMENT("WARNING: FPU unavailiable\n");
	MFSPR(i,SRR0);
COMMENT("Exception Point's PC:");
CPRINTW(i);
	LONG_JUMP2FAILED;
}                  

static void   System_Call(void) {
COMMENT("WARNING: Default System Call interrupt handler entered \n");
	LONG_JUMP2FAILED;
}
                  
static void   Aux_Proc_Unavail(void) {
COMMENT("WARNING: APU Unavailiable\n");
	LONG_JUMP2FAILED;
}                  

static void   Decrement_Int(void) {
COMMENT("WARNING: Default Decrement interrupt handler entered \n");
	LONG_JUMP2FAILED;
}                  

static void   Fixed_Timer_Int(void) {
COMMENT("WARNING: Default Fixed Timer interrupt handler entered \n");
	LONG_JUMP2FAILED;
}                  

static void   Watchdog_Timer_Int(void) {
COMMENT("WARNING: Default Watchdog Timer interrupt handler entered \n");
	LONG_JUMP2FAILED;
} 
                 
static void   Data_TLB_Err(void) {
	unsigned int i;
COMMENT("WARNING: Data TLB miss or TLB[I=W=1]\n");
	MFSPR(i,SRR0);
COMMENT("Exception Point's PC:");
CPRINTW(i);
   MTSPR(SRR0,i+4);
	MFSPR(i,DEAR);
COMMENT("Ld/St target addr is:");
CPRINTW(i);

	//LONG_JUMP2FAILED;
}     
             
static void   Instr_TLB_Err(void) {
	unsigned int i;
COMMENT("WARNING: Instruction TLB miss or TLB[I=W=1]\n");
	MFSPR(i,SRR0);
COMMENT("Exception Point's PC:");
CPRINTW(i);
   MTSPR(SRR0,i+4);
	//LONG_JUMP2FAILED;
} 
                 
static void   Debug_Int(void) {
	COMMENT("WARNING: Default internal debug interrupt handler entered\n");
	LONG_JUMP2FAILED;
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
}                
