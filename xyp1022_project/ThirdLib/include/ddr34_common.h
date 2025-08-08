#define DDR3
//#define S800D
//#define S1066F
#define DDR1600
//#define X8



#ifdef DDR4
      #ifdef RDIMM_MODE
      #define RDIMM 1
      #else //UDIMM
      #define RDIMM 0
      #endif
      
      #ifdef DDR4_1600 
      #define tWR 12    
      #else
          #ifdef DDR4_1866
          #define tWR 14    
          #else
             #ifdef DDR4_2133
             #define tWR 16   
             #else
             #define tWR 16
             #endif
          #endif
      #endif
      
      
      #ifdef DDR4_1600 // DDR 11-11-11 Spec
          #define FreqCK 1600
          #define tRCD 11  // 13.75ns/tCK
          #define tRP  11  // 13.75ns/tCK ??
          #define tRC  39 // 48.75ns/tCK
          #define tRAS 28 // 35ns/tCK
          #define tWTR 7  // MAX{7.5ns, 4Tck} 6Tck for safe
          #define tRTP 7  // MAX{7.5ns, 4Tck}
          #define tRRD 6  // MAX{6ns,  4Tck} 
          #define tRAS_MAX 72 // 90ns/tCK
      
          #define CL  11  
          #define AL (CL-1)
          #define CWL 9
          //#define PL  4
          #define PL  0
      
          #define Tctrl_delay        6     
          #define Tphy_wrdata        0 
          #define Tphy_wrlat        (((CWL+AL -3)>>1)-1) 
          #define Trdata_en         ( (CL+AL-3)>>1     )
          #define Tphy_rdlat         32
          #define Tdram_clk_disable  3
          #define Tdram_clk_enable   7
          #define Twdata_delay       9
          
          
          #define BL  0 // Fixed BL8
          #define tFAW 24 // 30ns/tCK
      
          #define Twr2pre (AL + CWL + 8/2 + tWR)  
          #define Txp   3
          #define Trd2pre (AL + tRTP)  
     
          #define tCCD_L  5
      #endif
      
      #ifdef DDR4_1866 // DDR 11-11-11 Spec
          #define FreqCK 1866
          #define tRCD 13  // 13.75ns/tCK
          #define tRP  13  // 13.75ns/tCK ??
          #define tRC  45 // 48.75ns/tCK
          #define tRAS 32 // 35ns/tCK
          #define tWTR 8  // MAX{7.5ns, 4Tck} 6Tck for safe
          #define tRTP 8  // MAX{7.5ns, 4Tck}
          #define tRRD 6  // MAX{6ns,  4Tck} 
          #define tRAS_MAX 83 // 90ns/tCK
      
          #define CL  13  
          #define AL (CL-1)
          #define CWL 10
          //#define PL  4
          #define PL  0
      
          #define Tctrl_delay        6     
          #define Tphy_wrdata        0 
          #define Tphy_wrlat        (((CWL+ AL -3)>>1)-1) 
          #define Trdata_en         ( (CL+AL-3)>>1     )
          #define Tphy_rdlat         32
          #define Tdram_clk_disable  3
          #define Tdram_clk_enable   7
          #define Twdata_delay       9
          
          #define BL  0 // Fixed BL8
          #define tFAW 24 // 30ns/tCK
      
          #define Twr2pre (AL + CWL + 8/2 + tWR)  
          #define Txp   3
          #define Trd2pre (AL + tRTP)  
     
          #define tCCD_L    5
      #endif
      
      #ifdef DDR4_2133 // 
          #define FreqCK 2133
          #define tRCD 15  // 
          #define tRP  15  // 13.75ns/tCK ??
          #define tRC  51 // 48.75ns/tCK
          #define tRAS 36 // 35ns/tCK
          #define tWTR 9  // MAX{7.5ns, 4Tck} 6Tck for safe
          #define tRTP 9  // MAX{7.5ns, 4Tck}
          #define tRRD 6  // MAX{6ns,  4Tck} 
          #define tRAS_MAX 96// 90/tCK
      
          #define CL  15  
          #define AL (CL-1)
          #define CWL 11
          //#define PL  4
          #define PL  0
      
          #define Tctrl_delay        6     
          #define Tphy_wrdata        0 
          #define Tphy_wrlat        (((CWL +AL -3)>>1)-1) 
          #define Trdata_en         ( (CL+AL-3)>>1     )
          #define Tphy_rdlat         32
          #define Tdram_clk_disable  3
          #define Tdram_clk_enable   7
          #define Twdata_delay       9
          
          #define BL  0 // Fixed BL8
          #define tFAW 24 // 30ns/tCK
      
          #define Twr2pre (AL + CWL + 8/2 +tWR)  
          #define Txp   3
          #define Trd2pre (AL + tRTP)  
    
          #define tCCD_L    6
      #endif
      
      
          #define tZQinit 1024 
      
      #define tMODE 24
      #define tMRD  8

      
      
      
      #define bit_clear(r,b) (r &= ~(1<<b))
      #define bits_clear(r,b,m)  (r &= ~(m<<b))
      
      #define RANK_START_ADDRBIT (29-6)
      #define BANK_START_ADDRBIT (22)
      
      #define apbm_vip_read(addr,rddata)   LDW(rddata,addr)
      #define apbm_vip_write(addr,wrdata)  STW(wrdata,addr)
      

