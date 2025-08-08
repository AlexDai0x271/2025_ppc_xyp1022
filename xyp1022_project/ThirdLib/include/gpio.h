#ifndef GPIO_H
#define GPIO_H

#define  GPIO_BASE_ADDR_HI           0x0
#define  GPIO_BASE_ADDR              0xE9640000

#define  GPIO_A_BASE_ADDR            (GPIO_BASE_ADDR + 0x0000)
#define  GPIO_B_BASE_ADDR            (GPIO_BASE_ADDR + 0x1000)
#define  GPIO_C_BASE_ADDR            (GPIO_BASE_ADDR + 0x2000)
#define  GPIO_D_BASE_ADDR            (GPIO_BASE_ADDR + 0x3000)
#define  GPIO_E_BASE_ADDR            (GPIO_BASE_ADDR + 0x4000)
#define  GPIO_F_BASE_ADDR            (GPIO_BASE_ADDR + 0x5000)
#define  GPIO_M_BASE_ADDR            (GPIO_BASE_ADDR + 0x6000)
#define  EPORT0_BASE_ADDR            (GPIO_BASE_ADDR + 0x9000)
#define  EPORT1_BASE_ADDR            (GPIO_BASE_ADDR + 0xA000)

#define     GPIO_BYTE0           0x0
#define     GPIO_BYTE1           0x1
#define     GPIO_BYTE2           0x2
#define     GPIO_BYTE3           0x3

#define     GPIO_OUT_OFFSET      0x000
#define     GPIO_DIR_OFFSET      0x400
#define     GPIO_INTMASK_OFFSET  0x410
#define     GPIO_PUE_OFFSET      0x420
#define     GPIO_IBE_OFFSET      0x430
#define     GPIO_ODE_OFFSET      0x440
#define     GPIO_IN_OFFSET       0x450
#define     GPIO_IOCTL_OFFSET    0x460
#define     GPIO_PDE_OFFSET      0x470
#define     GPIO_ST_OFFSET       0x480
#define     GPIO_DS_OFFSET       0x490
#define     GPIO_INT_OFFSET      0x4A0

#define     MARK_BIT0  ((0x01)<<2)
#define     MARK_BIT1  ((0x01<<1)<<2)
#define     MARK_BIT2  ((0x01<<2)<<2)
#define     MARK_BIT3  ((0x01<<3)<<2)
#define     MARK_BIT4  ((0x01<<4)<<2)
#define     MARK_BIT5  ((0x01<<5)<<2)
#define     MARK_BIT6  ((0x01<<6)<<2)
#define     MARK_BIT7  ((0x01<<7)<<2)
#define     ALL_BITS   (0xFF<<2)

#define     GPIO_A_INT_NUM   72
#define     GPIO_B_INT_NUM   71
#define     GPIO_C_INT_NUM   70
#define     GPIO_D_INT_NUM   69
#define     GPIO_E_INT_NUM   68

#define     GPIO_IDP15_INT   64
#define     GPIO_IDP14_INT   63
#define     GPIO_IDP13_INT   62
#define     GPIO_IDP12_INT   61
#define     GPIO_IDP11_INT   60
#define     GPIO_IDP10_INT   59
#define     GPIO_IDP9_INT    58
#define     GPIO_IDP8_INT    57
#define     GPIO_IDP7_INT    56
#define     GPIO_IDP6_INT    55
#define     GPIO_IDP5_INT    54
#define     GPIO_IDP4_INT    53
#define     GPIO_IDP3_INT    52
#define     GPIO_IDP2_INT    51
#define     GPIO_IDP1_INT    50
#define     GPIO_IDP0_INT    49

#define ALL_BITS_SET    0xFFFFFFFF//32bit

//[27:0]
#define MARK_A          0x0FFFFFFF 

//[30:0]
#define MARK_B          0x7FFFFFFF 

//[31:0]
#define MARK_C          0xFFFFFFFF 

//[26:0]
#define MARK_D          0x07FFFFFF 
//[26:0]
#define MARK_E          0x07FFFFFF        

#define MARK_M          0xFFFF


#define MASK_INT_GPIO MTDCR(MPIC_CTP0,MPIC_CTP_PRI_15)
#define EN_INT_GPIO   MTDCR(MPIC_CTP0,MPIC_CTP_PRI_0)


