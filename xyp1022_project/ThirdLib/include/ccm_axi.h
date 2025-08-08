#ifndef CCM_AXI_H
#define CCM_AXI_H

#define CCM_AXI_BASE_ADDR 0xEB009000ull

#define AXI_EN_B                (0x0  + CCM_AXI_BASE_ADDR)
#define JTAG_DISABLE            (0x4  + CCM_AXI_BASE_ADDR)
#define ENDIAN                  (0x8  + CCM_AXI_BASE_ADDR)
#define PCIE_TYPE               (0xC  + CCM_AXI_BASE_ADDR)
#define EPHY_INTF_SEL           (0x10 + CCM_AXI_BASE_ADDR)
#define CPU_EN                  (0x18 + CCM_AXI_BASE_ADDR)
#define STRAP_PIN               (0x1C + CCM_AXI_BASE_ADDR)
#define PCIE_PHYSTATUS          (0x20 + CCM_AXI_BASE_ADDR)
#define BOOT_STAT               (0x24 + CCM_AXI_BASE_ADDR)
#define TRNG_EN                 (0x28 + CCM_AXI_BASE_ADDR)
#define CCM_BOOT_CFG            (0x28 + CCM_AXI_BASE_ADDR)
#define GMAC0_HIGH_ADDR         (0x4C + CCM_AXI_BASE_ADDR) 
#define GMAC1_HIGH_ADDR         (0x30 + CCM_AXI_BASE_ADDR) 
#define GMAC2_HIGH_ADDR         (0x34 + CCM_AXI_BASE_ADDR) 
#define GMAC3_HIGH_ADDR         (0x38 + CCM_AXI_BASE_ADDR) 
#define GMAC4_HIGH_ADDR         (0x3C + CCM_AXI_BASE_ADDR) 
#define GMAC5_HIGH_ADDR         (0x40 + CCM_AXI_BASE_ADDR) 
#define GMAC6_HIGH_ADDR         (0x44 + CCM_AXI_BASE_ADDR) 
#define GMAC7_HIGH_ADDR         (0x48 + CCM_AXI_BASE_ADDR) 


//definition of CIR/CIP CFG0/1/2
//CFG0    [11:0]  :aw liodn
//        [19:12] :aw sbtag
//        [31:20] :ar liodn
//CFG1    [7:0]   :aw srcid
//        [23:16] :ar srcid
//        [31:24] :ar sbtag
//CFG2    [0]     :aw priority
//        [1]     :aw nosnp
//        [2]     :aw enhanced
//        [6:3]   :aw etype //actully used [5:3]
//        [8]     :ar priority
//        [9]     :ar nosnp
//        [10]    :ar enhanced
//        [14:11] :ar etype
//        [16]    :sos
//        [17]    :ebs
//

#define AR_LIODN_SHIFT    20
#define AR_ENHANCED_SHIFT 10
#define AR_ETYPE_SHIFT    11
#define AW_ENHANCED_SHIFT 2 
#define AW_ETYPE_SHIFT    3

#define CIR00_AXISB_CFG0        (0x50 + CCM_AXI_BASE_ADDR)
#define CIR00_AXISB_CFG1        (0x54 + CCM_AXI_BASE_ADDR)
#define CIR00_AXISB_CFG2        (0x58 + CCM_AXI_BASE_ADDR)
#define CIR01_AXISB_CFG0        (0x60 + CCM_AXI_BASE_ADDR)
#define CIR01_AXISB_CFG1        (0x64 + CCM_AXI_BASE_ADDR)
#define CIR01_AXISB_CFG2        (0x68 + CCM_AXI_BASE_ADDR)
#define CIR02_AXISB_CFG0        (0x70 + CCM_AXI_BASE_ADDR)
#define CIR02_AXISB_CFG1        (0x74 + CCM_AXI_BASE_ADDR)
#define CIR02_AXISB_CFG2        (0x78 + CCM_AXI_BASE_ADDR)
#define CIR03_AXISB_CFG0        (0x80 + CCM_AXI_BASE_ADDR)
#define CIR03_AXISB_CFG1        (0x84 + CCM_AXI_BASE_ADDR)
#define CIR03_AXISB_CFG2        (0x88 + CCM_AXI_BASE_ADDR)
#define CIR10_AXISB_CFG0        (0x90 + CCM_AXI_BASE_ADDR)
#define CIR10_AXISB_CFG1        (0x94 + CCM_AXI_BASE_ADDR)
#define CIR10_AXISB_CFG2        (0x98 + CCM_AXI_BASE_ADDR)
#define CIR11_AXISB_CFG0        (0xA0 + CCM_AXI_BASE_ADDR)
#define CIR11_AXISB_CFG1        (0xA4 + CCM_AXI_BASE_ADDR)
#define CIR11_AXISB_CFG2        (0xA8 + CCM_AXI_BASE_ADDR)
#define CIR12_AXISB_CFG0        (0xB0 + CCM_AXI_BASE_ADDR)
#define CIR12_AXISB_CFG1        (0xB4 + CCM_AXI_BASE_ADDR)
#define CIR12_AXISB_CFG2        (0xB8 + CCM_AXI_BASE_ADDR)
#define CIR13_AXISB_CFG0        (0xC0 + CCM_AXI_BASE_ADDR)
#define CIR13_AXISB_CFG1        (0xC4 + CCM_AXI_BASE_ADDR)
#define CIR13_AXISB_CFG2        (0xC8 + CCM_AXI_BASE_ADDR)

#define CIP0CIR_AXISB_CFG0      (0xD0 + CCM_AXI_BASE_ADDR)
#define CIP0CIR_AXISB_CFG1      (0xD4 + CCM_AXI_BASE_ADDR)
#define CIP0CIR_AXISB_CFG2      (0xD8 + CCM_AXI_BASE_ADDR)
#define CIP1CIR_AXISB_CFG0      (0xE0 + CCM_AXI_BASE_ADDR)
#define CIP1CIR_AXISB_CFG1      (0xE4 + CCM_AXI_BASE_ADDR)
#define CIP1CIR_AXISB_CFG2      (0xE8 + CCM_AXI_BASE_ADDR)

#endif
