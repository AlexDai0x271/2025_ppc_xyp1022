/*------- general description ------- 
  All rights reserved by C*Core Corporation
  date:   2012.12.21
  author: swang
 
  This header defines PPC macros.
  
  When you want to insert ASM to a C file,
  instead of using asm(" PPC Instruction"), 
  calling the following macros is more convenient and readable.

  When you want to add your macros in this header, 
  plz follow the naming rule: 
  1.All macros should be in capital letters.
  2.All macros should be defined with enough anotation ahead.
  3.If multi instructions are used in one macro, only '\' should be used between each instructions.
------- ------- ------- ------- */

#ifndef PPC_ASM_H	// avoid re-include the same header
#define PPC_ASM_H

#define MSYNC          asm volatile("msync")

// extract bits from a word, and right justify
#define EXTRACT(result,source,start_bit,end_bit)  asm volatile("extrwi. \
 %0,%1,%2,%3" :"=r"(result):"r"(source),"n"(end_bit-start_bit+1),"n"(start_bit))

// rotate left
#define ROTATE_LEFT(result,source,num)  asm volatile("rotlw \
 %0,%1,%2" :"=r"(result):"r"(source),"n"(num))

#define INSERT_RIGHT(result,source,source_bit,target_bit)   asm volatile("insrwi \
 %0,%1,%2,%3" : "=r"(result):"r"(source),"n"(source_bit),"n"(target_bit))
 
#define CLEAR_RIGHT(result,source,bit_num)  asm volatile("clrrwi \
 %0,%1,%2" :"=r"(result):"r"(source),"n"(bit_num))

#define TLBIVAX(addr) asm volatile("tlbivax 0,%0" : : "r" (addr)); \
                      asm volatile("tlbsync")

//#define TLBSX(data,addr) asm volatile("tlbsx. %0,0,%1" :"=r"(data) : "r" (addr))
#define TLBSX(addr) asm volatile("tlbsx 0,%0" : : "r" (addr))

#define TLBRE(data,addr,num) asm volatile("tlbre %0,%1,%2" :"=r"(data) : "r" (addr),"n"(num))

// Data Cache Block Zero
#define DCBZ(addr)  asm volatile("dcbz 0,%0" : : "r" (addr))

// L1 Inst Cache Block Invalid
#define ICBI(addr)  asm volatile("icbi 0,%0" : : "r" (addr))

// L1 Inst Cache Block Touch
#define ICBT(addr)  asm volatile("icbt 0,0,%0" : : "r" (addr))

// L2 Inst Cache Block Touch
#define ICBT2(addr)  asm volatile("icbt 2,0,%0" : : "r" (addr))

// L1 Inst Cache Block Read
#define ICREAD(addr)  asm volatile("icread 0,%0" : : "r" (addr))

// L1 Inst Cache Block Read
#define DCREAD(data,addr)  asm volatile("dcread %0,0,%1" :"=r"(data): "r" (addr))

// Data Cache Block Invalid
#define DCBI(addr)  asm volatile("dcbi 0,%0" : : "r" (addr))

// Data Cache Block Flush
#define DCBF(addr)  asm volatile("dcbf 0,%0" : : "r" (addr))

// L1 Data Cache Block Touch
#define DCBT(addr)  asm volatile("dcbt 0,%0" : : "r" (addr))

// L1 Data Cache Block Touch for Store
#define DCBTST(addr)  asm volatile("dcbtst 0,%0" : : "r" (addr))

// L2 Data Cache Block Touch
#define DCBT2(addr)  asm volatile("dcbt 2,0,%0" : : "r" (addr))

// L1 Data Cache Block Touch and Lock Set
#define DCBTLS(addr)  asm volatile("dcbtls 0,0,%0" : : "r" (addr))

// L2 Data Cache Block Touch and Lock Set
#define DCBTLS2(addr)  asm volatile("dcbtls 2,0,%0" : : "r" (addr))

// L1 Data Cache Block Touch for Store and Lock Set
#define DCBTSTLS(addr)  asm volatile("dcbtstls 0,0,%0" : : "r" (addr))

// L2 Data Cache Block Touch for Store and Lock Set
#define DCBTSTLS2(addr)  asm volatile("dcbtstls 2,0,%0" : : "r" (addr))

// Data Cache Block Store
#define DCBST(addr)  asm volatile("dcbst 0,%0" : : "r" (addr))

