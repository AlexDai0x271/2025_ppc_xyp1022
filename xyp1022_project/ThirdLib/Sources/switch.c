/*
 * switch.c
 *
 *  Created on: 2021-8-31
 *      Author: ccore
 */

#include "uart.h"
#include "switch.h"
#include "common.h"
#include "e500mc_asm.h"

void switch_wr_32(unsigned int Addr, unsigned int Wdata)
{
	unsigned int val;
	W32(SWITCH_REG_ADDR_H, 0);					//choose switch reg
	W32(SWITCH_REG_ADDR, Addr);					//choose switch reg	
	
	W32(SWITCH_WR_DATA_ADDR_H, 0);		
	W32(SWITCH_WR_DATA_ADDR, (Wdata&0xffffffff));		

	W32(SWITCH_WR_CMD_ADDR, SWITCH_WR_CMD);		//write command
	MSYNC;
	
	val = R32(SWITCH_WR_CMD_ADDR);				
	while(val != SWITCH_DONE_CMD)				//wait for command done
		W32(SWITCH_WR_CMD_ADDR, SWITCH_WR_CMD);
	MSYNC;
	delay(0x1000);
}

void switch_wr_64(unsigned int Addr, unsigned long long Wdata)
{
	unsigned int val;
	W32(SWITCH_REG_ADDR_H, 0);					//choose switch reg
	W32(SWITCH_REG_ADDR, Addr);					//choose switch reg	
	
	W32(SWITCH_WR_DATA_ADDR_H, (Wdata>>32));		
	W32(SWITCH_WR_DATA_ADDR, (Wdata&0xffffffff));		

	W32(SWITCH_WR_CMD_ADDR, SWITCH_WR_CMD);		//write command
	MSYNC;
	
	val = R32(SWITCH_WR_CMD_ADDR);				
	while(val != SWITCH_DONE_CMD)				//wait for command done
		W32(SWITCH_WR_CMD_ADDR, SWITCH_WR_CMD);
	MSYNC;
	delay(0x1000);
}

unsigned int switch_rd_32(unsigned int Addr)
{
	unsigned int val;
	unsigned int data32;
	
	W32(SWITCH_REG_ADDR_H, 0);					//choose switch reg
	W32(SWITCH_REG_ADDR, Addr);					//choose switch reg	
	W32(SWITCH_WR_CMD_ADDR, SWITCH_RD_CMD);		//read command
	MSYNC;
	
	delay(0x1000);
	val = R32(SWITCH_WR_CMD_ADDR);				
	while(val != SWITCH_DONE_CMD)				//wait for command done
		W32(SWITCH_WR_CMD_ADDR, SWITCH_WR_CMD);
	MSYNC;
	
	delay(0x1000);
	data32 = R32(SWITCH_RD_DATA_ADDR);
	return data32;
}

unsigned long long switch_rd_64(unsigned int Addr)
{
	unsigned int val;
	unsigned long long data64;
	
	W32(SWITCH_REG_ADDR_H, 0);					//choose switch reg
	W32(SWITCH_REG_ADDR, Addr);					//choose switch reg	
	W32(SWITCH_WR_CMD_ADDR, SWITCH_RD_CMD);		//read command
	MSYNC;

	val = R32(SWITCH_WR_CMD_ADDR);				
	while(val != SWITCH_DONE_CMD)				//wait for command done
		W32(SWITCH_WR_CMD_ADDR, SWITCH_WR_CMD);
	MSYNC;
	
	delay(0x1000);
	data64 = R32(SWITCH_RD_DATA_ADDR);
	data64 += (R64(SWITCH_RD_DATA_ADDR_H))<<32;
	return data64;
}

