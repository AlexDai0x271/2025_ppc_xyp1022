//#include "environment.h"
//#include "c0_raid_memory_map.h"
//#include "re_raid_memory_map.h"
//#include "c0.h"
//#include "util.h"
#include "ahci_atb.h"
//#include "re_ahci_atb.h"
//#include "iomacros.h"
//#include "axidma.h"
#include "iomacros.h"
#include "e500mc_asm.h"
#define SW_RST_CFG 0xeb0070c4

//#define SATA_SRAM_ADDR     0xe0030000
//#define SATA_SRAM_ADDR     0xe0050000   //sata test
//#define SATA_SRAM_CMD_ADDR     0xe0070000
#define SATA_SRAM_ADDR     0xe0062000   //sata test
#define SATA_SRAM_CMD_ADDR     0xe007a000
#define SATA_SRAM_ADDR_HI     0x00000004
#define  VTB_BIST_FAR_END_RETIMED 1
#define  VTB_BIST_FAR_END_ANALOG 2
#define  VTB_BIST_FAR_END_TRANSMIT_ONLY 3

#define SATA_INT_TEST 1
#define SATA_PME_TEST 0
#if SATA_INT_TEST
#define SATA_HOT_PLUG_TEST 0
#endif

#if SATA0_TEST
#define SATA_MISC_BASE         0xe9504000
#define SATA_SW_RST                   7
#define SATA_HOST_INT_NUM      75
#define SATA_HOST_MSI_INT_NUM  74
#define SATA_HOST_PME_INT_NUM  73
#endif
#if SATA1_TEST
#define SATA_MISC_BASE         0xe9505000
#define SATA_SW_RST                   8
#define SATA_HOST_INT_NUM      78
#define SATA_HOST_MSI_INT_NUM  77
#define SATA_HOST_PME_INT_NUM  76
#endif
#define SATA_PHY_PLL_SET       SATA_MISC_BASE|0x0a0
#define SATA_PHY_RX0_EN_RTERM  SATA_MISC_BASE|0x0c0
#define SATA_PHY_RESET_OUT     SATA_MISC_BASE|0x15c
#define SATA_PHY_REF_SSP_EN    SATA_MISC_BASE|0x044


//#define CMD_CODE_WRITE_DMA    	0x0CA
#define CMD_CODE_READ_DMA     	0x0C8
#define CMD_CODE_MEDIA_EJECT 	0x0E5
#define VTB_PM_PARTIAL			0x01
#define  VTB_PM_SLUMBER			0x02
//#define CMD_CODE_CFA_WRITE_MULTIPLE_W_OUT_ERASE          0x0CD
//#define CMD_CODE_READ_SECTOR_S                          0x020
UINT32 transfer_fns;
/*
#define IO_READ32(p)     (*(volatile  unsigned int *)(p))
#define IO_WRITE32(p,v)  (*((volatile  unsigned long *)(p)) = ( unsigned long)(v))

#define STW(data,addr)    IO_WRITE32(addr,data)
#define LDW(data,addr) 	  data = IO_READ32(addr)



#define MTDCR(addr,data) STW(data,addr)
#define MFDCR            LDW
*/
//#define MTDCR(addr,data) (*((  unsigned int *)(( unsigned long long)addr)) = ( unsigned int)(data))
//#define MFDCR(data,addr)    data = (*( unsigned int *)(addr))

//void MTDCR(unsigned int addr,unsigned int data )
//{
//	*(((  unsigned int *)addr)) = (data);
//}
//#define MFDCR(data,addr) 	  data = IO_READ32(addr)
//unsigned int MFDCR(unsigned int addr)
//{
//	return *(((  unsigned int *)addr));
//}
//#define MFDCR(( unsigned int)data,( unsigned int)addr) 	(data) = IO_READ32(addr)
//#define ISYNC
//#define MSYNC
//#define NOP10 asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");


UINT32 cap_data;
UINT32 pi_data;
UINT32 run_error;
UINT32 ncompare_en;


UINT32 g_dev_pm_support = 0;
UINT32 g_host_pm_support = 0;
UINT32 g_state_unplug = 0;

void delay1 ( UINT32 delay_tim )
{
  unsigned int count;

  for (count=0;count<= delay_tim;count++)
  {
    NOP10;
    NOP10;
    NOP10;
    NOP10;
  }
}


unsigned int swap_ward(unsigned int data)
{
	return (((data & 0x000000ff) << 24) | ((data & 0x0000ff00) << 8) | ((data & 0x00ff0000) >> 8) | ((data & 0xff000000) >> 24));
}
	

