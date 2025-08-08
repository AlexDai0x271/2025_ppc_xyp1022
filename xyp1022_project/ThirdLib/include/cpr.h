#define CPR_BASE_ADDR                   0xEB007000

#define CLK_SRC_SEL_OFFSET              0x0
#define PLL1_CFG_OFFSET                 0x4
#define PLL2_CFG_OFFSET                 0x8
#define PLL3_CFG_OFFSET                 0xC
#define PLL4_CFG_OFFSET                 0x10
#define PLL5_CFG_OFFSET                 0x14
#define PLL6_CFG_OFFSET                 0x18
#define PLL_PD_BYPASS_OFFSET            0x20
#define PLL_RST_CTRL_OFFSET             0x24
#define PLL_SW_RST_OFFSET               0x28
#define PLL_STATUS_OFFSET               0x2C

#define CPU_CLK_CFG_OFFSET              0x30
#define AXI_CLK_CFG_OFFSET              0x34

#define DDR3_MCLK_CFG_OFFSET            0x40

#define GTX0_CLK_CFG_OFFSET             0x50
#define GTX1_CLK_CFG_OFFSET             0x54
#define GTX2_CLK_CFG_OFFSET             0x58
#define GTX3_CLK_CFG_OFFSET             0x5C
#define GTX4_CLK_CFG_OFFSET             0x60
#define GTX5_CLK_CFG_OFFSET             0x64
#define GTX6_CLK_CFG_OFFSET             0x68
#define GTX7_CLK_CFG_OFFSET             0x6C

#define XTAL0_CLK_CFG_OFFSET            0x70
#define XTAL1_CLK_CFG_OFFSET            0x74
#define XTAL2_CLK_CFG_OFFSET            0x78
#define XTAL3_CLK_CFG_OFFSET            0x7C

#define PERI_CLK_CFG_OFFSET             0x80
#define APB_CLK_CFG_OFFSET              0x84
#define SD_CLK_CFG_OFFSET               0x88
#define RNG_CLK_CFG_OFFSET              0x8C
#define PKHA_CLK_CFG_OFFSET             0x90
#define RSCP_CLK_CFG_OFFSET             0x94
#define AES_CLK_CFG_OFFSET              0x98
#define DES_CLK_CFG_OFFSET              0x9C
#define SM4_CLK_CFG_OFFSET              0xA0
#define IP1_CLK_CFG_OFFSET              0xA4
#define IP3_CLK_CFG_OFFSET              0xA8
#define MDHA_CLK_CFG_OFFSET             0xAC
#define LCDC_PCLK_CFG_OFFSET            0xB0


#define EMAC_CLK_SEL_CFG_OFFSET         0xC0
#define SW_RST_CFG_OFFSET               0xC4
#define PCIE_RESET_CFG_OFFSET           0xC8


#define CLK_GATE_CFG0_OFFSET            0x100
#define CLK_GATE_CFG1_OFFSET            0x104
#define SYS_CLK_CFG_STATUS_OFFSET       0x108
#define CPU_CLK_EN_CFG_OFFSET           0x10C
#define RESET_STATUS_OFFSET             0x118
#define HOLD_PREADY_CTL_OFFSET          0x11C

#define CLK_SRC_SEL          (CPR_BASE_ADDR + CLK_SRC_SEL_OFFSET)
#define PLL1_CFG             (CPR_BASE_ADDR + PLL1_CFG_OFFSET)
#define PLL2_CFG             (CPR_BASE_ADDR + PLL2_CFG_OFFSET)
#define PLL3_CFG             (CPR_BASE_ADDR + PLL3_CFG_OFFSET)
#define PLL4_CFG             (CPR_BASE_ADDR + PLL4_CFG_OFFSET)
#define PLL5_CFG             (CPR_BASE_ADDR + PLL5_CFG_OFFSET)
#define PLL6_CFG             (CPR_BASE_ADDR + PLL6_CFG_OFFSET)
#define PLL_PD_BYPASS        (CPR_BASE_ADDR + PLL_PD_BYPASS_OFFSET)
#define PLL_RST_CTRL         (CPR_BASE_ADDR + PLL_RST_CTRL_OFFSET)
#define PLL_SW_RST           (CPR_BASE_ADDR + PLL_SW_RST_OFFSET)
#define PLL_STATUS           (CPR_BASE_ADDR + PLL_STATUS_OFFSET)

