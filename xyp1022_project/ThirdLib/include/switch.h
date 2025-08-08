/*
 * switch.h
 *
 *  Created on: 2021-8-31
 *      Author: ccore
 */

#ifndef SWITCH_H_
#define SWITCH_H_

#define SWITCH_BASE_ADDR    				0xE9510000ull				 	//Switch write/read address
#define SWITCH_REG_ADDR_H    				(SWITCH_BASE_ADDR + 0x0)	 	//Switch Reg High_32
#define SWITCH_REG_ADDR    					(SWITCH_BASE_ADDR + 0x4)	 	//Switch Reg Low_32
#define SWITCH_WR_DATA_ADDR_H    			(SWITCH_BASE_ADDR + 0x8)		//Switch WR Address High_32 	 
#define SWITCH_WR_DATA_ADDR    				(SWITCH_BASE_ADDR + 0xC)	 	//Switch WR Address	Low_32
#define SWITCH_WR_CMD_ADDR  				(SWITCH_BASE_ADDR + 0x10)	 	//Switch Command Address
#define SWITCH_RD_DATA_ADDR_H	    		(SWITCH_BASE_ADDR + 0x14)	 	//Switch RD Address  High_32 
#define SWITCH_RD_DATA_ADDR    				(SWITCH_BASE_ADDR + 0x18)	 	//Switch RD Address Low_32

#define SWITCH_WR_CMD				 		(0x01)	 	 					//Switch write command
#define SWITCH_RD_CMD				 		(0x02)	 	 					//Switch read command
#define SWITCH_DONE_CMD				 		(0x80000000)	 	 			//Switch done

/*端口0~7控制寄存器 PCR0（00-00; RW）~PCR7*/
#define SWITCH_PCR0							(SWITCH_BASE_ADDR + 0x0)
#define SWITCH_PCR1							(SWITCH_BASE_ADDR + 0x1)
#define SWITCH_PCR2							(SWITCH_BASE_ADDR + 0x2)
#define SWITCH_PCR3 						(SWITCH_BASE_ADDR + 0x3)
#define SWITCH_PCR4							(SWITCH_BASE_ADDR + 0x4)
#define SWITCH_PCR5							(SWITCH_BASE_ADDR + 0x5)
#define SWITCH_PCR6							(SWITCH_BASE_ADDR + 0x6)
#define SWITCH_PCR7							(SWITCH_BASE_ADDR + 0x7)

#define SWITCH_PCR(n)						(SWITCH_BASE_ADDR + n*0x1)
#define PCR_STP_State_DiSTP					(0x00)		
#define PCR_STP_State_Disable				(0x20)		
#define PCR_STP_State_Congestion			(0x40)		
#define PCR_STP_State_Listening				(0x60)		
#define PCR_STP_State_Learning				(0x80)		
#define PCR_STP_State_Transmit				(0xA0)		
#define PCR_Tx_Enable						(0<<1)
#define PCR_Tx_Disable						(1<<1)
#define PCR_Rx_Enable						(0<<0)
#define PCR_Rx_Disable						(1<<0)

/*交换模式寄存器 SMR（00-0B; RW）*/
#define SWITCH_SMR							(SWITCH_BASE_ADDR + 0xB)
#define SMR_SW_Transmit_Enable				(1<<1)
#define SMR_SW_Transmit_Disable				(0<<1)
#define SMR_SW_Management_Mode_Enable		(1<<1)
#define SMR_SW_Management_Mode_Disable		(0<<1)

/*快速老化控制寄存器 FACR（00-88; RW）*/
#define SWITCH_FACR							(SWITCH_BASE_ADDR + 0x88)
#define FACR_Fast_Aging_Enable				(1<<7)
#define FACR_Fast_Aging_Disable				(0<<7)
#define FACR_Static_Aging_Enable			(1<<0)

/*快速老化端口控制寄存器 FAPCR（00-89; RW）*/
#define SWITCH_FAPCR						(SWITCH_BASE_ADDR + 0x89)
#define FAPCR_All_Port_Fast_Aging_Enable	(1<<7)
#define FAPCR_All_Port_Fast_Aging_Disable	(0<<7)
#define FAPCR_Fast_Aging_Enable_Port(n)		(n<<0)

/*快速老化VID控制寄存器 FAVCR（00-8A, 00-8B; RW）*/
#define SWITCH_FAVCR						(SWITCH_BASE_ADDR + 0x8A)
#define FAVCR_All_VID_Fast_Aging_Enable		(1<<15)
#define FAVCR_All_VID_Fast_Aging_Disable	(0<<15)
#define FAVCR_Fast_Aging_Enable_VID(n)		(n<<0)