void switch_wr_rd_test()
{
	unsigned long long data64;
	unsigned int data32, i;
	unsigned short data16;
	unsigned char data8;
	
	//test1: same with IC
	//WR
	W32(SWITCH_REG_ADDR_H, 0x00000000);
	W32(SWITCH_REG_ADDR, 0x502);
	W32(SWITCH_WR_DATA_ADDR, 0x12345678);
	W32(SWITCH_WR_DATA_ADDR_H, 0x9abcdef0);
	W32(SWITCH_RD_DATA_ADDR, 0x55555555);
	W32(SWITCH_RD_DATA_ADDR_H, 0x66666666);
	W32(SWITCH_WR_CMD_ADDR, SWITCH_WR_CMD);

	MSYNC;
	data32 = R32(SWITCH_WR_CMD_ADDR);				
	while(data32 != SWITCH_DONE_CMD)				//wait for command done
		W32(SWITCH_WR_CMD_ADDR, SWITCH_WR_CMD);

	//RD
	W32(SWITCH_REG_ADDR_H, 0x00000000);
	W32(SWITCH_REG_ADDR, 0x502);
	W32(SWITCH_WR_CMD_ADDR, SWITCH_RD_CMD);
	MSYNC;
	data32 = R32(SWITCH_WR_CMD_ADDR);				
	while(data32 != SWITCH_DONE_CMD)				//wait for command done
		W32(SWITCH_WR_CMD_ADDR, SWITCH_RD_CMD);

	data32 = R32(SWITCH_RD_DATA_ADDR);
	if(data32 != 0x12345678)
		while(1);
	data32 = R32(SWITCH_RD_DATA_ADDR_H);
	if(data32 != 0x0000def0)
			while(1);
	MSYNC;
	
	//test2: write and read by 8bit
	data8 = 0x7a;
	switch_wr_32(0x0502, data8);
	data8 = 0;
	data8 = switch_rd_32(0x0502);
	if(data8 != 0x7a)
		while(1);
	MSYNC;
	
	//test3: write and read by 16bit
	data16 = 0x8b9c;
	switch_wr_32(0x0502, data16);
	data16 = 0;
	data16 = switch_rd_32(0x0502);
	if(data16 != 0x8b9c)
		while(1);
	MSYNC;
	
	//test4: write and read by 32bit
	data32 = 0x12345678;
	switch_wr_32(0x0502, data32);
	data32 = 0;
	data32 = switch_rd_32(0x0502);
	if(data32 != 0x12345678)
		while(1);
	MSYNC;
	
	//test5: write and read by 64bit	
	data64 = 0xabcdef9876543210;
	switch_wr_64(0x0502, data64);
	data64 = 0;
	data64 = switch_rd_64(0x0502);
	if(data64 != 0x0000ef9876543210)
		while(1);

	//test6:edge test
	data32 = switch_rd_32(SWITCH_PCR1);
	if(data32 != 0xA0)
		while(1);
	data32 |= 0x101;
	switch_wr_32(SWITCH_PCR1, data32);
	data32 = switch_rd_32(SWITCH_PCR1);
	if(data32 != 0xA1)
		while(1);
	data32 = switch_rd_32(SWITCH_PCR2);
	if(data32 != 0xA0)
		while(1);
	MSYNC;
}
	
