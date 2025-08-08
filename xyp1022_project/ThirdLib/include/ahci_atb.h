
#define UINT32 unsigned int
#define UINT16 unsigned short
#define UINT8  unsigned char
#define ULONG  unsigned long int

#define PM_PARTIAL 1
#define PM_SLUMBER 2

#define CMD_CODE_CFA_WRITE_MULTIPLE_W_OUT_ERASE 0xCD
#define CMD_CODE_WRITE_DMA 0xCA
#define CMD_CODE_READ_SECTOR_S 0x20


#define CMD_CODE_READ_FPDMA_QUEUED                       0x060
#define CMD_CODE_WRITE_FPDMA_QUEUED                      0x061
////command header[0]
#define PRDTL(value) ((value&0xffff)<<16)
#define PMP(value) ((value&0xF)<<12)
#define C_BUSY     (0x1<<10)
#define B_BIST     (0x1<<9)
#define R_Reset    (0x1<<8)
#define P_Prefet   (0x1<<7)
#define W_Write    (0x1<<6)
#define A_ATAPI    (0x1<<5)
#define CFL(value) (value&0x1F)

////Register FIS - Host to Device 0x27
#define Features_H2D(value) ((value&0xff)<<24)
#define Command_H2D(value) ((value&0xff)<<16)
#define C_H2D              (0x1<<15)
#define FisType(value)            (value)

#define DevHead(value) ((value&0xff)<<24)
#define CylHigh(value) ((value&0xff)<<16)
#define CylLow(value)  ((value&0xff)<<8)
#define SectorNumber(value) (value&0xff)

#define FeaturesExp(value) ((value&0xff)<<24)
#define CylHighExp(value)  ((value&0xff)<<16)
#define CylLowExp(value)   ((value&0xff)<<8)
#define SectorNumExp(value) (value&0xff)

#define Control_H2D(value)  ((value&0xff)<<24)
#define SectorCountExp(value) ((value&0xff)<<8)
#define SectorCount(value)  (value&0xff)
////////DCR Base Address////////////////////////////
#define SATA0_TEST                         1
#define SATA1_TEST                         0
#if SATA0_TEST 
#define AHCI_DCR_BASE             	       0xe9550000    
#endif
#if SATA1_TEST 
#define AHCI_DCR_BASE             	       0xe9560000    
#endif

#define AHCI_EXTEND_DCR_BASE               0x00210000
////////////////Generic Host Register Map//////////////////////
/*             device power management state                */
#define IDLE_IMMEDIATE                 0xe1
#define IDLE                           0xe3
#define STANDBY                        0xe2
#define STANDBY_IMMEDIATE              0xe0
#define SLEEP                          0xe6
#define DEVICE_RESET                   0x08
#define ATB_GCSR_CAP           AHCI_DCR_BASE|0x00 
#define S64A           (0x1<<31)
#define SNCQ           (0x1<<30)
#define SSNTF          (0x1<<29)
#define SMPS           (0x1<<28)
#define SSS            (0x1<<27)
#define SALP           (0x1<<26)
#define SAL            (0x1<<25)
#define SCLO           (0x1<<24)
#define ISS            (0xF<<20)
#define SNZO           (0x1<<19)
#define SAM            (0x1<<18)
#define SPM            (0x1<<17)
#define PMD            (0x1<<15)
#define SSC            (0x1<<14)
#define PSC            (0x1<<13)
#define NCS            (0x1F<<8)
#define CCCS           (0x1<<7)
#define EMS            (0x1<<6)
#define SXS            (0x1<<5)
#define NP             (0x1F)

#define ATB_GCSR_GHC           AHCI_DCR_BASE|0x04
#define AE_GHC             (0x1<<31)
#define IE_GHC             (0x1<<1)
#define HR_GHC             (0x1)

#define ATB_GCSR_IS            AHCI_DCR_BASE|0x08
#define IPS_P1          (0x1<<1)
#define IPS_P0          (0x1)
  
#define ATB_GCSR_PI            AHCI_DCR_BASE|0x0C
#define P1_PI           (0x1<<1)
#define P0_PI           (0x1)

#define VS            AHCI_DCR_BASE|0x10
#define MJR_VS             (0xFFFF<<16)
#define MNR_VS             (0xFFFF)

#define CCC_CTL       AHCI_DCR_BASE|0x14
#define TV_CCC_CTL         (0xFFFF<<16)
#define CC_CCC_CTL         (0xFF<<8)
#define INT_CCC_CTL        (0x1F<<3)
#define EN_CCC_CTL         (0x1)

#define CCC_PORTS     AHCI_DCR_BASE|0x18

