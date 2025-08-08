#include "environment.h"


//**************************************
//  
//          This for DDR34
//
//**************************************
//Strc:
//
//  #ifdef DDR4
//      ddr4_init()
//  #else
//      ddr3_init()
//  #endif
//
//
//
//  #ifndef DDR_STUB
//      #ifdef DDR4
//          void ddr_init()
//          {
//            void ddr4_init();
//          }
//      #else
//          void ddr_init()
//          {
//            void ddr3_init();       
//          }
//      #endif
//  #else
//      void ddr_init() 
//      {
//        COMMENT("DDR stub in ddr_init()");
//      }
//  #endif
//
//
//
//
//**************************************



#include "ddr34_common.h"
#include "ddr34_mc_regs.h"
#include "ddr34_misc_regs.h"
#include "ddr34_phy_regs.h"







unsigned int roundup(unsigned int indata) {
   volatile unsigned int tmp_data;
   
         if((indata & 1) == 1) 
         tmp_data = (indata >> 1) + 1;
         else 
         tmp_data = indata >> 1;
        return tmp_data; 
   }

unsigned int rounddown(unsigned int indata) {
   volatile unsigned int tmp_data;
         tmp_data = indata >> 1;
        return tmp_data; 
   }


void apb_wr_cmd(unsigned int base_addr, unsigned int offset_addr, unsigned int stop, unsigned int start, unsigned int data)
{
    unsigned int temp1;
    unsigned int temp2;
    unsigned int temp3;
   
    unsigned int addr;

    addr=base_addr+offset_addr;
    
    //READ DATA
     apbm_vip_read(addr,temp1);
     temp2 = data;

    //EXCHANGE DATA
    if(start == 0 && stop == 31)
        temp3 = temp2;
    else if(start == 0) 
    {
        temp1 = temp1 >> (stop+1);
        temp1 = temp1 << (stop+1);
        temp2 = temp2  << (31-stop);
        temp2 = temp2  >> (31-stop);
        temp3 = temp1 | temp2;
    }
    else if(stop == 31)
    {
        temp1 = temp1 << (31-start+1);
        temp1 = temp1 >> (31-start+1);
        temp2 = temp2  << start;
        temp3 = temp1 | temp2;
    }
    else 
    {
        temp3 = ((temp1 >> (stop+1)) << (stop+1)) |
                ((temp1 << (31-start+1)) >> (31-start+1)) |
                ((temp2 << (32-(stop-start+1))) >> (31-stop));
    }

    //Write data
    apbm_vip_write(addr,temp3);
}


unsigned int apb_rd_cmd(unsigned int base_addr, unsigned int offset_addr, unsigned int stop, unsigned int start)
{
      unsigned int addr;
      unsigned int dout;
      addr=base_addr+offset_addr;
      
      apbm_vip_read(addr,dout);
      return dout;
}

void ddrn_phy_write_leveling(unsigned int cs_sel)
{
  unsigned int read_data;
  unsigned int mr1_data=0;
  read_data = 0;
        
  apb_wr_cmd(DDRPHY_BASE,reg_dq_wr_train_auto ,0);

 //sel rabk0 ,low is valid
  apb_wr_cmd(DDRPHY_BASE,reg_wlcs_sel, cs_sel );

  #ifdef DDR4
      mr1_data = (CL-AL) << 3 | 0<<2 | 0<<1 | 1<<0;
      apb_wr_cmd(DDRPHY_BASE, reg_wl_loadmode,  (0x4100 | (mr1_data & 0x00ff)) );
  #else //DDR3
      mr1_data = (CL-AL) << 3 | 1 <<2 | 0<<1 | 0;     
      apb_wr_cmd(DDRPHY_BASE, reg_wl_loadmode,  (0x4000 | (mr1_data & 0x00ff))  );
  #endif
  
  //enable 
  apb_wr_cmd(DDRPHY_BASE,reg_wl_enable, 1 );
   
   while( (read_data & 0x01ff0000) != 0x01ff0000)    
     {
       read_data=apb_rd_cmd(DDRPHY_BASE,wl_done_byte);
       COMMENT("Write leveling");
       CPRINTW(read_data);
     }

   COMMENT("Write leveling PASS");

   //close it
   apb_wr_cmd(DDRPHY_BASE, reg_wl_enable, 0x0  );
   apb_wr_cmd(DDRPHY_BASE, reg_wlcs_sel,  0x0 );
}

void ddrn_phy_dqs_calib(unsigned int cs_sel)
{
unsigned int read_data;
read_data = 0;

#ifdef DDR4
                apb_wr_cmd(DDRPHY_BASE,reg_calib_mode_sel,0);
                //DQ_LANE1
                    apb_wr_cmd(DDRPHY_BASE,reg_a_l_weakpub_reg,  0);
                
                //DQ_LANE2
                    apb_wr_cmd(DDRPHY_BASE,reg_a_h_weakpub_reg,  0);
                
                //DQ_LANE3
                    apb_wr_cmd(DDRPHY_BASE,reg_b_l_weakpub_reg,  0);
                
                //DQ_LANE4
                    apb_wr_cmd(DDRPHY_BASE,reg_b_h_weakpub_reg,  0);
                
                //DQ_LANE5
                    apb_wr_cmd(DDRPHY_BASE,reg_c_l_weakpub_reg,  0);
                
                //DQ_LANE6
                    apb_wr_cmd(DDRPHY_BASE,reg_c_h_weakpub_reg,  0);
                
                //DQ_LANE7
                    apb_wr_cmd(DDRPHY_BASE,reg_d_l_weakpub_reg,  0);
                
                //DQ_LANE8
                    apb_wr_cmd(DDRPHY_BASE,reg_d_h_weakpub_reg,  0);
                
                //DQ_LANE9
                    apb_wr_cmd(DDRPHY_BASE,reg_e_l_weakpub_reg,  0);
#endif

//enable calibration
 apb_wr_cmd(DDRPHY_BASE,reg_calcs_sel, cs_sel);
 apb_wr_cmd(DDRPHY_BASE,reg_start_calib, 0x1);
 
 while( (read_data & 0x400) != 0x400) 
   {
      read_data=apb_rd_cmd(DDRPHY_BASE,calib_end);
      COMMENT("Wait Calibration end...");
   }

COMMENT("End Calibration");


read_data = 0;
while( (read_data & 0x1ff) !=0x1ff)    
  {
      read_data=apb_rd_cmd(DDRPHY_BASE,calib_done_byte);
      COMMENT("Wait Calibration done");
  }
COMMENT("Complete Calibration");

read_data = 0;
//Disable calibration
apb_wr_cmd(DDRPHY_BASE,reg_start_calib, 0);

apb_wr_cmd(DDRPHY_BASE,reg_calcs_sel, 0xc);
}



void ddrn_phy_rd_training(unsigned int cs_sel)
{
  unsigned int readreg_data0;
  unsigned int read_data;
  unsigned int rdword;
  unsigned int wrword;

readreg_data0       = 0;

//choose to do the read train of rank0
apb_wr_cmd(DDRPHY_BASE,reg_rdtrain_cs_sel, cs_sel );   

//open reg_phy_refresh_en
apb_wr_cmd(DDRPHY_BASE,reg_phy_refresh_en, 1 );     

//DQ_LANE1
apb_wr_cmd( DDRPHY_BASE,reg_a_l_rd_train_dqs_default,0x1f );    //reg_rdtrain dqs default_l a
//DQ_LANE2
apb_wr_cmd( DDRPHY_BASE,reg_a_h_rd_train_dqs_default,0x1f );    //reg_rdtrain dqs default_r a
//DQ_LANE3
apb_wr_cmd( DDRPHY_BASE,reg_b_l_rd_train_dqs_default,0x1f );    //reg_rdtrain dqs default_l b
//DQ_LANE4
apb_wr_cmd( DDRPHY_BASE,reg_b_h_rd_train_dqs_default,0x1f );    //reg_rdtrain dqs default_r b
//DQ_LANE5
apb_wr_cmd( DDRPHY_BASE,reg_c_l_rd_train_dqs_default,0x1f );    //reg_rdtrain dqs default_r b
//DQ_LANE6
apb_wr_cmd( DDRPHY_BASE,reg_c_h_rd_train_dqs_default,0x1f );    //reg_rdtrain dqs default_r a
//DQ_LANE7
apb_wr_cmd( DDRPHY_BASE,reg_d_l_rd_train_dqs_default,0x1f );    //reg_rdtrain dqs default_l b
//DQ_LANE8
apb_wr_cmd( DDRPHY_BASE,reg_d_h_rd_train_dqs_default,0x1f );    //reg_rdtrain dqs default_r b
//DQ_LANE9
apb_wr_cmd( DDRPHY_BASE,reg_e_l_rd_train_dqs_default,0x1f );    //reg_rdtrain dqs default_l b

//enable
apb_wr_cmd( DDRPHY_BASE,reg_dq_rd_train_en,1 );

//-------------------------------------------------------------
//For read training, the phy will wait for 
//the controller send at least one auto-refresh command
//-------------------------------------------------------------
 apbm_vip_read(DDRMC_RFSHCTL3,rdword);
 wrword = rdword & ~(1<<0);
 apbm_vip_write(DDRMC_RFSHCTL3,wrword);


readreg_data0=0;
while( (readreg_data0 & 0x1) != 0x1 )    
  {
    readreg_data0=apb_rd_cmd(DDRPHY_BASE,train_true_done);
    COMMENT("wait rd train done  ...");
	}

//check error
readreg_data0=apb_rd_cmd(DDRPHY_BASE, reg_train_error_for_rd_byte);   
if( (readreg_data0 & 0x001f0000) != 0x00000000 )
  {
     COMMENT("DQ32 rd_l and rd_r AUTO TRAIN error  ...");
  }
else
  {
    COMMENT("rd AUTO TRAIN pass  ...");
  }



readreg_data0=0;

//close
apb_wr_cmd(DDRPHY_BASE, reg_dq_rd_train_en, 0 );         
COMMENT("rd TRAIN DONE");


apb_wr_cmd(DDRPHY_BASE, reg_rd_train_dqs_range_bypass, 0 );    //reg_rdtrain range bypass
apb_wr_cmd(DDRPHY_BASE, reg_rdtrain_cs_sel, 0 );              //choose to do the read train of rank0
apb_wr_cmd(DDRPHY_BASE, reg_phy_refresh_en, 0 );               //open reg_phy_refresh_en
}