void switch_reg_test()	
{
	unsigned long long data64;
	unsigned int data32, i;
	unsigned short data16;
	unsigned char data8;

	for(i=0; i<8; i++){
		data8 = switch_rd_32(SWITCH_PCR(i));
		uart_printf_buf("##### Switch PCR%x State is %x. #####\n", i, data8);
		if(data8 != 0xa0)
			while(1);
	}
		
	for(i=0; i<8; i++){
		switch_wr_32(SWITCH_PCR(i), PCR_STP_State_Congestion | PCR_Tx_Disable | PCR_Rx_Disable);
		data8 = switch_rd_32(SWITCH_PCR(i));
		uart_printf_buf("##### Switch PCR%x State is %x. should be 0x43 #####\n", i, data8);
		if(data8 != 0x43)
			while(1);
	}
	
	data8 = switch_rd_32(SWITCH_SMR);
	uart_printf_buf("##### Switch SMR State1 is %x. #####\n", data8);
	if(data8 != 0x7)
		while(1);
	switch_wr_32(SWITCH_SMR, SMR_SW_Management_Mode_Disable | SMR_SW_Transmit_Disable);
	data8 = switch_rd_32(SWITCH_SMR);
	if(data8 != 0x4)
		while(1);
	
	data8 = switch_rd_32(SWITCH_FACR);
	uart_printf_buf("##### Switch FACR State1 is %x. #####\n", data8);
	if(data8 != 0x0)
		while(1);
	switch_wr_32(SWITCH_FACR, FACR_Fast_Aging_Enable);
	data8 = switch_rd_32(SWITCH_FAPCR);
	if(data8 != 0x80)
		while(1);

	data8 = switch_rd_32(SWITCH_FAPCR);
	uart_printf_buf("##### Switch FAPCR State1 is %x. #####\n", data8);
	if(data8 != 0x80)
		while(1);
	switch_wr_32(SWITCH_FAPCR, FAPCR_All_Port_Fast_Aging_Disable);
	data8 = switch_rd_32(SWITCH_FAPCR);
	if(data8 != 0x00)
		while(1);
	
	switch_wr_32(SWITCH_FAPCR, FAPCR_Fast_Aging_Enable_Port(5));
	data8 = switch_rd_32(SWITCH_FAPCR);
	if(data8 != 0x5)
		while(1);

	data16 = switch_rd_32(SWITCH_FAVCR);
	uart_printf_buf("##### Switch FAVCR State1 is %x. #####\n", data16);
	if(data16 != 0x8000)
		while(1);
	switch_wr_32(SWITCH_FAVCR, FAVCR_All_VID_Fast_Aging_Disable);
	data16 = switch_rd_32(SWITCH_FAVCR);
	if(data16 != 0x0)
		while(1);
	switch_wr_32(SWITCH_FAVCR, FAVCR_Fast_Aging_Enable_VID(7));
	data16 = switch_rd_32(SWITCH_FAVCR);
	if(data16 != 0x7)
		while(1);

	data16 = switch_rd_32(SWITCH_LSS);
	uart_printf_buf("##### Switch LSS State1 is %x. #####\n", data16);
	if(data16 != 0x1ff)
		while(1);

	data32 = switch_rd_32(SWITCH_PSS);
	uart_printf_buf("##### Switch PSS State1 is %x. #####\n", data32);
	if(data32 != 0x15555)
		while(1);

	data16 = switch_rd_32(SWITCH_DSS);
	uart_printf_buf("##### Switch DSS State1 is %x. #####\n", data16);
	if(data16 != 0x1ff)
		while(1);

	data32 = switch_rd_32(SWITCH_FCSS);
	uart_printf_buf("##### Switch FCSS State1 is %x. #####\n", data32);
	/* default: Port0 receive is enabled */
	if(data32 != 0x200)
		while(1);

	data32 = switch_rd_32(SWITCH_ATCR);
	uart_printf_buf("##### Switch ATCR State1 is %x. #####\n", data32);
	switch_wr_32(SWITCH_ATCR, 0x5D);
	data32 = switch_rd_32(SWITCH_ATCR);
	if(data32 != 0x5D)
		while(1);

	data8 = switch_rd_32(SWITCH_ATRWCR);
	uart_printf_buf("##### Switch ATRWCR State1 is %x. #####\n", data8);
	if(data8 != 0x0)
		while(1);
	switch_wr_32(SWITCH_ATRWCR, ATRWCR_Operate_Trigger | ARL_Read);
	data8 = switch_rd_32(SWITCH_ATRWCR);
	if(data8 != 0x01)
		while(1);
	
	data64 = switch_rd_64(SWITCH_MACAIR);
	uart_printf_buf("##### Switch MACAIR State1 is %x. #####\n", data64);
	if(data64 != 0x0)
		while(1);
	switch_wr_64(SWITCH_MACAIR, 0xabcdef9876543210);
	data64 = switch_rd_64(SWITCH_MACAIR);
	if(data64 != 0x0000ef9876543210)
		while(1);

	data16 = switch_rd_32(SWITCH_VLANIDIR);
	uart_printf_buf("##### Switch VLANIDIR State1 is %x. #####\n", data16);
	if(data16 != 0x0)
		while(1);
	switch_wr_32(SWITCH_VLANIDIR, 0xfff);
	data16 = switch_rd_32(SWITCH_VLANIDIR);
	if(data16 != 0xfff)
		while(1);

	data64 = switch_rd_64(SWITCH_ATMVE0R);
	uart_printf_buf("##### Switch ATMVE0R State1 is %x. #####\n", data64);
	if(data64 != 0x0)
		while(1);
	switch_wr_64(SWITCH_ATMVE0R, 0xabcdef9876543210);
	data64 = switch_rd_64(SWITCH_ATMVE0R);
	if(data64 != 0x0bcdef9876543210)
		while(1);

	data16 = switch_rd_32(SWITCH_ATDE0R);
	uart_printf_buf("##### Switch ATDE0R State1 is %x. #####\n", data16);
	if(data16 != 0x0)
		while(1);
	switch_wr_32(SWITCH_ATDE0R, ATDE0R_Valid_Identification | ATDE0R_Statics_Identification | ATDE0R_Age_Identification | ATDE0R_Priority(3));
	data16 = switch_rd_32(SWITCH_ATDE0R);
	if(data16 != 0xec00)
		while(1);

	data64 = switch_rd_64(SWITCH_ATMVE1R);
	uart_printf_buf("##### Switch ATMVE1R State1 is %x. #####\n", data64);
	if(data64 != 0x0)
		while(1);
	switch_wr_64(SWITCH_ATMVE1R, 0xabcdef9876543210);
	data64 = switch_rd_64(SWITCH_ATMVE1R);
	if(data64 != 0x0bcdef9876543210)
		while(1);

	data16 = switch_rd_32(SWITCH_ATDE1R);
	uart_printf_buf("##### Switch ATDE1R State1 is %x. #####\n", data16);
	if(data16 != 0x0)
		while(1);
	switch_wr_32(SWITCH_ATDE1R, ATDE0R_Valid_Identification | ATDE0R_Statics_Identification | ATDE0R_Age_Identification | ATDE0R_Priority(3));
	data16 = switch_rd_32(SWITCH_ATDE1R);
	if(data16 != 0xec00)
		while(1);

	data8 = switch_rd_32(SWITCH_VLANTRWCR);
	uart_printf_buf("##### Switch VLANTRWCR State1 is %x. #####\n", data8);
	if(data8 != 0x0)
		while(1);
	switch_wr_32(SWITCH_VLANTRWCR, VLANTRWCR_Operate_Trigger | VLAN_Read);
	data8 = switch_rd_32(SWITCH_VLANTRWCR);
	if(data8 != 0x01)
		while(1);
	
	data16 = switch_rd_32(SWITCH_VLANTAIR);
	uart_printf_buf("##### Switch VLANTAIR State1 is %x. #####\n", data16);
	if(data16 != 0x0)
		while(1);
	switch_wr_32(SWITCH_VLANTAIR, 0xfff);
	data16 = switch_rd_32(SWITCH_VLANTAIR);
	if(data16 != 0xfff)
		while(1);

	data32 = switch_rd_32(SWITCH_VLANTER);
	uart_printf_buf("##### Switch VLANTER State1 is %x. #####\n", data32);
	if(data32 != 0x0)
		while(1);
	switch_wr_32(SWITCH_VLANTER, VLANTER_Valid_Identification | VLANTER_ugTag_Port_Mapping(1) | VLANTER_Transmit_Mapping(1));
	data32 = switch_rd_32(SWITCH_VLANTER);
	if(data32 != 0x40201)
		while(1);

	data8 = switch_rd_32(SWITCH_IPMRWCR);
	uart_printf_buf("##### Switch IPMRWCR State1 is %x. #####\n", data8);
	if(data8 != 0x0)
		while(1);
//	switch_wr_32(SWITCH_IPMRWCR, IPMRWCR_Operate_Trigger | IP_Read);
	switch_wr_32(SWITCH_IPMRWCR, IP_Read);
	data8 = switch_rd_32(SWITCH_IPMRWCR);
	if(data8 != 0x01)
		while(1);
	
	data32 = switch_rd_32(SWITCH_IPMRWAR);
	uart_printf_buf("##### Switch IPMRWAR State1 is %x. #####\n", data32);
	if(data32 != 0x0)
		while(1);
	switch_wr_32(SWITCH_IPMRWAR, 0x12345678);
	data32 = switch_rd_32(SWITCH_IPMRWAR);
	if(data32 != 0x12345678)
		while(1);
	
#if 0
	data64 = switch_rd_64(SWITCH_IPME0R1);
	uart_printf_buf("##### Switch IPME0R1 State1 is %x. #####\n", data64);
	if(data64 != 0x0)
		while(1);
	switch_wr_64(SWITCH_IPME0R1, 0x1234567890abcdef);
	data64 = switch_rd_64(SWITCH_IPME0R1);
	if(data64 != 0x0000567890abcdef)
		while(1);

	data64 = switch_rd_64(SWITCH_IPME0R2);
	uart_printf_buf("##### Switch IPME0R2 State1 is %x. #####\n", data64);
	if(data64 != 0x0)
		while(1);
	switch_wr_64(SWITCH_IPME0R2, 0xabcdef1234567890);
	data64 = switch_rd_64(SWITCH_IPME0R2);
	if(data64 != 0x0000ef1234567890)
		while(1);
	
	data64 = switch_rd_64(SWITCH_IPME0R3);
	uart_printf_buf("##### Switch IPME0R3 State1 is %x. #####\n", data64);
	if(data64 != 0x0)
		while(1);
	switch_wr_64(SWITCH_IPME0R3, Entry0_Valid | (0x12345678)<<Entry0_IP_Shift | Entry0_VLAN_Prior(2) | (0xfff)<Entry0_VLAN_ID_Shift | Entry0_Port_Transmit(2));
	data64 = switch_rd_64(SWITCH_IPME0R3);
	if(data64 != 0x0000ef1234567890)
		while(1);

	data64 = switch_rd_64(SWITCH_IPME1R1);
	uart_printf_buf("##### Switch IPME1R1 State1 is %x. #####\n", data32);
	if(data64 != 0x0)
		while(1);
	switch_wr_64(SWITCH_IPME1R1, 0x1234567890abcdef);
	data64 = switch_rd_64(SWITCH_IPME1R1);
	if(data64 != 0x0000567890abcdef)
		while(1);

	data64 = switch_rd_64(SWITCH_IPME1R2);
	uart_printf_buf("##### Switch IPME1R2 State1 is %x. #####\n", data32);
	if(data64 != 0x0)
		while(1);
	switch_wr_64(SWITCH_IPME1R2, 0xabcdef1234567890);
	data64 = switch_rd_64(SWITCH_IPME1R2);
	if(data64 != 0x0000ef1234567890)
		while(1);

	data64 = switch_rd_64(SWITCH_IPME1R3);
	uart_printf_buf("##### Switch IPME1R3 State1 is %x. #####\n", data32);
	if(data64 != 0x0)
		while(1);
	switch_wr_64(SWITCH_IPME1R3, Entry1_Valid | (0x12345678)<<Entry1_IP_Shift | Entry1_VLAN_Prior(2) | (0xfff)<Entry1_VLAN_ID_Shift | Entry1_Port_Transmit(2));
	data64 = switch_rd_64(SWITCH_IPME1R3);
	if(data64 != 0x0000ef1234567890)
		while(1);

	data8 = switch_rd_32(SWITCH_QGCR);
	uart_printf_buf("##### Switch QGCR State1 is %x. #####\n", data8);
	if(data8 != 0x0)
		while(1);
	switch_wr_32(SWITCH_QGCR, Port_QoS_Enable | QoS_Prior(1));
	data8 = switch_rd_32(SWITCH_QGCR);
	if(data8 != 0x48)
		while(1);
#endif
	
}