#define CPU_CLK_CFG          (CPR_BASE_ADDR + CPU_CLK_CFG_OFFSET)
#define AXI_CLK_CFG          (CPR_BASE_ADDR + AXI_CLK_CFG_OFFSET)
#define DDR3_MCLK_CFG        (CPR_BASE_ADDR + DDR3_MCLK_CFG_OFFSET)

#define GTX0_CLK_CFG         (CPR_BASE_ADDR + GTX0_CLK_CFG_OFFSET)
#define GTX1_CLK_CFG         (CPR_BASE_ADDR + GTX1_CLK_CFG_OFFSET)
#define GTX2_CLK_CFG         (CPR_BASE_ADDR + GTX2_CLK_CFG_OFFSET)
#define GTX3_CLK_CFG         (CPR_BASE_ADDR + GTX3_CLK_CFG_OFFSET)
#define GTX4_CLK_CFG         (CPR_BASE_ADDR + GTX4_CLK_CFG_OFFSET)
#define GTX5_CLK_CFG         (CPR_BASE_ADDR + GTX5_CLK_CFG_OFFSET)
#define GTX6_CLK_CFG         (CPR_BASE_ADDR + GTX6_CLK_CFG_OFFSET)
#define GTX7_CLK_CFG         (CPR_BASE_ADDR + GTX7_CLK_CFG_OFFSET)

#define XTAL0_CLK_CFG        (CPR_BASE_ADDR + XTAL0_CLK_CFG_OFFSET)
#define XTAL1_CLK_CFG        (CPR_BASE_ADDR + XTAL1_CLK_CFG_OFFSET)
#define XTAL2_CLK_CFG        (CPR_BASE_ADDR + XTAL2_CLK_CFG_OFFSET)
#define XTAL3_CLK_CFG        (CPR_BASE_ADDR + XTAL3_CLK_CFG_OFFSET)

#define PERI_CLK_CFG         (CPR_BASE_ADDR + PERI_CLK_CFG_OFFSET)
#define APB_CLK_CFG          (CPR_BASE_ADDR + APB_CLK_CFG_OFFSET)
#define SD_CLK_CFG           (CPR_BASE_ADDR + SD_CLK_CFG_OFFSET)
#define RNG_CLK_CFG          (CPR_BASE_ADDR + RNG_CLK_CFG_OFFSET)
#define PKHA_CLK_CFG         (CPR_BASE_ADDR + PKHA_CLK_CFG_OFFSET)
#define RSCP_CLK_CFG         (CPR_BASE_ADDR + RSCP_CLK_CFG_OFFSET)
#define AES_CLK_CFG          (CPR_BASE_ADDR + AES_CLK_CFG_OFFSET)
#define DES_CLK_CFG          (CPR_BASE_ADDR + DES_CLK_CFG_OFFSET)
#define SM4_CLK_CFG          (CPR_BASE_ADDR + SM4_CLK_CFG_OFFSET)
#define IP1_CLK_CFG          (CPR_BASE_ADDR + IP1_CLK_CFG_OFFSET)
#define IP3_CLK_CFG          (CPR_BASE_ADDR + IP3_CLK_CFG_OFFSET)
#define MDHA_CLK_CFG         (CPR_BASE_ADDR + MDHA_CLK_CFG_OFFSET)
#define LCDC_PCLK_CFG        (CPR_BASE_ADDR + LCDC_PCLK_CFG_OFFSET)

#define EMAC_CLK_SEL_CFG     (CPR_BASE_ADDR + EMAC_CLK_SEL_CFG_OFFSET)
#define SW_RST_CFG           (CPR_BASE_ADDR + SW_RST_CFG_OFFSET)
#define PCIE_RESET_CFG       (CPR_BASE_ADDR + PCIE_RESET_CFG_OFFSET)

