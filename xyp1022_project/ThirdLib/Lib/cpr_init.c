#include "environment.h"

#define  SEL_SYS_CLK_CFG_STATUS    0xFFFFFFFF

void wait_busy()
{
   unsigned int  sys_data;
   do{
      LDW(sys_data, SYS_CLK_CFG_STATUS);
      sys_data = sys_data & SEL_SYS_CLK_CFG_STATUS;
   }while (sys_data);    // if sys_data = 0x0, exit the while!!!!
}


// aes_clk = pll3_clk / aes_clk_cfg[11:4]+1
void  aes_clk_cfg(unsigned int freq)
{
   unsigned int div_aes_clk, read_data;
   div_aes_clk = 1200/freq - 1;
   div_aes_clk = div_aes_clk << 4;
   STW(div_aes_clk, AES_CLK_CFG);
   do{
   LDW(read_data, AES_CLK_CFG);
   } while (read_data != div_aes_clk);

   wait_busy();
}


// des_clk = pll3_clk / des_clk_cfg[11:4]+1
void  des_clk_cfg(unsigned int freq)
{
   unsigned int div_des_clk, read_data;
   div_des_clk = 1200/freq - 1;
   div_des_clk = div_des_clk << 4;
   STW(div_des_clk, DES_CLK_CFG);
   do{
   LDW(read_data, DES_CLK_CFG);
   } while (read_data != div_des_clk);

   wait_busy();
}


// sm4_clk = pll3_clk / sm4_clk_cfg[11:4]+1
void  sm4_clk_cfg(unsigned int freq)
{
   unsigned int div_sm4_clk, read_data;
   div_sm4_clk = 1200/freq - 1;
   div_sm4_clk = div_sm4_clk << 4;
   STW(div_sm4_clk, SM4_CLK_CFG);
   do{
   LDW(read_data, SM4_CLK_CFG);
   } while (read_data != div_sm4_clk);

   wait_busy();
}


// ip1_clk = pll3_clk / ip1_clk_cfg[11:4]+1
void  ip1_clk_cfg(unsigned int freq)
{
   unsigned int div_ip1_clk, read_data;
   div_ip1_clk = 1200/freq - 1;
   div_ip1_clk = div_ip1_clk << 4;
   STW(div_ip1_clk, IP1_CLK_CFG);
   do{
   LDW(read_data, IP1_CLK_CFG);
   } while (read_data != div_ip1_clk);

   wait_busy();
}


// ip3_clk = pll3_clk / ip3_clk_cfg[11:4]+1
void  ip3_clk_cfg(unsigned int freq)
{
   unsigned int div_ip3_clk, read_data;
   div_ip3_clk = 1200/freq - 1;
   div_ip3_clk = div_ip3_clk << 4;
   STW(div_ip3_clk, IP3_CLK_CFG);
   do{
   LDW(read_data, IP3_CLK_CFG);
   } while (read_data != div_ip3_clk);

   wait_busy();
}


// mdha_clk = pll3_clk / mdha_clk_cfg[11:4]+1
void  mdha_clk_cfg(unsigned int freq)
{
   unsigned int div_mdha_clk, read_data;
   div_mdha_clk = 1200/freq - 1;
   div_mdha_clk = div_mdha_clk << 4;
   STW(div_mdha_clk, MDHA_CLK_CFG);
   do{
   LDW(read_data, MDHA_CLK_CFG);
   } while (read_data != div_mdha_clk);

   wait_busy();
}


// rscp_clk = pll3_clk / rscp_clk_cfg[11:4]+1
void  rscp_clk_cfg(unsigned int freq)
{
   unsigned int div_rscp_clk, read_data;
   div_rscp_clk = 1200/freq - 1;
   div_rscp_clk = div_rscp_clk << 4;
   STW(div_rscp_clk, RSCP_CLK_CFG);
   do{
   LDW(read_data, RSCP_CLK_CFG);
   } while (read_data != div_rscp_clk);

   wait_busy();
}