/*
 * SMR: frame switch mode test
 * test1: disable frame switch 
 * test2: enable frame switch 
 */
void swicth_mode_test()
{
	unsigned char val;
	
	/*test1: by default, port0 receive function is enabled. Frame can arrive the opposite end */
	val = switch_rd_32(SWITCH_SMR);
	if(val != 0x7){
		uart_printf_buf("switch SMR default err! \n");
		while(1);
	}
	
	val &= (~SMR_SW_Transmit_Enable);
	switch_wr_32(SWITCH_SMR, val);
	
	val = switch_rd_32(SWITCH_SMR);
	if(val != 0x5){
		uart_printf_buf("switch SMR disable frame switch err! \n");
		while(1);
	}
	/* Now PC1 transmit frame by Port0, PC2 should not receive this frame by Port2*/

	/*test2: recover test*/
	val |= SMR_SW_Transmit_Enable;
	switch_wr_32(SWITCH_SMR, val);
	
	val = switch_rd_32(SWITCH_SMR);
	if(val != 0x7){
		uart_printf_buf("switch SMR recover frame switch err! \n");
		while(1);
	}	
	/* Now PC1 transmit a frame to Port0, Port2 should receive this frame by PC2*/
}

/*
 * PCRx transmit and receive disable test 
 * test1: PCR0 disable receive
 * test2; recover test
 * test3: PCR2 disable transmit
 * test4; recover test
 * test5: STP_disable
 */