/*链路状态寄存器 LSS（01-00, 01-01; RO）*/
#define SWITCH_LSS							(SWITCH_BASE_ADDR + 0x100)

/*端口速率状态寄存器 PSS（01-04, 01-05, 01-06, 01-07; RO）*/
#define SWITCH_PSS							(SWITCH_BASE_ADDR + 0x104)

/*双工状态寄存器 DSS（01-08, 01-09; RO）*/
#define SWITCH_DSS							(SWITCH_BASE_ADDR + 0x108)

/*流控状态寄存器 FCSS（01-0A, 01-0B, 01-0C, 01-0D; RO）*/
#define SWITCH_FCSS							(SWITCH_BASE_ADDR + 0x10A)

/*老化时间控制寄存器 ATCR（02-06,02-07,02-08,02-09; RW）*/
#define SWITCH_ATCR							(SWITCH_BASE_ADDR + 0x206)

/*ARL表读写控制寄存器 ATRWCR（05-00; RW）*/
#define SWITCH_ATRWCR						(SWITCH_BASE_ADDR + 0x500)
#define ATRWCR_Operate_Trigger				(1<<7)
#define ARL_Read							(1<<0)
#define ARL_Write							(0<<0)


/*MAC地址索引寄存器 MACAIR（05-02, 05-03, 05-04, 05-05, 05-06, 05-07,; RW）*/
#define SWITCH_MACAIR						(SWITCH_BASE_ADDR + 0x502)

/*VLAN ID索引寄存器 VLANIDIR（05-08, 05-09; RW）*/
#define SWITCH_VLANIDIR						(SWITCH_BASE_ADDR + 0x508)

/*ARL表MAC/VID条目0寄存器 ATMVE0R（05-10, 05-11~05-17; RW）*/
#define SWITCH_ATMVE0R						(SWITCH_BASE_ADDR + 0x510)

/*ARL表数据条目0寄存器 ATDE0R（05-18, 05-19; RW）*/
#define SWITCH_ATDE0R						(SWITCH_BASE_ADDR + 0x518)
#define ATDE0R_Valid_Identification			(1<<15)
#define ATDE0R_Statics_Identification		(1<<14)
#define ATDE0R_Age_Identification			(1<<13)
#define ATDE0R_Priority(n)					(n<<10)
#define ATDE0R_Transmit_Mapping(n)			(n<<0)

/*ARL表MAC/VID条目1寄存器 ATMVE1R（05-20, 05-21~05-27; RW）*/
#define SWITCH_ATMVE1R						(SWITCH_BASE_ADDR + 0x520)

/*ARL表数据条目1寄存器 ATDE1R（05-28, 05-29; RW）*/
#define SWITCH_ATDE1R						(SWITCH_BASE_ADDR + 0x528)
#define ATDE1R_Valid_Identification			(1<<15)
#define ATDE1R_Statics_Identification		(1<<14)
#define ATDE1R_Age_Identification			(1<<13)
#define ATDE1R_Priority(n)					(n<<10)
#define ATDE1R_Transmit_Mapping(n)			(n<<0)

/*VLAN表读写控制寄存器 VLANTRWCR（05-60; RW）*/
#define SWITCH_VLANTRWCR					(SWITCH_BASE_ADDR + 0x560)
#define VLANTRWCR_Operate_Trigger			(1<<7)
#define VLAN_Read							(1<<0)
#define VLAN_Write							(0<<0)

/*VLAN表地址索引寄存器 VLANTAIR（05-61, 05-62; RW）*/
#define SWITCH_VLANTAIR						(SWITCH_BASE_ADDR + 0x561)

/*VLAN表条目寄存器 VLANTER（05-63, 05-64,05-65, 05-66; RW）*/
#define SWITCH_VLANTER						(SWITCH_BASE_ADDR + 0x563)
#define VLANTER_Valid_Identification		(1<<18)
#define VLANTER_ugTag_Port_Mapping(n)		(n<<9)
#define VLANTER_Transmit_Mapping(n)			(n<<0)

/*三层表读写控制寄存器 IPMRWCR（05-70; RW）*/
#define SWITCH_IPMRWCR						(SWITCH_BASE_ADDR + 0x570)
#define IPMRWCR_Operate_Trigger				(1<<7)
#define IP_Read								(1<<0)
#define IP_Write							(0<<0)