//=====================Little Endian============================
// Byte 0x0 : DATA[7:0]  
// Byte 0x1 : DATA[15:8] 
// Byte 0x2 : DATA[23:16]
// Byte 0x3 : DATA[31:24]
//
// Half 0x0 : DATA[15:0]
// Half 0x2 : DATA[31:16]
//=====================Little Endian============================
//  #define  EPORT0_EPDR    (*(volatile UINT8 *)(EPORT0_BASE_ADDR + 0x000))  // Eport Data Register              ipp_do_
//  #define  EPORT0_EPPDR   (*(volatile UINT8 *)(EPORT0_BASE_ADDR + 0x200))  // Eport Pin Data Register          ipp_ind_
//  #define  EPORT0_EPPAR   (*(volatile UINT16*)(EPORT0_BASE_ADDR + 0x400))  // Eport Pin Assignment Register
//  #define  EPORT0_EPDDR   (*(volatile UINT8 *)(EPORT0_BASE_ADDR + 0x402))  // Eport Data Direction Register    ipp_obe_
//  #define  EPORT0_EPFER   (*(volatile UINT8 *)(EPORT0_BASE_ADDR + 0x403))  // Eport Interrupt Enable Register
//  #define  EPORT0_EPFR    (*(volatile UINT8 *)(EPORT0_BASE_ADDR + 0x404))  // Eport Flag Register
//  #define  EPORT0_EPUER   (*(volatile UINT8 *)(EPORT0_BASE_ADDR + 0x405))  // Eport Pad Pull Up Enable         ipp_pue_
//  #define  EPORT0_ELPR    (*(volatile UINT8 *)(EPORT0_BASE_ADDR + 0x406))  // Eport Level Polarity Register
//  #define  EPORT0_EODER   (*(volatile UINT8 *)(EPORT0_BASE_ADDR + 0x407))  // Eport Pad Opern Drain Enable     ipp_ode_
//  #define  EPORT0_EPDER   (*(volatile UINT8 *)(EPORT0_BASE_ADDR + 0x408))  // Eport Pad Pull Down Enable       ipp_pde_
//  
//  
//  #define  EPORT1_EPDR    (*(volatile UINT8 *)(EPORT1_BASE_ADDR + 0x000))  // Eport Data Register              ipp_do_
//  #define  EPORT1_EPPDR   (*(volatile UINT8 *)(EPORT1_BASE_ADDR + 0x200))  // Eport Pin Data Register          ipp_ind_
//  #define  EPORT1_EPPAR   (*(volatile UINT16*)(EPORT1_BASE_ADDR + 0x400))  // Eport Pin Assignment Register
//  #define  EPORT1_EPDDR   (*(volatile UINT8 *)(EPORT1_BASE_ADDR + 0x402))  // Eport Data Direction Register    ipp_obe_
//  #define  EPORT1_EPFER   (*(volatile UINT8 *)(EPORT1_BASE_ADDR + 0x403))  // Eport Interrupt Enable Register
//  #define  EPORT1_EPFR    (*(volatile UINT8 *)(EPORT1_BASE_ADDR + 0x404))  // Eport Flag Register
//  #define  EPORT1_EPUER   (*(volatile UINT8 *)(EPORT1_BASE_ADDR + 0x405))  // Eport Pad Pull Up Enable         ipp_pue_
//  #define  EPORT1_ELPR    (*(volatile UINT8 *)(EPORT1_BASE_ADDR + 0x406))  // Eport Level Polarity Register
//  #define  EPORT1_EODER   (*(volatile UINT8 *)(EPORT1_BASE_ADDR + 0x407))  // Eport Pad Opern Drain Enable     ipp_ode_
//  #define  EPORT1_EPDER   (*(volatile UINT8 *)(EPORT1_BASE_ADDR + 0x408))  // Eport Pad Pull Down Enable       ipp_pde_


//=====================Big Endian============================
// Byte 0x3 : DATA[7:0]  
// Byte 0x2 : DATA[15:8]
// Byte 0x1 : DATA[23:16]
// Byte 0x0 : DATA[31:24]
//
// Half 0x2 : DATA[15:0]
// Half 0x0 : DATA[31:16]
//=====================Big Endian============================
// ips_port_size == 2'b01 & ips_size = 2'b00:
// ips_addr = 2'b00 : ips_byte = 4'b0001 : ips_byte_7_0  = 1
//            2'b01 : ips_byte = 4'b0010 : ips_byte_15_8 = 1
//            2'b10 : ips_byte = 4'b0001 : ips_byte_7_0  = 1
//            2'b11 : ips_byte = 4'b0010 : ips_byte_15_8 = 1
#define  EPORT0_EPDR    (*(volatile UINT8 *)(EPORT0_BASE_ADDR + 0x001))  // ips_byte_15_8, Eport Data Register              ipp_do_
#define  EPORT0_EPPDR   (*(volatile UINT8 *)(EPORT0_BASE_ADDR + 0x201))  // ips_byte_15_8, Eport Pin Data Register          ipp_ind_
#define  EPORT0_EPPAR   (*(volatile UINT16*)(EPORT0_BASE_ADDR + 0x400))  //                Eport Pin Assignment Register
#define  EPORT0_EPDDR   (*(volatile UINT8 *)(EPORT0_BASE_ADDR + 0x403))  // ips_byte_15_8, Eport Data Direction Register    ipp_obe_
#define  EPORT0_EPFER   (*(volatile UINT8 *)(EPORT0_BASE_ADDR + 0x402))  // ips_byte_7_0,  Eport Interrupt Enable Register
#define  EPORT0_EPFR    (*(volatile UINT8 *)(EPORT0_BASE_ADDR + 0x405))  // ips_byte_15_8, Eport Flag Register
#define  EPORT0_EPUER   (*(volatile UINT8 *)(EPORT0_BASE_ADDR + 0x404))  // ips_byte_7_0,  Eport Pad Pull Up Enable         ipp_pue_
#define  EPORT0_ELPR    (*(volatile UINT8 *)(EPORT0_BASE_ADDR + 0x407))  // ips_byte_15_8, Eport Level Polarity Register
#define  EPORT0_EODER   (*(volatile UINT8 *)(EPORT0_BASE_ADDR + 0x406))  // ips_byte_7_0,  Eport Pad Opern Drain Enable     ipp_ode_
#define  EPORT0_EPDER   (*(volatile UINT8 *)(EPORT0_BASE_ADDR + 0x409))  // ips_byte_15_8, Eport Pad Pull Down Enable       ipp_pde_