void swicth_portx_txrx_disable_test()
{
	unsigned int val;
	
	//test1: by default, port0 receive function is enabled. Disable port0 receive function and send frame by port0. Frame should not arrive the opposite end.
	val = switch_rd_32(SWITCH_PCR0);
	if(val != 0xA0){
		uart_printf_buf("switch port0 default err! \n");
		while(1);
	}
	
	val |= PCR_Rx_Disable;
	switch_wr_32(SWITCH_PCR0, val);
	
	val = switch_rd_32(SWITCH_PCR0);
	if(val != 0xA1){
		uart_printf_buf("switch port0 disable Rx err! \n");
		while(1);
	}
	/* Now PC1 transmit frame by Port0, PC2 should not receive this frame by Port2*/

	//test2: recover test
	val &= (~PCR_Rx_Disable);
	switch_wr_32(SWITCH_PCR0, val);
	
	val = switch_rd_32(SWITCH_PCR0);
	if(val != 0xA0){
		uart_printf_buf("switch port0 recover Rx err! \n");
		while(1);
	}	
	/* Now PC1 transmit a frame to Port0, Port2 should receive this frame by PC2*/

	//test3: by default, port2 transmit function is enabled. Disable port2 transmit function and get frame by port2. Frame should not be arrived.
	val = switch_rd_32(SWITCH_PCR2);
	if(val != 0xA0){
		uart_printf_buf("switch port2 default err! \n");
		while(1);
	}
	
	val |= PCR_Tx_Disable;
	switch_wr_32(SWITCH_PCR2, val);
	
	val = switch_rd_32(SWITCH_PCR2);
	if(val != 0xA2){
		uart_printf_buf("switch port2 disable Tx err! \n");
		while(1);
	}
	/* Now PC1 transmit a frame to Port0, Port2 should can't receive this frame by PC2*/

	//test4: recover test
	val &= (~PCR_Tx_Disable);
	switch_wr_32(SWITCH_PCR2, val);
	
	val = switch_rd_32(SWITCH_PCR2);
	if(val != 0xA0){
		uart_printf_buf("switch port2 recover Tx err! \n");
		while(1);
	}	
	/* Now PC1 transmit a frame to Port0, Port2 should receive this frame by PC2*/
}