/*三层表读写地址寄存器 IPMRWAR（05-72, 05-73, 05-74, 05-75; RW）*/
#define SWITCH_IPMRWAR						(SWITCH_BASE_ADDR + 0x572)

/*三层表条目0读写寄存器1 IPME0R1（05-76, 05-77, ~05-7B,; RW）*/
#define SWITCH_IPME0R1						(SWITCH_BASE_ADDR + 0x576)

/*三层表条目0读写寄存器2 IPME0R2（05-7C, 05-7D, ~05-81,; RW）*/
#define SWITCH_IPME0R2						(SWITCH_BASE_ADDR + 0x57C)

/*三层表条目0读写寄存器3 IPME0R3（05-82, 05-83, ~05-89,; RW）*/
#define SWITCH_IPME0R3						(SWITCH_BASE_ADDR + 0x582)
#define Entry0_Valid						(1<<51)
#define Entry0_IP_Shift						19
#define Entry0_VLAN_Prior(n)				(n<<16)
#define Entry0_VLAN_ID_Shift				4
#define Entry0_Port_Transmit(n)				(n<<0)				

/*三层表条目1读写寄存器1 IPME1R1（05-8A, 05-8B, ~05-8F,; RW）*/
#define SWITCH_IPME1R1						(SWITCH_BASE_ADDR + 0x58A)

/*三层表条目1读写寄存器2 IPME1R2（05-90, 05-91, ~05-95,; RW）*/
#define SWITCH_IPME1R2						(SWITCH_BASE_ADDR + 0x590)

/*三层表条目1读写寄存器3 IPME1R3（05-96, 05-97, ~05-9D,; RW）*/
#define SWITCH_IPME1R3						(SWITCH_BASE_ADDR + 0x596)
#define Entry1_Valid						(1<<51)
#define Entry1_IP_Shift						19
#define Entry1_VLAN_Prior(n)				(n<<16)
#define Entry1_VLAN_ID_Shift				4
#define Entry1_Port_Transmit(n)				(n<<0)				

/*QoS全局控制寄存器 QGCR（30-00; RW）*/
#define SWITCH_QGCR							(SWITCH_BASE_ADDR + 0x3000)
#define Port_QoS_Enable						(1<<6)
#define Port_QoS_Disable					(0<<6)
#define QoS_Prior(n)						(n<<2)




/*入口优先级ID映射寄存器 IPPIM0R（端口0）~（端口8）（30-50,30-51; RW）*/
#define SWITCH_IPPIM0R_Port(n)				(SWITCH_BASE_ADDR + 0x3050 + n*0x2)

/*发送队列控制寄存器 TXQCR（30-80; RW）*/
#define SWITCH_TXQCR						(SWITCH_BASE_ADDR + 0x3080)

#define TXQCR_Mode_Queue_Single				(0<<0x2)
#define TXQCR_Mode_Queue_Double				(1<<0x2)
#define TXQCR_Mode_Queue_Triple				(2<<0x2)
#define TXQCR_Mode_Queue_Quadruple			(3<<0x2)

#define TXQCR_High_Priority_Grab_En			(1<<0x4)

/*发送队列权重寄存器 TXQWR（30-81, 30-82, 30-83, 30-84; RW）*/
#define SWITCH_TXQWR						(SWITCH_BASE_ADDR + 0x3081)

/*默认IEEE 802.1Q Tag寄存器（端口0） DIT0R（34-10, 34-11; RW）*/
#define SWITCH_DITR(n)						(SWITCH_BASE_ADDR + 0x3410 + n*0x2)

/*VLAN控制寄存器（端口0） PBVLANC0R（31-00, 31-01; RW）*/
#define SWITCH_PBVLANCR(n)					(SWITCH_BASE_ADDR + 0x3100 + n*0x2)

#define PBVLAN_Transmit_Mask_Port(x)		(x<<0)

/*全局802.1Q寄存器 G8021QR（34-00; RW）*/
#define SWITCH_G8021QR						(SWITCH_BASE_ADDR + 0x3400)

#define G8021_Enable						(1<<7)
#define G8021_Disable						(0<<7)

/*巨型帧屏蔽寄存器 JFPMR（40-01, 40-02, 40-03, 40-04; RW）*/
#define SWITCH_JFPMR						(SWITCH_BASE_ADDR + 0x4001)

#define JFP_10_100_Enable					(1<<24)
#define JFP_Mask_Port(n)					(n<<0)





#endif /* SWITCH_H_ */