#define CAP2          AHCI_DCR_BASE|0x24

#define BISTAFR       AHCI_DCR_BASE|0xA0
#define NCP_BISTAFR        (0xFF<<8)
#define PD_BISTAFR         (0xFF)

#define ATB_GCSR_BISTCR        AHCI_DCR_BASE|0xA4
#define LATE_PHY_READY_BISTCR (0x1<<24)
#define FERLB_BISTCR       (0x1<<20)
#define TXO_BISTCR         (0x1<<18)
#define CNTCLR_BISTCR      (0x1<<17)
#define NEALB_BISTCR       (0x1<<16)
#define LLB_BISTCR         (0x1<<15)
#define QPHYINIT_BISTCR    (0x1<<14)
#define ERRLOSSEN_BISTCR   (0x1<<13)
#define SDFE_BISTCR        (0x1<<12)
#define PRD_LLC_BISTCR     (0x1<<10)
#define DESCRAM_LLC_BISTCR (0x1<<9)
#define SCRAM_LLC_BISTCR   (0x1<<8)
#define ERREN_BISTCR       (0x1<<6)
#define FLIP_BISTCR        (0x1<<5)
#define PV_BISTCR          (0x1<<4)
#define PATTERN_BISTCR     (0xF)

#define BISTFCTR      AHCI_DCR_BASE|0xA8

#define BISTSR        AHCI_DCR_BASE|0xAC
#define BRSTERR_BISTSR     (0xFF<<16)
#define FRAMERR_BISTSR     (0xFFFF)

#define BISTDECR      AHCI_DCR_BASE|0xB0

#define ATB_GCSR_OOBR          AHCI_DCR_BASE|0xBC
#define WE_OOBR            (0x1<<31)
#define cwMin_OOBR         (0x7F<<24)
#define cwMax_OOBR         (0xFF<<16)
#define ciMin_OOBR         (0xFF<<8)
#define ciMax_OOBR         (0xFF)

#define GPCR          AHCI_DCR_BASE|0xD0

#define GPSR          AHCI_DCR_BASE|0xD4

#define ENDIAN          AHCI_DCR_BASE|0xD8

#define TIMER1MS      AHCI_DCR_BASE|0xE0

#define GPARAM1R      AHCI_DCR_BASE|0xE8
#define ALGN_M_GPARM1R     (0x1<<31)
#define RX_BUFFER_GPARM1R  (0x1<<30)
#define PHY_DATA_GPARM1R   (0x3<<28)
#define PHY_RST_GPARM1R    (0x1<<27)
#define PHY_CTRL_GPARM1R   (0x3F<<21)
#define PHY_STAT_GPARM1R   (0x3F<<15)
#define LATCH_M_GPARM1R    (0x1<<14)
#define BIST_M_GPARM1R     (0x1<<13)
#define PHY_TYPE_GPARM1R   (0x3<<11)
#define RETURN_ERR_GPARM1R (0x1<<10)
#define AHB_ENDIAN_GPARM1R (0x3<<8)
#define S_HADDR_GPARM1R    (0x1<<7)
#define M_HADDR_GPARM1R    (0x1<<6)

#define GPARAM2R      AHCI_DCR_BASE|0xEC
#define DEV_CP_GPARM2R     (0x1<<14)
#define DEV_MP_GPARM2R     (0x1<<13)
#define ENCODE_M_GPARM2R   (0x1<<12)
#define RXOOB_CLK_M_GPARM2R (0x1<<11)
#define RX_OOB_M_GPARM2R    (0x1<<10)
#define TX_OOB_M_GPARM2R    (0x1<<9)
#define RXOOB_CLK_GPARM2R   (0x1FF)

#define PPARAMR       AHCI_DCR_BASE|0xF0
#define TX_MEM_M_PPARAMR   (0x1<<11)
#define TX_MEM_S_PPARAMR   (0x1<<10)
#define RX_MEM_M_PPARAMR   (0x1<<9)
#define RX_MEM_S_PPARAMR   (0x1<<8)
#define TXFIFO_DEPTH_PPARAMR (0xF<<4)
#define RXFIFO_DEPTH_PPARAMR (0xF)

#define ATB_GCSR_TESTR         AHCI_DCR_BASE|0xF4
#define PSEL_TESTR         (0x7<<16)
#define TEST_IF_TESTR      (0x1)

#define VERSIONR      AHCI_DCR_BASE|0xF8

#define IDR          AHCI_DCR_BASE|0xFC

/////////////////////PORT 0/1 Register//////////////////
#define ATB_PCSR_P0CLB        AHCI_DCR_BASE|0x100
#define CLB_P0CLB         (0x3F_FFFF<<10)