void hba_init(UINT32 port_num)
{
  UINT32 cmd_data;
  UINT32 bistcr_val;
  UINT32 ssts_data;
  UINT32 wrdata;
  UINT32 rdata;
  UINT32 sctl_data;
  UINT8 cwmin;
  UINT8 cwmax;
  UINT8 cimin;
  UINT8 cimax;
  UINT8 quick_init;
  UINT8 pt_cnt;
  UINT32 attempts;
  UINT32 idx;

  // Read Capabilities
  LDW(cap_data,ATB_GCSR_CAP);
  STW(0x0,ATB_GCSR_CAP);
  LDW(cap_data,ATB_GCSR_CAP);
  // Read Ports Implemented
  pi_data = 0x0;
 for( pt_cnt = 0; pt_cnt <= port_num; pt_cnt++ ) 
  pi_data=pi_data|(0x1<<pt_cnt);
  STW(pi_data,ATB_GCSR_PI);
  LDW(pi_data,ATB_GCSR_PI);
  LDW(bistcr_val,ATB_GCSR_BISTCR);
}



UINT32 diag_x=0;
int port_init(UINT32 port_num, UINT32 P0CLB, UINT32 P0CLBU, UINT32 P0FB, UINT32 P0FBU, UINT8 speed_sel)
{
  UINT32 cmd_data;
  UINT32 ssts_data;
  UINT32  sctl_data;
  UINT32 tfd_data;
  UINT32 ghc_data;
  UINT32 serr_data;
  UINT32 port_ready;
  UINT32 attempts;
  UINT32 spin_up_attempts;
  UINT32 tmp;
	port_ready = 0x0;
#if SATA_INT_TEST
	diag_x = 0;
#else
  LDW(serr_data,ATB_PCSR_P0SERR+0x80*port_num);
  if (serr_data&(DIAG_X_P0SERR))
	  STW(DIAG_X_P0SERR,ATB_PCSR_P0SERR+0x80*port_num);
 	diag_x = 0;
#endif
  LDW(cmd_data,ATB_PCSR_P0CMD+0x80*port_num);
  // Program CLB and FB registers
  STW(P0CLB,ATB_PCSR_P0CLB+0x80*port_num);
  STW(P0CLBU,ATB_PCSR_P0CLBU+0x80*port_num);
  STW(P0FB,ATB_PCSR_P0FB+0x80*port_num);
  STW(P0FBU,ATB_PCSR_P0FBU+0x80*port_num);
  // Program CMD.FRE
  cmd_data = cmd_data|0x10;// P#CMD.FRE
  STW(cmd_data,ATB_PCSR_P0CMD+0x80*port_num);
  uart_printf("speed is sata %d .0\n",speed_sel);
 
	 LDW(sctl_data,ATB_PCSR_P0SCTL+0x80*port_num);
//  sctl_data [3:0] = 4'b0001;
	 sctl_data = sctl_data & 0xffffff00;
	// sctl_data = sctl_data | 0x021;
	 sctl_data = sctl_data | 0x1 | (speed_sel<<4);
     STW(sctl_data,ATB_PCSR_P0SCTL+0x80*port_num);
	 delay1(10);
     sctl_data = sctl_data & 0xfffffff0;
	STW(sctl_data,ATB_PCSR_P0SCTL+0x80*port_num);
#if SATA_INT_TEST
	delay1(0x2000);
	attempts = 0;
	LDW(tmp,ATB_PCSR_P0SSTS);
	tmp &= DET_P0SSTS;
	if(tmp == 0){
		g_state_unplug = 1;//unplugged
		uart_printf("please insert device......\n");
	}	
	do{
		attempts++;
	}while(diag_x == 0);
#else	
  // Wait for DIAG_X to be set
  attempts = 0;
  while ((diag_x == 0x0) & (attempts < 0x2000)) 
  {
	  LDW(serr_data,ATB_PCSR_P0SERR+0x80*port_num);
     if (serr_data & DIAG_X_P0SERR)
        diag_x = 0x1;
     attempts = attempts + 1;
      
  }// while
#endif
 if (diag_x == 0x1) 
	{
    // Program SERR (clear DIAG_X bit)
    STW(DIAG_X_P0SERR,ATB_PCSR_P0SERR+0x80*port_num);
	}
	else 
	{
	uart_printf("[ERROR] port_init : Did not find DIAG_X set after %0d tries\n",attempts);
	while(1);
	}
  // Wait for the device to be detected
  attempts = 0;
    while (port_ready == 0)
         {
    	LDW(ssts_data,ATB_PCSR_P0SSTS+0x80*port_num);
    	LDW(tfd_data,ATB_PCSR_P0TFD+0x80*port_num);
     if (((ssts_data&0x3)==0x3)&((tfd_data&BSY_TFD)==0x0)&((tfd_data&DRQ_TFD) == 0x0)&((tfd_data&ERR_TFD) == 0x0))
                         port_ready = 1;
     attempts = attempts + 1;
         }
     uart_printf("port_init : Port %d Spinning after %d tries\n",port_num, attempts);
         // Once detected, set the drive spinning
   cmd_data = cmd_data|0x1; // P#CMD.ST
   STW(cmd_data,ATB_PCSR_P0CMD+0x80*port_num);
   uart_printf("port_init : Configuring Port  ... Don" );
   return 0;
}

