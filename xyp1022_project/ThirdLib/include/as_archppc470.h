/*----------------------------------------------------------------------------+
|       COPYRIGHT   I B M   CORPORATION 2011
|       LICENSED MATERIAL  -  PROGRAM PROPERTY OF I B M
|	US Government Users Restricted Rights - Use, duplication or
|       disclosure restricted by GSA ADP Schedule Contract with
|	IBM Corp.
+----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------+
| $Author: adamsod@us.ibm.com $
| $Date: 2011-06-02 10:59:17 -0400 (Thu, 02 Jun 2011) $
| $Revision: 14359 $
+----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------+
| EPOS
| Author: Maciej P. Tyrlik
| Component: Include file.
| File: sys/as_archppc470.h
| Purpose: Assembler include file for PPC476 core.
| Changes:
| Date:		Comment:
| -----         --------
| 25-Jan-11     Created							    MPT
| 02-Mar-11     CCR1 register updates					    MPT
| 07-Apr-11     Updated CCR0 bits					    MPT
| 01-Jun-11     Updated MCSR bits                       ALS
+----------------------------------------------------------------------------*/

#ifndef AS_ARCHPPC470_H_
#define AS_ARCHPPC470_H_
/*----------------------------------------------------------------------------+
| PVR value for 470 core.  
+----------------------------------------------------------------------------*/
#define PVR_476         	0x00052000                          