void ddrn_phy_wr_training(unsigned int cs_sel)
{
unsigned  readreg_data0;
unsigned  wrword;
unsigned  rdword;

readreg_data0   = 0;
apb_wr_cmd(DDRPHY_BASE, reg_wrtrain_check_data_value_random_gen, 1 );

//#ifdef DDR4
//	//#ifdef DDR4_DBI
//	//    apb_wr_cmd(reg_ddr4_dbi, 1 );   //open reg_ddr4_dbi
//	//#endif
//#endif


COMMENT("Choose to train the RANKx.");
apb_wr_cmd(DDRPHY_BASE, reg_wrtrain_cs_sel, cs_sel );

//Enable the synchronize of the auto-refresh between the phy and controller
apb_wr_cmd(DDRPHY_BASE,reg_phy_refresh_en, 1 );

//Disable the DQS scan and keep the fix valud.", $time);
apb_wr_cmd(DDRPHY_BASE,reg_wr_train_dqs_default_bypass, 0 );   //reg dqs default bypass

//Choose the Auto Train mode
apb_wr_cmd(DDRPHY_BASE, reg_dq_wr_train_auto,  1 );


//Start the write train
apb_wr_cmd(DDRPHY_BASE, reg_dq_wr_train_en,  1 );//wr_train_en  and reg_dq_train_auto(mux)

//-------------------------------------------------------------
//For Write Training that the Controller 
//needs to send the auto-refresh at least one time
//-------------------------------------------------------------
 apbm_vip_read(DDRMC_RFSHCTL3,rdword);
 wrword = rdword & ~(1<<0);
 apbm_vip_write(DDRMC_RFSHCTL3,wrword);



readreg_data0=0;
while( (readreg_data0 & 0x40)!= 0x40 )    
  {
    readreg_data0=apb_rd_cmd(DDRPHY_BASE,train_step1_delay_done);  //step1_done;
	  COMMENT("wait step1 delay done  ...");
	}

readreg_data0=0;
while( (readreg_data0 & 0x80) !=0x80 )    
  {
	    readreg_data0=apb_rd_cmd(DDRPHY_BASE, train_all_step_done);  //step1_done;
	    COMMENT("wait all step done  ...");
	}

readreg_data0=0;
readreg_data0=apb_rd_cmd(DDRPHY_BASE, train_step1_error);  //step1 error;
if( (readreg_data0 & 0x4) ==0x4  ) 
  {
	    COMMENT("step1  error  ...");
	}

readreg_data0=0;
readreg_data0=apb_rd_cmd(DDRPHY_BASE,train_step2_error);  //step2 error;
if ( (readreg_data0 & 0x2) == 0x2 ) 
  {
    COMMENT("step2  error  ...");
  }

readreg_data0=0;
readreg_data0=apb_rd_cmd(DDRPHY_BASE, train_step3_error);  //step3 error;
if ( (readreg_data0 & 0x1)== 0x1 ) 
  {  
    COMMENT("step3  error  ...");
	}

if ( ( (readreg_data0 & 0x1)==0x1) | ( (readreg_data0 & 0x2)==0x2) | ( (readreg_data0 & 0x4)==0x4))
	    {
       COMMENT("all step error  ...");
      }

   else
	    COMMENT("all step pass  ...");

    COMMENT("wr TRAIN [DONE]");
    COMMENT("DQS dqs TRAIN [DONE]");

//Stop the write train
apb_wr_cmd(DDRPHY_BASE,reg_dq_wr_train_en, 0 );

//Disable the DQS bypass mode at the write train
apb_wr_cmd(DDRPHY_BASE,reg_wr_train_dqs_default_bypass, 0 );


//Recovery to the two rank switch mode
apb_wr_cmd(DDRPHY_BASE,reg_wrtrain_cs_sel, 0 );

//Disable the synchronize of the auto-refresh between the phy and controller
apb_wr_cmd(DDRPHY_BASE,reg_phy_refresh_en, 0 );
}