void SATA_interrupt_isr(void)
{
	uart_printf_buf("SATA Interrupt \n");
	volatile U32 pending;
	UINT32 port_num;
	UINT32 tmp;
	
		LDW(port_num,ATB_GCSR_IS);
		port_num = port_num/2;//which port interrupt
		uart_printf("port %d interrupt \n",port_num);
		LDW(pending,ATB_PCSR_P0IS+0x80*port_num);
		//uart_printf("IS data is 0x%x \n",pending);
		if(pending & DHRE_P0IE){
			STW(DHRE_P0IE,ATB_PCSR_P0IS+0x80*port_num);
			uart_printf("------Device-to-Host register FIS");
		}
		if(pending & PSE_P0IE){
			STW(PSE_P0IE,ATB_PCSR_P0IS+0x80*port_num);
			uart_printf("------PIO Setup FIS");
		}
		if(pending & DPE_P0IE){
			STW(DPE_P0IE,ATB_PCSR_P0IS+0x80*port_num);
			uart_printf("------Descriptor Processed");
		}
		if(pending & PCE_P0IE){
			STW(PCE_P0IE,ATB_PCSR_P0IS+0x80*port_num);
			STW(DIAG_X_P0SERR,ATB_PCSR_P0SERR+0x80*port_num);
			diag_x = 1;
			uart_printf("------Port Connect Change");
			LDW(tmp,ATB_PCSR_P0SSTS);
			tmp &= DET_P0SSTS;
			if((tmp == 3) & (g_state_unplug == 1)){
				g_state_unplug = 0;
				uart_printf("device is detect...... \n");
			}
//			switch(tmp & DET_P0SSTS){
//			
//			case 0 : uart_printf("No device detected and PHY communication not established");break;
//			case 1 : uart_printf("Device presence detected but PHY communication not established (COMINIT is detected)");break;
//			case 3 : uart_printf("Device presence detected and PHY communication established (?¡ãPHY Ready?¡À is detected)");break;
//			case 4 : uart_printf("PHY in offline mode as a result of the interface being disabled or running in a BIST loopback mode");break;
//			}	
		}
		if(pending & PRCE_P0IE){
			STW(PRCE_P0IE,ATB_PCSR_P0IS+0x80*port_num);
			STW(DIAG_N_P0SERR,ATB_PCSR_P0SERR+0x80*port_num);
			uart_printf("------PHY Ready Change");
			LDW(tmp,ATB_PCSR_P0SSTS);
			tmp &= DET_P0SSTS;
			if(tmp == 0){
				g_state_unplug = 1;
#if SATA_HOT_PLUG_TEST
				uart_printf("please insert device...... \n");
#endif
			}
//			switch(tmp & DET_P0SSTS){
//			
//			case 0 : uart_printf("No device detected and PHY communication not established");break;
//			case 1 : uart_printf("Device presence detected but PHY communication not established (COMINIT is detected)");break;
//			case 3 : uart_printf("Device presence detected and PHY communication established (?¡ãPHY Ready?¡À is detected)");break;
//			case 4 : uart_printf("PHY in offline mode as a result of the interface being disabled or running in a BIST loopback mode");break;
//			}
		}
		if(pending & OFE_P0IE){
			STW(OFE_P0IE,ATB_PCSR_P0IS+0x80*port_num);
			uart_printf("------Overflow");
		}
		if(pending & INFE_P0IE){
			STW(INFE_P0IE,ATB_PCSR_P0IS+0x80*port_num);
			uart_printf("------Interface Non-fatal Error");
		}
		if(pending & IFE_P0IE){
			STW(IFE_P0IE,ATB_PCSR_P0IS+0x80*port_num);
			uart_printf("------Interface Fatal Error");
		}
		LDW(pending,ATB_PCSR_P0IS+0x80*port_num);
	if(pending == 0){
		STW(1 << port_num,ATB_GCSR_IS);
	}
}

void SATA_MSI_interrupt_isr(void)
{
	uart_printf_buf("SATA MSI Interrupt \n");	
}

void SATA_PME_interrupt_isr(void)
{
	uart_printf_buf("SATA PME Interrupt \n");	
}