/*----------------------------------------------------------------------------+
| Special Purpose Registers.  Xer, lr, ctr, srr0, srr1, sprg0, sprg1, sprg2,
| sprg3, pvr, tblr, tbur registers are defined in as_archppc.h.
+----------------------------------------------------------------------------*/
#define SPR_DEC			0x016	/* decrementer            		     */
#define SPR_PID			0x030	/* process ID                                */
#define SPR_DECAR    	0x036	/* decrementera auto reload write only       */
#define SPR_CSRR0    	0x03A	/* critical save resrotr 0                   */
#define SPR_CSRR1    	0x03B	/* critical save resrotr 1                   */
#define SPR_DEAR    	0x03D	/* data exeption address                     */
#define SPR_ESR         0x03E	/* execption syndrome                        */
#define SPR_IVPR    	0x03F	/* exeption vector prefix                    */
#define SPR_USPRG0      0x100	/* user special puspose general 0            */
#define SPR_SPRG3R		0x103   /* special purpose general 3 - read only     */
#define SPR_SPRG4R     	0x104   /* special purpose general 4 - read only     */
#define SPR_SPRG5R     	0x105   /* special purpose general 5 - read only     */
#define SPR_SPRG6R     	0x106   /* special purpose general 6 - read only     */
#define SPR_SPRG7R     	0x107   /* special purpose general 7 - read only     */
#define SPR_SPRG0W     	0x110   /* special purpose general 0 - write only    */
#define SPR_SPRG1W     	0x111   /* special purpose general 1 - write only    */
#define SPR_SPRG2W     	0x112   /* special purpose general 2 - write only    */
#define SPR_SPRG3W     	0x113   /* special purpose general 3 - write only    */
#define SPR_SPRG4W     	0x114   /* special purpose general 4 - write only    */
#define SPR_SPRG5W     	0x115   /* special purpose general 5 - write only    */
#define SPR_SPRG6W     	0x116   /* special purpose general 6 - write only    */
#define SPR_SPRG7W    	0x117   /* special purpose general 7 - write only    */
#define SPR_TBL       	0x11C   /* time base lower - privileged write only   */
#define SPR_TBU       	0x11D   /* time base upper - privileged write only   */
#define SPR_PIR    		0x11E   /* processor ID read only                    */
#define SPR_DBSR    	0x130	/* debug status                              */
#define SPR_DBCR0    	0x134	/* debug conreol 0              	     */
#define SPR_DBCR1    	0x135	/* debug conreol 1                           */
#define SPR_DBCR2    	0x136	/* debug conreol 2                           */
#define SPR_IAC1    	0x138	/* instruction address compare 1             */
#define SPR_IAC2    	0x139	/* instruction address compare 2             */
#define SPR_IAC3    	0x13A	/* instruction address compare 3             */
#define SPR_IAC4    	0x13B	/* instruction address compare 4             */
#define SPR_DAC1    	0x13C	/* data address compare 1                    */
#define SPR_DAC2    	0x13D	/* data address compare 2                    */
#define SPR_DVC1    	0x13E	/* data value compare 1                      */
#define SPR_DVC2    	0x13F	/* data value compare 2                      */
#define SPR_TSR    		0x150	/* timer status register                     */
#define SPR_TCR    		0x154	/* timer control register                    */
#define SPR_IVOR0    	0x190	/* interrupt vecotr offset 0                 */
#define SPR_IVOR1    	0x191	/* interrupt vecotr offset 1                 */
#define SPR_IVOR2    	0x192	/* interrupt vecotr offset 2                 */
#define SPR_IVOR3    	0x193	/* interrupt vecotr offset 3                 */
#define SPR_IVOR4    	0x194	/* interrupt vecotr offset 4                 */
#define SPR_IVOR5    	0x195	/* interrupt vecotr offset 5                 */
#define SPR_IVOR6    	0x196	/* interrupt vecotr offset 6                 */
#define SPR_IVOR7    	0x197	/* interrupt vecotr offset 7                 */
#define SPR_IVOR8    	0x198	/* interrupt vecotr offset 8                 */
#define SPR_IVOR9    	0x199	/* interrupt vecotr offset 9                 */
#define SPR_IVOR10    	0x19A	/* interrupt vecotr offset 10                */
#define SPR_IVOR11    	0x19B	/* interrupt vecotr offset 11                */
#define SPR_IVOR12    	0x19C	/* interrupt vecotr offset 12                */
#define SPR_IVOR13    	0x19D	/* interrupt vecotr offset 13                */
#define SPR_IVOR14    	0x19E	/* interrupt vecotr offset 14                */
#define SPR_IVOR15    	0x19F	/* interrupt vecotr offset 15                */
#define SPR_MCSRR0    	0x23A	/* machine check save restore 0              */
#define SPR_MCSRR1    	0x23B	/* machine check save restore 1              */
#define SPR_MCSR      	0x23C	/* machine check status register             */
#define SPR_SPRG8		0x25C	/* special purpose general 8		     */
#define SPR_MMUBE0      0x334   /* mmu bolted entry 0                        */
#define SPR_MMUBE1      0x335   /* mmu bolted entry 1                        */
#define SPR_RMPD        0x339   /* real mode page description                */
#define SPR_MCSR_CLR	0x33C	/* machine check status clear register       */
#define SPR_ISPCR       0x33D   /* invalidate search priority register       */
#define SPR_SSPCR       0x33E   /* supervisor search priority                */
#define SPR_USPCR       0x33F   /* user search priority                      */
#define SPR_PMUCC0_U    0x34A   /* performance monitor control user access   */
#define SPR_DCESR       0x352   /* data cache exception syndrom register     */
#define SPR_ICESR       0x353   /* instruction cache exception syndrom reg.  */
#define SPR_PMUCC0      0x35A   /* performance monitor control               */
#define SPR_IOCCR       0x35C   /* instruction opcode compare control        */
#define SPR_IOCR1       0x35D   /* instruction opcode compare 1              */
#define SPR_IOCR2       0x35E   /* instruction opcode compare 2              */
#define SPR_PWM         0x376   /* pule width modulation                     */
#define SPR_CCR1        0x378	/* core configuration for 440x5 only	     */
#define SPR_CCR2        0x379	/* core configuration for 440x5 only	     */
#define SPR_DCRIPR		0x37B	/* DCR imm prefix 			     */
#define SPR_RSTCFG		0x39B   /* reset configuration                       */
#define SPR_DCDBTRL     0x39C	/* data cache debug tag low                  */
#define SPR_DCDBTRH    	0x39D	/* data cache debug tag high                 */
#define SPR_ICDBTRL     0x39E	/* instruction cache debug tag low           */
#define SPR_ICDBTRH    	0x39F	/* instruction cache debug tag high          */
#define SPR_MMUCR      	0x3B2	/* memory management unit control            */
#define SPR_CCR0        0x3B3   /* core configuration                        */
#define SPR_ICDBDR      0x3D3   /* instrustion cache debug data              */ 
#define SPR_ICDBDR0     0x3D3   /* instruction cache data register 0         */
#define SPR_ICDBDR1     0x3D4   /* instruction cache data register 1         */
#define SPR_DBDR        0x3F3   /* debug data register                       */ 