#define ATB_PCSR_P1CLB        AHCI_DCR_BASE|0x180
#define CLB_P1CLB         (0x3F_FFFF<<10)

#define ATB_PCSR_P0CLBU       AHCI_DCR_BASE|0x104

#define ATB_PCSR_P1CLBU       AHCI_DCR_BASE|0x184

#define ATB_PCSR_P0FB         AHCI_DCR_BASE|0x108
#define FB_P0FB           (0xFF_FFFF<<8)

#define ATB_PCSR_P1FB         AHCI_DCR_BASE|0x188
#define FB_P1FB           (0xFF_FFFF<<8)

#define ATB_PCSR_P0FBU        AHCI_DCR_BASE|0x10C

#define ATB_PCSR_P1FBU        AHCI_DCR_BASE|0x18C

#define ATB_PCSR_P0IS         AHCI_DCR_BASE|0x110
#define CPDS_P0IS         (0x1<<31)
#define TFES_P0IS         (0x1<<30)
#define HBFS_P0IS         (0x1<<29)
#define HBDS_P0IS         (0x1<<28)
#define IFS_P0IS          (0x1<<27)
#define INFS_P0IS         (0x1<<26)
#define OFS_P0IS          (0x1<<24)
#define IPMS_P0IS         (0x1<<23)
#define PRCS_P0IS         (0x1<<22)
#define DMPS_P0IS         (0x1<<7)
#define PCS_P0IS          (0x1<<6)
#define DPS_P0IS          (0x1<<5)
#define UFS_P0IS          (0x1<<4)
#define SDBS_P0IS         (0x1<<3)
#define DSS_P0IS          (0x1<<2)
#define PSS_P0IS          (0x1<<1)
#define DHRS_P0IS         (0x1)

#define ATB_PCSR_P1IS         AHCI_DCR_BASE|0x190
#define CPDS_P1IS         (0x1<<31)
#define TFES_P1IS         (0x1<<30)
#define HBFS_P1IS         (0x1<<29)
#define HBDS_P1IS         (0x1<<28)
#define IFS_P1IS          (0x1<<27)
#define INFS_P1IS         (0x1<<26)
#define OFS_P1IS          (0x1<<24)
#define IPMS_P1IS         (0x1<<23)
#define PRCS_P1IS         (0x1<<22)
#define DMPS_P1IS         (0x1<<7)
#define PCS_P1IS          (0x1<<6)
#define DPS_P1IS          (0x1<<5)
#define UFS_P1IS          (0x1<<4)
#define SDBS_P1IS         (0x1<<3)
#define DSS_P1IS          (0x1<<2)
#define PSS_P1IS          (0x1<<1)
#define DHRS_P1IS         (0x1)

#define ATB_PCSR_P0IE         AHCI_DCR_BASE|0x114
#define CPDE_P0IE         (0x1<<31)
#define TFEE_P0IE         (0x1<<30)
#define HBFE_P0IE         (0x1<<29)
#define HBDE_P0IE         (0x1<<28)
#define IFE_P0IE          (0x1<<27)
#define INFE_P0IE         (0x1<<26)
#define OFE_P0IE          (0x1<<24)
#define IPME_P0IE         (0x1<<23)
#define PRCE_P0IE         (0x1<<22)
#define DMPE_P0IE         (0x1<<7)
#define PCE_P0IE          (0x1<<6)
#define DPE_P0IE          (0x1<<5)
#define UFE_P0IE          (0x1<<4)
#define SDBE_P0IE         (0x1<<3)
#define DSE_P0IE          (0x1<<2)
#define PSE_P0IE          (0x1<<1)
#define DHRE_P0IE         (0x1)

#define ATB_PCSR_P1IE         AHCI_DCR_BASE|0x194
#define CPDE_P1IE         (0x1<<31)
#define TFEE_P1IE         (0x1<<30)
#define HBFE_P1IE         (0x1<<29)
#define HBDE_P1IE         (0x1<<28)
#define IFE_P1IE          (0x1<<27)
#define INFE_P1IE         (0x1<<26)
#define OFE_P1IE          (0x1<<24)
#define IPME_P1IE         (0x1<<23)
#define PRCE_P1IE         (0x1<<22)
#define DMPE_P1IE         (0x1<<7)
#define PCE_P1IE          (0x1<<6)
#define DPE_P1IE          (0x1<<5)
#define UFE_P1IE          (0x1<<4)
#define SDBE_P1IE         (0x1<<3)
#define DSE_P1IE          (0x1<<2)
#define PSE_P1IE          (0x1<<1)
#define DHRE_P1IE         (0x1)