// L1 Inst Cache Block Touch and Lock Set
#define ICBTLS(addr)  asm volatile("icbtls 0,0,%0" : : "r" (addr))

// L2 Inst Cache Block Touch and Lock Set
#define ICBTLS2(addr)  asm volatile("icbtls 2,0,%0" : : "r" (addr))

// L1 Inst Cache Block Lock Clear
#define ICBLC(addr)  asm volatile("icblC 0,0,%0" : : "r" (addr))

// L2 Inst Cache Block Lock clear
#define ICBLC2(addr)  asm volatile("icblc 2,0,%0" : : "r" (addr))

// L1 Data Cache Block Lock clear
#define DCBLC(addr)  asm volatile("dcblc 0,0,%0" : : "r" (addr))

// L2 Data Cache Block Lock clear
#define DCBLC2(addr)  asm volatile("dcblc 2,0,%0" : : "r" (addr))

//following 'Save_regs' macro can be used in the very begining of any interrupt routine,
//to save r0 to sprg0,cr to sprg1,lr to sprg2,ctr to sprg3,xer to sprg4,
//create a 124-deep stack, r1 as pointer, and save r2-r31 to stack
#define SAVE_REGS asm volatile("mtspr 0x110,0");\
                  asm volatile("mfcr  0 ");\
                  asm volatile("mtspr 0x111,0");\
                  asm volatile("mflr  0 ");\
                  asm volatile("mtspr 0x112,0");\
                  asm volatile("mfctr  0 ");\
                  asm volatile("mtspr 0x113,0");\
                  asm volatile("mfxer  0 ");\
                  asm volatile("mtspr 0x114,0");\
                  asm volatile("stwu 1,-124(1)");\
                  asm volatile("stmw 2,4(1) ")

//following 'Restore_regs' macro is used in the very end of any critical interrupt routine,
//to restore r2-r31 from stack,delete stack and restore r1,restore cr,
//restore lr,restore ctr,restore xer,restore r0,and jump back to main
#define RESTORE_REGS asm volatile("lmw 2,4(1) ");\
                     asm volatile("lwz 1,0(1) ");\
                     asm volatile("mfspr 0,0x111");\
		     asm volatile("mtcr  0 ");\
		     asm volatile("mfspr 0,0x112");\
		     asm volatile("mtlr  0 ");\
		     asm volatile("mfspr 0,0x113");\
		     asm volatile("mtctr 0 ");\
		     asm volatile("mfspr 0,0x114");\
		     asm volatile("mtxer 0 ");\
		     asm volatile("mfspr 0,0x110");\
		     asm volatile("rfci")

// insert a no-operation instruction
#define NOP asm volatile("nop" : : )

// insert 10 no-operation instructions
#define NOP10 NOP;NOP;NOP;NOP;NOP;\
              NOP;NOP;NOP;NOP;NOP
/*
#define R32( address, destvar ) \
    do{MSYNC; ( destvar ) = ( address );}while(0) \

#define R16( address, destvar ) \
    do{MSYNC; ( destvar ) = ( address );}while(0) \

#define R8( address, destvar ) \
    do{MSYNC; ( destvar ) = ( address );}while(0) \
                                                                       
//#define R16( address, destvar ) 	( destvar ) = ( address )
//#define R8( address, destvar ) 	( destvar ) = ( address )
#define W32( address, writedata )   \
    do{MSYNC; (address) = (writedata);}while(0)  \

#define W16( address, writedata )   \
    do{MSYNC; (address) = (writedata);}while(0)  \

#define W8( address, writedata )   \
  do{MSYNC; (address) = (writedata);}while(0)  \

//#define W16( address, writedata )	(address) = (writedata)
//#define W8( address, writedata )	(address) = (writedata)
//#define WR32( Reg, WrData ) 		                             		\
//do { 										\
//    UINT32 tmp;									\
//    (Reg)=(UINT32)(WrData);                           			\
//    tmp = (volatile UINT32)(Reg); 						\
//} while( 0 )
*/
//#define RE32( Reg, ExpData )							\
//do {										\
//    UINT32 tmp;									\
//    tmp = (volatile UINT32)(Reg);           \
//    if(tmp !==  ExpData )                 \            
//    } while( 0 )
//    if(tmp !==  ExpData )                 \            
    //COMMENT("the read data is not exception please check \n");                 \
