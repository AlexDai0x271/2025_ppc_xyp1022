#ifndef TD_H
#define TD_H

  #define TD_BASE_ADDR    0xEB004000
  #define TD_UP_ADDR     0x00000000

  #define TDCR1     (TD_BASE_ADDR + 0x00 )   //Average control 
  #define TDCR2     (TD_BASE_ADDR + 0x04 )   //TD1 control
  #define TDTIR     (TD_BASE_ADDR + 0x08 )
  #define TDDR1     (TD_BASE_ADDR + 0x0C )   //Int flag data which is used to generate the flag 
  #define TDDR2     (TD_BASE_ADDR + 0x10 )   //Int flag data which is used to generate the flag 
  #define TDDR3     (TD_BASE_ADDR + 0x14 )   //Int flag data which is used to generate the flag 
  #define TDDR4     (TD_BASE_ADDR + 0x18 )   //Int flag data which is used to generate the flag 
  #define TDDR5     (TD_BASE_ADDR + 0x1C )   //Int flag data which is used to generate the flag 
  #define TDDR6     (TD_BASE_ADDR + 0x20 )   //Int flag data which is used to generate the flag 
  #define TDDR7     (TD_BASE_ADDR + 0x24 )   //Int flag data which is used to generate the flag 
  #define TDDR8     (TD_BASE_ADDR + 0x28 )   //Int flag data which is used to generate the flag 
  #define TDIFR     (TD_BASE_ADDR + 0x2C )   //Int flag data which is used to generate the flag 
  #define TDIER     (TD_BASE_ADDR + 0x30 )   //Int flag data which is used to generate the flag 
  #define TDEVTR    (TD_BASE_ADDR + 0x34 )   //Int flag data which is used to generate the flag 
  #define TDTESTCR1 (TD_BASE_ADDR + 0x38 )   //Int flag data which is used to generate the flag 
  #define TDTESTCR2 (TD_BASE_ADDR + 0x3C )   //Int flag data which is used to generate the flag 
  #define TDIR_SAMPLE    (TD_BASE_ADDR + 0x08 )   // T2
  #define TDIR_SHIFT     (TD_BASE_ADDR + 0x40 )   // T3
  #define TDIR_UPDATE    (TD_BASE_ADDR + 0x44 )   // T4
 
  #define TD_USERLV  3
  #define TD_TDEVT  0x500
  #define TD_TDTIR  0xf0
  #define TD_TDSIR  0x10

  #define TDCR1_MASK           0xFC00FF7F   //
  #define TDCR2_MASK           0x0000FF7F
  #define TDTIR_MASK           0xFFFFFFFF
  #define TDDR1_MASK           0x00000000    //Not writeable 
  #define TDDR2_MASK           0x00000000    //Not writeable 
  #define TDDR3_MASK           0x00000000    //Not writeable 
  #define TDDR4_MASK           0x00000000    //Not writeable 
  #define TDDR5_MASK           0x00000000    //Not writeable 
  #define TDDR6_MASK           0x00000000    //Not writeable 
  #define TDDR7_MASK           0x00000000    //Not writeable 
  #define TDDR8_MASK           0x00000000    //Not writeable 
  #define TDIFR_MASK           0x00000000    
  #define TDIER_MASK           0xEE00C0F1
  #define TDEVTR_MASK          0xFFFFFFFF
  #define TDTESTCR1_MASK       0xffffffff     
  #define TDTESTCR1_MASK       0xffffffff     

// TDIFR
  #define TDHIGH_OUT          (1<<6)   // (1<<5)
  #define TDLOW_OUT           (1<<5)   // (1<<4)
  #define HT_INT_OUT          (1<<4)   // (1<<31)     //Average temperature
  #define LT_INT_OUT          (1<<3)   // (1<<30)     //Average temperature
  #define SP_INT_OUT          (1<<2)   // (1<<29)     //Average temperature  
  #define ERR_FLAG_OUT        (1<<1)   // (1<<15)
  #define DONE_INT            (1<<0) 

// TDIER
  #define TDHIGH_INT_IE        (1<<6)
  #define TDLOW_INT_IE         (1<<5)
  #define HT_INT_IE            (1<<4)  // (1<<31)
  #define LT_INT_IE            (1<<3)  // (1<<30)
  #define SP_INT_IE            (1<<2)  // (1<<29)
  #define EFLAG_INT_IE         (1<<1)  // (1<<15)
  #define DONE_INT_IE          (1<<0)


  //TDCR0
  #define P_BG_DIS             (1<<31)
  #define P_TD_EN              (1<<30)
  #define P_LT_HYS1            (1<<28)
  #define P_HT_HYS1            (1<<29)
  #define P_MODE(n)            (n<<26)
  #define P_VT_ADJ(n)          (n<<12)
  #define P_CTAT_ADJ(n)        (n<<8)
  #define P_VREF_ADJ(n)        (n<<0)

  //TDCR1
  #define P_LOW_TH(n)       (n<<0)
  #define P_HIGH_TH(n)      (n<<8)

  #define DABORT_EXP_NUM     127

#define EXIT_DABORT \
do {         \
            UINT32 rdata;   \
            ISYNC;  \
            MFSPR(rdata, SRR0); \
            rdata += 0x4 ; \
            MTSPR(SRR0, rdata); \
            ISYNC;  \
} while (0)

#endif