#define ATB_PCSR_P0CMD        AHCI_DCR_BASE|0x118
#define ICC_P0CMD         (0xF<<28)
#define PM_ACTIVE         (0x1)
#define PM_PARTIAL        (0x2)
#define PM_SLUMBER        (0x6)
#define ASP_P0CMD         (0x1<<27)
#define ALPE_P0CMD        (0x1<<26)
#define DLAE_P0CMD        (0x1<<25)
#define ATAPI_P0CMD       (0x1<<24)
#define APSTE_P0CMD       (0x1<<23)
#define ESP_P0CMD         (0x1<<21)
#define CPD_P0CMD         (0x1<<20)
#define MPSP_P0CMD        (0x1<<19)
#define HPCP_P0CMD        (0x1<<18)
#define PMA_P0CMD         (0x1<<17)
#define CPS_P0CMD         (0x1<<16)
#define CR_P0CMD          (0x1<<15)
#define FR_P0CMD          (0x1<<14)
#define MPSS_P0CMD        (0x1<<13)
#define CCS_P0CMD         (0x1F<<8)
#define FRE_P0CMD         (0x1<<4)
#define CLO_P0CMD         (0x1<<3)
#define POD_P0CMD         (0x1<<2)
#define SUD_P0CMD         (0x1<<1)
#define ST_P0CMD          (0x1)

#define ATB_PCSR_P1CMD        AHCI_DCR_BASE|0x198
#define ICC_P1CMD         (0xF<<28)
#define ASP_P1CMD         (0x1<<27)
#define ALPE_P1CMD        (0x1<<26)
#define DLAE_P1CMD        (0x1<<25)
#define ATAPI_P1CMD       (0x1<<24)
#define APSTE_P1CMD       (0x1<<23)
#define ESP_P1CMD         (0x1<<21)
#define CPD_P1CMD         (0x1<<20)
#define MPSP_P1CMD        (0x1<<19)
#define HPCP_P1CMD        (0x1<<18)
#define PMA_P1CMD         (0x1<<17)
#define CPS_P1CMD         (0x1<<16)
#define CR_P1CMD          (0x1<<15)
#define FR_P1CMD          (0x1<<14)
#define MPSS_P1CMD        (0x1<<13)
#define CCS_P1CMD         (0x1F<<8)
#define FRE_P1CMD         (0x1<<4)
#define CLO_P1CMD         (0x1<<3)
#define POD_P1CMD         (0x1<<2)
#define SUD_P1CMD         (0x1<<1)
#define ST_P1CMD          (0x1)

#define ATB_PCSR_P0TFD        AHCI_DCR_BASE|0x120
#define ERR_P0TFD         (0xFF<<8)
#define BSY_TFD           (0x1<<7)
#define DRQ_TFD           (0x1<<3)
#define ERR_TFD           0x1

#define ATB_PCSR_P1TFD        AHCI_DCR_BASE|0x1A0
#define ERR_P1TFD         (0xFF<<8)
#define STS_P1TFD         (0xFF)

#define ATB_PCSR_P0SIG        AHCI_DCR_BASE|0x124

#define ATB_PCSR_P1SIG        AHCI_DCR_BASE|0x1A4

#define ATB_PCSR_P0SSTS       AHCI_DCR_BASE|0x128
#define IPM_P0SSTS        (0xF<<8)
#define SPD_P0SSTS        (0xF<<4)
#define DET_P0SSTS        (0xF)

#define ATB_PCSR_P1SSTS       AHCI_DCR_BASE|0x1A8
#define IPM_P1SSTS        (0xF<<8)
#define SPD_P1SSTS        (0xF<<4)
#define DET_P1SSTS        (0xF)

#define ATB_PCSR_P0SCTL       AHCI_DCR_BASE|0x12C
#define IPM_P0SCTL        (0xF<<8)
#define SPD_P0SCTL        (0xF<<4)
#define DET_P0SCTL        (0xF)

#define ATB_PCSR_P1SCTL       AHCI_DCR_BASE|0x1AC
#define IPM_P1SCTL        (0xF<<8)
#define SPD_P1SCTL        (0xF<<4)
#define DET_P1SCTL        (0xF)