// {                                           \ 
//     COMMENT("the read data is not exception please check \n"); \
//     COMMENT("the read data is");                              \ 
//     CPRINTW(ExpData);                                        \ 
//      COMMENT("the except data is");                            \
//     CPRINTW(tmp);                                                              \
//    } 

#define WRE32( Reg, WrData, ExpData )                               \
do {                          \
  UINT32 tmp;                 \
  /* write the reg */               \
  (Reg)=(UINT32)(WrData);                                               \
  /* read the reg */                \
  tmp = (volatile UINT32)(Reg);             \
	/* test the reg */                \
	if ( (UINT32)(tmp) != (UINT32)(ExpData) ) { /* means the test failed */     \
	  COMMENT("ERRORS:the read data is not exception please check");            \
    COMMENT("the read data is");\
    CPRINTW(tmp); \
    COMMENT("the except data is");\
    CPRINTW(ExpData);\
	} else { /* means test succeeded */           \
	  COMMENT("WRE32 Successful");            \
	} /* if test */               \
} while( 0 )

#define RE32( Reg, ExpData )							\
do {										\
    UINT32 tmp;									\
    tmp = (volatile UINT32)(Reg);           \
    if(tmp !=  ExpData )\
    { \
    COMMENT("ERRORS:the read data is not exception please check \n"); \
    COMMENT("the read data is");\
    CPRINTW(tmp); \
    COMMENT("the except data is");\
    CPRINTW(ExpData);\
    } \
}while(0) 

#define WRE16( Reg, WrData, ExpData )                               \
do {                          \
  UINT16 tmp;                 \
  /* write the reg */               \
  (Reg)=(UINT16)(WrData);                                               \
  /* read the reg */                \
  tmp = (volatile UINT16)(Reg);             \
	/* test the reg */                \
	if ( (UINT16)(tmp) != (UINT16)(ExpData) ) { /* means the test failed */     \
	  COMMENT("ERRORS:the read data is not exception please check");            \
    COMMENT("the read data is");\
    CPRINTW(tmp); \
    COMMENT("the except data is");\
    CPRINTW(ExpData);\
	} else { /* means test succeeded */           \
	  COMMENT("WRE32 Successful");            \
	} /* if test */               \
} while( 0 )

#define RE16( Reg, ExpData )							\
do {										\
    UINT16 tmp;									\
    tmp = (volatile UINT16)(Reg);           \
    if(tmp !=  ExpData )\
    { \
    COMMENT("ERRORS:the read data is not exception please check \n"); \
    COMMENT("the read data is");\
    CPRINTW(tmp); \
    COMMENT("the except data is");\
    CPRINTW(ExpData);\
    } \
}while(0) 

#define WRE8( Reg, WrData, ExpData )                               \
do {                          \
  UINT8 tmp;                 \
  /* write the reg */               \
  (Reg)=(UINT8)(WrData);                                               \
  /* read the reg */                \
  tmp = (volatile UINT8)(Reg);             \
	/* test the reg */                \
	if ( (UINT8)(tmp) != (UINT8)(ExpData) ) { /* means the test failed */     \
	  COMMENT("ERRORS:the read data is not exception please check");            \
    COMMENT("the read data is");\
    CPRINTW(tmp); \
    COMMENT("the except data is");\
    CPRINTW(ExpData);\
	} else { /* means test succeeded */           \
	  COMMENT("WRE32 Successful");            \
	} /* if test */               \
} while( 0 )

#define RE8( Reg, ExpData )							\
do {										\
    UINT8 tmp;									\
    tmp = (volatile UINT8)(Reg);           \
    if(tmp !=  ExpData )\
    { \
    COMMENT("ERRORS:the read data is not exception please check \n"); \
    COMMENT("the read data is");\
    CPRINTW(tmp); \
    COMMENT("the except data is");\
    CPRINTW(ExpData);\
    } \
}while(0)

#define WR32( Reg, WrData ) 		                             		\
do { 										\
    UINT32 tmp;									\
    /* write the reg */								\
    (Reg)=(UINT32)(WrData);                                         			\
    /* read the reg */								\
    tmp = (volatile UINT32)(Reg); 							\
    /* write the CPRNT */							\
    /*CPRNT_ADDR = (UINT32)&(Reg); 	*/					\
   /* CPRNT_32BIT = (WrData);*/ 							\
   /* CPRNT_32BIT = tmp; 		*/						\
    /* test the reg */								\
    if ( (UINT32)(tmp) != (UINT32)(WrData)) {       			\
      COMMENT("ERRORS:the read data is not exception please check");}						\
   /* } else {  this means the test succeeded 		*/		\
     /* COMMENT("GOOD:the read data is exception please check");	*/					\
    /*}  if test  */								\
} while( 0 )									
                                                                       