// rng_clk = pll3_clk / rng_clk_cfg[11:4]+1
void  rng_clk_cfg(unsigned int freq)
{
   unsigned int div_rng_clk, read_data;
   div_rng_clk = 1200/freq - 1;
   div_rng_clk = div_rng_clk << 4;
   STW(div_rng_clk, RNG_CLK_CFG);
   do{
   LDW(read_data, RNG_CLK_CFG);
   } while (read_data != div_rng_clk);

   wait_busy();
}


// pkha_clk = pll3_clk / pkha_clk_cfg[11:4]+1
void  pkha_clk_cfg(unsigned int freq)
{
   unsigned int div_pkha_clk, read_data;
   div_pkha_clk = 1200/freq - 1;
   div_pkha_clk = div_pkha_clk << 4;
   STW(div_pkha_clk, PKHA_CLK_CFG);
   do{
   LDW(read_data, PKHA_CLK_CFG);
   } while (read_data != div_pkha_clk);

   wait_busy();
}


void cpr_init(void) {
volatile unsigned int  plb6_value;
volatile unsigned int  dcr_value;

volatile unsigned char strap_cfg_sys_clk_freq;
volatile unsigned char strap_core_speed      ;
volatile unsigned char strap_ddr_speed       ;
volatile unsigned char strap_bus_freq       ;
// Look Up Strap Pin
//        strap_cfg_sys_clk_freq = strap_pins_input[31];
//        strap_efuse_clk_sel    = strap_pins_input[30];
//        strap_core_speed       = strap_pins_input[29:28];
//        strap_ddr_speed        = strap_pins_input[27:25];
//        strap_bus_clk_freq     = strap_pins_input[24:23];
//        strap_cfg_rom_loc      = strap_pins_input[22];
//        strap_cfg_sb_dis       = strap_pins_input[21];
//        strap_pcie0_type       = strap_pins_input[20];
//        strap_pcie1_type       = strap_pins_input[19];
//        strap_pcie2_type       = strap_pins_input[18];
//        strap_pcie3_type       = strap_pins_input[17];
//        strap_cpu_en           = strap_pins_input[16:14];
//        strap_perst_n_mode     = strap_pins_input[13];
//        strap_halt_cfg_b       = strap_pins_input[8];
//        strap_ephy7_intf_sel   = strap_pins_input[7];
//        strap_ephy6_intf_sel   = strap_pins_input[6];
//        strap_ephy5_intf_sel   = strap_pins_input[5];
//        strap_ephy4_intf_sel   = strap_pins_input[4];
//        strap_ephy3_intf_sel   = strap_pins_input[3];
//        strap_ephy2_intf_sel   = strap_pins_input[2];
//        strap_ephy1_intf_sel   = strap_pins_input[1];
//        strap_ephy0_intf_sel   = strap_pins_input[0];
LDW(dcr_value, STRAP_PIN); //CCM Strap Pin register
NOP10;
strap_core_speed       = (dcr_value & (3<<28)) >> 28;
NOP10;
strap_bus_freq         = (dcr_value & (3<<23)) >> 23;
NOP10;
strap_ddr_speed        = (dcr_value & (7<<25)) >> 25;
NOP10;
strap_cfg_sys_clk_freq = 0x0;      // Remove strap_cfg_sys_clk_freq in strap_pins. XCLK is tied to 12MHz.

//====================================================================
// Switch Clock Source from PLL to XCLK if CORE-RESET happens.
STW(0x0, CLK_SRC_SEL);


//                    M              1
// PLL_CLK = XCLK * ----- * ------------------
//                    N      ODDIV1 * ODDIV2
// ODDV1 = 2^(ODDV1[0]+2XODDV1[1])
// ODDV2 = 2^(ODDV2[0]+2XODDV2[1])
//
// 1. XCLK / N = 1 ~ 40MHz
// 2. 1.6G <= XCLK * M / N <=  3.2GHz
// 3. N>= 1; M>= 2;
//                    M / PLL_CFG[7:0]                            1
// PLL_CLK = XCLK * --------------------- * ----------------------------------------------------------------
//                    N / PLL_CFG[13:8]     (2^ODDIV1[1:0]/PLL_CFG[15:14]) * (2^ODDIV2[1:0]/PLL_CFG[17:16])
//====================================================================
// CFG PLL5 : CPU_CLK
//====================================================================
//       250/0xFA      1                   125/0x7D       1
// 12 * ---------- * ----- = 1.5GHz;  48 * ---------- * ----- = 1.5GHz ; 0x041FA ; 0x0427D
//        1            2                    2             2
//
//       221/0xDD      1                   221/0xDD       1
// 12 * ---------- * ----- = 1.33GHz; 48 *----------- * ----- = 1.33GHz; 0x041DD ; 0x044DD
//        1            2                    4             2
//
//       200/0xC8      1                   100/0x64       1
// 12 * ---------- * ----- = 1.2GHz;  48 * ---------- * ----- = 1.2GHz ; 0x041C8 ; 0x04264
//        1            2                    2             2
//
//       167/0xA7      1                   125/0x7D       1
// 12 * ---------- * ----- = 1.0GHz;  48 * ---------- * ----- = 1.0GHz ; 0x041A7 ; 0x0437D
//        1            2                    3             2
if (strap_cfg_sys_clk_freq == 0x1)
{ //48MHz
        if (strap_core_speed == 0x3) { //1.0GHz
            plb6_value = 0x0437D;
            }
        else if(strap_core_speed == 0x2) { // 1.2GHz
            plb6_value = 0x04264;
            }
        else if(strap_core_speed == 0x1) { // 1.33GHz
            plb6_value = 0x044DD;
            }
         else { //0.6GHz
           plb6_value = 0x08264;		//edit by zjjin
            }
} else { //12MHz
        if (strap_core_speed == 0x3) { //1.0GHz
            plb6_value = 0x041A7;
            }
        else if(strap_core_speed == 0x2) { // 1.2GHz
            plb6_value = 0x041C8;
            }
        else if(strap_core_speed == 0x1) { // 1.33GHz
            plb6_value = 0x041DD;
            }
        else {
//          plb6_value = 0x081C8;   	//edit by zjjin,	0.6GHz
          plb6_value = 0x041FA;		//edit by sytu,		1.5GHz
//            plb6_value = 0x416F; 		// CORE 1330MHz
             }
}

//plb6_value = 0x041FA;                   //edit by sytu,		1.5GHz
//plb6_value = 0x041C8;                   //edit by sytu,		1.2GHz
//NOP10;
//STW(plb6_value, PLL5_CFG);

//====================================================================
// CFG PLL1 : BUS_CLK
//====================================================================
    if (strap_cfg_sys_clk_freq == 0x1) { //48MHz
      plb6_value = 0x04264;  //1.2GHz
    } else {  // 12MHz
      plb6_value = 0x041C8;  //1.2GHz
    }
 STW(plb6_value, PLL1_CFG);


//====================================================================
// CFG PLL2
//====================================================================
//       177/0xB1      1                   177/0xB1     1
// 12 * --------- * ----- = 265.5MHz; 48 * --------- * ----- = 265.5MHz  0x0C1B1; 0x0C4B1
//        1            8                    4           8
//
//       255/0xff      1                   100/0x64     1
// 12 * --------- * ----- = 382.5MHz; 48 * ----------- * ----- = 400MHz  0x0C1FF; 0x08364
//        1            8                    3           4
//
//       177/0xB1      1                   177/0xB1     1
// 12 * --------- * ----- = 531MHz;   48 * ----------- * ----- = 531MHz  0x081B1; 0x084B1
//        1            4                    4           4
//
//       222/0xDE      1                   222/0xDE     1
// 12 * --------- * ----- = 666MHz; 48 * ----------- * ----- = 666MHz  0x081DE; 0x084DE
//        1            4                    4           4
//
//       255/0xFF      1                   100/0x64     1
// 12 * --------- * ----- = 765MHz;   48 * ----------- * ----- = 800MHz  0x081FF; 0x04364
//        1            4                    3           2
//
//       155/0x9B      1                   155/0x9B     1
// 12 * --------- * ----- = 930MHz;   48 * ----------- * ----- = 930MHz  0x0419B; 0x0449B
//        1            2                    4           2
//
//       177/0xB1      1                   133/0x85     1
// 12 * --------- * ----- = 1062MHz;  48 * ---------- * ----- = 1064MHz 0x041B1; 0x04385
//        1            2                    3           2
//
//       200/0xC8      1                   100/0x64     1
// 12 * --------- * ----- = 1200MHz;  48 * ---------- * ----- = 1.2GHz  0x041C8; 0x04264;
//        1            2                    2           2
#if 1
    if (strap_cfg_sys_clk_freq == 0x1) { //48MHz
        switch (strap_ddr_speed) {
            case 0x7: plb6_value = 0x0C4B1; break; // DDR533 : 266MHz
            case 0x6: plb6_value = 0x08364; break; // DDR800 : 400MHz
            case 0x5: plb6_value = 0x084B1; break; // DDR1066: 533MHz
            case 0x4: plb6_value = 0x084DE; break; // DDR1333: 667MHz
            case 0x3: plb6_value = 0x04364; break; // DDR1600: 800MHz
            case 0x2: plb6_value = 0x0449C; break; // DDR1866: 933MHz
            case 0x1: plb6_value = 0x04386; break; // DDR2133: 1066MHz
            case 0x0: plb6_value = 0x04264; break; // DDR2400: 1200MHz
            default:  plb6_value = 0x04264; break; // DDR2400: 1200MHz
        }
    } else { //12MHz
 /*        switch (strap_ddr_speed) {
           case 0x7: plb6_value = 0x0C1B1; break; // DDR533 : 266MHz    -->>set to 265.5MHz for DDR2/DDR3      ,sim set more litter
            case 0x6: plb6_value = 0x0C1FF; break; // DDR800 : 400MHz    -->>set to 382.5MHz for DDR2/DDR3      ,sim set more litter
            case 0x5: plb6_value = 0x081B1; break; // DDR1066: 533MHz    -->>set to 531  MHz for DDR2/DDR3      ,sim set more litter
            case 0x4: plb6_value = 0x081DE; break; // DDR1333: 666.5MHz  -->>set to 666  MHz for DDR3           ,sim set more litter
            case 0x3: plb6_value = 0x081FF; break; // DDR1600: 800MHz    -->>set to 765  MHz for DDR3/DDR4      ,sim set more litter
            case 0x2: plb6_value = 0x0419B; break; // DDR1866: 933MHz    -->>set to 930  MHz for DDR4           ,sim set more litter
            case 0x1: plb6_value = 0x041B1; break; // DDR2133: 1066MHz   -->>set to 1062 MHz for DDR4           ,sim set more litter
            case 0x0: plb6_value = 0x041C8; break; // DDR2400: 1200MHz
            default:  plb6_value = 0x041C8; break; // DDR2400: 1200MHz
 */
    	plb6_value = 0x081FF; 						// DDR1600: 800MHz
//    	plb6_value = 0x081B1;						// DDR1066: 533MHz
//        plb6_value = 0x0C1FF; 						// DDR800 : 400MHz    -->>set to 382.5MHz for DDR2/DDR3      ,sim set more litter
//        }

    }
STW(plb6_value, PLL2_CFG);
#endif


//====================================================================
// CFG PLL3
//====================================================================
    if (strap_cfg_sys_clk_freq == 0x1) { //48MHz
      plb6_value = 0x04264;  //1.2GHz
    } else {  // 12MHz
      plb6_value = 0x041C8;  //1.2GHz
    }
 STW(plb6_value, PLL3_CFG);


//====================================================================
// CFG PLL4 / PLL6
//====================================================================
    if (strap_cfg_sys_clk_freq == 0x1) { //48MHz
        //plb6_value = 0x0437D; //1.0GHz
    	//plb6_value = 0x04364;	//765MHz
    	plb6_value = 0x04243;	//804MHz
    } else { //12MHz
        //plb6_value = 0x041A7; //1.0GHz
    	//plb6_value = 0x081FF; 	//765MHz
    	plb6_value = 0x04186; 	//804MHz
    }
 STW(plb6_value, PLL4_CFG);
 //STW(plb6_value, PLL6_CFG);
#if 0
 STW(5<<4, CPR_BASE_ADDR + 0x50);		//gmac clk cfg
 STW(5<<4, CPR_BASE_ADDR + 0x54);
 STW(5<<4, CPR_BASE_ADDR + 0x58);
 STW(5<<4, CPR_BASE_ADDR + 0x5C);
#else
 STW(7<<4, CPR_BASE_ADDR + 0x50);
 STW(7<<4, CPR_BASE_ADDR + 0x54);
 STW(7<<4, CPR_BASE_ADDR + 0x58);
 STW(7<<4, CPR_BASE_ADDR + 0x5C);
#endif
 //====================================================================
 // CFG PLL5
 //====================================================================
 //	if (strap_cfg_sys_clk_freq == 0x1 ) 
 	{
 		plb6_value = 0x41FA; // 1500MHz
 	//	plb6_value = 0x016F; // 1332MHz
	// 	 plb6_value = 0x0196; // 1800MHz  TIMER GPIO ok
	// 	 plb6_value = 0x01a6; // 2000MHz  TIMER GPIO ok
 	// plb6_value = 0x01AF; // 2100MHz  TIMER GPIO ERR
	 //	 plb6_value = 0x01B7; // 2200MHz  TIMER GPIO ERR
	// 	plb6_value = 0x0191; // 1750MHz	  TIMER GPIO
	//	plb6_value = 0x018d; // 1700MHz	  DRYSTONE
 	//	plb6_value = 0x416F; 		// CORE 1330MHz
 	}
 		
 	STW(plb6_value, PLL5_CFG);

  //====================================================================
 // CFG PLL6
 //====================================================================
     if (strap_cfg_sys_clk_freq == 0x1) { //48MHz
         plb6_value = 0x0437D; //1.0GHz
     	//plb6_value = 0x04364;	//765MHz
     	//plb6_value = 0x04243;	//804MHz
     } else { //12MHz
        plb6_value = 0x041A7; //1.0GHz
     	//plb6_value = 0x04186; 	//804MHz
     }

  STW(plb6_value, PLL6_CFG);

  STW(3, CPR_BASE_ADDR + 0xb4);		//gmac cfg
//====================================================================
// PowerOn PLL1 ~ PLL6
//====================================================================
plb6_value = 0x0;
STW(plb6_value, PLL_PD_BYPASS);

//Reset PLL1 ~ PLL6
plb6_value = 0x3F;
STW(plb6_value,PLL_SW_RST);

// Look Up PLL_STATUS
dcr_value = 0x0;
while (dcr_value != 0x3F)
{
    LDW(dcr_value,PLL_STATUS);
}


//====================================================================
// Config PLL1 Clock Domain
//====================================================================
//`define      CKD_BUS_ACLK_NDIV     3:0
//`define      CKD_ROM_CLK_NDIV      7:4
//`define      CKD_USB_CLK_NDIV     11:8
//`define      CKD_SEC_CLK_NDIV     15:12
//`define      CKD_GMAC_CLK_NDIV    19:16
//`define      CKD_EBC_HCLK_NDIV    25:20   -- fix at 0x1F -- 1200/32=37.5MHz
//`define      CKD_CPU_CLK_NDIV     31:28
//
//`define      CKD_DDR_CLK_NDIV      3:0
//`define      CKD_PCIE_CLK_NDIV     7:4
//`define      CKD_SATA_CLK_NDIV     11:8
//`define      CKD_LCDC_CLK_NDIV     15:12
//`define      CKD_XGMAC_CLK_NDIV    19:16
//`define      CKD_AURORA_CLK_NDIV   23:20
if(strap_bus_freq == 0)       // 1/2 core freq
{
plb6_value = 0x01F11111;
//dcr_value  = 0x00111111;
dcr_value  = 0x00011111;
}
else if (strap_bus_freq == 1) // 1/4 core freq
{
//plb6_value = 0x01F33333;
//dcr_value  = 0x00333333;

plb6_value = 0x01F44444;
dcr_value  = 0x00044444;
}
else if (strap_bus_freq == 2) // 1/6 core freq
{
plb6_value = 0x01F55555;
//dcr_value  = 0x00555555;
dcr_value  = 0x00055555;
}
else                          // 1/8 core freq
{
plb6_value = 0x03F77777;   /* 3F ebc need  */
//dcr_value  = 0x00777777;
dcr_value  = 0x00777777;
}
STW(plb6_value,CPU_CLK_CFG);
STW(dcr_value, AXI_CLK_CFG);


//====================================================================
// Config PLL2 Clock Domain
//====================================================================
// ddr_mclk = pll2_clk / 2
STW(0x1, DDR3_MCLK_CFG);
//STW(0x3, DDR3_MCLK_CFG);

//====================================================================
// Config PLL3 Clock Domain
//====================================================================
// pll3_clk=1200M
// uart1/2/3_clk = 1200 / 326 = 3.68MHz
// peri_clk      = 1200 / 12   = 100MHz
// apb_clk       = 1200 / 12  = 100MHz
// rng_clk       = 1200 / 15  = 80M
// mdha/rscp_clk = 1200 / 6   = 200MHz
// others_clk    = 1200 / 4   = 300MHz
STW(0x1450    ,XTAL0_CLK_CFG);	//uart1/2/3/4_clk = 1200 / 326 = 3.68MHz
STW(0x1450    ,XTAL1_CLK_CFG);
STW(0x1450    ,XTAL2_CLK_CFG);
STW(0x1450    ,XTAL3_CLK_CFG);
//STW(0x50      ,PERI_CLK_CFG);		
STW(0xB0      ,PERI_CLK_CFG);		//1200/12 = 100MHz
//STW(0x3B0      ,PERI_CLK_CFG);		//1200/60 = 20MHz
STW(0xB0      ,APB_CLK_CFG);
STW(0xE0      ,RNG_CLK_CFG);

STW(0x50      ,MDHA_CLK_CFG);
STW(0x50      ,RSCP_CLK_CFG);

STW(0x30      ,PKHA_CLK_CFG);
STW(0x30      ,AES_CLK_CFG);
STW(0x30      ,DES_CLK_CFG);
STW(0x30      ,SM4_CLK_CFG);
STW(0x30      ,IP1_CLK_CFG);
STW(0x30      ,IP3_CLK_CFG);


//====================================================================
// Config PLL4 Clock Domain
//====================================================================
// gtx_clk = 1000 / 8 = 125MHz
// sd_clk  = 1000 / 5 = 200MHz
// lcdc_pclk = 1000 / 14  = 71.43MHz : should be 55~80MHz

//STW(0x70      ,GTX0_CLK_CFG);
//STW(0x70      ,GTX1_CLK_CFG);
//STW(0x70      ,GTX2_CLK_CFG);
//STW(0x70      ,GTX3_CLK_CFG);
STW(0x30      ,SD_CLK_CFG);				//800MHz/4=200MHz
//STW(0xd0      ,LCDC_PCLK_CFG);

/*
//====================================================================
// Config PLL6 Clock Domain
//====================================================================
STW(0x1, DPAA_CLK_CFG);            // 1000MHz / 2 = 500MHz
STW(0x3, SWITCH_SYSCLK_CFG);       // 1000MHz / 4 = 250MHz
STW(0x7, SWITCH_GMICLK_CFG);       // 1000MHz / 8 = 125MHz
*/

//=============================================================================
// Switch Clock Source from XCLK to PLL at last!!
//=============================================================================
wait_busy();
STW(0x3F ,CPR_BASE_ADDR + CLK_SRC_SEL_OFFSET);
//STW(0x1D ,CPR_BASE_ADDR + CLK_SRC_SEL_OFFSET);		//PLL2 & PLL6 no need
wait_busy();
}