#else //DDR3
      #ifdef RDIMM_MODE
      #define RDIMM 1
      #else //UDIMM
      #define RDIMM 0
      #endif
      
      #ifdef DDR1600 
      #define tWR 14  //min=15ns/tCK
      #else
           #ifdef DDR1333
           #define tWR 10    // 15ns/tCK
           #else
              #ifdef S1066F
              #define tWR 8    // 15ns/tCK
              #else
              #define tWR 6
              #endif
           #endif
      #endif
      
      
      #ifdef S800D // 800MHz 5-5-5 Spec
          #define FreqCK 400
          #define tRCD 6  // 15ns/tCK
          #define tRP  6  // 15ns/tCK
          #define tRC  21 // 52.5ns/tCK
          #define tRAS 15 // 37.5ns/tCK
          #define tWTR 6  // 10ns/tCK Write to Read Delay
          #define tRTP 6  // 10ns/tCK READ to PRE delay
          #define tRRD 6  // 10ns/tCK ACT to ACT delay
      
          #define CL  6
          #define AL (CL-1)
          #define CWL 5
      
          #define Tctrl_delay        6     
          #define Tphy_wrdata        0 
          #define Tphy_wrlat        (((CWL + AL -3)>>1)-1) 
          #define Trdata_en         ( (CL+AL-3)>>1     )
          #define Tphy_rdlat         32
          #define Tdram_clk_disable  3
          #define Tdram_clk_enable   7
          #define Twdata_delay       9

          #define BL  0 // Fixed BL8
          #define tFAW 20 // 40ns/tck for 1KB page, 50ns/tck for 2KB page
      
          #define Twr2pre (AL + CWL + 8/2 + tWR)  
          #define Txp   3
          #define Trd2pre (AL + tRTP)  
      
      #endif
      
      #ifdef S1066F // 1066MHz 7-7-7 Spec
          #define FreqCK 1066
          #define tRCD 7  // 13.13ns/tCK
          #define tRP  8  // 13.13ns/tCK
          #define tRC  27 // 50.63ns/tCK
          #define tRAS 20 // 37.5ns/tCK
          #define tWTR 5  // MAX{7.5ns, 4Tck} 6Tck for safe
          #define tRTP 5
          #define tRRD 5  // MAX{7.5ns, 4Tck}        
      
          #define CL  7  
          #define AL (CL-1)
          #define CWL   6 
      
          #define Tctrl_delay        6 
          #define Tphy_wrdata        0 
          #define Tphy_wrlat        (((CWL + AL -3)>>1)-1) 
          #define Trdata_en          ((CL+AL-3)>>1    )
          #define Tphy_rdlat         32
          #define Tdram_clk_disable  3
          #define Tdram_clk_enable   7
          #define Twdata_delay       9
          
          #define BL  0 // Fixed BL8
          #define tFAW 24 // 30ns/tCK
      
          #define Twr2pre (AL + CWL + 8/2 + tWR)  
          #define Txp   3
          #define Trd2pre (AL + tRTP)  
      
      #endif
      
      #ifdef DDR1333 // DDR 9-9-9 Spec
          #define FreqCK 1333
          #define tRCD  9 // 13.5ns/tCK
          #define tRP   10 // 13.5ns/tCK ??
          #define tRC   34 // 49.5ns/tCK
          #define tRAS  25  // 36ns/tCK
          #define tWTR 6  // MAX{7.5ns, 4Tck} 6Tck for safe
          #define tRTP 6  // MAX{7.5ns, 4Tck}
          #define tRRD 5  // MAX{6ns,  4Tck} 
      
          #define CL  9  
          #define AL (CL-1)
          #define CWL 7
      
          #define Tctrl_delay        6     
          #define Tphy_wrdata        0 
          #define Tphy_wrlat        (((CWL +AL -3)>>1)-1) 
          #define Trdata_en         ( (CL+AL-3)>>1       )
          #define Tphy_rdlat         32
          #define Tdram_clk_disable  3
          #define Tdram_clk_enable   7
          #define Twdata_delay       9
          
          #define BL  0 // Fixed BL8
          #define tFAW 24 // 30ns/tCK
      
          #define Twr2pre (AL + CWL + 8/2 + tWR)  
          #define Txp   3
          #define Trd2pre (AL + tRTP)  
      
      #endif
      
      
      
      #ifdef DDR1600 // DDR 11-11-11 Spec
          #define FreqCK 1600
          #define tRCD 11  // 13.75ns/tCK
          #define tRP  12  // 13.75ns/tCK ??
          #define tRC  39 // 48.75ns/tCK
          #define tRAS 28 // 35ns/tCK
          #define tWTR 7  // MAX{7.5ns, 4Tck} 6Tck for safe
          #define tRTP 7  // MAX{7.5ns, 4Tck}
          #define tRRD 6  // MAX{6ns,  4Tck} 
      
          #define CL  11  
          #define AL (CL-1)
          #define CWL 8     
      
          #define Tctrl_delay        6     
          #define Tphy_wrdata        0 
          #define Tphy_wrlat        (((CWL+AL -3)>>1)-1) 
          #define Trdata_en         ( (CL+AL-3)>>1     )
          #define Tphy_rdlat         32
          #define Tdram_clk_disable  3
          #define Tdram_clk_enable   7
          #define Twdata_delay       9
          
          #define BL  0 // Fixed BL8
          #define tFAW 24 // 30ns/tCK
      
          #define Twr2pre (AL + CWL + 8/2 + tWR)  
          #define Txp   3
          #define Trd2pre (AL + tRTP)  
      
      #endif
      
          #define tZQinit 512
      
      #ifdef M2048                                                                                                                        
          #ifdef DDR3_32BIT
              #define DDR_M_SZ 7 //1GB
          #else                                           
              #define DDR_M_SZ 8 //2GB
          #endif
          #define DDR_ADDR_MODE 2 //nCOL_T = 10, NX10(8-bank)
          #define T_RFC_XPR (((170*FreqCK)%1000) ? (((170*FreqCK)/1000) + 1): ((170*FreqCK)/1000))  // 170ns/tCK
          #define T_REFI (((1953*FreqCK)%1000) ? (((1953*FreqCK)/1000) + 1): ((1953*FreqCK)/1000))  // 7.8us/tCK - 32768 lines
      #endif
      
      #ifdef M1024                                                                                                                       
          #ifdef DDR3_32BIT
              #define DDR_M_SZ 6 //512MB
          #else                                           
              #define DDR_M_SZ 7 //1GB
          #endif
          #define DDR_ADDR_MODE 2 //nCOL_T = 10, NX10(8-bank)
          #define T_RFC_XPR (((170*FreqCK)%1000) ? (((170*FreqCK)/1000) + 1): ((170*FreqCK)/1000))  // 170ns/tCK
          #define T_REFI (((1953*FreqCK)%1000) ? (((1953*FreqCK)/1000) + 1): ((1953*FreqCK)/1000))  // 7.8us/tCK - 32768 lines
      #endif
      
      #define tXSDLL 512 //512, set 32 only for simulaiton
      #define tDLLK  512 // DRAM DLL Locked Cycle
      #define tMODE 12
      #define tMRD  4
      #define tCCD  4 //-simulation
      
      
      #define bit_clear(r,b) (r &= ~(1<<b))
      #define bits_clear(r,b,m)  (r &= ~(m<<b))
      
      #define RANK_START_ADDRBIT (29-6)
      #define BANK_START_ADDRBIT (22)
      
      #define apbm_vip_read(addr,rddata)   LDW(rddata,addr)
      #define apbm_vip_write(addr,wrdata)  STW(wrdata,addr)
      

#endif // end DDR3/4