// store a word $data to $addr
#define STW(data,addr) asm volatile("stwx %0,0,%1"::"r"(data),"r"(addr))

// load a word $data from $addr
#define LDW(data,addr) asm volatile("lwzx %0,0,%1":"=r"(data):"r"(addr))

// store a byte $data to $addr
#define STB(data,addr) asm volatile("stbx %0,0,%1"::"r"(data),"r"(addr))

// load a byte $data from $addr
#define LDB(data,addr) asm volatile("lbzx %0,0,%1":"=r"(data):"r"(addr))

// store a halfword $data to $addr
#define STH(data,addr) asm volatile("sthx %0,0,%1"::"r"(data),"r"(addr))

// load a halfword $data from $addr
#define LDH(data,addr) asm volatile("lhzx %0,0,%1":"=r"(data):"r"(addr))

// store a word $data to $addr+bias
#define STWZ(data,addr,bias) asm volatile("stw %0,%1(%2)"::"r"(data),"n"(bias),"b"(addr))

// load a word $data from $addr+bias
#define LWZ(data,addr,bias) asm volatile("lwz %0,%1(%2)":"=r"(data):"n"(bias),"b"(addr))

// atom store (store word conditional indexed)
#define STWCX(data,addr) asm volatile("stwcx. %0,0,%1"::"r"(data),"r"(addr))

// atom load (load word and reserve indexed)
#define LWARX(data,addr) asm volatile("lwarx %0,0,%1":"=r"(data):"r"(addr))

// read $data from $spr index
#define MFSPR(data,spr) asm volatile("mfspr %0,%1" : "=r" (data) : "n" (spr))

// write $data to $spr index
#define MTSPR(spr,data) asm volatile("mtspr %0,%1" : : "n" (spr), "r" (data))

// read $data from MSR
#define MFMSR(data)      asm volatile("mfmsr %0" : "=r" (data) : )

// write $data to MSR
#define MTMSR(data)      asm volatile("mtmsr %0" : : "r" (data))

// read $data from $dcr address
#define MFDCR(data,dcr)  asm volatile("mfdcrx %0,%1" : "=r" (data) : "r" (dcr))
#define BMFDCR_B(data,dcr) asm volatile("mfdcrx %0,%1" : "=r" (data) : "r" (dcr|0x80000000))
#define BMFDCR_H(data,dcr) asm volatile("mfdcrx %0,%1" : "=r" (data) : "r" (dcr|0xA0000000))
#define BMFDCR_W(data,dcr) asm volatile("mfdcrx %0,%1" : "=r" (data) : "r" (dcr|0xC0000000))


// wirte $data to $dcr address
#define MTDCR(dcr,data)  asm volatile("mtdcrx %0,%1" : : "r" (dcr), "r" (data))
#define BMTDCR_B(dcr,data)  asm volatile("mtdcrx %0,%1" : : "r" (dcr|0x80000000), "r" (data))
#define BMTDCR_H(dcr,data)  asm volatile("mtdcrx %0,%1" : : "r" (dcr|0xA0000000), "r" (data))
#define BMTDCR_W(dcr,data)  asm volatile("mtdcrx %0,%1" : : "r" (dcr|0xC0000000), "r" (data))

//#define BDNZ(loop)		asm volatile("bdnz %0"::"n"(loop));
// insert a conditional soft-breakpoint
#define COND_TRAP(cond,ra,rb) asm volatile("tw %0,%1,%2"::"n"(cond),"r"(ra),"r"(rb))

// unconditional jump to address, which is saved in CTR
#define BCTR           asm volatile("bctr")

// unconditional jump to addr, which is saved in CTR and save return addr into LR
#define BCTRL          asm volatile("bctrl")

// unconditional jump to addr, which is saved in LR
#define BLR          asm volatile("blr")

// ensure load and store order
//#define MSYNC          asm volatile("msync")

// insert a smal gap between this ld/st and next ld/st, like a small msync
#define MBAR           asm volatile("mbar")