void real_sata_read_32(unsigned int port, unsigned int lba32, unsigned int sectors) {
  unsigned int *cmd = (unsigned int *)(SATA_SRAM_CMD_ADDR );
  cmd[0] = swap_ward(PRDTL(0x1)|CFL(0x5)); // is_read
  cmd[1] = swap_ward(0x0);//PRDBC
  cmd[2] = swap_ward((SATA_SRAM_CMD_ADDR + 0x400));
  cmd[3] = swap_ward(SATA_SRAM_ADDR_HI);
  unsigned int *fis = (unsigned int *)(SATA_SRAM_CMD_ADDR + 0x400);
  fis[0] = swap_ward(Command_H2D(0x25)|C_H2D|FisType(0x27)); // is_read
  fis[1] = swap_ward((0x40 << 24) | (lba32 & 0x00ffffff));
  fis[2] =  swap_ward((lba32 & 0xff000000)>>24);
  fis[3] = swap_ward(sectors);
  fis[4] = swap_ward(0x0);
  unsigned int *prd = (unsigned int *)(SATA_SRAM_CMD_ADDR + 0x480);
  prd[0] =swap_ward((SATA_SRAM_ADDR + 0xc000));
  prd[1] = swap_ward(SATA_SRAM_ADDR_HI);
  prd[2] = swap_ward(0);
  prd[3] = swap_ward(0x80000000 | (sectors * 512 - 1));
  unsigned int ci;
  STW(1,ATB_PCSR_P0CI);
  do {
	  LDW(ci,ATB_PCSR_P0CI);
  } while (ci != 0);
}
void real_sata_write_32(unsigned int port, unsigned int lba32, unsigned int sectors) {
  unsigned int *cmd = (unsigned int *)(SATA_SRAM_CMD_ADDR );
  cmd[0] = swap_ward(PRDTL(0x1)|CFL(0x5)|W_Write); // is_read
  cmd[1] = swap_ward(0x0);//PRDBC
  cmd[2] = swap_ward((SATA_SRAM_CMD_ADDR + 0x400));
  cmd[3] = swap_ward(SATA_SRAM_ADDR_HI);
  unsigned int *fis = (unsigned int *)(SATA_SRAM_CMD_ADDR + 0x400);
  fis[0] = swap_ward(Command_H2D(0x35)|C_H2D|FisType(0x27)); // is_read
  fis[1] = swap_ward((0x40 << 24) | (lba32 & 0x00ffffff));
  fis[2] =  swap_ward((lba32 & 0xff000000)>>24);
  fis[3] = swap_ward(sectors);
  fis[4] = swap_ward(0x0);
  unsigned int *prd = (unsigned int *)(SATA_SRAM_CMD_ADDR + 0x480);
  prd[0] =swap_ward((SATA_SRAM_ADDR + 0x8000));
  prd[1] = swap_ward(SATA_SRAM_ADDR_HI);
  prd[2] = swap_ward(0);
  prd[3] = swap_ward(0x80000000 | (sectors * 512 - 1));
  //prd[3] = swap_ward(0x80000000 | 0x1FF);
  //prd[3] = swap_ward(0x80000000 | 0);
  unsigned int ci;
  STW(1,ATB_PCSR_P0CI);
  do {
	  LDW(ci,ATB_PCSR_P0CI);
  } while (ci != 0);
}
void pm_agg_sata_write_32(unsigned int port, unsigned int lba32, unsigned int sectors) {
  unsigned int *cmd = (unsigned int *)(SATA_SRAM_CMD_ADDR );
  cmd[0] = swap_ward(PRDTL(0x1)|CFL(0x5)|W_Write); // is_read
  cmd[1] = swap_ward(0x0);//PRDBC
  cmd[2] = swap_ward((SATA_SRAM_CMD_ADDR + 0x400));
  cmd[3] = swap_ward(SATA_SRAM_ADDR_HI);
  unsigned int *fis = (unsigned int *)(SATA_SRAM_CMD_ADDR + 0x400);
  fis[0] = swap_ward(Command_H2D(0x35)|C_H2D|FisType(0x27)); // is_read
  fis[1] = swap_ward((0x40 << 24) | (lba32 & 0x00ffffff));
  fis[2] =  swap_ward((lba32 & 0xff000000)>>24);
  fis[3] = swap_ward(sectors);
  fis[4] = swap_ward(0x0);
  unsigned int *prd = (unsigned int *)(SATA_SRAM_CMD_ADDR + 0x480);
  prd[0] =swap_ward((SATA_SRAM_ADDR + 0x8000));
  prd[1] = swap_ward(SATA_SRAM_ADDR_HI);
  prd[2] = swap_ward(0);
  prd[3] = swap_ward(0x80000000 | (sectors * 512 - 1));
  //prd[3] = swap_ward(0x80000000 | 0);  
  unsigned int ci,tmp;
  LDW(tmp,ATB_PCSR_P0CMD);
  tmp &= 0x0fffffff;
  tmp |= (PM_ACTIVE << 28);//Aggressive Link Power Management need active
  STW(tmp,ATB_PCSR_P0CMD);
  STW(1,ATB_PCSR_P0CI);
  do {
	  LDW(ci,ATB_PCSR_P0CI);
  } while (ci != 0);
}
void device_pme(unsigned int state,unsigned int standby_time){
	unsigned int *cmd = (unsigned int *)(SATA_SRAM_CMD_ADDR );
	cmd[0] = swap_ward(PRDTL(0x1)|CFL(0x5)|W_Write); // is_read
	cmd[1] = swap_ward(0x0);//PRDBC
	cmd[2] = swap_ward((SATA_SRAM_CMD_ADDR + 0x400));
	cmd[3] = swap_ward(SATA_SRAM_ADDR_HI);
	unsigned int *fis = (unsigned int *)(SATA_SRAM_CMD_ADDR + 0x400);
	fis[0] = swap_ward(0x00008027 | (state << 16));
	fis[1] = swap_ward(0x40000000);
	fis[2] = swap_ward(0x0);
	fis[3] = swap_ward(standby_time);
	fis[4] = swap_ward(0x0);
	unsigned int ci;
	STW(1,ATB_PCSR_P0CI);
	do {
        LDW(ci,ATB_PCSR_P0CI);
	} while (ci != 0);
}
void device_interface_pme()
{
	unsigned int *cmd = (unsigned int *)(SATA_SRAM_CMD_ADDR );
	cmd[0] = swap_ward(PRDTL(0x1)|CFL(0x5)|W_Write); // is_read
	cmd[1] = swap_ward(0x0);//PRDBC
	cmd[2] = swap_ward((SATA_SRAM_CMD_ADDR + 0x400));
	cmd[3] = swap_ward(SATA_SRAM_ADDR_HI);
	unsigned int *fis = (unsigned int *)(SATA_SRAM_CMD_ADDR + 0x400);
	fis[0] = swap_ward(0x10ef8027);
	fis[1] = swap_ward(0x40000000);
	fis[2] = swap_ward(0x0);
	fis[3] = swap_ward(0x03);
	fis[4] = swap_ward(0x0);
	unsigned int ci;
	STW(1,ATB_PCSR_P0CI);
	do {
        LDW(ci,ATB_PCSR_P0CI);
	} while (ci != 0);	
}
void device_interface_pmde()
{
	unsigned int *cmd = (unsigned int *)(SATA_SRAM_CMD_ADDR );
	cmd[0] = swap_ward(PRDTL(0x1)|CFL(0x5)|W_Write); // is_read
	cmd[1] = swap_ward(0x0);//PRDBC
	cmd[2] = swap_ward((SATA_SRAM_CMD_ADDR + 0x400));
	cmd[3] = swap_ward(SATA_SRAM_ADDR_HI);
	unsigned int *fis = (unsigned int *)(SATA_SRAM_CMD_ADDR + 0x400);
	fis[0] = swap_ward(0x90ef8027);
	fis[1] = swap_ward(0x40000000);
	fis[2] = swap_ward(0x0);
	fis[3] = swap_ward(0x03);
	fis[4] = swap_ward(0x0);
	unsigned int ci;
	STW(1,ATB_PCSR_P0CI);
	do {
        LDW(ci,ATB_PCSR_P0CI);
	} while (ci != 0);	
}
void device_identify_get()
{
	unsigned int tmp;
	unsigned int *cmd = (unsigned int *)(SATA_SRAM_CMD_ADDR );
	cmd[0] = swap_ward(PRDTL(0x1)|CFL(0x5)|W_Write); // is_read
	cmd[1] = swap_ward(0x0);//PRDBC
	cmd[2] = swap_ward((SATA_SRAM_CMD_ADDR + 0x400));
	cmd[3] = swap_ward(SATA_SRAM_ADDR_HI);
	unsigned int *fis = (unsigned int *)(SATA_SRAM_CMD_ADDR + 0x400);
	fis[0] = swap_ward(0x00ec8027);
	fis[1] = swap_ward(0x40000000);
	fis[2] = swap_ward(0x0);
	fis[3] = swap_ward(0x0);
	fis[4] = swap_ward(0x0);
	unsigned int *prd = (unsigned int *)(SATA_SRAM_CMD_ADDR + 0x480);
	prd[0] =swap_ward((SATA_SRAM_ADDR + 0xc000));
	prd[1] = swap_ward(SATA_SRAM_ADDR_HI);
	prd[2] = swap_ward(0);
	prd[3] = swap_ward(0x80000000 | (512 - 1));
	unsigned int ci;
	STW(1,ATB_PCSR_P0CI);
	do {
        LDW(ci,ATB_PCSR_P0CI);
	} while (ci != 0);
	LDW(tmp,SATA_SRAM_ADDR + 0xc000 + 76*2);
	tmp = le32_to_cpu( tmp );
	//uart_printf("value is 0x%x\n",tmp);
	if(tmp & (1 << 9)){
		g_host_pm_support = 1;
		uart_printf("Supports receipt of host-initiated interface power management requests\n");
	}else{
		uart_printf("Don't Supports receipt of host-initiated interface power management requests\n");
	}
	LDW(tmp,SATA_SRAM_ADDR + 0xc000 + 78*2);
	tmp = le32_to_cpu( tmp );
	//uart_printf("value is 0x%x\n",tmp);
	if(tmp & (1 << 3)){
		g_dev_pm_support = 1;
		uart_printf("Device supports initiating interface power management\n");
	}else{
		uart_printf("Device don't supports initiating interface power management\n");
	}
	if(tmp & (1 << 19)){
		uart_printf("Device initiating interface power management enabled\n");
	}else{
		uart_printf("Device initiating interface power management disabled\n");
	}
	LDW(tmp,SATA_SRAM_ADDR + 0xc000 + 82*2);
	tmp = le32_to_cpu( tmp );
	//uart_printf("value is 0x%x\n",tmp);
	if(tmp & (1 << 19)){
		uart_printf(" Advanced Power Management feature set supported \n");
	}else{
		uart_printf("Advanced Power Management feature set not supported\n");
	}
	if(tmp & (1 << 9)){
		uart_printf(" DEVICE RESET command supported \n");
	}else{
		uart_printf("DEVICE RESET command not supported \n");
	}
	if(tmp & (1 << 21)){
		uart_printf("Power-Up In Standby feature set supported \n");
	}else{
		uart_printf("Power-Up In Standby feature set not supported\n");
	}
	LDW(tmp,SATA_SRAM_ADDR + 0xc000 + 48*2);
	tmp = le32_to_cpu( tmp );
	//uart_printf("value is 0x%x\n",tmp);
	if(tmp & (1 << 29)){
		uart_printf("  Standby timer values as specified in this standard are supported \n");
	}else{
		uart_printf("Standby timer values shall be managed by the device \n");
	}
}
void device_check_power_mode(){
	unsigned int tmp;
	unsigned int *cmd = (unsigned int *)(SATA_SRAM_CMD_ADDR );
	cmd[0] = swap_ward(PRDTL(0x1)|CFL(0x5)|W_Write); // is_read
	cmd[1] = swap_ward(0x0);//PRDBC
	cmd[2] = swap_ward((SATA_SRAM_CMD_ADDR + 0x400));
	cmd[3] = swap_ward(SATA_SRAM_ADDR_HI);
	unsigned int *fis = (unsigned int *)(SATA_SRAM_CMD_ADDR + 0x400);
	fis[0] = swap_ward(0x00e58027);
	fis[1] = swap_ward(0x40000000);
	fis[2] = swap_ward(0x0);
	fis[3] = swap_ward(0x00);
	fis[4] = swap_ward(0x0);
	unsigned int ci;
	STW(1,ATB_PCSR_P0CI);
	do {
        LDW(ci,ATB_PCSR_P0CI);
	} while (ci != 0);	
	LDW(tmp,SATA_SRAM_CMD_ADDR + 0x800 + 0x40 + 3*0x4);
	tmp = le32_to_cpu( tmp );
	uart_printf("value is 0x%x\n",tmp);
	tmp &= 0xff;
	switch(tmp){
	case 0x00 : uart_printf("device is in Standby mode\n");break;
	case 0x80 : uart_printf("device is in Idle mode\n");break;
	case 0xff : uart_printf("device is in Active mode or Idle mode\n");break;
	default   : uart_printf("error \n");break;
	}
}
void check_pm_state(U32	state)
{
	unsigned int tmp;
	do{
		 LDW(tmp,ATB_PCSR_P0SSTS);
		 tmp &= IPM_P0SSTS;
	}while((tmp >> 8) != state);
    	PM_state_print(0);
}
void sata_phy_init()
{

	UINT32 phy_reset_out;
	STW(0x80000000,SATA_PHY_PLL_SET);
	STW(0x00000000,SATA_PHY_PLL_SET);
	STW(1,SATA_PHY_RX0_EN_RTERM);
	delay(10);
	STW(1,SATA_PHY_REF_SSP_EN);
	do{
		LDW(phy_reset_out,SATA_PHY_RESET_OUT);
	}while (0xffffffff & phy_reset_out);
	//STW(0xff,0xe954d000+0x400+23*4);
	STW(0x00000000|3<<29,0xe954d000+0x400+40*4);
	STW(0x00000000|3<<29,0xe954E000+0x400+40*4);
	STW(0x00000000|3<<29,0xe954f000+0x400+40*4);
	//STW(0|3<<29,0xe954F000+0x400+40*4);
	//STW(0x00000000|1<<29,0xe95040A0);
	//STW(0x00000000|3<<29|1<<26,0xe95050A0);
	//STW(0xff,0xe954E000+0x400+23*4);
	//STW(0xff,0xe954F000+0x400+23*4);
}
inline const char *SATA_RegsMemMap_string( U32 Offset )
{
	switch (Offset) {
		case ATB_GCSR_CAP:		return "ATB_GCSR_CAP";
		case ATB_GCSR_GHC:		return "ATB_GCSR_GHC";
		case ATB_GCSR_IS:		return "ATB_GCSR_IS";
		case ATB_GCSR_PI:		return "ATB_GCSR_PI";
		case VS:		        return "VS";
		case CCC_CTL:		    return "CCC_CTL";
		case CCC_PORTS:		    return "CCC_PORTS";
		case CAP2:		        return "CAP2";
		
		case ATB_PCSR_P0CLB:		return "ATB_PCSR_P0CLB";
		case ATB_PCSR_P0CLBU:		return "ATB_PCSR_P0CLBU";
		case ATB_PCSR_P0FB:		    return "ATB_PCSR_P0FB";
		case ATB_PCSR_P0FBU:	    return "ATB_PCSR_P0FBU";
		case ATB_PCSR_P0IS:		    return "ATB_PCSR_P0IS";
		case ATB_PCSR_P0IE:		    return "ATB_PCSR_P0IE";
		case ATB_PCSR_P0CMD:		return "ATB_PCSR_P0CMD";
		
		case ATB_PCSR_P0TFD:		return "ATB_PCSR_P0TFD";
		case ATB_PCSR_P0SIG:		return "ATB_PCSR_P0SIG";
		case ATB_PCSR_P0SSTS:		return "ATB_PCSR_P0SSTS";
		case ATB_PCSR_P0SCTL:		return "ATB_PCSR_P0SCTL";
		case ATB_PCSR_P0SERR:		return "ATB_PCSR_P0SERR";
		case ATB_PCSR_P0SACT:		return "ATB_PCSR_P0SACT";
		case ATB_PCSR_P0CI:		    return "ATB_PCSR_P0CI";
		case ATB_PCSR_P0SNTF:		return "ATB_PCSR_P0SNTF";
		case ATB_PCSR_P0FBS:		return "ATB_PCSR_P0FBS";
		case ATB_PCSR_P0DEVSLP:		return "ATB_PCSR_P0DEVSLP";
		
		case ATB_PCSR_P0DMACR:		return "ATB_PCSR_P0DMACR";
		case ATB_PCSR_P0PHYCR:		return "ATB_PCSR_P0PHYCR";
		case ATB_PCSR_P0PHYSR:		return "ATB_PCSR_P0PHYSR";
	}
	return "SDMMC_UNKNOWN";
}
void PM_state_print(unsigned int port_num)
{
	unsigned int tmp;
    LDW(tmp,ATB_PCSR_P0SSTS + 0x80*port_num);
    tmp &= IPM_P0SSTS;
    switch(tmp >> 8){
    case 0x00 : uart_printf("Device not present or communication not established");break;
    case 0x01 : uart_printf("Interface in active state");break;
    case 0x02 : uart_printf("Interface in Partial power management state");break; 
    case 0x06 : uart_printf("Interface in Slumber power management state");break;
    default : uart_printf("Reserved value");break;}
}
void sata_init()
{
    unsigned int i,j=0;
	unsigned int sata_reg;
	unsigned int tmp;
	unsigned int port_num;
	
	for( i = 0; i <= 0x24; i += 4 )
	{
		LDW(sata_reg,AHCI_DCR_BASE +i);
		if( sata_reg != 0x00000000 )
		{
			uart_printf(" Reg not 0x0,0x%x=0x%x    [%s]\r\n",i,
					sata_reg,
					SATA_RegsMemMap_string(AHCI_DCR_BASE +i) );
		}
	}
	port_num = 0;
	for( i = 0; i <= 0x7c; i += 4 )
	{
		LDW(sata_reg,ATB_PCSR_P0CLB + 0x80*port_num + i);
		if( sata_reg != 0x00000000 )
		{
			uart_printf(" Reg not 0x0,0x%x=0x%x    [%s]\r\n",ATB_PCSR_P0CLB + 0x80*port_num + i,
					sata_reg,
					SATA_RegsMemMap_string(ATB_PCSR_P0CLB + i) );
		}
	}

	unsigned int r;
	LDW(r,SW_RST_CFG);
	r |= 1 << SATA_SW_RST;
	STW(r,SW_RST_CFG);
	do{
		LDW(r,SW_RST_CFG);
	}while(((r >> SATA_SW_RST) & 1) == 0);
	r &= ~(1 << SATA_SW_RST);
	STW(r,SW_RST_CFG);
	do{
		LDW(r,SW_RST_CFG);
	}while(((r >> SATA_SW_RST) & 1) == 1);
	delay(0x1000);

	unsigned int sram_addr_sata = SATA_SRAM_ADDR;
	for(i = sram_addr_sata;i <= sram_addr_sata + 0x10000;i = i + 4)
	{
		*(volatile unsigned int*)(i) = i;
	}
	for(i = sram_addr_sata;i <= sram_addr_sata + 0x10000;i = i + 4)
	{
		if(*(volatile unsigned int*)(i) != i)
			while(1);
	}
	
    //STW(0x186a0*4, ATB_GCSR_CAP);
  //  STW(0x186a0, ATB_GCSR_CAP);
    STW(0x186a0,ATB_GCSR_CAP);
    LDW(sata_reg,ATB_GCSR_CAP);
    uart_printf("0x%x\n",sata_reg);
	    
    uart_printf("Global HBA Reset");
      STW(1,ATB_GCSR_GHC);
      LDW(sata_reg,ATB_GCSR_GHC);
      while (sata_reg & 0x1) 
      {
    	  LDW(sata_reg,ATB_GCSR_GHC);
      } 
      // wait for DWC_ahsata clears this bit when the reset action is done
#if SATA_INT_TEST
	InstallInterruptHandler0( SATA_HOST_INT_NUM ,SATA_interrupt_isr);
	InstallInterruptHandler0( SATA_HOST_MSI_INT_NUM ,SATA_MSI_interrupt_isr);
//	InstallInterruptHandler0( SATA_HOST_PME_INT_NUM ,SATA_PME_interrupt_isr);
	tmp = 0;
	tmp |= DHRE_P0IE;//bit0 Device-to-Host register FIS Interrupt
	tmp |= PSE_P0IE;//bit1 PIO Setup FIS Interrupt
	tmp |= DPE_P0IE;//bit5 Descriptor Processed Interrupt
	tmp |= PCE_P0IE;//bit6 Port Connect Change Interrupt
	tmp |= PRCE_P0IE;//bit22 PHY Ready Change Interrupt
	tmp |= OFE_P0IE;//bit24 Overflow Interrupt
	tmp |= INFE_P0IE;//bit26 Interface Non-fatal Error Interrupt
	tmp |= IFE_P0IE;//bit27 Interface Fatal Error Interrupt
	STW(tmp,ATB_PCSR_P0IE);
	//Enable global interrupt
	UINT32 ghc_data;
    LDW(ghc_data,ATB_GCSR_GHC);
    ghc_data=ghc_data|0x2;
    STW(ghc_data,ATB_GCSR_GHC);
#endif
      delay(0x2000);
      sata_phy_init();
      hba_init(0);
      unsigned int res = port_init(0,SATA_SRAM_CMD_ADDR,SATA_SRAM_ADDR_HI,SATA_SRAM_CMD_ADDR + 0x800,SATA_SRAM_ADDR_HI,2);
      COMMENT("ports_init : Initializing Ports ... Done");	
}
void sata_test()
{
	unsigned int i=0;
	unsigned int sram_addr_sata = SATA_SRAM_ADDR;
#if SATA_HOT_PLUG_TEST
      uart_printf("please remove device...... \n");
      do{
    	  delay(1);
      }while(g_state_unplug == 0);
      do{
    	  delay(1);
      }while(g_state_unplug == 1);
      uart_printf("sata hot plug test success...... \n");
#endif
#if SATA_PME_TEST
    LDW(tmp,ATB_GCSR_CAP);
    tmp &= (SSC | PSC);
    //uart_printf("cap register value is 0x%x \n",tmp);
    if(tmp == 0)
    	while(1);
    if(tmp == (SSC | PSC)){
    	uart_printf("both partial and slumber state support");
    }else if(tmp == SSC){
    	uart_printf("only slumber support");
    }else{
    	uart_printf("only partial support");
    }
    device_identify_get();
    //device power management
    device_check_power_mode();
    device_pme(STANDBY,0);
    device_check_power_mode();
    real_sata_write_32(0,0,0x10);
    device_check_power_mode();    
    //Device Initiated
    if(g_dev_pm_support)
    {
    	device_interface_pme();//enable device power state transitions
    	real_sata_write_32(0,0,0x10);
    	check_pm_state(PM_SLUMBER);
    	device_interface_pmde();
    	real_sata_write_32(0,0,0x10);
    	check_pm_state(PM_ACTIVE);
    	uart_printf("device initiate power state transitions success");
    }
    if(g_host_pm_support)
    {
    	//System Software Initiated
    	LDW(tmp,ATB_PCSR_P0CMD);
    	tmp &= 0x0fffffff;
    	tmp |= (PM_PARTIAL << 28);
    	STW(tmp,ATB_PCSR_P0CMD);
    	check_pm_state(PM_PARTIAL);
    	real_sata_write_32(0,0,0x10);
    	check_pm_state(PM_ACTIVE);
    	uart_printf("host system initiate power state transitions success");
    	//HBA Initiated
    	LDW(tmp,ATB_GCSR_CAP);//Supports Aggressive Link Power Management
    	if((tmp & SALP) != 0){
    		LDW(tmp,ATB_PCSR_P0CMD);
    		tmp &= 0x0fffffff;
    		tmp |= ((PM_SLUMBER << 28) | ASP_P0CMD | ALPE_P0CMD);//Aggressive Link Power Management Enable
    		STW(tmp,ATB_PCSR_P0CMD);
    		check_pm_state(PM_SLUMBER);
    		//pm_agg_sata_write_32(0,0,0x10);
    		real_sata_write_32(0,0,0x10);
    		check_pm_state(PM_SLUMBER);
    		LDW(tmp,ATB_PCSR_P0CMD);
    		tmp &= 0x0fffffff;
    		tmp |= ((PM_ACTIVE << 28));//back to active
    		tmp &= ~ALPE_P0CMD;//Aggressive Link Power Management Disable
    		STW(tmp,ATB_PCSR_P0CMD);
    		check_pm_state(PM_ACTIVE);
    		uart_printf("host HBA initiate aggressive power state transitions success");
    	}
    }
#endif
          real_sata_write_32(0,0,0x10);
          real_sata_read_32(0,0,0x10);
      	for(i = sram_addr_sata;i < sram_addr_sata + 0x10*0x200;i = i + 4)
      	{
      		if(*(volatile unsigned int*)(i+0x8000) != *(volatile unsigned int*)(i+0xc000))
      			while(1);
      	}
}