#ifdef DDR4
void ddr4_init()
{
unsigned int wrword;
unsigned int rdword;
unsigned int word0;
unsigned int word1;
unsigned int word2;
unsigned int word3;
unsigned int addr;
unsigned int wr2rd;
unsigned int rd2wr;
unsigned int wr2pre;
unsigned int t_faw;
unsigned int t_ras_min;
unsigned int t_ras_max;
unsigned int t_rc;
unsigned int mr0_data=0;
unsigned int mr1_data=0;
unsigned int mr2_data=0;
unsigned int mr3_data=0;
unsigned int mr4_data=0;
unsigned int mr5_data=0;
unsigned int mr6_data=0;
unsigned int CL_MODE =0;
unsigned int TWR_MODE=0;
unsigned int TCCD_L_MODE=0;
unsigned int device_config=0;
    //-----------------Program the MC registers-----------------------//
    /*
      INIT0:Ctl SDRAM INIT
            ship_dram_init (QD)=2'b00,let MC init SDRAM 
            post_cke_x1024 (ST):tXPR=max(5*tck,tRFC+10ns);UNIT:dficlk not tck!!!
            pre_cke_x1024  (ST):DDR3/4>500us
    */
    
    #ifdef DDR4_SIM_ONLY
    apbm_vip_write(DDRMC_INIT0,0);
    wrword = 0<<30 | 2<<16 | 0<<0;//post_cke_x1024 must be>=2
    apbm_vip_write(DDRMC_INIT0,wrword);
    #else
    apbm_vip_read(DDRMC_INIT0,rdword);
    wrword = rdword | 0<<30 ;
    apbm_vip_write(DDRMC_INIT0,wrword);
    #endif
    
    //INIT5
    //tZQinit=1024 * tck.
    //we can set more large,can be two times.
    //so set to tZQinit/32=32
    apbm_vip_read(DDRMC_INIT5,rdword);   
    word0=rdword & 0xff00ffff;
    wrword = word0 | (tZQinit/32)  <<16;
    apbm_vip_write(DDRMC_INIT5,wrword);

    //PWRCTL.selfref_sw = 1'b1
    apbm_vip_read(DDRMC_PWRCTL,rdword);
    wrword = rdword | 1<<5;
    apbm_vip_write(DDRMC_PWRCTL,wrword);

    /*
      INNO PHY:must use phy_clk,don't use dfi_clk
      //dfi_rddata_use_dfi_phy_clk
      //dfi_wrdata_use_dfi_phy_clk
      apbm_vip_read(`DDRMC_DFITMG0,rdword);
      wrword = rdword | 1<<15 | 1<<23;
      apbm_vip_write(`DDRMC_DFITMG0,wrword);
    */


   //apbm_vip_read(DDRMC_RANKCTL,rdword);
   //wrword =  15<<12 | 10<<8 | 12<<4 | 15;
   //apbm_vip_write(DDRMC_RANKCTL,wrword);


    wrword = 6 << 24 | (8-6)<<8 | (CL-CWL)<<2;
    apbm_vip_write(DDRMC_ODTCFG, wrword);

    //clear rank0_rd_odt and rank1_rd_odt
    apbm_vip_read(DDRMC_ODTMAP,rdword);
    word0  = bits_clear(rdword,4,0xf);
    wrword = bits_clear(word0,12,0xf);
    apbm_vip_write(DDRMC_ODTMAP, wrword);

//-----------------------------------------------address map config----------------------------------------//
//
//(1) 2 RANKS
//    When all Cap:8GB 
//    RANK0:4GB; RANK1:4GB
//    
//AXI[32]:use to sel rank
//AXI[31:0]:map to HIF than to SDRAM,so SDRAM should:BANK+BG+COL+ROL<=29bit
//
//case1:SDRAM: 4Gb X8,need 8 pcs; 
//      example:BG[1:0]+BA[1:0]+ROW[14:0]+COL[9:0];
//
//case2:SDRAM: 8Gb X16,need 4 pcs;
//      example:BG0+BA[1:0]+A[15:0]+COL[9:0];
//
//
////////////////////////////////////////////////////////////////////////////////////
//
//(2) 2 RANKS
//    When all Cap:16GB 
//    RANK0:8GB; RANK1:8GB
//    
//AXI[33]:use to sel rank
//AXI[32:0]:map to HIF than to SDRAM,so SDRAM should:BANK+BG+COL+ROL<=30bit
//
//In this case,when in S1020 Proj:AXI[33:32] form TLB,
//        so should use TLB control AXI[32] to switch RANx(0-4GB) or RANKx[5-7GB];
//
///////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////
//below use: 
//case1:SDRAM: 4Gb X8,need 8 pcs
//BG[1:0]+BA[1:0]+ROW[14:0]+COL[9:0]=29 BIT Width
///////////////////////////////////////////////////
    //cs0-->HIF[29]-->AXI[32] 
    #ifdef DDR_RANK_1 //when have two rank0 & rank1
      wrword = (29-6)<<0; 
      apbm_vip_write(DDRMC_ADDRMAP0,wrword);
    #else             //when only enable rank0
      wrword = 31; 
      apbm_vip_write(DDRMC_ADDRMAP0,wrword);
    #endif

    //bank0-->HIF[4]
    //bank1-->HIF[5]
    wrword = (4-2)<<0 | (5-3)<<8 | 63 <<16;
    apbm_vip_write(DDRMC_ADDRMAP1,wrword);

    //col2-->HIF[2],recommended when in Full Bus Mode and BL=8
    //col3-->HIF[3]
    //col4-->HIF[6]
    //col5-->HIF[7]
    wrword = (2-2)<<0 | (3-3)<<8 | (6-4)<<16 | (7-5)<<24;
    apbm_vip_write(DDRMC_ADDRMAP2,wrword);

    //col6-->HIF[8]
    //col7-->HIF[9]
    //col8-->HIF[10]
    //col9-->HIF[11]
    wrword = (8-6)<<0 | (9-7)<<8 | (10-8)<<16 | (11-9)<<24;
    apbm_vip_write(DDRMC_ADDRMAP3,wrword);

    //col10 --unused
    //col11 --unused
    wrword = 31<<0 | 31<<8;
    apbm_vip_write(DDRMC_ADDRMAP4,wrword);

    //row0   -->HIF[14]
    //row1   -->HIF[15]
    //row2_10-->HIF[16~24]
    //row11  -->HIF[25]
    wrword = (14-6)<<0 | (15-7)<<8 | (16-8)<<16 | (25-17)<<24 ;
    apbm_vip_write(DDRMC_ADDRMAP5,wrword);

    //row12-->HIF[26]
    //row13-->HIF[27]
    //row14-->HIF[28]
    //row15-->unused
    wrword = (26-18)<<0 | (27-19)<<8 | (28-20)<<16 | 15<<24 ;
    apbm_vip_write(DDRMC_ADDRMAP6,wrword);

    //r0w16-->unused
    //r0w17-->unused
    wrword = 15<<0 | 15<<8  ;
    apbm_vip_write(DDRMC_ADDRMAP7,wrword);

    //bg0-->HIF[12]
    //bg1-->HIF[13]
    wrword = (12-2)<<0 | (13-3)<<8  ;
    apbm_vip_write(DDRMC_ADDRMAP8,wrword);

    //cid not use

//----------------------
    #ifdef DDR4_X4
      device_config=0;
    #else 
      #ifdef DDR4_X8
        device_config=1;
      #else
        #ifdef DDR4_X16
            device_config=2;
        #else
            #ifdef DDR4_X32
               device_config=3;
            #endif
        #endif
      #endif
    #endif 


    #ifdef DDR_RANK_1 // only enable rank0 & rank1 ,2T=0,Sel DDR4 SDRAM;
    apbm_vip_read(DDRMC_MSTR,rdword);
    word0 = bits_clear(rdword,24,0x3);
    word0 = bits_clear(word0, 0 ,0x3f);
    wrword = word0 | device_config<<30 | 3<<24 | 1<<4;
    apbm_vip_write(DDRMC_MSTR,wrword);
    COMMENT("DDR4 Enable two ranks...");

        #ifdef DDR4_UDIMM_MODE
            #ifdef DDR4_MIRROR_MODE 
                apbm_vip_read(DDRMC_DIMMCTL,rdword);
                wrword = rdword | 1<<1;//set dimm_addr_mirr_en=1
                apbm_vip_write(DDRMC_DIMMCTL,wrword);
                #ifdef DDR4_X16
                      apbm_vip_read(DDRMC_DIMMCTL,rdword);
                      wrword = rdword | 1<<5;//dimm_dis_bg_mirroring=1
                      apbm_vip_write(DDRMC_DIMMCTL,wrword);
                #endif
                COMMENT("DDR4 MC Enable DDR4_MIRROR_MODE");        
            #endif 
        #endif

    #else
    apbm_vip_read(DDRMC_MSTR,rdword);
    word0 = bits_clear(rdword,24,0x3);
    word0 = bits_clear(word0, 0 ,0x3f);
    wrword = word0 | device_config<<30 | 1<<24 | 1<<4;
    apbm_vip_write(DDRMC_MSTR,wrword);
    COMMENT("DDR4 Enable one rank...");
    #endif

  /*
   * About ECC configure:
   *
   * -----------------------------------------------------------------------------------------------
   * MEMC_DRAM_DATA_WIDTH  |  Mode(set MSTR.data_bus_width)      |    SDRAM Valid Data Width + ECC
   *-----------------------|------------------------------------------------------------------------
   *                       |  Full data-bus width mode(Default)  |    64+8
   *                       |------------------------------------------------------------------------
   *   Fixed to 64bit      |  Half data-bus width mode           |    32+7
   *                       |------------------------------------------------------------------------
   *                       |  Quarter data-bus width mode        |
   *                       |  (should set MEMC_DBUS_SUPPORT=1,   |
   *                       |   current umctl2 not support!!)     |    16+6
   * -----------------------------------------------------------------------------------------------
   */
    
    //apbm_vip_read(DDRMC_MSTR,rdword);
    //wrword = rdword | 0x1<<12;
    //apbm_vip_write(DDRMC_MSTR,wrword);
    //COMMENT("DDR4 Enable Half data-bus width mode ...");        

    #ifdef DDR4_ECCEN
        apbm_vip_read(DDRMC_ECCCFG0,rdword);
        word0 = bits_clear(rdword,0,0x7);
        wrword = word0 | 4<<0;
        apbm_vip_write(DDRMC_ECCCFG0,wrword);
        COMMENT("DDR4 Enable ECC...");
    #endif

//----------------------

    //De-assert reset signal core_ddrc_rstn
    apbm_vip_write(DDRMISC_RSTOK, 1);

    //disable auto-refreshes, self-refresh, powerdown and assertion of dfi_dram_clk_disable by setting
    // RFSHCTL3.dis_auto_refresh = 1
    apbm_vip_read(DDRMC_RFSHCTL3,rdword);
    wrword = rdword | 1<<0;
    apbm_vip_write(DDRMC_RFSHCTL3,wrword);

    // PWRCTL.powerdown_en = 0
    // PWRCTL.selfref_en = 0
    // PWRCTL.en_dfi_dram_clk_disable = 0
    apbm_vip_read(DDRMC_PWRCTL,rdword);
    wrword = rdword & (~(1<0)) & (~(1<1)) & (~(1<3));
    apbm_vip_write(DDRMC_PWRCTL,wrword);

    //5 set SWCTL.sw_done to 0
    apbm_vip_read(DDRMC_SWCTL,rdword);
    wrword = rdword & ~(1<<0);
    apbm_vip_write(DDRMC_SWCTL,wrword);

    //6 set DFIMISC.dfi_init_complete_en to 0
    apbm_vip_read(DDRMC_DFIMISC,rdword);
    wrword = rdword & ~(1<<0);
    apbm_vip_write(DDRMC_DFIMISC,wrword);

    //7 set SWCTL.sw_done to 1
    apbm_vip_read(DDRMC_SWCTL,rdword);
    wrword = rdword | (1<<0);
    apbm_vip_write(DDRMC_SWCTL,wrword);
    //polling SWSTAT 
    rdword = 0;
    while ((rdword & 1) != 1) 
      {
        apbm_vip_read(DDRMC_SWSTAT,rdword);
      }




///////////////////////////////////////////////////////////////////////////////////////
//
//                      PHY Init Start
//
///////////////////////////////////////////////////////////////////////////////////////
   COMMENT("PHY PLL cfg start");

    //(1)
    //reset PHY
    apbm_vip_write(DDRMISC_PHY_RSOK ,0);

    //(2).
    //PLL CFG 
    //0x84[3]  =1'b1,
    //0x84[2:0]=3'b000,
    apb_wr_cmd(DDRPHY_BASE ,reg_pllpostdiven_ls ,0x1  );
    apb_wr_cmd(DDRPHY_BASE ,reg_pllpostdiv_ls   ,0x0);



   //---------------------------PHY BASIC CONFIG Start-----------------------------------// 
   
   COMMENT("PHY BASIC CONFIG Start!!!");
    
    //bufferen_core    = 1;
    apbm_vip_write(DDRMISC_PHY_BUFFEREN_CORE, 1);
   
   //Automatically gate the clock when the train logic is in idle state.
   apb_wr_cmd(DDRPHY_BASE,reg_train_reg_update_en, 0);
    
   //set DQ72bit 
   apb_wr_cmd(DDRPHY_BASE,reg_channel_en,0x1ff);
   
   //#ifdef CMD_2T
   //  apb_wr_cmd(DDRPHY_BASE,reg_cmd_2t_mode_t, 1);
   //  COMMENT("SET PHY CMD 2T MODE");
   //#endif
   
  #ifdef DDR4_UDIMM_MODE
     #ifdef DDR4_MIRROR_MODE
        apb_wr_cmd(DDRPHY_BASE,reg_ddrphy_udimm_addr_mirror, 0x2);
        COMMENT("DDR4 PHY Enable DDR4_MIRROR_MODE");                
     #endif
  #endif

    //set CWL at FP0
    apb_wr_cmd(DDRPHY_BASE,CWL_FRE_OP0,CWL);

    //set CL  at FP0
    apb_wr_cmd(DDRPHY_BASE,CL_FRE_OP0,CL);

    //set AL at FP0
    apb_wr_cmd(DDRPHY_BASE,AL_FRE_OP0,AL);
   
   //for DDR4
   #ifdef DDR4_1600
    //The register is used to control the auto-refresh interval when the phy do the training. 
    //The auto-refresh interval = reg_phy_trfc *Tdfi_clk1x.
    apb_wr_cmd(DDRPHY_BASE,reg_phy_trfc,  0x1b8 );
    //The value to control the timing of the Trefi when the phy is in training mode.Unit:dfi_clk1x.
    apb_wr_cmd(DDRPHY_BASE,reg_phy_trefi, 2400);
   #endif

   #ifdef DDR4_1866
    //The register is used to control the auto-refresh interval when the phy do the training. 
    //The auto-refresh interval = reg_phy_trfc *Tdfi_clk1x.
    apb_wr_cmd(DDRPHY_BASE,reg_phy_trfc,  0x1b8 );
    //The value to control the timing of the Trefi when the phy is in training mode.Unit:dfi_clk1x.
    apb_wr_cmd(DDRPHY_BASE,reg_phy_trefi, 2799);
   #endif

   #ifdef DDR4_2133
    //The register is used to control the auto-refresh interval when the phy do the training. 
    //The auto-refresh interval = reg_phy_trfc *Tdfi_clk1x.
    apb_wr_cmd(DDRPHY_BASE,reg_phy_trfc,  0x1b8 );
    //The value to control the timing of the Trefi when the phy is in training mode.Unit:dfi_clk1x.
    apb_wr_cmd(DDRPHY_BASE,reg_phy_trefi, 3199);
  #endif



   //CHOOSE MEM TYPE:DDR4
   apb_wr_cmd(DDRPHY_BASE,mem_select_t, 0x4);

   //------------------------------PHY BASIC CONFIG end---------------------------------------//        

    //Release the PHY reset
    apbm_vip_write(DDRMISC_PHY_RSOK ,1);

    //set DQ_Lane1-DQ_Lane9
    apb_wr_cmd(DDRPHY_BASE,reg_a_l_rxen_lp4,0);
    apb_wr_cmd(DDRPHY_BASE,reg_a_h_rxen_lp4,0);
    apb_wr_cmd(DDRPHY_BASE,reg_b_l_rxen_lp4,0);
    apb_wr_cmd(DDRPHY_BASE,reg_b_h_rxen_lp4,0);
    apb_wr_cmd(DDRPHY_BASE,reg_c_l_rxen_lp4,0);
    apb_wr_cmd(DDRPHY_BASE,reg_c_h_rxen_lp4,0);
    apb_wr_cmd(DDRPHY_BASE,reg_d_l_rxen_lp4,0);
    apb_wr_cmd(DDRPHY_BASE,reg_d_h_rxen_lp4,0);
    apb_wr_cmd(DDRPHY_BASE,reg_e_l_rxen_lp4,0);

   //------------------------------PHY Initial Start---------------------------------------//        
    //Enable the PHY initial by MC:dfi_init_start= 1'b1,
    //dfi_init_start is Quasi-dynamic.
    /////////////////////////////////
    //
    //  set dfi_init_start=1
    //
    /////////////////////////////////
    //set SWCTL.sw_done to 0
    apbm_vip_read(DDRMC_SWCTL,rdword);
    wrword = rdword & ~(1<<0);
    apbm_vip_write(DDRMC_SWCTL,wrword);

    //Set DFIMISC.dfi_init_start to 1
    apbm_vip_read(DDRMC_DFIMISC,rdword);
    wrword = rdword | (1<<5);
    apbm_vip_write(DDRMC_DFIMISC,wrword);
    
    //Set SWCTL.sw_done to 1
    apbm_vip_read(DDRMC_SWCTL,rdword);
    wrword = rdword | (1<<0); 
    apbm_vip_write(DDRMC_SWCTL,wrword);
 
    //Polling SWSTAT.sw_done_ack
    rdword = 0;
    while ((rdword & 1) != 1) 
    {
    apbm_vip_read(DDRMC_SWSTAT,rdword);
    }
    
    ////////////////////////////////////
    //
    //Wait DFISTAT.dfi_init_complete=1
    //
    ////////////////////////////////////
    rdword = 0;
    while ((rdword & 1) != 1) 
    {
    apbm_vip_read(DDRMC_DFISTAT,rdword);
    }

   ///////////////////////////////////////////
   //
   // set dfi_init_start=0
   //
   ///////////////////////////////////////////
   // Set SWCTL.sw_done to 0
    apbm_vip_read(DDRMC_SWCTL,rdword);
    wrword = rdword & ~(1<<0);
    apbm_vip_write(DDRMC_SWCTL,wrword);
   
   //Set DFIMISC.dfi_init_start to 0
    apbm_vip_read(DDRMC_DFIMISC,rdword);
    wrword = rdword & ~(1<<5);
    apbm_vip_write(DDRMC_DFIMISC,wrword);
    
    //Set SWCTL.sw_done to 1
    apbm_vip_read(DDRMC_SWCTL,rdword);
    wrword = rdword | (1<<0); 
    apbm_vip_write(DDRMC_SWCTL,wrword);
 
    //Polling SWSTAT.sw_done_ack
    rdword = 0;
    while ((rdword & 1) != 1) 
    {
    apbm_vip_read(DDRMC_SWSTAT,rdword);
    }
   
   COMMENT("PHY Initial End!!!");
   
//------------------------------PHY Initial End---------------------------------------//        

//-----SRAM Tmining & DFI Timing & SDRAM INIT register config start------------------//                           
// Set SWCTL.sw_done to 0
apbm_vip_read(DDRMC_SWCTL,rdword);
wrword = rdword & ~(1<<0);
apbm_vip_write(DDRMC_SWCTL,wrword);

    
    /////////////////////
    //
    //SDRAM tming regs
    //
    /////////////////////
    wr2pre = roundup(Twr2pre);
    t_faw = roundup(tFAW);

    t_ras_min = rounddown(tRAS);
    t_ras_max = rounddown(tRAS_MAX);
    wrword = wr2pre << 24 | t_faw << 16 | t_ras_max << 8 | t_ras_min;
    apbm_vip_write(DDRMC_DRAMTMG0,wrword);

    apbm_vip_read(DDRMC_DRAMTMG0,rdword);
    if(rdword != wrword)
    {
      COMMENT("DDRMC_DRAMTMG0 have static config bits");
    }	

    t_rc = roundup(tRC);

    wrword = roundup(Txp+PL)<<16 | (Trd2pre/2)<<8 | t_rc<<0; 
    apbm_vip_write(DDRMC_DRAMTMG1,wrword);

    apbm_vip_read(DDRMC_DRAMTMG1,rdword);
    if(rdword != wrword)
    {
      COMMENT("DDRMC_DRAMTMG1 have static config bits");
    }	

    rd2wr = roundup(AL+CL+PL + (8/2) + 1 + 1 - CWL-AL-PL);

    wr2rd = roundup(CWL + PL + 8/2 + tWTR);

    apbm_vip_read(DDRMC_DRAMTMG2,rdword);
    wrword =  rd2wr<<8 | wr2rd << 0 | roundup(AL+CL)<<16 | roundup(CWL+AL+PL) << 24;
    apbm_vip_write(DDRMC_DRAMTMG2,wrword);

    apbm_vip_read(DDRMC_DRAMTMG2,rdword);
    if(rdword != wrword)
    {
      COMMENT("DDRMC_DRAMTMG2 have static config bits");
    }

    
    wrword = roundup(tMRD) << 12 | roundup(tMODE) ; 
    apbm_vip_write(DDRMC_DRAMTMG3,wrword);
    apbm_vip_read(DDRMC_DRAMTMG3,rdword);
    if(rdword != wrword)
    {
      COMMENT("DDRMC_DRAMTMG3 have static config bits");	
    }

    wrword = roundup(tRCD-AL) << 24 | roundup(tCCD_L) << 16 | roundup(tRRD) << 8 | (rounddown(tRP)+1);
    apbm_vip_write(DDRMC_DRAMTMG4,wrword);
    apbm_vip_read(DDRMC_DRAMTMG4,rdword);
    if(rdword != wrword)
    {
      COMMENT("DDRMC_DRAMTMG4 have static config bits");	
    }

    
    /////////////////////
    //
    //DFI tming regs
    //
    /////////////////////

    wrword = Tctrl_delay << 24 | Trdata_en << 16 | Tphy_wrdata<<8 | Tphy_wrlat <<0;
    apbm_vip_write(DDRMC_DFITMG0,wrword);

    apbm_vip_read(DDRMC_DFITMG0,rdword);
    if(rdword != wrword)
    {
      COMMENT("DDRMC_DFITMG0 have static config bits");	
    }


    apbm_vip_read(DDRMC_DFITMG1,rdword);
    word0 = bits_clear(rdword,16,0x1f); 
    word1 = bits_clear(word0 ,8 ,0x1f);   
    word2 = bits_clear(word1 ,0 ,0x1f);   
    wrword = word0 | roundup(Twdata_delay) << 16 | Tdram_clk_enable<<0 | Tdram_clk_disable<<8;
    apbm_vip_write(DDRMC_DFITMG1,wrword);

    apbm_vip_read(DDRMC_DFITMG1,rdword);
    if(rdword != wrword)
    {
      COMMENT("DDRMC_DFITMG1 have static config bits");	
    }

    /////////////////////
    //
    //SRAM init regs
    //
    /////////////////////

    //----------------MR1--------------------------------//
    //BIT[4:3]    :AL=2'b01,AL=CL-1;
    //Bit[2,1]    :ODT=2'b00,RZQ/7(34)
    //Bit[0]      :DLL=1,enable DLL(normal);
    mr1_data = (CL-AL) << 3 | 0<<2 | 0<<1 | 1<<0;
    
    //-----------------MR3-------------------------------//
    mr3_data = 0;
     
    //-----------------MR6-------------------------------//
      if (tCCD_L == 4) 
         TCCD_L_MODE = 0;
      else if (tCCD_L == 5) 
         TCCD_L_MODE = 1;
      else if (tCCD_L == 6) 
         TCCD_L_MODE = 2;
      else if (tCCD_L == 7) 
         TCCD_L_MODE = 3;
      else if (tCCD_L == 8) 
         TCCD_L_MODE = 4;
      else
        {
         COMMENT("Error: TCCD_L_MODE Not Supported!");
         failed();            
        }
    mr6_data=   TCCD_L_MODE<<10  | 15<<0;

    //-----------------MR5--------------------------------//
    //BIT[10]:Data mask=1,enable
    //BIT[11]:Write DBI=0,disable
    //BIT[12]:Read  DBI=0,disable
    //mr5_data=  0<<12 | 1<<11 | 1<<10;//enable read dbi 
      mr5_data=  0<<12 | 0<<11 | 1<<10;//diable read dbi

   //------------------MR4--------------------------------//
    mr4_data=0;

    //------------------MR2-------------------------------//
    //BIT[7:6]:LPASR=2'b10; Manual mode (-40C ~ 105C)
    //BIT[53] :CWL
    mr2_data = 2<<6 | (CWL-9) << 3;

    //-------------------MR0------------------------------//
    //BIT[13,11:9] :WR       
    //BIT[8]       :DLL=1,DLL reset
    //BIT[3]       :BT=1,Interleave
    //BIT[12,6:4,2]:CL
    //BIT[0]       :BL=00,fixed 8.
    
    if (CL == 9) 
        CL_MODE = 0;
     else if (CL == 10) 
        CL_MODE = 1;
     else if (CL == 11) 
        CL_MODE = 2;
     else if (CL == 12) 
        CL_MODE = 3;
     else if (CL == 13) 
        CL_MODE = 4;
     else if (CL == 14) 
        CL_MODE = 5;
     else if (CL == 15) 
        CL_MODE = 6;
     else if (CL == 16) 
        CL_MODE = 7;
     else if (CL == 17) 
        CL_MODE = 13;
     else if (CL == 18) 
        CL_MODE = 8;
     else if (CL == 19) 
        CL_MODE = 14;
     else if (CL == 20) 
        CL_MODE = 9;
     else 
     {
        COMMENT ("Error: CL Not Supported!");
        failed();            
     }	

    if (tWR == 10) 
        TWR_MODE = 0;
     else if (tWR == 12) 
        TWR_MODE = 1;
     else if (tWR == 14) 
        TWR_MODE =2 ;
     else if (tWR == 16) 
        TWR_MODE =3;
     else if (tWR == 18) 
        TWR_MODE =4;
     else 
     {
        COMMENT ("Error: tWR Not Supported!");
        failed();            
     }
  
    mr0_data =((TWR_MODE>>3) & 0x1) <<13 | ((CL_MODE>>4)) & 0x1  <<12 | (TWR_MODE) << 9 | 1<<8 |   ((CL_MODE >>1) & 0x07) << 4 | 1<<3 | (CL_MODE & 0x01) << 2 | BL;
    


    
    apbm_vip_write(DDRMC_INIT3,0);//clear to all 0
    wrword = mr0_data <<16 | mr1_data<<0;
    apbm_vip_write(DDRMC_INIT3,wrword);
    
    apbm_vip_write(DDRMC_INIT4,0);//clear to all 0.
    wrword = mr2_data <<16 | mr3_data<<0;
    apbm_vip_write(DDRMC_INIT4,wrword);

    apbm_vip_write(DDRMC_INIT6,0);//clear to all 0.
    wrword = mr4_data <<16 | mr5_data<<0;
    apbm_vip_write(DDRMC_INIT6,wrword);

    apbm_vip_write(DDRMC_INIT7,0);//clear to all 0.
    wrword = mr6_data<<0;
    apbm_vip_write(DDRMC_INIT7,wrword);




//------------------------------SRAM Initial Start---------------------------------------//        
   //This step shoule compelte use the controller!!!
    COMMENT("SRAM Initial Start!!!");

   // Trigger SDRAM INIT
   // Set DFIMISC.dfi_init_complete_en to 1
    apbm_vip_read(DDRMC_DFIMISC,rdword);
    wrword = rdword | (1<<0);
    apbm_vip_write(DDRMC_DFIMISC,wrword);
   // Set PWRCTL.selfref_sw to 0 
    apbm_vip_read(DDRMC_PWRCTL,rdword);
    wrword = rdword & ~(1<<5);
    apbm_vip_write(DDRMC_PWRCTL,wrword);


//Set SWCTL.sw_done to 1
apbm_vip_read(DDRMC_SWCTL,rdword);
wrword = rdword | (1<<0);
apbm_vip_write(DDRMC_SWCTL,wrword);
//polling SWSTAT 
rdword = 0;
while ((rdword & 1) != 1)
{
apbm_vip_read(DDRMC_SWSTAT,rdword);
}



//---------------Wait for DWC_ddr_umctl2 to move to normal operating mode by monitoring STAT.operation_mode signal-----------------//
    rdword = 0;
    while ((rdword & 0x3) != 1) 
    {
    apbm_vip_read(DDRMC_STAT,rdword);
    }

   /*
      //RFSHCTL3.dis_auto_refresh = 0
      apbm_vip_read(`DDRMC_RFSHCTL3,rdword);
      wrword = rdword & ~(1<<0);
      apbm_vip_write(`DDRMC_RFSHCTL3,wrword);

      //PWRCTL.powerdown_en keep 0
      //PWRCTL.selfref_en keep 0
      //PWRCTL.en_dfi_dram_clk_disable = 1
      apbm_vip_read(`DDRMC_PWRCTL,rdword);
      wrword = rdword | (1<<3);
      apbm_vip_write(`DDRMC_PWRCTL,wrword);
   */


    //Write PCTRL.port_en = 1, AXI ports are no longer blocked from taking transaction
    //Write SBRCTL.scrub_en = 1, Enable SBR if desired 

    apbm_vip_read(DDRMC_PCTRL_0,rdword);
    wrword = rdword | (1<<0);
    apbm_vip_write(DDRMC_PCTRL_0,wrword);

    //apbm_vip_read(`DDRMC_SBRCTL,rdword);
    //wrword = rdword | (1<<0);
    //apbm_vip_write(`DDRMC_SBRCTL,wrword);

COMMENT("DDR Enter normal operating mode");


//----------------------------------Write Leveling Start-------------------------------------------------------//
 //******************************************
 //
 //RFSHCTL3.dis_auto_refresh = 1,
 //close atuo-refresh
 //
 //********************************************
 apbm_vip_read(DDRMC_RFSHCTL3,rdword);
 wrword = rdword | (1<<0);
 apbm_vip_write(DDRMC_RFSHCTL3,wrword);


COMMENT("RANK0 write_leveling_start >>>");

//sel RANK0
ddrn_phy_write_leveling(0xe);
#ifdef DDR_RANK_1
COMMENT("RANK1 write_leveling_start >>>");
ddrn_phy_write_leveling(0xd);
#endif



//-------------------------------------DSQ-gating CALIB Start------------------------------------------------//
COMMENT("RANK0 RX DQS Gating Start >>>");

ddrn_phy_dqs_calib(0xe);
#ifdef DDR_RANK_1
COMMENT("RANK1 RX DQS Gating Start >>>");
ddrn_phy_dqs_calib(0xd);
#endif


//-------------------------------------Read-train Start------------------------------------------------//
COMMENT("RANK0 Read Training Start>>>");

ddrn_phy_rd_training(0xe);
#ifdef DDR_RANK_1
COMMENT("RANK1 Read Training Start>>>");
ddrn_phy_rd_training(0xd);
#endif


//-------------------------------------Write-train Start------------------------------------------------//
COMMENT("RANK0 Write Training Start>>>");

ddrn_phy_wr_training(0xe);
#ifdef DDR_RANK_1
COMMENT("RANK1 Write Training Start>>>");
ddrn_phy_wr_training(0xd);
#endif


//--------------------------------------Normal operation------------------------------------------------//
COMMENT("Normal WR >>>");
}


