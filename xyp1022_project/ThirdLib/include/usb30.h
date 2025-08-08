//------------------------------------------------------------------
// Description   : This file define the parameters of the usb30_otg
// Module Name   : usb30.h
// Author        : gylan
// First Edit    : 2019-08-15
// Last Modified : 2019-08-15
//------------------------------------------------------------------

#ifndef USB30_H
#define USB30_H

#ifdef DDR_EXIST
#define MEM_BASE_ADDR           0x00000000  //DDR Low Address
#define MEM_BASE_ADDR_UP        0x00000000  //DDR High Address
#else
#define MEM_BASE_ADDR           0xE0000000  //SRAM0 ADDR From 4_E000_0000 ~ 4_E003_FFFF 
#define MEM_BASE_ADDR_UP        0x00000004  //SRAM0 ADDR From 4_E000_0000 ~ 4_E003_FFFF
#endif

#ifndef USB1_TEST
#define USB30_BASE_ADDR               0xE9300000
#define USB30_BASE_ADDR_UP            0x00000004   //For CPU access USB CSR space
#define USB30_MISCE_BASE_ADDR         0xE9500000
#define USB30_MISCE_BASE_ADDR_UP      0x00000004   //For CPU access USB CSR space

#define USB30_INT_NUM                 39
#define USB30_OTG_INT_NUM             38
#define USB30_HST_SMI_INT_NUM         37
#else
#define USB30_BASE_ADDR               0xE9400000
#define USB30_BASE_ADDR_UP            0x00000004   //For CPU access USB CSR space
#define USB30_MISCE_BASE_ADDR         0xE9520000
#define USB30_MISCE_BASE_ADDR_UP      0x00000004   //For CPU access USB CSR space

#define USB30_INT_NUM                 42
#define USB30_OTG_INT_NUM             41
#define USB30_HST_SMI_INT_NUM         40
#endif

#define IO_READ32_USB(p,v)   asm volatile("lwzx %0,0,%1":"=r"(v):"r"(p))

#ifdef DDR_EXIST
#define xHC_Transfer_TRB_addr              (MEM_BASE_ADDR + 0x000E0000) 
#define xHC_Command_TRB_addr               (MEM_BASE_ADDR + 0x000F0000) 
#define xHC_Event_TRB_addr                 (MEM_BASE_ADDR + 0x00100000)    
#define xHC_DCBAR_addr                     (MEM_BASE_ADDR + 0x00200000) 
#define xHC_Device_Context_addr            (MEM_BASE_ADDR + 0x00200800)
#define xHC_Input_Context_addr             (MEM_BASE_ADDR + 0x00400000)    
#define xHC_Scratch_Pad_Array_addr         (MEM_BASE_ADDR + 0x00401000)
#define xHC_Scratch_Pad_buf0_addr          (MEM_BASE_ADDR + 0x00402000)
#define xHC_EventRing_Segment_addr         (MEM_BASE_ADDR + 0x00220000)   
#define xHC_EventRing_Segment_table_addr   (MEM_BASE_ADDR + 0x002B0000)  
#define xHC_CommandRing_Segment_addr       (MEM_BASE_ADDR + 0x00230000)   
#define xHC_TransferRing_Segment_addr      (MEM_BASE_ADDR + 0x00500000)   
#define xHC_Data_Buffer_addr               (MEM_BASE_ADDR + 0x00608000)
#else
#define xHC_Transfer_TRB_addr              (MEM_BASE_ADDR + 0x00000000) 
#define xHC_Command_TRB_addr               (MEM_BASE_ADDR + 0x00001000) 
#define xHC_Event_TRB_addr                 (MEM_BASE_ADDR + 0x00002000)    
#define xHC_DCBAR_addr                     (MEM_BASE_ADDR + 0x00003000) 
#define xHC_Device_Context_addr            (MEM_BASE_ADDR + 0x00004000)
#define xHC_Input_Context_addr             (MEM_BASE_ADDR + 0x00005000)    
#define xHC_Scratch_Pad_Array_addr         (MEM_BASE_ADDR + 0x00006000)
#define xHC_Scratch_Pad_buf0_addr          (MEM_BASE_ADDR + 0x00007000)
#define xHC_EventRing_Segment_addr         (MEM_BASE_ADDR + 0x00008000)   
#define xHC_EventRing_Segment_table_addr   (MEM_BASE_ADDR + 0x00009000)  
#define xHC_CommandRing_Segment_addr       (MEM_BASE_ADDR + 0x0000A000)   
#define xHC_TransferRing_Segment_addr      (MEM_BASE_ADDR + 0x0000B000)   
#define xHC_Data_Buffer_addr               (MEM_BASE_ADDR + 0x00010000)
#endif

//Extensible Host Control Capability Registers
#define  CAPLENGTH         (USB30_BASE_ADDR + 0x0)
#define  HCSPARAMS1        (USB30_BASE_ADDR + 0x4)
#define  HCSPARAMS2        (USB30_BASE_ADDR + 0x8)
#define  HCSPARAMS3        (USB30_BASE_ADDR + 0xC)
#define  HCCPARAMS1        (USB30_BASE_ADDR + 0x10)
#define  DBOFF             (USB30_BASE_ADDR + 0x14)
#define  RTSOFF            (USB30_BASE_ADDR + 0x18)
#define  HCCPARAMS2        (USB30_BASE_ADDR + 0x1C)

#endif