#define ATB_PCSR_P0SERR       AHCI_DCR_BASE|0x130
#define DIAG_X_P0SERR     (0x1<<26)
#define DIAG_F_P0SERR     (0x1<<25)
#define DIAG_T_P0SERR     (0x1<<24)
#define DIAG_S_P0SERR     (0x1<<23)
#define DIAG_H_P0SERR     (0x1<<22)
#define DIAG_C_P0SERR     (0x1<<21)
#define DIAG_D_P0SERR     (0x1<<20)
#define DIAG_B_P0SERR     (0x1<<19)
#define DIAG_W_P0SERR     (0x1<<18)
#define DIAG_I_P0SERR     (0x1<<17)
#define DIAG_N_P0SERR     (0x1<<16)
#define ERR_E_P0SERR     (0x1<<11)
#define ERR_P_P0SERR     (0x1<<10)
#define ERR_C_P0SERR     (0x1<<9)
#define ERR_T_P0SERR     (0x1<<8)
#define ERR_M_P0SERR     (0x1<<1)
#define ERR_I_P0SERR     (0x1)

#define ATB_PCSR_P1SERR       AHCI_DCR_BASE|0x1B0
#define DIAG_X_P1SERR     (0x1<<26)
#define DIAG_F_P1SERR     (0x1<<25)
#define DIAG_T_P1SERR     (0x1<<24)
#define DIAG_S_P1SERR     (0x1<<23)
#define DIAG_H_P1SERR     (0x1<<22)
#define DIAG_C_P1SERR     (0x1<<21)
#define DIAG_D_P1SERR     (0x1<<20)
#define DIAG_B_P1SERR     (0x1<<19)
#define DIAG_W_P1SERR     (0x1<<18)
#define DIAG_I_P1SERR     (0x1<<17)
#define DIAG_N_P1SERR     (0x1<<16)
#define ERR_E_P1SERR     (0x1<<11)
#define ERR_P_P1SERR     (0x1<<10)
#define ERR_C_P1SERR     (0x1<<9)
#define ERR_T_P1SERR     (0x1<<8)
#define ERR_M_P1SERR     (0x1<<1)
#define ERR_I_P1SERR     (0x1)

#define ATB_PCSR_P0SACT     AHCI_DCR_BASE|0x134

#define ATB_PCSR_P1SACT     AHCI_DCR_BASE|0x1B4

#define ATB_PCSR_P0CI       AHCI_DCR_BASE|0x138

#define ATB_PCSR_P1CI       AHCI_DCR_BASE|0x1B8

#define ATB_PCSR_P2CI       AHCI_DCR_BASE|0x238
#define ATB_PCSR_P3CI       AHCI_DCR_BASE|0x2B8
#define ATB_PCSR_P4CI       AHCI_DCR_BASE|0x338
#define ATB_PCSR_P5CI       AHCI_DCR_BASE|0x3B8
#define ATB_PCSR_P6CI       AHCI_DCR_BASE|0x438
#define ATB_PCSR_P7CI       AHCI_DCR_BASE|0x4B8

#define ATB_PCSR_P0SNTF     AHCI_DCR_BASE|0x13C
#define PMN_P0SNTF      (0xFFFF<<16)

#define ATB_PCSR_P1SNTF     AHCI_DCR_BASE|0x1BC
#define PMN_P1SNTF      (0xFFFF<<16)

#define ATB_PCSR_P0FBS      AHCI_DCR_BASE|0x140
#define ATB_PCSR_P0DEVSLP   AHCI_DCR_BASE|0x144

#define ATB_PCSR_P0DMACR    AHCI_DCR_BASE|0x170
#define RXTS_P0DMACR    (0xF<<4)
#define TXTS_P0DMACR    (0xF)

#define ATB_PCSR_P1DMACR    AHCI_DCR_BASE|0x1F0
#define RXTS_P1DMACR    (0xF<<4)
#define TXTS_P1DMACR    (0xF)

#define ATB_PCSR_P0PHYCR    AHCI_DCR_BASE|0x178
#define ATB_PCSR_P1PHYCR    AHCI_DCR_BASE|0x1F8

#define ATB_PCSR_P0PHYSR    AHCI_DCR_BASE|0x17C
#define ATB_PCSR_P1PHYSR    AHCI_DCR_BASE|0x1FC

#define EXT_PHY_CFG0        AHCI_EXTEND_DCR_BASE|0x0
#define EXT_PHY_CFG1        AHCI_EXTEND_DCR_BASE|0x4
#define EXT_PHY_CFG2        AHCI_EXTEND_DCR_BASE|0x8
#define EXT_UABUS           AHCI_EXTEND_DCR_BASE|0xC

// PRD: physical region descriptor
typedef struct {
  unsigned int dw0; //dba
  unsigned int dw1; //dbau
  unsigned int dw2; //rev
  unsigned int dw3; //i, dbc
} PhysicalRegionDescriptor;