#define CLK_GATE_CFG0        (CPR_BASE_ADDR + CLK_GATE_CFG0_OFFSET)
#define CLK_GATE_CFG1        (CPR_BASE_ADDR + CLK_GATE_CFG1_OFFSET)
#define SYS_CLK_CFG_STATUS   (CPR_BASE_ADDR + SYS_CLK_CFG_STATUS_OFFSET)
#define CPU_CLK_EN_CFG       (CPR_BASE_ADDR + CPU_CLK_EN_CFG_OFFSET)
#define RESET_STATUS         (CPR_BASE_ADDR + RESET_STATUS_OFFSET)
#define HOLD_PREADY_CTL      (CPR_BASE_ADDR + HOLD_PREADY_CTL_OFFSET)


// GATE_CFG0
//----------------------------------------------------
#define  CLK_GATE_CFG0_INIT     0x00000000
#define  CPU1_GATE        1
#define  CPU2_GATE        2
#define  CPU3_GATE        3
#define  DDR3_GATE        4      
#define  PCIE0_GATE       5    
#define  PCIE1_GATE       6    
#define  PCIE2_GATE       7   
#define  PCIE3_GATE       8   
#define  USB0_GATE        9  
#define  USB1_GATE        10  
#define  SATA0_GATE       11 
#define  SATA1_GATE       12 
#define  XGMAC0_GATE      13 
#define  XGMAC1_GATE      14 
#define  AURORA_GATE      15   
#define  LCDC_GATE        16

#define  SEC_GATE         17
#define  EBC_GATE         18      // gate bus_ebc_aclk, ebc_hclk
#define  DMAC0_AXI_GATE   19      // gate dmac0_aclk
#define  DMAC1_AXI_GATE   20      // gate dmac1_aclk
#define  GMAC_GATE        21 
//#define  GMAC2_GATE       19 
//#define  GMAC3_GATE       20 
//#define  GMAC4_GATE       21 
//#define  GMAC5_GATE       22 
//#define  GMAC6_GATE       23 
//#define  GMAC7_GATE       24 
#define  SD_GATE          22      // gate sd_aclk & sd_clk
#define  ROM_GATE         23 
#define  SRAM0_AXI_GATE   24      // gate sram0_aclk
#define  SRAM1_AXI_GATE   25      // gate sram1_aclk


// GATE_CFG1
//----------------------------------------------------
#define  RNG_GATE         0
#define  PKHA_GATE        1
#define  RSCP_GATE        2
#define  AES_GATE         3
#define  DES_GATE         4
#define  SM4_GATE         5
#define  IP1_GATE         6
#define  IP3_GATE         7
#define  MDHA_GATE        8

#define  UART0_GATE       16      // gate xtal1_clk
#define  UART1_GATE       17      // gate xtal1_clk
#define  UART2_GATE       18      // gate xtal2_clk
#define  UART3_GATE       19      // gate xtal3_clk
#define  IIC0_GATE        20
#define  IIC1_GATE        21
#define  IIC2_GATE        22
#define  IIC3_GATE        23
#define  SPI0_GATE        24


//----------------------------------------------------
// SYS_CLK_CFG_STATUS
//----------------------------------------------------
// SYS_CLK_CFG_STATUS[0] = MDHA
// SYS_CLK_CFG_STATUS[1] = IP3
// SYS_CLK_CFG_STATUS[2] = IP1
// SYS_CLK_CFG_STATUS[3] = SM4
// SYS_CLK_CFG_STATUS[4] = DES
// SYS_CLK_CFG_STATUS[5] = AES
// SYS_CLK_CFG_STATUS[6] = RSCP
// SYS_CLK_CFG_STATUS[7] = PKHA
// SYS_CLK_CFG_STATUS[8] = RNG
// SYS_CLK_CFG_STATUS[9] = SD
// SYS_CLK_CFG_STATUS[10] = APB
// SYS_CLK_CFG_STATUS[11] = PERI
// SYS_CLK_CFG_STATUS[12] = GTX8
// SYS_CLK_CFG_STATUS[13] = GTX7
// SYS_CLK_CFG_STATUS[14] = GTX6
// SYS_CLK_CFG_STATUS[15] = GTX5
// SYS_CLK_CFG_STATUS[16] = GTX4
// SYS_CLK_CFG_STATUS[17] = GTX3
// SYS_CLK_CFG_STATUS[18] = GTX2
// SYS_CLK_CFG_STATUS[19] = GTX1
// SYS_CLK_CFG_STATUS[20] = XTAL3
// SYS_CLK_CFG_STATUS[21] = XTAL2
// SYS_CLK_CFG_STATUS[22] = XTAL1
// SYS_CLK_CFG_STATUS[23] = CPU
// SYS_CLK_CFG_STATUS[24] = DDR
// SYS_CLK_CFG_STATUS[25:31] = 7'b0
#define      SYS_STATUS_MDHA       0x00000001     //[0]             -- PLL3
#define      SYS_STATUS_IP3        0x00000002     //[1]             -- PLL3
#define      SYS_STATUS_IP1        0x00000004     //[2]             -- PLL3
#define      SYS_STATUS_SM4        0x00000008     //[3]             -- PLL3
#define      SYS_STATUS_DES        0x00000010     //[4]             -- PLL3
#define      SYS_STATUS_AES        0x00000020     //[5]             -- PLL3
#define      SYS_STATUS_RSCP       0x00000040     //[6]             -- PLL3
#define      SYS_STATUS_PKHA       0x00000080     //[7]             -- PLL3
#define      SYS_STATUS_RNG        0x00000100     //[8]             -- PLL3
#define      SYS_STATUS_APB        0x00000400     //[10]            -- PLL3
#define      SYS_STATUS_PERI       0x00000800     //[11]            -- PLL3