#define  EPORT1_EPDR    (*(volatile UINT8 *)(EPORT1_BASE_ADDR + 0x001))  // Eport Data Register              ipp_do_
#define  EPORT1_EPPDR   (*(volatile UINT8 *)(EPORT1_BASE_ADDR + 0x201))  // Eport Pin Data Register          ipp_ind_
#define  EPORT1_EPPAR   (*(volatile UINT16*)(EPORT1_BASE_ADDR + 0x400))  // Eport Pin Assignment Register
#define  EPORT1_EPDDR   (*(volatile UINT8 *)(EPORT1_BASE_ADDR + 0x403))  // Eport Data Direction Register    ipp_obe_
#define  EPORT1_EPFER   (*(volatile UINT8 *)(EPORT1_BASE_ADDR + 0x402))  // Eport Interrupt Enable Register
#define  EPORT1_EPFR    (*(volatile UINT8 *)(EPORT1_BASE_ADDR + 0x405))  // Eport Flag Register
#define  EPORT1_EPUER   (*(volatile UINT8 *)(EPORT1_BASE_ADDR + 0x404))  // Eport Pad Pull Up Enable         ipp_pue_
#define  EPORT1_ELPR    (*(volatile UINT8 *)(EPORT1_BASE_ADDR + 0x407))  // Eport Level Polarity Register
#define  EPORT1_EODER   (*(volatile UINT8 *)(EPORT1_BASE_ADDR + 0x406))  // Eport Pad Opern Drain Enable     ipp_ode_
#define  EPORT1_EPDER   (*(volatile UINT8 *)(EPORT1_BASE_ADDR + 0x409))  // Eport Pad Pull Down Enable       ipp_pde_


#define     EPORT1_INT7_NUM   64
#define     EPORT1_INT6_NUM   63
#define     EPORT1_INT5_NUM   62
#define     EPORT1_INT4_NUM   61
#define     EPORT1_INT3_NUM   60
#define     EPORT1_INT2_NUM   59
#define     EPORT1_INT1_NUM   58
#define     EPORT1_INT0_NUM   57

#define     EPORT0_INT7_NUM   56
#define     EPORT0_INT6_NUM   55
#define     EPORT0_INT5_NUM   54
#define     EPORT0_INT4_NUM   53
#define     EPORT0_INT3_NUM   52
#define     EPORT0_INT2_NUM   51
#define     EPORT0_INT1_NUM   50
#define     EPORT0_INT0_NUM   49
#endif

/*************** bit defines **************************************************/
/******************************************************************************/

/******************************************************************/
// REG DEFINITION
#define GPIO_OUT_REG  	*(volatile unsigned int *)(GPIO_M_BASE_ADDR+0x000)
#define GPIO_DIR_REG  	*(volatile unsigned int *)(GPIO_M_BASE_ADDR+0x400)
#define GPIO_INTEN_REG  *(volatile unsigned int *)(GPIO_M_BASE_ADDR+0x410)
#define GPIO_PUE_REG  	*(volatile unsigned int *)(GPIO_M_BASE_ADDR+0x420)
#define GPIO_IBE_REG  	*(volatile unsigned int *)(GPIO_M_BASE_ADDR+0x430)
#define GPIO_ODE_REG  	*(volatile unsigned int *)(GPIO_M_BASE_ADDR+0x440)
#define GPIO_IN_REG  	*(volatile unsigned int *)(GPIO_M_BASE_ADDR+0x450)
#define GPIO_CTL_REG  	*(volatile unsigned int *)(GPIO_M_BASE_ADDR+0x460)
/******************************************************************/