void switch_state_monitor()	
{
	unsigned long long data64;
	unsigned int data32, i;
	unsigned short data16;

	data16 = switch_rd_32(SWITCH_LSS);
	uart_printf_buf("##### Switch LSS State1 is %x. #####\n", data16);
	if(data16 != 0x1ff)
		while(1);
	
	data16 = switch_rd_32(SWITCH_DSS);
	uart_printf_buf("##### Switch DSS State1 is %x. #####\n", data16);
	if(data16 != 0x1ff)
		while(1);

	data32 = switch_rd_32(SWITCH_PSS);
	uart_printf_buf("##### Switch PSS State1 is %x. #####\n", data32);
	if(data32 != 0x15555)
		while(1);

	data32 = switch_rd_32(SWITCH_FCSS);
	uart_printf_buf("##### Switch FCSS State1 is %x. #####\n", data32);
	/* default: Port0 receive is enabled */
	if(data32 != 0x200)
		while(1);
}

/*
 * switch vlan ablilty test
 */
void switch_vlan_test()
{
	unsigned long long data64;
	unsigned int data32, i;
	unsigned char data16;
	
	for(i=0; i<8; i++){
		data32 = switch_rd_32(SWITCH_PBVLANCR(i));
		if(data32 != 0x1ff){
			uart_printf_buf("##### Switch PBVLANCR State1 is %x. #####\n", data32);
			while(1);
		}
	}
	
	data32 = 0x788;																//Vlan ID is 0x788
	switch_wr_32(SWITCH_VLANIDIR, data32);										//Prepare VLAN ID
	switch_wr_32(SWITCH_VLANTRWCR, VLAN_Write | VLANTRWCR_Operate_Trigger);		//Tirgger Write		
	switch_wr_32(SWITCH_VLANTAIR, data32);										//Reference VLAN ID 0x788
	data32 = switch_rd_32(SWITCH_VLANTER);
	if(data32 != 0x1FB){
			uart_printf_buf("##### Switch VLANTER State is %x. #####\n", data32);
			while(1);
		}
	
	/*disable Port0 transmit to Port2*/
	data32 = 0x1FB;
	switch_wr_32(SWITCH_PBVLANCR(0), data32);
	data32 = switch_rd_32(SWITCH_PBVLANCR(0));
	if(data32 != 0x1FB){
		uart_printf_buf("##### Switch PBVLANCR State1 is %x. #####\n", data32);
		while(1);
	}
	
	/*enable Port0 transmit to Port2*/
	data32 = 0x1FF;
	switch_wr_32(SWITCH_PBVLANCR(0), data32);
	data32 = switch_rd_32(SWITCH_PBVLANCR(0));
	if(data32 != 0x1FF){
		uart_printf_buf("##### Switch PBVLANCR State1 is %x. #####\n", data32);
		while(1);
	}
	
}


