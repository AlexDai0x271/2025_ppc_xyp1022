
#ifndef __PPC_TIMER__
#define __PPC_TIMER__

#define PCTBEN 	0xE80E41A0ull		//by T1040RM
#define HID0 1008

#define HZ  (12*1000*1000)			//EVB,12M
#define MHZ (133*10)				//CPU speed, EVB

//#define HZ  (7494*100)			//749.4Khz,FPGA
//#define MHZ (40)					//40 48 60, CPU, FPGA

#define PSRPRITF ee_printf
extern unsigned long long get_tb(void);
extern unsigned long long get_tb_f(void);
#endif
