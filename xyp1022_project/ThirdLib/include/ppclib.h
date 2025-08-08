/*----------------------------------------------------------------------------+
|       COPYRIGHT   I B M   CORPORATION 2000, 2011
|       LICENSED MATERIAL  -  PROGRAM PROPERTY OF I B M
|       US Government Users Restricted Rights - Use, duplication or
|       disclosure restricted by GSA ADP Schedule Contract with
|       IBM Corp.
+----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------+
| $Author: tyrlik@us.ibm.com $
| $Date: 2011-06-29 14:13:02 -0400 (Wed, 29 Jun 2011) $
| $Revision: 14924 $
+----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------+
| 476PFE BSP for EPOS
| Author: Maciej P. Tyrlik
| Component: Include file.
| File: ppclib.h
| Purpose: PPC functions.
| Changes:
| Date          Comment:
| ---------     --------
| 17-Sep-10     Port to GTC						    MPT
| 11-Mar-11     Add reset_chip				 		    SAW
| 23-Mar-11     Added ppctlbsx(), TLB word 1 defines			    MPT
| 26-Mar-11     Fixed ppctlbsx prototype				    MPT
| 05-Apr-11     Added TLB word 3 bits				            MPT
| 12-Apr-11     Added ppc_is_cacheable()				    MPT
| 14-Apr-11     Added ppc_strtoul()					    MPT
| 25-Apr-11     Renamed reset to reset_core				    MPT
| 27-Apr-11     Added dcr_read_l2() and dcr_write_l2()			    WB
| 27-Apr-11     Updted dcr_read_l2() and dcr_write_l2() prototype           MPT
| 19-May-11     Added ppctlb_write_perm()				    MPT
| 22-May-11     Added test functions					    MPT
| 25-May-11     Added another test function				    MPT
| 09-Jun-11     Added additional TLB defines				    MPT
| 24-Jun-11     Added outword_nosync/inword_nosync			    MPT
+----------------------------------------------------------------------------*/

#ifndef _ppclib_h_
#define _ppclib_h_

#ifdef __cplusplus
extern "C" {
#endif



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


#define ATBL 526
#define ATBU 527

#define TBL_R 528
#define TBU_R 529

#define TBL_W 584
#define TBU_W 585


// store a word $data to $addr
//#define STW(data,addr) asm volatile("stwx %0,0,%1"::"r"(data),"r"(addr))

// load a word $data from $addr
//#define LDW(data,addr) asm volatile("lwzx %0,0,%1":"=r"(data):"r"(addr))

// store a byte $data to $addr
#define STB(data,addr) asm volatile("stbx %0,0,%1"::"r"(data),"r"(addr))

// load a byte $data from $addr
#define LDB(data,addr) asm volatile("lbzx %0,0,%1":"=r"(data):"r"(addr))

// store a halfword $data to $addr
#define STH(data,addr) asm volatile("sthx %0,0,%1"::"r"(data),"r"(addr))

// load a halfword $data from $addr
#define LDH(data,addr) asm volatile("lhzx %0,0,%1":"=r"(data):"r"(addr))

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

// wirte $data to $dcr address
#define MTDCR(dcr,data)  asm volatile("mtdcrx %0,%1" : : "r" (dcr), "r" (data))

// insert a conditional soft-breakpoint
#define COND_TRAP(cond,ra,rb) asm volatile("tw %0,%1,%2"::"n"(cond),"r"(ra),"r"(rb))

// unconditional jump to address, which is saved in CTR
#define BCTR           asm volatile("bctr")

// unconditional jump to addr, which is saved in CTR and save return addr into LR
#define BCTRL          asm volatile("bctrl")

// ensure load and store order
#define MSYNC          asm volatile("msync")

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


/*----------------------------------------------------------------------------+
| Instructions.
+----------------------------------------------------------------------------*/
void ppcSync( void );
void ppcMsync( void );
void ppcMbar( void );
void ppcIsync( void );
unsigned long ppcCntlzw( unsigned long 		value );
void ppcDcbi(void *addr );
void ppcDcbf(void *addr );
void ppcDcbz(void *addr );
void ppcDcbst(void *addr );
void ppcDcil1(void );
void ppcDcil2( void );
void ppcIcbi(void *addr );
void ppcIcil1();
void ppcIcil2();

// declare a function to set TLB, with 5 arguments
// mode: way select and bolted set
// va:   virtual address, CPU's 32bits addr
// size: TLB page size
// attr: TLB page's attributes, like cacke_disable, read,write,exe protection, endian, and etc.
// ra:   real address, SoC's 42bits addr
extern void SetTLB(int mode, unsigned va, unsigned size, unsigned attr, unsigned ra);

// declare a function to clear TLB, with only one argument: TLB entry index
extern void ClrTLB(unsigned va, unsigned size);

// declare 4 interrupt handler entrance address table for each cpu
extern void (* volatile vec_table0[])(void);
extern void (* volatile vec_table1[])(void);
extern void (* volatile vec_table2[])(void);
extern void (* volatile vec_table3[])(void);

// Installs a user defined function into the vec_table as the handler for an interrupt
// Example Call: InstallInterruptHandler0( BUSERR, bus_error_handler );
// where, $bus_error_handler is the user defined inteerrupt handler for $BUSERR interrupt
//#define InstallInterruptHandler0( VectorNum, Fxn )\
//do{\
//  vec_table0[ (VectorNum) ] =  (Fxn);\
//}while(0)
//
//#define InstallInterruptHandler1( VectorNum, Fxn )\
//do{\
//  vec_table0[ (VectorNum) ] =  (Fxn);\
//}while(0)
//
//#define InstallInterruptHandler2( VectorNum, Fxn )\
//do{\
//  vec_table0[ (VectorNum) ] =  (Fxn);\
//}while(0)
//
//#define InstallInterruptHandler3( VectorNum, Fxn )\
//do{\
//  vec_table0[ (VectorNum) ] =  (Fxn);\
//}while(0)

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
//#define LONG_JUMP2FAILED  //asm("lis 16, failed@h"); \
		          asm("ori 16, 16, failed@l"); \
		          asm("mtctr 16 "); \
		          asm("bctr")

//#define LONG_JUMP2PASSED  //asm("lis 16, passed@h"); \
		          asm("ori 16, 16, passed@l"); \
		          asm("mtctr 16 "); \
		          asm("bctr")

// read $data from $dcr address
#define MFDCR(data,dcr)  asm volatile("mfdcrx %0,%1" : "=r" (data) : "r" (dcr))
// wirte $data to $dcr address
#define MTDCR(dcr,data)  asm volatile("mtdcrx %0,%1" : : "r" (dcr), "r" (data))

#define MFREG_B(val, addr)					val = (*(volatile unsigned char *)(addr))
#define MFREG_HW(val, addr)					val = (*(volatile unsigned short *)(addr))
#define MFREG_W(val, addr)					val = (*(volatile unsigned int *)(addr))

#define MTREG_B(addr, val)					(*(volatile unsigned char *)(addr)) = val
#define MTREG_HW(addr, val)					(*(volatile unsigned short *)(addr)) = val
#define MTREG_W(addr, val)					(*(volatile unsigned int *)(addr)) = val

/*----------------------------------------------------------------------------+
| Time base.
+----------------------------------------------------------------------------*/
typedef struct tb {
   unsigned long tbu;
   unsigned long tbl;
} tb_t;
void ppcMttb(tb_t *clock_data_ptr);
void ppcMftb(tb_t *clock_data_ptr);

extern void ppc_cache_on(unsigned int addr);

#ifdef __cplusplus
}
#endif

#endif /* _ppclib_h_ */