/*----------------------------------------------------------------------------+
| Machine State Register.  MSR_EE, MSR_PR, MSR_FP, MSR_ME, MSR_FE0, MSR_FE1,
| register bits are defined in ppc.inc. 
+----------------------------------------------------------------------------*/
#define MSR_WE  	0x00040000     	/* wait state enable                 */
#define MSR_CE  	0x00020000  	/* critical interrupt enable         */
#define MSR_DWE 	0x00000400     	/* debug wait enable                 */
#define MSR_DE  	0x00000200  	/* debug interrupt enable            */
#define MSR_IS 		0x00000020 	/* instruction address space	     */
#define MSR_DS  	0x00000010  	/* data address space	             */
#define MSR_EE				0x00008000
#define MSR_PR				0x00004000
#define MSR_FP				0x00002000
#define MSR_ME				0x00001000
#define MSR_FE0				0x00000800
#define MSR_FE1				0x00000100

/*----------------------------------------------------------------------------+
| Timer status register TSR.
+----------------------------------------------------------------------------*/
#define TSR_ENW           		0x80000000
#define TSR_WIS           		0x40000000
#define TSR_WRS_MASK      		0x30000000
#define TSR_WD_CORE       		0x10000000
#define TSR_WD_CHIP       		0x20000000
#define TSR_WD_SYS        		0x30000000
#define TSR_PIT	          		0x08000000
#define TSR_FIT	          		0x04000000

/*----------------------------------------------------------------------------+
| Timer control register.
+----------------------------------------------------------------------------*/
#define TCR_WP_MASK	  		0xC0000000
#define TCR_WRC_MASK 	  		0x30000000
#define TCR_WD_CORE 	  		0x10000000
#define TCR_WD_CHIP 	  		0x20000000
#define TCR_WD_SYS  	  		0x30000000
#define TCR_WD_ENABLE 	  		0x08000000
#define TCR_PIT_ENABLE 	  		0x04000000
#define TCR_FIT_MASK  	  		0x03000000
#define TCR_FIT_ENABLE 	  		0x00800000
#define TCR_ARE        	  		0x00400000

/*----------------------------------------------------------------------------+
| Debug status register.
+----------------------------------------------------------------------------*/
#define DBSR_IDE	  		0x80000000
#define DBSR_UDE	  		0x40000000
#define DBSR_MRR_MASK 	  		0x30000000
#define DBSR_MRR_NORST	  		0x00000000
#define DBSR_MRR_CORE	  		0x10000000
#define DBSR_MRR_CHIP	  		0x20000000
#define DBSR_MRR_SYS	  		0x30000000
#define DBSR_ICMP	  		0x08000000
#define DBSR_BT		  		0x04000000
#define DBSR_IRPT	  		0x02000000
#define DBSR_TRAP	  		0x01000000
#define DBSR_IAC1	  		0x00800000
#define DBSR_IAC2	  		0x00400000
#define DBSR_IAC3	  		0x00200000
#define DBSR_IAC4	  		0x00100000
#define DBSR_DR1	  		0x00080000
#define DBSR_DW1	  		0x00040000
#define DBSR_DR2	  		0x00020000
#define DBSR_DW2	  		0x00010000
#define DBSR_RET	  		0x00008000
#define DBSR_IAC12ATS     	0x00000002
#define DBSR_IAC34ATS     	0x00000001


//------- CCR0 ------- {{{
#define CCR0_PRE_MASK	0x40000000
#define CCR0_CRPE_MASK	0x08000000
#define CCR0_ICS_MASK 	0x00200000
#define CCR0_DAPUIB_MASK	0x00100000
#define CCR0_ICWRIDX_MASK	0x000F0000
#define CCR0_DTB_MASK 	0x00008000
#define CCR0_FLSTA_MASK	0x00000100
#define CCR0_DQWPM_MASK	0x0000000C
#define CCR0_IQWPM_MASK	0x00000002
//------- ------- ------- }}}

//------- CCR1 ------- {{{
#define CCR1_ICDPEI_MASK 0x0C000000
#define CCR1_ICLPEI_MASK 0x03000000
#define CCR1_ICTPEI_MASK 0x00C00000
#define CCR1_DCDPEI_MASK 0x00300000
#define CCR1_DCLPEI_MASK 0x000C0000
#define CCR1_DCTPEI_MASK 0x00030000
#define CCR1_MMUTPEI_MASK	0x00008000
#define CCR1_MMUDPEI_MASK	0x00004000
#define CCR1_TSS_MASK	0x00001000
#define CCR1_DPC_MASK 	0x00000400
#define CCR1_TCS_MASK	0x00000300
//------- ------- ------- }}}