#else //********************DDR3**************************
void ddr3_init() 
{
unsigned int wrword;
unsigned int rdword;
unsigned int word0;
unsigned int word1;
unsigned int word2;
unsigned int word3;
unsigned int addr;
unsigned int wr2rd;
unsigned int rd2wr;
unsigned int wr2pre;
unsigned int t_faw;
unsigned int t_ras_min;
unsigned int t_ras_max;
unsigned int t_rc;
unsigned int mr0_data=0;
unsigned int mr1_data=0;
unsigned int mr2_data=0;
unsigned int mr3_data=0;
unsigned int mr4_data=0;
unsigned int mr5_data=0;
unsigned int mr6_data=0;
unsigned int CL_MODE =0;
unsigned int TWR_MODE =0;
unsigned int TCCD_L_MODE=0;
    //-----------------Program the MC registers-----------------------//
    /*
      INIT0:Ctl SDRAM INIT
            ship_dram_init (QD)=2'b00,let MC init SDRAM 
            post_cke_x1024 (ST):tXPR=max(5*tck,tRFC+10ns);UNIT:dficlk not tck!!!
            pre_cke_x1024  (ST):DDR3/4>500us
    */
    
    #ifdef DDR3_SIM_ONLY
      apbm_vip_write(DDRMC_INIT0,0);
      wrword = 0<<30 |2<<16 |0<<0 ;//post_cke_x1024 must be>=2
      apbm_vip_write(DDRMC_INIT0,wrword);
    #else 
      apbm_vip_read(DDRMC_INIT0,rdword);
      wrword = rdword | 0<<30 ;
      apbm_vip_write(DDRMC_INIT0,wrword);
    #endif
    
    //set ZQCTL0:zq_short_nop=0x50,because when DDR3-1600, PLL only can be set to DDR3-1068,so tZQCS should more large !!!
    apbm_vip_read(DDRMC_ZQCTL0,rdword);  
    word0=rdword & 0xfffffc00;
    wrword = word0 |  0x50 <<0;
    apbm_vip_write(DDRMC_ZQCTL0,wrword);

    
    //INIT5
    //tZQinit=1024 * tck.
    //we can set more large,can be two times.
    //so set to tZQinit/32=32
    apbm_vip_read(DDRMC_INIT5,rdword);   
    word0=rdword & 0xff00ffff;
    wrword = word0 | (tZQinit/32)  <<16;
    apbm_vip_write(DDRMC_INIT5,wrword);

    //PWRCTL.selfref_sw = 1'b1
    apbm_vip_read(DDRMC_PWRCTL,rdword);
    wrword = rdword | 1<<5;
    apbm_vip_write(DDRMC_PWRCTL,wrword);

    /*
      INNO PHY:must use phy_clk,don't use dfi_clk
      //dfi_rddata_use_dfi_phy_clk
      //dfi_wrdata_use_dfi_phy_clk
      apbm_vip_read(`DDRMC_DFITMG0,rdword);
      wrword = rdword | 1<<15 | 1<<23;
      apbm_vip_write(`DDRMC_DFITMG0,wrword);
    */

   //apbm_vip_read(DDRMC_RANKCTL,rdword);
   //wrword =  15<<12 | 10<<8 | 12<<4 | 15;
   //apbm_vip_write(DDRMC_RANKCTL,wrword);


    wrword = 6 << 24 | (8-6)<<8 | (CL-CWL)<<2;
    apbm_vip_write(DDRMC_ODTCFG, wrword);

    //clear rank0_rd_odt and rank1_rd_odt
    apbm_vip_read(DDRMC_ODTMAP,rdword);
    word0 = bits_clear(rdword,4,0xf);
    wrword = bits_clear(word0,12,0xf);
    apbm_vip_write(DDRMC_ODTMAP, wrword);

//------------------------address config---------------------------//
    
//-----------------------------------------------address map config----------------------------------------//
//
//(1) 2 RANKS
//    When all Cap:8GB 
//    RANK0:4GB; RANK1:4GB
//    
//AXI[32]:use to sel rank
//AXI[31:0]:map to HIF than to SDRAM,so SDRAM should:BANK+BG+COL+ROL<=29bit
//
//case1:SDRAM: 4Gb X8,need 8 pcs; 
//      example:BG[1:0]+BA[1:0]+ROW[14:0]+COL[9:0];
//
//case2:SDRAM: 8Gb X16,need 4 pcs;
//      example:BG0+BA[1:0]+A[15:0]+COL[9:0];
//
//
////////////////////////////////////////////////////////////////////////////////////
//
//(2) 2 RANKS
//    When all Cap:16GB 
//    RANK0:8GB; RANK1:8GB
//    
//AXI[33]:use to sel rank
//AXI[32:0]:map to HIF than to SDRAM,so SDRAM should:BANK+BG+COL+ROL<=30bit
//
//In this case,when in S1020 Proj:AXI[33:32] form TLB,
//        so should use TLB control AXI[32] to switch RANx(0-4GB) or RANKx[5-7GB];
//
    
//(3) 2 RANKS
//    When all Cap:4GB 
//    RANK0:2GB; RANK1:2GB
//    
//AXI[31]:use to sel rank
//AXI[30:0]:map to HIF than to SDRAM,so SDRAM should:BANK+BG+COL+ROL<=28bit
//
//case1:SDRAM: 4Gb X16,need 4 pcs; 
//      example:BA[2:0]+ROW[14:0]+COL[9:0];
///////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////
//below use: 
//case1:SDRAM: 4Gb X8,need 8 pcs
//BG[1:0]+BA[1:0]+ROW[14:0]+COL[9:0]=29 BIT Width
///////////////////////////////////////////////////
    
    
    //cs0
    #ifdef DDR_RANK_1 //when have two rank0 & rank1
      wrword = (29-6)<<0; 
      apbm_vip_write(DDRMC_ADDRMAP0,wrword);
      COMMENT("Enable two ranks..");
    #else             //when only have rank0
      wrword = 31; 
      apbm_vip_write(DDRMC_ADDRMAP0,wrword);
    #endif

    //bank address arragement
    wrword = (4-2)<<0 | (5-3)<<8 | (6-4)<<16;
    apbm_vip_write(DDRMC_ADDRMAP1,wrword);

    //col 2,3,4,5
    wrword = (2-2)<<0 | (3-3)<<8 | (7-4)<<16 | (8-5)<<24;
    apbm_vip_write(DDRMC_ADDRMAP2,wrword);

    //col 6,7,8,9
    wrword = (9-6)<<0 | (10-7)<<8 | (11-8)<<16 | (12-9)<<24;
    apbm_vip_write(DDRMC_ADDRMAP3,wrword);

    //col 10, 11
    wrword = 31<<0 | 31<<8;
    apbm_vip_write(DDRMC_ADDRMAP4,wrword);

    //raw 0, 1, 2_10,11
    wrword = (13-6)<<0 | (14-7)<<8 | (15-8)<<16 | (24-17)<<24 ;
    apbm_vip_write(DDRMC_ADDRMAP5,wrword);

    //raw 12, 13, 14, 15
//    wrword = (25-18)<<0 | (26-19)<<8 | (27-20)<<16 | (28-21)<<24 ;
    wrword = (25-18)<<0 | (26-19)<<8 | (27-20)<<16 | (15)<<24 ;
    apbm_vip_write(DDRMC_ADDRMAP6,wrword);

#if 1
    //r0w16-->unused
     //r0w17-->unused
     wrword = 15<<0 | 15<<8  ;
     apbm_vip_write(DDRMC_ADDRMAP7,wrword);
#endif
    //raw 16,17
    //wrword = 7<<0 | 15<<8  ;
    //apbm_vip_write(`DDRMC_ADDRMAP7,wrword);

    //bg 0 ,1 not use;

    //raw2,3,4,5;6,7,8,9; 10,cid not use


    #ifdef DDR_RANK_1 //enable rank0 & rank1; 2T=0;DDR3;
        apbm_vip_read(DDRMC_MSTR,rdword);
        word0 = bits_clear(rdword,24,0x3);
        wrword = word0 | 3<<24;
        apbm_vip_write(DDRMC_MSTR,wrword);
        COMMENT("DDR3 Enable two ranks...");

        #ifdef DDR3_UDIMM_MODE
            #ifdef DDR3_MIRROR_MODE //set dimm_addr_mirr_en=1
                apbm_vip_read(DDRMC_DIMMCTL,rdword);
                wrword = rdword | 1<<1;
                apbm_vip_write(DDRMC_DIMMCTL,wrword);
                COMMENT("DDR3 MC Enable DDR3_MIRROR_MODE");        
            #endif 
        #endif

    #else            //enable  rank0 ; 2T=0;
        apbm_vip_read(DDRMC_MSTR,rdword);
        word0 = bits_clear(rdword,24,0x3);
        wrword = word0 | 1<<24;
        apbm_vip_write(DDRMC_MSTR,wrword);
        COMMENT("DDR3 Enable one rank...");        
    #endif

  /*
   * About ECC configure:
   *
   * -----------------------------------------------------------------------------------------------
   * MEMC_DRAM_DATA_WIDTH  |  Mode(set MSTR.data_bus_width)      |    SDRAM Valid Data Width + ECC
   *-----------------------|------------------------------------------------------------------------
   *                       |  Full data-bus width mode(Default)  |    64+8
   *                       |------------------------------------------------------------------------
   *   Fixed to 64bit      |  Half data-bus width mode           |    32+7
   *                       |------------------------------------------------------------------------
   *                       |  Quarter data-bus width mode        |
   *                       |  (should set MEMC_DBUS_SUPPORT=1,   |
   *                       |   current umctl2 not support!!)     |    16+6
   * -----------------------------------------------------------------------------------------------
   */
    
    //apbm_vip_read(DDRMC_MSTR,rdword);
    //wrword = rdword | 0x1<<12;
    //apbm_vip_write(DDRMC_MSTR,wrword);
    //COMMENT("DDR3 Enable Half data-bus width mode ...");        

    #ifdef DDR3_ECCEN
        apbm_vip_read(DDRMC_ECCCFG0,rdword);
        word0 = bits_clear(rdword,0,0x7);
        wrword = word0 | 4<<0;
        apbm_vip_write(DDRMC_ECCCFG0,wrword);
        COMMENT("DDR3 Enable ECC...");        
    #endif

//----------------------

    //De-assert reset signal core_ddrc_rstn
    apbm_vip_write(DDRMISC_RSTOK, 1);

    //disable auto-refreshes, self-refresh, powerdown and assertion of dfi_dram_clk_disable by setting
    // RFSHCTL3.dis_auto_refresh = 1
    apbm_vip_read(DDRMC_RFSHCTL3,rdword);
    wrword = rdword | 1<<0;
    apbm_vip_write(DDRMC_RFSHCTL3,wrword);

    // PWRCTL.powerdown_en = 0
    // PWRCTL.selfref_en = 0
    // PWRCTL.en_dfi_dram_clk_disable = 0
    apbm_vip_read(DDRMC_PWRCTL,rdword);
    wrword = rdword & (~(1<0)) & (~(1<1)) & (~(1<3));
    apbm_vip_write(DDRMC_PWRCTL,wrword);

    //5 set SWCTL.sw_done to 0
    apbm_vip_read(DDRMC_SWCTL,rdword);
    wrword = rdword & ~(1<<0);
    apbm_vip_write(DDRMC_SWCTL,wrword);

    //6 set DFIMISC.dfi_init_complete_en to 0
    apbm_vip_read(DDRMC_DFIMISC,rdword);
    wrword = rdword & ~(1<<0);
    apbm_vip_write(DDRMC_DFIMISC,wrword);

    //7 set SWCTL.sw_done to 1
    apbm_vip_read(DDRMC_SWCTL,rdword);
    wrword = rdword | (1<<0);
    apbm_vip_write(DDRMC_SWCTL,wrword);
    //polling SWSTAT 
    rdword = 0;
    while ((rdword & 1) != 1) 
      {
        apbm_vip_read(DDRMC_SWSTAT,rdword);
      }




///////////////////////////////////////////////////////////////////////////////////////
//
//                      PHY Init Start
//
///////////////////////////////////////////////////////////////////////////////////////
   COMMENT("PHY PLL cfg start");

    //(1)
    //reset PHY
    apbm_vip_write(DDRMISC_PHY_RSOK ,0);

    
    
#if 0  
    //add point 2
    rdword = apb_rd_cmd(DDRPHY_BASE, reg_fb1xclk_invdelaysel_dqcmd);
    apb_wr_cmd(DDRPHY_BASE, reg_fb1xclk_invdelaysel_dqcmd, 0x12);
 //   apb_wr_cmd(DDRPHY_BASE, reg_fb1xclk_invdelaysel_dqcmd, 0x1f);

    rdword = apb_rd_cmd(DDRPHY_BASE, reg_fb1xclk_invdelaysel_dqcmd);
#endif
    
    
    
    
    //(2).
    //PLL CFG 
    //0x84[3]  =1'b1,
    //0x84[2:0]=3'b000,
    apb_wr_cmd(DDRPHY_BASE ,reg_pllpostdiven_ls ,0x1  );
 //    apb_wr_cmd(DDRPHY_BASE ,reg_pllpostdiv_ls   ,0x0);	//former
    apb_wr_cmd(DDRPHY_BASE ,reg_pllpostdiv_ls   ,0x1);	//point 1
//  apb_wr_cmd(DDRPHY_BASE ,reg_pllpostdiv_ls   ,0x2);	//point 2


   //---------------------------PHY BASIC CONFIG Start-----------------------------------// 
   
   COMMENT("PHY BASIC CONFIG Start!!!");
    
    //bufferen_core    = 1;
    apbm_vip_write(DDRMISC_PHY_BUFFEREN_CORE, 1);
   
   //Automatically gate the clock when the train logic is in idle state.
   apb_wr_cmd(DDRPHY_BASE,reg_train_reg_update_en, 0);
    
   //set DQ72bit 
   apb_wr_cmd(DDRPHY_BASE,reg_channel_en,0x1ff);
   
   //#ifdef CMD_2T
   //  apb_wr_cmd(DDRPHY_BASE,reg_cmd_2t_mode_t, 1);
   //  COMMENT("SET PHY CMD 2T MODE");
   //#endif
 
  #ifdef DDR3_UDIMM_MODE
      #ifdef DDR3_MIRROR_MODE
          apb_wr_cmd(DDRPHY_BASE,reg_ddrphy_udimm_addr_mirror, 0x2);
          COMMENT("DDR3 PHY Enable DDR3_MIRROR_MODE");        
      #endif
  #endif


    //set CWL at FP0
    apb_wr_cmd(DDRPHY_BASE,CWL_FRE_OP0,CWL);

    //set CL  at FP0
    apb_wr_cmd(DDRPHY_BASE,CL_FRE_OP0,CL);

    //set AL at FP0
    apb_wr_cmd(DDRPHY_BASE,AL_FRE_OP0,AL);
    

  //for DDR3
  #ifdef S800D
    //The register is used to control the auto-refresh interval when the phy do the training. 
    //The auto-refresh interval = reg_phy_trfc *Tdfi_clk1x.
    apb_wr_cmd(DDRPHY_BASE,reg_phy_trfc, 0x149 );
    //The value to control the timing of the Trefi when the phy is in training mode.Unit:dfi_clk1x.
    apb_wr_cmd(DDRPHY_BASE,reg_phy_trefi, 1360);
  #endif
   
 #ifdef S1066F
    //The register is used to control the auto-refresh interval when the phy do the training. 
    //The auto-refresh interval = reg_phy_trfc *Tdfi_clk1x.
    apb_wr_cmd(DDRPHY_BASE,reg_phy_trfc, 0x149 );
    //The value to control the timing of the Trefi when the phy is in training mode.Unit:dfi_clk1x.
    apb_wr_cmd(DDRPHY_BASE,reg_phy_trefi, 1812);
  #endif
   
 #ifdef DDR1333
    //The register is used to control the auto-refresh interval when the phy do the training. 
    //The auto-refresh interval = reg_phy_trfc *Tdfi_clk1x.
    apb_wr_cmd(DDRPHY_BASE,reg_phy_trfc, 0x149 );
    //The value to control the timing of the Trefi when the phy is in training mode.Unit:dfi_clk1x.
    apb_wr_cmd(DDRPHY_BASE,reg_phy_trefi, 2266);
  #endif
 
 #ifdef DDR1600
    //The register is used to control the auto-refresh interval when the phy do the training. 
    //The auto-refresh interval = reg_phy_trfc *Tdfi_clk1x.
    apb_wr_cmd(DDRPHY_BASE,reg_phy_trfc, 0x149 );
    //The value to control the timing of the Trefi when the phy is in training mode.Unit:dfi_clk1x.
    apb_wr_cmd(DDRPHY_BASE,reg_phy_trefi, 2400);
  #endif
   
   
   //CHOOSE MEM TYPE:DDR3
   apb_wr_cmd(DDRPHY_BASE,mem_select_t, 0x2);

   //------------------------------PHY BASIC CONFIG end---------------------------------------//        

    //Release the PHY reset
    apbm_vip_write(DDRMISC_PHY_RSOK ,1);

    //set DQ_Lane1-DQ_Lane9
    apb_wr_cmd(DDRPHY_BASE,reg_a_l_rxen_lp4,0);
    apb_wr_cmd(DDRPHY_BASE,reg_a_h_rxen_lp4,0);
    apb_wr_cmd(DDRPHY_BASE,reg_b_l_rxen_lp4,0);
    apb_wr_cmd(DDRPHY_BASE,reg_b_h_rxen_lp4,0);
    apb_wr_cmd(DDRPHY_BASE,reg_c_l_rxen_lp4,0);
    apb_wr_cmd(DDRPHY_BASE,reg_c_h_rxen_lp4,0);
    apb_wr_cmd(DDRPHY_BASE,reg_d_l_rxen_lp4,0);
    apb_wr_cmd(DDRPHY_BASE,reg_d_h_rxen_lp4,0);
    apb_wr_cmd(DDRPHY_BASE,reg_e_l_rxen_lp4,0);

   //------------------------------PHY Initial Start---------------------------------------//        
    //Enable the PHY initial by MC:dfi_init_start= 1'b1,
    //dfi_init_start is Quasi-dynamic.
    /////////////////////////////////
    //
    //  set dfi_init_start=1
    //
    /////////////////////////////////
    //set SWCTL.sw_done to 0
    apbm_vip_read(DDRMC_SWCTL,rdword);
    wrword = rdword & ~(1<<0);
    apbm_vip_write(DDRMC_SWCTL,wrword);

    //Set DFIMISC.dfi_init_start to 1
    apbm_vip_read(DDRMC_DFIMISC,rdword);
    wrword = rdword | (1<<5);
    apbm_vip_write(DDRMC_DFIMISC,wrword);
    
    //Set SWCTL.sw_done to 1
    apbm_vip_read(DDRMC_SWCTL,rdword);
    wrword = rdword | (1<<0); 
    apbm_vip_write(DDRMC_SWCTL,wrword);
 
    //Polling SWSTAT.sw_done_ack
    rdword = 0;
    while ((rdword & 1) != 1) 
    {
    apbm_vip_read(DDRMC_SWSTAT,rdword);
    }

    
    ////////////////////////////////////
    //
    //Wait DFISTAT.dfi_init_complete=1
    //
    ////////////////////////////////////
    rdword = 0;
    while ((rdword & 1) != 1) 
    {
    apbm_vip_read(DDRMC_DFISTAT,rdword);
    }

   ///////////////////////////////////////////
   //
   // set dfi_init_start=0
   //
   ///////////////////////////////////////////
   // Set SWCTL.sw_done to 0
    apbm_vip_read(DDRMC_SWCTL,rdword);
    wrword = rdword & ~(1<<0);
    apbm_vip_write(DDRMC_SWCTL,wrword);
   
   //Set DFIMISC.dfi_init_start to 0
    apbm_vip_read(DDRMC_DFIMISC,rdword);
    wrword = rdword & ~(1<<5);
    apbm_vip_write(DDRMC_DFIMISC,wrword);
    
    //Set SWCTL.sw_done to 1
    apbm_vip_read(DDRMC_SWCTL,rdword);
    wrword = rdword | (1<<0); 
    apbm_vip_write(DDRMC_SWCTL,wrword);
 
    //Polling SWSTAT.sw_done_ack
    rdword = 0;
    while ((rdword & 1) != 1) 
    {
    apbm_vip_read(DDRMC_SWSTAT,rdword);
    }
   
   COMMENT("PHY Initial End!!!");
   
//------------------------------PHY Initial End---------------------------------------//        

//-----SRAM Tmining & DFI Timing & SDRAM INIT register config start------------------//                           
// Set SWCTL.sw_done to 0
apbm_vip_read(DDRMC_SWCTL,rdword);
wrword = rdword & ~(1<<0);
apbm_vip_write(DDRMC_SWCTL,wrword);

    
    /////////////////////
    //
    //SDRAM tming regs
    //
    /////////////////////
    wr2pre = roundup(Twr2pre);
    t_faw = roundup(tFAW);

    t_ras_min = tRAS>>1;
    t_ras_max = 26;
    wrword = wr2pre << 24 | t_faw << 16 | t_ras_max << 8 | t_ras_min;
    apbm_vip_write(DDRMC_DRAMTMG0,wrword);

    apbm_vip_read(DDRMC_DRAMTMG0,rdword);
    if(rdword != wrword)
    {
      COMMENT("DDRMC_DRAMTMG0 have static config bits");	
    }

    t_rc = roundup(tRC);

    wrword = Txp<<16 | (Trd2pre/2)<<8 | t_rc<<0; 
    apbm_vip_write(DDRMC_DRAMTMG1,wrword);

    apbm_vip_read(DDRMC_DRAMTMG1,rdword);
    if(rdword != wrword)
    {
      COMMENT("DDRMC_DRAMTMG1 have static config bits");	
    }

    rd2wr = roundup(AL + CL + (8/2) + 2 - AL - CWL);

    wr2rd = roundup(CWL + 8/2 + tWTR);

    apbm_vip_read(DDRMC_DRAMTMG2,rdword);
    wrword =  rd2wr<<8 | wr2rd<<0;
    apbm_vip_write(DDRMC_DRAMTMG2,wrword);

    apbm_vip_read(DDRMC_DRAMTMG2,rdword);
    if(rdword != wrword)
    {
      COMMENT("DDRMC_DRAMTMG2 have static config bits");	
    }

    
    wrword = roundup(tMRD) << 12 | roundup(tMODE) ; 
    apbm_vip_write(DDRMC_DRAMTMG3,wrword);
    apbm_vip_read(DDRMC_DRAMTMG3,rdword);
    if(rdword != wrword)
    {
      COMMENT("DDRMC_DRAMTMG3 have static config bits");	
    }

    wrword = roundup(tRCD-AL) << 24 | roundup(tCCD) << 16 | roundup(tRRD) << 8 | (rounddown(tRP)+1);
    apbm_vip_write(DDRMC_DRAMTMG4,wrword);
    apbm_vip_read(DDRMC_DRAMTMG4,rdword);
    if(rdword != wrword)
    {
      COMMENT("DDRMC_DRAMTMG4 have static config bits");	
    }

    
    /////////////////////
    //
    //DFI tming regs
    //
    /////////////////////

    wrword = Tctrl_delay << 24 | Trdata_en << 16 | Tphy_wrdata<<8 | Tphy_wrlat <<0;
    apbm_vip_write(DDRMC_DFITMG0,wrword);

    apbm_vip_read(DDRMC_DFITMG0,rdword);
    if(rdword != wrword)
    {
      COMMENT("DDRMC_DFITMG0 have static config bits");	
    }


    apbm_vip_read(DDRMC_DFITMG1,rdword);
    word0 = bits_clear(rdword,16,0x1f); 
    word1 = bits_clear(word0 ,8 ,0x1f);   
    word2 = bits_clear(word1 ,0 ,0x1f);   
    wrword = word0 | roundup(Twdata_delay) << 16 | Tdram_clk_enable<<0 | Tdram_clk_disable<<8;
    apbm_vip_write(DDRMC_DFITMG1,wrword);

    apbm_vip_read(DDRMC_DFITMG1,rdword);
    if(rdword != wrword)
    {
      COMMENT("DDRMC_DFITMG1 have static config bits");	
    }

    /////////////////////
    //
    //SRAM init regs
    //
    /////////////////////

    //----MR2 & MR3-------//
    //SMR2
    //Bit[21:22] RTT_WR 2'b00 Dynamic ODT Off
    //Bit24 SRT: Self Refresh Temperature Range 
    //0 - Normal (0C ~ 85C)
    //1 - Extended (0C ~ 95C)
    //Bit25 ASR Auto Self Refresh
    //Bit[26:28] CWL CAS Write Latency  000-5tCK
    //Bit[29:31] PASR Partial Array Self Refresh

    mr2_data = (CWL-5) << 3;
    mr3_data =0;
     
    apbm_vip_write(DDRMC_INIT4,0x0);//clear to all 0.
    wrword = mr2_data <<16 | mr3_data<<0;
    apbm_vip_write(DDRMC_INIT4,wrword);

    //------------MR1 & MR0---------------
    //SMR1
    //BIT[4:3]    :AL=2'b01,AL=CL-1;
    //BIT[8,6,2]  :RTT[2:0]=001,RZQ/4;
    //Bit[5,1]    :ODT=2'b01,RZQ/7;ODT=2'b00,RZQ/6
    //Bit[0]      :DLL=0,enable DLL(normal);
    
    //mr1_data = (`CL-`AL) << 3 | 1 <<2 | 1<<1 | 0;
    mr1_data = (CL-AL) << 3 | 1 <<2 | 0<<1 | 0;


    //SMR0
    //Bit19: PPD
    //0 - Slow Exit         
    //1 - Fast Exit
    //Bit[20:22] tWR 15ns
    // 010-6 cyles
    //Bit23 DLL Not Used
    //Bit24 TM  Not Used
    //Bit[25:27] CL[3:1] 001.0 5 cycles
    //Bit28 RBT Read Burst Type 0-Sequential 1-Reserved
    //Bit29 CL[0] CL LSB
    //Bit[30:31] BL 2'b00 Fixed BL8
    
    if (CL == 5) 
        CL_MODE = 2;
     else if (CL == 6) 
        CL_MODE = 4;
     else if (CL == 7) 
        CL_MODE = 6;
     else if (CL == 8) 
        CL_MODE = 8;
     else if (CL == 9) 
        CL_MODE = 10;
     else if (CL == 10) 
        CL_MODE = 12;
     else if (CL == 11) 
        CL_MODE = 14;
     else 
     {
        COMMENT("Error: CL Not Supported!");
        failed();            
     }

 if (tWR == 5) 
        TWR_MODE = 1;
     else if (tWR == 6) 
        TWR_MODE = 2;
     else if (tWR == 7) 
        TWR_MODE = 3;
     else if (tWR == 8) 
        TWR_MODE = 4;
     else if (tWR == 10) 
        TWR_MODE = 5;
     else if (tWR == 12) 
        TWR_MODE = 6;
     else if (tWR == 14) 
        TWR_MODE = 7;
     else 
     {
        COMMENT("Error: tWR Not Supported!");
        failed();            
    }
  
    //BIT[12] PD=1,DLL on(fast exit)
    //BIT[8]  DLL=1,DLL reset
    mr0_data = 1<<12 | (TWR_MODE) << 9 | 1<<8 | (CL_MODE >>1) << 4 | (CL_MODE & 1) << 2 | BL;
    

    apbm_vip_write(DDRMC_INIT3,0x0);//clear to all 0
    wrword = mr0_data <<16 | mr1_data<<0;
    apbm_vip_write(DDRMC_INIT3,wrword);


//------------------------------SRAM Initial Start---------------------------------------//        
   //This step shoule compelte use the controller!!!
    COMMENT("DDR SRAM Initial Start!!!");

   // Trigger SDRAM INIT
   // Set DFIMISC.dfi_init_complete_en to 1
    apbm_vip_read(DDRMC_DFIMISC,rdword);
    wrword = rdword | (1<<0);
    apbm_vip_write(DDRMC_DFIMISC,wrword);
   // Set PWRCTL.selfref_sw to 0 
    apbm_vip_read(DDRMC_PWRCTL,rdword);
    wrword = rdword & ~(1<<5);
    apbm_vip_write(DDRMC_PWRCTL,wrword);


//Set SWCTL.sw_done to 1
apbm_vip_read(DDRMC_SWCTL,rdword);
wrword = rdword | (1<<0);
apbm_vip_write(DDRMC_SWCTL,wrword);
//polling SWSTAT 
rdword = 0;
while ((rdword & 1) != 1) 
{
apbm_vip_read(DDRMC_SWSTAT,rdword);
}



//---------------Wait for DWC_ddr_umctl2 to move to normal operating mode by monitoring STAT.operation_mode signal-----------------//
    rdword = 0;
    while ((rdword & 0x3) != 1) 
    {
    apbm_vip_read(DDRMC_STAT,rdword);
    }

   /*
      //RFSHCTL3.dis_auto_refresh = 0
      apbm_vip_read(DDRMC_RFSHCTL3,rdword);
      wrword = rdword & ~(1<<0);
      apbm_vip_write(DDRMC_RFSHCTL3,wrword);

      //PWRCTL.powerdown_en keep 0
      //PWRCTL.selfref_en keep 0
      //PWRCTL.en_dfi_dram_clk_disable = 1
      apbm_vip_read(DDRMC_PWRCTL,rdword);
      wrword = rdword | (1<<3);
      apbm_vip_write(DDRMC_PWRCTL,wrword);
   */


    //Write PCTRL.port_en = 1, AXI ports are no longer blocked from taking transaction
    //Write SBRCTL.scrub_en = 1, Enable SBR if desired 

    apbm_vip_read(DDRMC_PCTRL_0,rdword);
    wrword = rdword | (1<<0);
    apbm_vip_write(DDRMC_PCTRL_0,wrword);

    //apbm_vip_read(DDRMC_SBRCTL,rdword);
    //wrword = rdword | (1<<0);
    //apbm_vip_write(DDRMC_SBRCTL,wrword);

COMMENT("DDR SDRAM Initial DONE");


//----------------------------------Write Leveling Start-------------------------------------------------------//
 //******************************************
 //
 //RFSHCTL3.dis_auto_refresh = 1,
 //close atuo-refresh
 //
 //********************************************
 apbm_vip_read(DDRMC_RFSHCTL3,rdword);
 wrword = rdword | (1<<0);
 apbm_vip_write(DDRMC_RFSHCTL3,wrword);


COMMENT("RANK0 write_leveling_start >>>");
//sel RANK0
ddrn_phy_write_leveling(0xe);
#ifdef DDR_RANK_1  
COMMENT("RANK1 write_leveling_start >>>");
ddrn_phy_write_leveling(0xd);
#endif


//-------------------------------------DSQ-gating CALIB Start------------------------------------------------//
COMMENT("RANK0 RX DQS Gating Start >>>");

ddrn_phy_dqs_calib(0xe);
#ifdef DDR_RANK_1
COMMENT("RANK1 RX DQS Gating Start >>>");
ddrn_phy_dqs_calib(0xd);
#endif


//-------------------------------------Read-train Start------------------------------------------------//
COMMENT("RANK0 Read Training Start>>>");

ddrn_phy_rd_training(0xe);
#ifdef DDR_RANK_1
COMMENT("RANK1 Read Training Start>>>");
ddrn_phy_rd_training(0xd);
#endif

//-------------------------------------Write-train Start------------------------------------------------//
COMMENT("RANK0 Write Training Start>>>");
ddrn_phy_wr_training(0xe);
#ifdef DDR_RANK_1
COMMENT("RANK1 Write Training Start>>>");
ddrn_phy_wr_training(0xd);
#endif


//--------------------------------------Normal operation------------------------------------------------//
COMMENT("Normal WR >>>");
}
#endif








#ifndef DDR_STUB
    #ifdef DDR4
        void ddr_init()
        {
          COMMENT("DDR4 INIT START...");
          ddr4_init();
        }
    #else
        void ddr_init()
        {
        	// uart0_16750_init(UART_XTAL, 115200);
        	COMMENT("DDR3 INIT START...");
          ddr3_init();       
        }
    #endif
#else
    void ddr_init() 
    {
      COMMENT("DDR stub in ddr_init()");
    }
#endif

