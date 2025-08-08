#ifndef VD_H
#define VD_H


#define SOC_VD_BASE_ADDR         0xEB000000

#define VDCR_MASK            0x3F3F3F01
#define VDIER_MASK           0x00003F3F
#define VDSR_MASK            0x0000003F

#define VDCR_SOC             (SOC_VD_BASE_ADDR + 0x00)
#define VDIER_SOC            (SOC_VD_BASE_ADDR + 0x04)
#define VDSR_SOC             (SOC_VD_BASE_ADDR + 0x08)   //Read Only
#define VD_RESERVED          (SOC_VD_BASE_ADDR + 0x0C)

#define VD_HVD_3P3_STAT      (1<<1)
#define VD_LVD_3P3_STAT      (1<<0)
#define VD_HVD_1P8_STAT      (1<<3)
#define VD_LVD_1P8_STAT      (1<<2)
#define VD_HVD_0P9_STAT      (1<<5)
#define VD_LVD_0P9_STAT      (1<<4)

#define   VD_EN              (0x1<<0)

#define   LVD_IE_3P3         (0x1<<8)
#define   HVD_IE_3P3         (0x1<<9) 
#define   LVD_IE_1P8         (0x1<<10)
#define   HVD_IE_1P8         (0x1<<11) 
#define   LVD_IE_0P9         (0x1<<12)
#define   HVD_IE_0P9         (0x1<<13) 

#define   HVD_EN_3P3         (0x1<<9) 
#define   LVD_EN_3P3         (0x1<<8)
#define   HVD_EN_1P8         (0x1<<17) 
#define   LVD_EN_1P8         (0x1<<16)
#define   HVD_EN_0P9         (0x1<<25) 
#define   LVD_EN_0P9         (0x1<<24)

#define   HVD_CR_3P3(n)      (n<<12)
#define   LVD_CR_3P3(n)      (n<<10)
#define   HVD_CR_1P8(n)      (n<<20)
#define   LVD_CR_1P8(n)      (n<<18)
#define   HVD_CR_0P9(n)      (n<<28)
#define   LVD_CR_0P9(n)      (n<<26)

#endif