// ensure instruction excution order
#define ISYNC          asm volatile("isync")

// start a system call exception
#define SYSTEM_CALL    asm volatile("sc")

// clear 0:$num-1 bits of $data
#define CLR_HIGH_BITS(data,num)	asm volatile("rlwinm %0,%1,0,%2,31" : "=r" (data) : "r" (data),"n" (num))

// enable trace function, set CCR0[0]=1 by r2
#define TRACE_ENABLE    asm volatile("mfspr 2,0x3B3");\
			asm volatile("oris  2,2,0x8000");\
			asm volatile("mtspr 0x3B3,2")

// disable trace function, set CCR0[0]=0 by r2
#define TRACE_DISABLE   asm volatile("mfspr 2,0x3B3");\
			asm volatile("clrlwi  2,2,0x1");\
			asm volatile("mtspr 0x3B3,2")

// declare a function to set TLB, with 5 arguments
// mode: way select and bolted set
// va:   virtual address, CPU's 32bits addr
// size: TLB page size
// attr: TLB page's attributes, like cacke_disable, read,write,exe protection, endian, and etc.
// ra:   real address, SoC's 42bits addr 
void SetTLB(int mode, unsigned va, unsigned size, unsigned attr, unsigned ra);

// declare a function to clear TLB, with only one argument: TLB entry index
void ClrTLB(unsigned va, unsigned size);

// define printer's trigger address
#define CPRNT_COMMENT		(*(unsigned int volatile *)0xF6000034)
#define CPRNT_32BIT		(*(unsigned int volatile *)0xF6000038)
#define CPRNT_16BIT		(*(unsigned int volatile *)0xF600003C)
#define CPRNT_8BIT		(*(unsigned int volatile *)0xF6000040)

// print a string(should not have any variable) to .out file
//#define COMMENT(String)  	(CPRNT_COMMENT) = (volatile)(String)

// print word, halfword and byte variable in hex format to .out file
//#define CPRINTW(data)\
//do {\
//    CPRNT_32BIT = (volatile unsigned int)(data);\
//} while( 0 )

#define COMMENT	
#define CPRINTW		uart_printf

#define CPRINTHW(data)\
do {\
    CPRNT_16BIT = (volatile unsigned int)(data);\
} while( 0 )
								  
#define CPRINTBYTE(data)\
do {\
    CPRNT_8BIT = (volatile unsigned int)(data);\
} while( 0 )
// DUMPFSDB
#define DUMPFASBOFF_ADDR  (*(unsigned int volatile *)0xF6000044)
#define DUMPFSDBOFF_DATA   0x7654321F
#define DUMPOFF          DUMPFASBOFF_ADDR = DUMPFSDBOFF_DATA
#define DUMPFSDBON_ADDR   (*(unsigned int volatile *)0xF6000048)
#define DUMPFSDBON_DATA   0xF1234567
#define DUMPON            DUMPFSDBON_ADDR = DUMPFSDBON_DATA 

// declare 4 interrupt handler entrance address table for each cpu	
extern void (* volatile vec_table0[])(void);
extern void (* volatile vec_table1[])(void);
extern void (* volatile vec_table2[])(void);
extern void (* volatile vec_table3[])(void);

extern void mpic_unmask_Num(int VectorNum);

// Installs a user defined function into the vec_table as the handler for an interrupt
// Example Call: InstallInterruptHandler0( BUSERR, bus_error_handler );
// where, $bus_error_handler is the user defined inteerrupt handler for $BUSERR interrupt
#define InstallInterruptHandler0( VectorNum, Fxn )\
do{\
  vec_table0[ (VectorNum) ] =  (Fxn);\
  mpic_unmask_Num(VectorNum);\
}while(0)

#define InstallInterruptHandler1( VectorNum, Fxn )\
do{\
  vec_table1[ (VectorNum) ] =  (Fxn);\
}while(0)

#define InstallInterruptHandler2( VectorNum, Fxn )\
do{\
  vec_table2[ (VectorNum) ] =  (Fxn);\
}while(0)

#define InstallInterruptHandler3( VectorNum, Fxn )\
do{\
  vec_table3[ (VectorNum) ] =  (Fxn);\
}while(0)	
// Mask MPIC interrupt
#define MASK_INT(int_num)  MTDCR( (int_num*0x20 + 0x10000 + MPIC_BASE_ADDR), MPIC_VP_PRI_0)  