//------- CCR2 ------- {{{
#define CCR2_DSTG_MASK	0xC0000000
#define CCR2_DLFPD_MASK 0x20000000
#define CCR2_DSTI_MASK 	0x08000000
#define CCR2_PMUD_MASK 	0x00400000
#define CCR2_DCSTGW_MASK	0x00100000
#define CCR2_STGCTR_MASK	0x000F0000
#define CCR2_DISTG_MASK	0x00008000
#define CCR2_SPC5C1_MASK	0x00000800
#define CCR2_MCDTO_MASK	0x00000400


/*----------------------------------------------------------------------------+
| Core Configuration/MMU configuration.
+----------------------------------------------------------------------------*/
#define CCR0_PRE       	  	0x40000000
#define CCR0_CRPE      	  	0x08000000
#define CCR0_ICS			0x00200000
#define CCR0_DAPUIB	  		0x00100000
#define CCR0_DTB	  		0x00008000
#define CCR0_FLSTA 	  		0x00000100
#define CCR1_TSS_MASK       0x00001000
#define CCR1_TSS_INTCLK     0x00000000
#define CCR1_TSS_EXTCLK     0x00001000
#define CCR1_TCS_MASK  	  		0x00000300
#define CCR1_TCS_1 	 		0x00000000
#define CCR1_TCS_4  			0x00000100
#define CCR1_TCS_8  			0x00000200
#define CCR1_TCS_16  			0x00000300

#define MMUCR_REALE        		0x80000000
#define MMUCR_LWAY_MASK        		0x60000000
#define MMUCR_LVALID        		0x10000000
#define MMUCR_DULXE        		0x08000000
#define MMUCR_IULXE        		0x04000000
#define MMUCR_LINDEX_MASK     		0x01E00000
#define MMUCR_STS         		0x00010000
#define MMUCR_STID_MASK   		0x0000FFFF

/*----------------------------------------------------------------------------+
| Machine check status Register for 470x6.
+----------------------------------------------------------------------------*/
#define MCSR_MCS          		0x80000000
#define MCSR_TLB          		0x08000000
#define MCSR_IP           		0x04000000
#define MCSR_DP           		0x02000000
#define MCSR_GPR          		0x01000000
#define MCSR_FPR          		0x00800000
#define MCSR_IMP          		0x00400000
#define MCSR_L2           		0x00200000
#define MCSR_DCR          		0x00100000

#define MCSR_MSC_MASK	0x80000000
#define MCSR_TLB_MASK	0x08000000
#define MCSR_IC_MASK	0x04000000
#define MCSR_DC_MASK	0x02000000
#define MCSR_GPR_MASK	0x01000000
#define MCSR_FPR_MASK	0x00800000
#define MCSR_IMP_MASK	0x00400000
#define MCSR_L2_MASK	0x00200000
#define MCSR_DCR_MASK	0x00100000

/*----------------------------------------------------------------------------+
| Debug control register 0.
+----------------------------------------------------------------------------*/
#define DBCR0_EDM	  		0x80000000
#define DBCR0_IDM	  		0x40000000
#define DBCR0_CORE_RESET  		0x10000000
#define DBCR0_CHIP_RESET  		0x20000000
#define DBCR0_SYS_RESET	  		0x30000000
#define DBCR0_I_COMPLETE  		0x08000000
#define DBCR0_BT	  		0x04000000
#define DBCR0_IRPT	  		0x02000000
#define	DBCR0_TRAP	  		0x01000000
#define DBCR0_IA1	  		0x00800000
#define	DBCR0_IA2	  		0x00400000
#define DBCR0_IA3	  		0x00200000
#define DBCR0_IA4	  		0x00100000
#define DBCR0_D1R	  		0x00080000
#define DBCR0_D1W	  		0x00040000
#define DBCR0_D2R	  		0x00020000
#define DBCR0_D2W	  		0x00010000
#define DBCR0_RET	  		0x00008000
#define DBCR0_FT	  		0x00000001