#define      SYS_STATUS_SD         0x00000200     //[9]                     -- PLL4
#define      SYS_STATUS_GTX7       0x00001000     //[12]                    -- PLL4
#define      SYS_STATUS_GTX6       0x00002000     //[13]                    -- PLL4
#define      SYS_STATUS_GTX5       0x00004000     //[14]                    -- PLL4
#define      SYS_STATUS_GTX4       0x00008000     //[15]                    -- PLL4
#define      SYS_STATUS_GTX3       0x00010000     //[16]                    -- PLL4
#define      SYS_STATUS_GTX2       0x00020000     //[17]                    -- PLL4
#define      SYS_STATUS_GTX1       0x00040000     //[18]                    -- PLL4
#define      SYS_STATUS_GTX0       0x00080000     //[19]                    -- PLL4

#define      SYS_STATUS_XTAL3      0x00100000     //[20]            -- PLL3
#define      SYS_STATUS_XTAL2      0x00200000     //[21]            -- PLL3
#define      SYS_STATUS_XTAL1      0x00400000     //[22]            -- PLL3
#define      SYS_STATUS_XTAL0      0x00800000     //[23]            -- PLL3
#define      SYS_STATUS_LCDCP      0x01000000     //[24]            -- PLL3

#define      SYS_STATUS_CPU        0x02000000     //[25]       -- PLL1
#define      SYS_STATUS_DDR3       0x04000000     //[26]       -- PLL2
#define      SYS_STATUS_CORE       0x08000000     //[27]       -- PLL5

#define      SYS_STATUS_PLL1       0x02000000
#define      SYS_STATUS_PLL2       0x04000000
#define      SYS_STATUS_PLL3       0x00700DFF
#define      SYS_STATUS_PLL4       0x01FFF200
#define      SYS_STATUS_PLL5       0x08000000

//----------------------------------------------------
// CLK_SRC_SEL
//----------------------------------------------------
#define      PLL1_SEL_XCLK         0x1E    // 5'b1_1110
#define      PLL2_SEL_XCLK         0x1D    // 5'b1_1101
#define      PLL3_SEL_XCLK         0x1B    // 5'b1_1011
#define      PLL4_SEL_XCLK         0x17    // 5'b1_0111
#define      PLL5_SEL_XCLK         0x0F    // 5'b0_1111


//----------------------------------------------------
// RESET_STATUS
//----------------------------------------------------
#define      RESET_STATUS_SOFT_SYS       0x40    // [6]
#define      RESET_STATUS_SECMON         0x20    // [5]
#define      RESET_STATUS_POR            0x10    // [4]
#define      RESET_STATUS_PCIE_PERST_3   0x08    // [3]
#define      RESET_STATUS_PCIE_PERST_2   0x04    // [2]
#define      RESET_STATUS_PCIE_PERST_1   0x02    // [1]
#define      RESET_STATUS_PCIE_PERST_0   0x01    // [0]