// un-mask MPIC interrupt, default priorty=3, trigger=posedge, vector=int_num
#define EN_INT(int_num)   MTDCR( (int_num*0x20 + 0x10000 + MPIC_BASE_ADDR), ( (MPIC_VP_PRI_3 | MPIC_VP_POL_HP) | int_num) )

// un-mask MPIC interrupt, default priorty=3, trigger=posedge, vector=int_num level sensative
#define EN_INT_LVL(int_num) MTDCR( (int_num*0x20 + 0x10000 + MPIC_BASE_ADDR), ( (MPIC_VP_PRI_3 | MPIC_VP_POL_HP | MPIC_VP_SEN_LV) | int_num) )

// CHECK_SPR can be used in an exception handler, 
// its function is to save exception SPRs to GPRs for better debugging
// DEAR to r17,  ESR to r18,   MCSR to r19,   SRR0 to r20,  SRR1 to r21
// CSRR0 to r22, CSRR1 to r23, MCSRR0 to r24, MCSRR1 to r25
#define CHECK_SPR       asm("mfspr 17, 0x3d ");\
 			asm("mfspr 18, 0x3e "); \
			asm("mfspr 19, 0x23c");  \
			asm("mfspr 20, 26");\
			asm("mfspr 21, 27");\
			asm("mfspr 22, 0x3a");\
			asm("mfspr 23, 0x3b");\
			asm("mfspr 24, 0x23a");\
			asm("mfspr 25, 0x23b") 

// ywang's magic spell
#define LONG_JUMP2FAILED  asm("b $")
		         // asm("ori 16, 16, failed@l"); \
		        //  asm("mtctr 16 "); \
		        //  asm("bctr")
		     
#define LONG_JUMP2PASSED  asm("lis 16, passed@h"); \
		          asm("ori 16, 16, passed@l"); \
		          asm("mtctr 16 "); \
		          asm("bctr")

/*******************************************************************************
stuff for the Comparitors 
*******************************************************************************/
/* Macros */
#define TRIG_DEBUG	0x80

#define TRIG_WORD	0x10
#define TRIG_HALF	0x08
#define TRIG_BYTE	0x04

#define TRIG_32BIT	0x10
#define TRIG_16BIT	0x08
#define TRIG_8BIT	0x04

//#define TRIG_READ	0x02
#define TRIG_WRITE	0x01
#define TRIG_DISABLED	0x00

#define TRIG0	0x00
#define TRIG1 	0x01
#define TRIG2 	0x02
#define TRIG3 	0x03
#define TRIG4 	0x04
#define TRIG5 	0x05
#define TRIG6 	0x06
#define TRIG7 	0x07


#define FLAGS0		(*(volatile char  *)0xF600FF60)
#define FLAGS1		(*(volatile char  *)0xF600FF61)
#define FLAGS2		(*(volatile char  *)0xF600FF62)
#define FLAGS3		(*(volatile char  *)0xF600FF63)
#define FLAGS4		(*(volatile char  *)0xF600FF64)
#define FLAGS5		(*(volatile char  *)0xF600FF65)
#define FLAGS6		(*(volatile char  *)0xF600FF66)
#define FLAGS7		(*(volatile char  *)0xF600FF67)

#define ADDRESS0	(*(volatile void **)0xF600FF68)
#define ADDRESS1	(*(volatile void **)0xF600FF6c)
#define ADDRESS2	(*(volatile void **)0xF600FF70)
#define ADDRESS3	(*(volatile void **)0xF600FF74)
#define ADDRESS4	(*(volatile void **)0xF600FF78)
#define ADDRESS5	(*(volatile void **)0xF600FF7c)
#define ADDRESS6	(*(volatile void **)0xF600FF80)
#define ADDRESS7	(*(volatile void **)0xF600FF84)

/******************************************************************************
 * Macro:    SetTrigger  (Initializes the specific address compare unit)
 *
 * Parameters:  	UINT8 Unit	The trigger unit to setup. Possible values are:
					TRIG0	
					TRIG1	
					TRIG2	
					TRIG3	
					TRIG4	
					TRIG5	
					TRIG6	
					TRIG7	
	VOID *Addr	The address which will fire the trigger
	UINT8 Flags 	Flags which determine what kind of bus cycle the trigger matches.
				Possible flags:
				TRIG_DEBUG	This turns on trigger debugging - you will see
						every cycle and whether the trigger matches
				TRIG_BYTE	Match on Byte size cycles
				TRIG_HALF	Match on Half Word size cycles
				TRIG_WORD	Match on Word size cycles
				TRIG_READ	Match on read cycles
				TRIG_WRITE	Match on write cycles
		

 *
 * Returns:     
 *
 * Notes:      
 *****************************************************************************/