/*----------------------------------------------------------------------------+
| Exception Syndrome Register.
+----------------------------------------------------------------------------*/
#define ESR_MCI     			0x80000000
#define ESR_SS     			0x40000000
#define ESR_POT1     			0x20000000
#define ESR_POT2     			0x10000000
#define ESR_PIL				0x08000000
#define ESR_PPR				0x04000000
#define ESR_PTR				0x02000000
#define ESR_FP				0x01000000
#define ESR_ST				0x00800000
#define ESR_DLK_MASK			0x00300000
#define ESR_DLK_NOLOCK			0x00000000
#define ESR_DLK_DCBF			0x00100000
#define ESR_DLK_ICBI			0x00200000
#define ESR_AP				0x00080000
#define ESR_PUO				0x00040000
#define ESR_BO				0x00020000
#define ESR_PIE				0x00010000
#define ESR_PCRE			0x00000010
#define ESR_PCMP			0x00000008
#define ESR_PCRF_MASK			0x00000007

/*----------------------------------------------------------------------------+
| TLB
+----------------------------------------------------------------------------*/
//------- TLB ------- {{{
#define TLB_WAY0_MASK	0x80000000
#define TLB_WAY1_MASK	0xA0000000
#define TLB_WAY2_MASK	0xC0000000
#define TLB_WAY3_MASK	0xE0000000
#define TLB_BOLT0_MASK	0x08000000
#define TLB_BOLT1_MASK	0x09000000
#define TLB_BOLT2_MASK	0x0A000000
#define TLB_BOLT3_MASK	0x0B000000
#define TLB_BOLT4_MASK	0x0C000000
#define TLB_BOLT5_MASK	0x0D000000

#define TLB0_EPN_MASK	0xFFFFF000
#define TLB0_V_MASK	0x00000800
#define TLB0_TS_MASK	0x00000400
#define TLB0_SIZE_MASK	0x000003F0
#define TLB0_SIZE_4K	0x00000000
#define TLB0_SIZE_16K	0x00000010
#define TLB0_SIZE_64K	0x00000030
#define TLB0_SIZE_1M	0x00000070
#define TLB0_SIZE_16M	0x000000F0
#define TLB0_SIZE_256M	0x000001F0
#define TLB0_SIZE_1G	0x000003F0

#define TLB1_RPN_MASK	0xFFFFF000
#define TLB1_ERPN_MASK 	0x000003FF

#define TLB2_IL1I_MASK 	0x00020000
#define TLB2_IL1D_MASK 	0x00010000
#define TLB2_U0_MASK	0x00008000
#define TLB2_U1_MASK	0x00004000
#define TLB2_U2_MASK	0x00002000
#define TLB2_U3_MASK	0x00001000
#define TLB2_W_MASK 	0x00000800
#define TLB2_I_MASK 	0x00000400
#define TLB2_M_MASK 	0x00000200
#define TLB2_G_MASK	0x00000100
#define TLB2_E_MASK	0x00000080
#define TLB2_E_BIG	0x00000000
#define TLB2_E_LIT 	0x00000080

#define TLB2_U_MASK 	0x00000038
#define TLB2_U_X 	0x00000020
#define TLB2_U_W	0x00000010
#define TLB2_U_R 	0x00000008
#define TLB2_U_WX 	0x00000030
#define TLB2_U_RX 	0x00000028
#define TLB2_U_RW 	0x00000018
#define TLB2_U_RWX 	0x00000038
#define TLB2_S_MASK 	0x00000007
#define TLB2_S_X	0x00000004
#define TLB2_S_W	0x00000002
#define TLB2_S_R	0x00000001
#define TLB2_S_WX	0x00000006
#define TLB2_S_RX	0x00000005
#define TLB2_S_RW	0x00000003
#define TLB2_S_RWX	0x00000007
//------- ------- ------- }}}
/*----------------------------------------------------------------------------+
| TLB defines.
+----------------------------------------------------------------------------*/
#define PPC_TLBWE_USE_WAY		0x80000000
#define PPC_TLBWE_WAY_NUM_MASK		0x60000000
#define PPC_TLBWE_BOLT			0x08000000
#define PPC_TLBWE_BOLT_NUM_MASK		0x07000000

#define PPC_TLBW1_RPN			0xFFFFF000
#define PPC_TLBW1_RPN_PARITY		0x00000800
#define PPC_TLBW1_ERPN_PARITY		0x00000400
#define PPC_TLBW1_ERPN			0x000003FF
#define PPC_TLBW2_W_BIT			0x00000800
#define PPC_TLBW2_I_BIT			0x00000400
#define PPC_TLBW2_M_BIT			0x00000200
#define PPC_TLBW2_G_BIT			0x00000100
#define PPC_TLBW2_E_BIT			0x00000080

#endif /* _sys_as_archppc470_h_ */
