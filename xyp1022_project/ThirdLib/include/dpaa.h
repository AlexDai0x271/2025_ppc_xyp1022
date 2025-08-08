//---------------system address map---------------//
#define   pme_base         	(0x00316000 | 0xea000000) 
#define   quem_base        	(0x00318000 | 0xea000000)
#define   bufm_base        	(0x0031a000 | 0xea000000)
#define   fram1_base       	(0x00400000 | 0xea000000)
#define   fram2_base		(0x00500000 | 0xea000000)
//-----------frame manager address map------------//
#define   fram_mem_base    	(fram1_base | 0x00000000) 
#define   fram_port_base   	(fram1_base | 0x00090000)
#define   policer_base     	(fram1_base | 0x000c0000)
#define   keygen_base      	(fram1_base | 0x000c1000)
#define   soft_parser_base 	(fram1_base | 0x000c7000)
#define	  bmi_base		(fram1_base | 0x480000)
#define   qmi_base		(fram1_base | 0x488000)
#define	  fman_ctrl_base	(fram1_base | 0x4c4000)
//---------------paser address map---------------//
#define   paser_mem_base   	0x00000000
#define   paser_mem_offset 	0x0003f07c
#define   paser_reg_base   	0x00000080
#define   paser_reg_offset 	0x0003f3fc
#define   sp_mem_base      	0x00000000
#define   sp_mem_offset    	0x000007ff
#define   sp_reg_base      	0x00000800
#define   sp_reg_offset    	0x00000fff
//---------------bufm address map---------------//
#define   bufm_cena_base   	0x00600000
#define   bufm_cena_offset 	0x00003fff 
#define   bufm_cinh_base   	0x00700000 
#define   bufm_cinh_offset 	0x00000fff
#define   bufm_cfg_base    	0x00000000
#define   bufm_cfg_offset  	0x00000e10
//---------------------------------------------//
#define	  QCSP0_CR_BASE		0x3800
#define	  QMAN_SOFT_PORTAL_BASE	0xea800000 
#define	  BMAN_SOFT_PORTAL_BASE 0xea600000
//mac controller config register
#define	  mac0_cfg_base		(fram1_base | 0x4d0000) 
#define	  mac1_cfg_base		(fram1_base | 0x4d4000)
#define	  mac2_cfg_base		(fram1_base | 0x4d8000)
#define	  mac3_cfg_base		(fram1_base | 0x4dc000)
#define	  xgmac_cfg_base	(fram1_base | 0x4e0000)
//port config
#define	  oh_1_base		(bmi_base | (0x1  <<10)) 
#define	  oh_2_base		(bmi_base | (0x2  <<10))
#define	  oh_3_base		(bmi_base | (0x3  <<10))
#define	  oh_4_base		(bmi_base | (0x4  <<10))
#define	  oh_5_base		(bmi_base | (0x5  <<10))
#define	  oh_6_base		(bmi_base | (0x6  <<10))
#define	  oh_7_base		(bmi_base | (0x7  <<10))
#define	  rx_port0_base		(bmi_base | (0x8  <<10)) 
#define	  rx_port1_base		(bmi_base | (0x9  <<10))
#define	  rx_port2_base		(bmi_base | (0xa  <<10))
#define	  rx_port3_base		(bmi_base | (0xb  <<10))
#define	  rx_port4_base		(bmi_base | (0x10 <<10))
#define	  tx_port0_base		(bmi_base | (0xc  <<10)) 
#define	  tx_port1_base		(bmi_base | (0xd  <<10))
#define	  tx_port2_base		(bmi_base | (0xe  <<10))
#define	  tx_port3_base		(bmi_base | (0xf  <<10))
#define	  tx_port4_base		(bmi_base | (0x11 <<10))
//#define  DMA_SDRAM_EXTERNAL 
#define FQID0			0x000000 
#define FQID1			0x000100
#define FQID2			0x000200
#define FQID3			0x000300
#define INIT_COUNTER 		0x0100
//[15:13] channel num
//[12:10] work queue num
//channel 	0x40~0x4b
//sec channel	0x80
//pme channel	0xa0
#define DEST_WQ0		0x4701 
#define DEST_WQ1		0x4801
#define DEST_WQ2		0x4901
#define DEST_WQ3		0x4a01
#define DEST_WQ4		0x4b01
#define FQ_CTRL0		0x0600  		//{5'h0,2'b11,9'h0} 
#define FQ_CTRL1		0x0600			//
#define FQ_CTRL2		0x0600			//
#define FQ_CTRL3		0x0600			//
#define VERB			0x1
#define INI_FQ_SCHEDULE		0x41
#define INI_FQ_LEAV_PARKED	0x40
#define WE_MASK_NON		0xffff

#define FQD_BARE		(0xa000	 <<0 )
#define FQD_BAR			(0xa1000 <<12)
#define FQD_SIZE		(0x12)			//set as 512KB,2^(size+1)
#define	PFDR_BARE		(0xb000  <<0)	
#define	PFDR_BAR		(0xb2000 <<12)
#define PFDR_SIZE		(0x40)

#define BUF0_DDR_ADDR		0x004800		//temp use
#define PCR_BASE		0x1000
#define IOCD_vld		0x1

#define ROOT_AD			0x30000
#define MATCH_TABLE 		0x30010
#define CC_BASE			0x30100	