void switch_three_layer_test()	
{
	unsigned long long data64, DA, SA;
	unsigned int data32, i;
	unsigned short data16;
	unsigned char data8;
	
	/* Switch Config */
	/* Set SWITCH MAC Addr, ARL Table index this MAC Addr*/
	DA = 0x002222222222;
	switch_wr_64(SWITCH_MACAIR, DA);
	if(switch_rd_64(SWITCH_MACAIR) != DA)
		while(1);
	
	/* Set SWITCH MAC Addr at ARL Table MAC/VID Entry 0 */
	switch_wr_64(SWITCH_ATMVE0R, DA);
	if(switch_rd_64(SWITCH_ATMVE0R) != DA)
		while(1);

	/* Set ARL Table Data Entry0 Register. Valid MAC of DA at Entry0 */
	data16 = 0xE200;
	switch_wr_32(SWITCH_ATDE0R, data16);
	if(switch_rd_32(SWITCH_ATDE0R) != data16)
		while(1);

	/* Set ARL Table Read/Write Control Register. Trigger Read/Write by write bit8 */
	data8 = 0x80;
	switch_wr_32(SWITCH_ATRWCR, data8);
	if(switch_rd_32(SWITCH_ATRWCR) != 0)
		while(1);
	
	/* Set IP Read/Write Addr Register, PC-B IP is 192.168.0.13*/
	data32 = 0xC0A8000D;
	switch_wr_32(SWITCH_IPMRWAR, data32);
	if(switch_rd_32(SWITCH_IPMRWAR) != data32)
		while(1);
	
	/* Set SRC MAC Address, here is the SWITCH Address */
	SA = 0x002222222222;
	switch_wr_64(SWITCH_IPME0R1, SA);
	data64 = switch_rd_64(SWITCH_IPME0R1);
	if(switch_rd_64(SWITCH_IPME0R1) != SA)
		while(1);

	/* Set DST MAC Address, here is the PC-B Address */
	DA = 0x003333333333;
	switch_wr_64(SWITCH_IPME0R2, DA);
	data64 = switch_rd_64(SWITCH_IPME0R2);
	if(switch_rd_64(SWITCH_IPME0R2) != DA)
		while(1);

	/* Set IP Entry0 Register 3, Entry0 transmit by Port2, Valid Entry0, IP is 192.168.0.13*/
	data64 = 0xE054000680002;
	switch_wr_64(SWITCH_IPME0R3, data64);
	if(switch_rd_64(SWITCH_IPME0R3) != data64)
		while(1);

	/* IP Write/Read Trigger*/
	data8 = 0x80;
	switch_wr_32(SWITCH_IPMRWCR, data8);
	if(switch_rd_32(SWITCH_IPMRWCR) != 0)
		while(1);
}