#define SetTrigger( Unit, Addr, FLAGS )		\
do {					\
  if( (Unit) == TRIG0 ) {		\
    ADDRESS0 	= (Addr);		\
    FLAGS0 	= (FLAGS);		\
  } else if( (Unit) == TRIG1 ) {	\
    ADDRESS1 	= (Addr);		\
    FLAGS1 	= (FLAGS);		\
  } else if( (Unit) == TRIG2 ) {	\
    ADDRESS2 	= (Addr);		\
    FLAGS2 	= (FLAGS);		\
  }  else if( (Unit) == TRIG3 ) {	\
    ADDRESS3 	= (Addr);		\
    FLAGS3 	= (FLAGS);		\
  }  else if( (Unit) == TRIG4 ) {	\
    ADDRESS4 	= (Addr);		\
    FLAGS4 	= (FLAGS);		\
  }  else if( (Unit) == TRIG5 ) {	\
    ADDRESS5 	= (Addr);		\
    FLAGS5 	= (FLAGS);		\
  }  else if( (Unit) == TRIG6 ) {	\
    ADDRESS6 	= (Addr);		\
    FLAGS6 	= (FLAGS);		\
  }  else if( (Unit) == TRIG7 ) {	\
    ADDRESS7 	= (Addr);		\
    FLAGS7 	= (FLAGS);		\
  } else {		\
    COMMENT("ERROR: SetTrigger(): unrecognized Trigger_unit \n");		\
  }					\
} while( 0 )
       

#define  TRIG_EVENT0_ADDRESS  (*(volatile int *)0xF600FF90)
#define  TRIG_EVENT1_ADDRESS  (*(volatile int *)0xF600FF94)
#define  TRIG_EVENT2_ADDRESS  (*(volatile int *)0xF600FF98)
#define  TRIG_EVENT3_ADDRESS  (*(volatile int *)0xF600FF9c)
#define  TRIG_EVENT4_ADDRESS  (*(volatile int *)0xF600FFa0)
#define  TRIG_EVENT5_ADDRESS  (*(volatile int *)0xF600FFa4)
#define  TRIG_EVENT6_ADDRESS  (*(volatile int *)0xF600FFa8)
#define  TRIG_EVENT7_ADDRESS  (*(volatile int *)0xF600FFac)


#define TRIG_EVENT0     TRIG_EVENT0_ADDRESS = 0x0DDBEE;
#define TRIG_EVENT1     TRIG_EVENT1_ADDRESS = 0x0DDBEE;
#define TRIG_EVENT2     TRIG_EVENT2_ADDRESS = 0x0DDBEE;
#define TRIG_EVENT3     TRIG_EVENT3_ADDRESS = 0x0DDBEE;
#define TRIG_EVENT4     TRIG_EVENT4_ADDRESS = 0x0DDBEE;
#define TRIG_EVENT5     TRIG_EVENT5_ADDRESS = 0x0DDBEE;
#define TRIG_EVENT6     TRIG_EVENT6_ADDRESS = 0x0DDBEE;
#define TRIG_EVENT7     TRIG_EVENT7_ADDRESS = 0x0DDBEE;


#define FLG0_DAT  (*(volatile unsigned int*)0xF6000100)
#define FLG1_DAT  (*(volatile unsigned int*)0xF6000104)
#define FLG2_DAT  (*(volatile unsigned int*)0xF6000108)
#define FLG3_DAT  (*(volatile unsigned int*)0xF600010C)
#define FLG4_DAT  (*(volatile unsigned int*)0xF6000110)
#define FLG5_DAT  (*(volatile unsigned int*)0xF6000114)
#define FLG6_DAT  (*(volatile unsigned int*)0xF6000118)
#define FLG7_DAT  (*(volatile unsigned int*)0xF600011C)

// rst_state[15:0] is a half-word register
#define RST_STATE (*(volatile unsigned short*)0xF6000120)

#endif /* PPC_ASM_H */
