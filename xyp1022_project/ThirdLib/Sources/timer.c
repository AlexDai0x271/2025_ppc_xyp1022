
#include "ppctimer.h"
#include "e500mc_asm.h"
#include "e500mc.h"

volatile unsigned int dec_irq_flag;
volatile unsigned int fit_irq_flag;
volatile unsigned int wdog_irq_flag;

/*
tb_t clock_data;
tb_t *clock_data_ptr;

void timer_init(void)
{
	clock_data_ptr = &clock_data;
	clock_data_ptr->tbu = 0;
	clock_data_ptr->tbl = 0;
	ppcMttb(clock_data_ptr);
}

unsigned long long timer_get_cpu_clock(void)
{
	long long time = 0;

	ppcMftb(clock_data_ptr);
	time = clock_data_ptr->tbu;
	time = time << 32;
	time = time | clock_data_ptr->tbl;

	return time;
}

unsigned long timer_get_us(void)
{
	long long time = 0;

	ppcMftb(clock_data_ptr);
	time = clock_data_ptr->tbu;
	time = time << 32;
	time = time | clock_data_ptr->tbl;

	time = time/CPU_CLOCK;
	return time&0xffffffff;
}

unsigned long timer_get_ms(void)
{
	long long time = 0;

	ppcMftb(clock_data_ptr);
	time = clock_data_ptr->tbu;
	time = time << 32;
	time = time | clock_data_ptr->tbl;

	time = time/CPU_CLOCK/1000;
	return time&0xffffffff;
}
*/
/*
float get_time(void)
{
	float timeL;
	float timeH;
	float time;

	ppcMftb(clock_data_ptr);
	timeL = clock_data_ptr->tbl;
	timeH = clock_data_ptr->tbu;

	time = (timeL/CPU_CLOCK/1000/1000) + (timeH*4293.00/CPU_CLOCK);

	return time;
}
*/

void delaySysClk(nClk)
{
	unsigned int i = 0;

	for(i = 0; i < (nClk/4); i++)
	{
		//ISYNC;
		NOP;
	}
}

void set_time_base(unsigned int tbu,unsigned int tbl)
{
	unsigned int hid0=0;
	MTSPR(TBL_W,0);
	MTSPR(TBU_W,tbu);
	MTSPR(TBL_W,tbl);
//	MFSPR(hid0,HID0);
//	MTSPR(HID0,hid0|0x4000);
}

void get_time_base(unsigned int *tbu,unsigned int *tbl)
{
	unsigned int tmp=0;

	do
	{
		MFSPR(*tbu,TBU);
		MFSPR(*tbl,TBL);
		MFSPR(tmp,TBU);
	}while(*tbu!=tmp);
}

unsigned long long get_tb(void)
{
	unsigned long long tb;

	unsigned int  tbu;
	unsigned int  tbl;

	get_time_base(&tbu,&tbl);
		
	tb =(unsigned long long)(tbu);
	tb =(unsigned long long)(tb<<32|tbl);

	return (unsigned long long)tb;
}

unsigned long long get_tb_f(void)
{
	return (unsigned long long)(get_tb());
}

void timer_register_test()
{
	unsigned int data32, data;
	
	MFSPR(data32, TBL_R);
	uart_printf("######### timer test value TBL_R = [0x%x]\r\n", data32);
	MFSPR(data32, TBU_R);
	uart_printf("######### timer test value TBU_R = [0x%x]\r\n", data32);
	MFSPR(data32, ATBL);
	uart_printf("######### timer test value ATBL = [0x%x]\r\n", data32);
	MFSPR(data32, ATBU);
	uart_printf("######### timer test value ATBU = [0x%x]\r\n", data32);
	MFSPR(data32, DEC);
	uart_printf("######### timer test value DEC = [0x%x]\r\n", data32);
	MFSPR(data32, TCR);
	uart_printf("######### timer test value TCR = [0x%x]\r\n", data32);
	MFSPR(data32, TSR);
	uart_printf("######### timer test value TSR = [0x%x]\r\n", data32);
	
	data = 0x00000000;
	MTSPR(TBL_W, data);
	MFSPR(data32, TBL_R);
	uart_printf("######### timer test value TBL_R = [0x%x]\r\n", data32);

	data = 0x50000000;
	MTSPR(TBU_W, data);
	MFSPR(data32, TBU_R);
	uart_printf("######### timer test value TBU_R = [0x%x]\r\n", data32);

	data = 0x20000000;
	MTSPR(DEC, data);
	MFSPR(data32, DEC);
	uart_printf("######### timer test value DEC = [0x%x]\r\n", data32);

	data = 0xffffffff;
	MTSPR(TSR, data);
	MFSPR(data32, TSR);
	uart_printf("######### timer test value TSR = [0x%x]\r\n", data32);

}

void tbl_test()
{
	unsigned int i, data32 = 0;
	unsigned int tmp1 = 0;
	unsigned int tmp2 = 0;
	
	MFSPR(data32, TBL_R);
	uart_printf("######### timer test value TBL_R = [0x%x]\r\n", data32);

	MTSPR(TBL_W, data32);

	for(i=0; i<0x20; i++)
	{
		MFSPR(tmp1, TBL_R);
		delay(10);
		MFSPR(tmp2, TBL_R);

		if(tmp1 == tmp2)
			uart_printf(" tbl test error !\n");
		else
			uart_printf("tmp1=[0x%x], tmp2=[0x%x]\n", tmp1, tmp2);
	}
}

void tbu_test()
{
	unsigned int i, data32 = 0;
	unsigned int tmp1 = 0;
	unsigned int tmp2 = 0;
	
	MTSPR(TBU_W, data32);
	MTSPR(TBL_W, data32);

	for(i=0; i<0x20; i++)
	{
		MFSPR(tmp1, TBU_R);
		if(tmp1)
			uart_printf(" tbu test error !\n");

		MTSPR(TBL_W, 0xffffffff);
		delay(10);
		MFSPR(tmp1, TBU_R);
		if(tmp1 != 0x1)
			uart_printf(" tbu test error !\n");

		MTSPR(TBU_W, data32);
		MTSPR(TBL_W, data32);

		delay(10);
		MFSPR(tmp1, TBU_R);
		if(tmp1)
			uart_printf(" tbu test error !\n");
	}
	uart_printf(" tbu test ok !\n");
}

void atb_test()
{
	unsigned int i, data32 = 0;
	unsigned int tmp1 = 0;
	unsigned int tmp2 = 0;
	
	MFSPR(data32, ATBL);
	uart_printf("######### timer test value ATBL = [0x%x]\r\n", data32);

	for(i=0; i<0x20; i++)
	{
		MFSPR(tmp1, ATBL);
		delay(10);
		MFSPR(tmp2, ATBL);

		if(tmp1 == tmp2)
			uart_printf(" tbl test error !\n");
	}

	uart_printf(" atb test ok !\n");
}

void tb_count_test()
{
	unsigned int i, data32 = 0;
	int tmp1 = 0;
	int tmp2 = 0;
	
	MTSPR(TBU_W, data32);
	
	MFSPR(tmp1, TBL_R);
	delay(0x100000);
	MFSPR(tmp2, TBL_R);
	uart_printf(" the time cost %d s !\n", (tmp2-tmp1)/HZ);
}

void dec_test()
{
	unsigned int data32 = 0, i;
	uart_printf(" dec test \n");
	
	MTSPR(TCR, TCR_DIE_DISEN);
	
	for(i=0; i<0x1; i++)
	{
		//cpu 749.4Khz, div=0, 10s
		data32 = 0x725970;
		MTSPR(DEC, data32);
		do{
			MFSPR(data32, TSR);
		}while(!(data32 & TSR_DIS_MASK));
		
		//clear DIS status
		MTSPR(TSR, TSR_DIS_MASK);
		
		uart_printf(" dec0 exception occour ---- ok \n");
	}
}

void decar_test()
{
	unsigned int data32 = 0,i = 0;
	uart_printf(" decar test \n");
	
	//cpu 749.4Khz, div=0, 10s
	data32 = 0x725970;

	MTSPR(DEC, data32);
	MTSPR(DECAR, data32);
	data32 = TCR_DIE_DISEN | TCR_ARE_MASK;
	MTSPR(TCR, data32);
	
	while(i<0x4)
	{
		do{
			MFSPR(data32, TSR);
		}while(!(data32 & TSR_DIS_MASK));
		
		//clear DIS status
		MTSPR(TSR, TSR_DIS_MASK);
		
		uart_printf(" dec1 exception occour ---- ok \n");
		i++;
	}
}

void isr_dec()
{
	unsigned int data32 = 0;

	uart_printf(" ######## go into dec isr ######## \n");
	dec_irq_flag = 1;

	//clear DIS status
	MTSPR(TSR, TSR_DIS_MASK);	
}

void isr_decar1()
{
	unsigned int data32 = 0;

	uart_printf(" ######## go into decar1 isr ######## \n");
	dec_irq_flag = 1;
	//clear DIS status
	MTSPR(TSR, TSR_DIS_MASK);	
}


void dec_irq_test()
{
	unsigned int data32 = 0,i = 0;
	dec_irq_flag = 0;

	InstallInterruptHandler0(202, isr_dec);

	data32 = 0x5555;
	while(i<0x1){
		MTSPR(DEC, data32);
		MTSPR(TCR, TCR_DIE_MASK);
		
		while(!dec_irq_flag);
		uart_printf(" ######## dec_irq test ok ######## \n");
		dec_irq_flag = 0;
		i++;
	}
}
/*
 * the real number count is determined by the data of DECAR
 */
void decar_irq1_test()
{
	unsigned int data32 = 0;
	dec_irq_flag = 0;

	InstallInterruptHandler0(202, isr_decar1);

	data32 = 0x1000;
	MTSPR(DEC, data32);
	data32 = 0x55555;
	MTSPR(DECAR, data32);
	
	while(1){
		MTSPR(TCR, TCR_DIE_MASK | TCR_ARE_MASK);
		while(!dec_irq_flag);
		uart_printf(" ######## decar_irq1 test ok ######## \n");
		dec_irq_flag = 0;
	}
}

void decar_irq2_test()
{
	unsigned int data32 = 0;
	dec_irq_flag = 0;

	InstallInterruptHandler0(202, isr_decar1);
	
	data32 = 0x72597*5;			//5s
	MTSPR(DEC, data32);
	data32 = 0x72597;			//1s
	MTSPR(DECAR, data32);
	MTSPR(TCR, TCR_DIE_MASK | TCR_ARE_MASK);

	while(!dec_irq_flag);
	uart_printf(" ######## decar_irq2 test ok ######## \n");
//	while(1);
}


void fit_test_test1()
{
	unsigned int data32, i=0, tmp1, tmp2;
	uart_printf(" fit test !\n");

/*exception on a transition when TBL byte3_bit7 from 0 to 1 */
	data32 = TCR_FP_TBL_BYTE3 | TCR_FP_BIT7 | TCR_FIE_DISEN;
	MTSPR(TCR, data32);
	data32 = 0x7e000000;
	MTSPR(TBL_W, data32);
	data32 = 0x00000000;
	MTSPR(TBU_W, data32);
	
	while(i<0x4)
	{
		do{
			MFSPR(data32, TSR);
		}while(!(data32 & TSR_FIS_MASK));
	
		MFSPR(tmp1, TBL_R);
		MFSPR(tmp2, TBU_R);
		//clear FIS status
		MTSPR(TSR, TSR_FIS_MASK);
		
		uart_printf(" fit exception occour ---- ok, TBL:0x%x, TBU:0x%x\n", tmp1, tmp2);
		i++;
		data32 = 0x7e000000;
		MTSPR(TBL_W, data32);
	}
}

void fit_test_test2()
{
	unsigned int data32, i=0, tmp1, tmp2;
	uart_printf(" fit test !\n");

/*exception on a transition when TBU byte0_bit0 from 0 to 1 */
	data32 = TCR_FP_TBU_BYTE0 | TCR_FP_BIT0 | TCR_FIE_DISEN;
	MTSPR(TCR, data32);
	data32 = 0xfe000000;
	MTSPR(TBL_W, data32);
	data32 = 0x00000000;
	MTSPR(TBU_W, data32);
	
	while(i<0x4)
	{
		do{
			MFSPR(data32, TSR);
		}while(!(data32 & TSR_FIS_MASK));
	
		MFSPR(tmp1, TBL_R);
		MFSPR(tmp2, TBU_R);
		//clear FIS status
		MTSPR(TSR, TSR_FIS_MASK);
		
		uart_printf(" fit exception occour ---- ok, TBL:0x%x, TBU:0x%x\n", tmp1, tmp2);
		i++;
		
		data32 = 0xfe000000;
		MTSPR(TBL_W, data32);

		MTSPR(TBU_W, tmp2+1);
	}
}

void fit_test_test3()
{
	unsigned int data32, i=0, tmp1, tmp2;
	uart_printf(" fit test !\n");

/*exception on a transition when TBL byte2_bit5 from 0 to 1 */
	data32 = TCR_FP_TBL_BYTE2 | TCR_FP_BIT5 | TCR_FIE_DISEN;
	MTSPR(TCR, data32);
	data32 = 0x00000000;
	MTSPR(TBL_W, data32);
	data32 = 0x00000000;
	MTSPR(TBU_W, data32);
	
	while(i<0x4)
	{
		do{
			MFSPR(data32, TSR);
		}while(!(data32 & TSR_FIS_MASK));
	
		MFSPR(tmp1, TBL_R);
		MFSPR(tmp2, TBU_R);
		//clear FIS status
		MTSPR(TSR, TSR_FIS_MASK);
		
		uart_printf(" fit exception occour ---- ok, TBL:0x%x, TBU:0x%x\n", tmp1, tmp2);
		i++;
		
		data32 = 0x00000000;
		MTSPR(TBL_W, data32);
	}
}

void isr_fit()
{
	unsigned int tmp1, tmp2, data32 = 0;

	uart_printf(" ######## go into fit isr ######## \n");

	MFSPR(tmp1, TBL_R);
	MFSPR(tmp2, TBU_R);
	//clear FIS status
	MTSPR(TSR, TSR_FIS_MASK);
	
	fit_irq_flag = 1;
	uart_printf(" fit exception occour ---- ok, TBL:0x%x, TBU:0x%x\n", tmp1, tmp2);
}

void fit_test_irq()
{
	unsigned int data32 = 0;
	fit_irq_flag = 0;
	
	InstallInterruptHandler0(203, isr_fit);
	
	/*exception on a transition when TBU byte0_bit0 from 0 to 1 */
	data32 = TCR_FP_TBU_BYTE0 | TCR_FP_BIT1 | TCR_FIE_MASK;	
//	data32 = TCR_FP_TBL_BYTE1 | TCR_FP_BIT1 | TCR_FIE_MASK;

	MTSPR(TCR, data32);
	data32 = 0xfff00000;
	MTSPR(TBL_W, data32);
	data32 = 0x00000001;
	MTSPR(TBU_W, data32);
		
	while(!fit_irq_flag);
	uart_printf(" ######## fit_irq test ok ######## \n");
//	while(1);
}

/*
 * trigger when TBL byte1_bit5 from 0 to 1
 * clear TSR and continue
 */
void wdog_test1()
{
	unsigned int data32 = 0, i = 0,data, tmp1, tmp2, tmp3;
	
	/*exception on a transition when TBL byte1_bit5 from 0 to 1 */
	data32 = TCR_WDOG_BYTE1 | TCR_WDOG_BIT5 | TCR_WRC_MASK | TCR_WIE_DISEN;
	MTSPR(TCR, data32);
	MFSPR(data, TCR);

	data32 = 0;
	MTSPR(TBL_W, data32);
	
	//clear ENW/WIS
	data32 = 0xc0000000;
	MTSPR(TSR, data32);
	
	while(i<0x4)
	{
		MFSPR(data32, TSR);
		if((data32 & TSR_ENW_MASK) == TSR_ENW_MASK)
		{
			MFSPR(tmp1, TBL_R);
			MFSPR(tmp2, TBU_R);
			MFSPR(tmp3, TSR);
			uart_printf(" wdog exception occour ---- ok, TBL:0x%x, TBU:0x%x, TSR:0x%x\n", tmp1, tmp2, tmp3);
			i++;
			//clear ENW/WIS
			MTSPR(TSR, 0xc0000000);
			continue;
		}		
	}
}

/*
 * trigger when TBL byte2_bit3 from 0 to 1
 * clear TSR and continue
 */
void wdog_test2()
{
	unsigned int data32 = 0, tmp1, tmp2, tmp3;
	
	/*exception on a transition when TBL byte2_bit3 from 0 to 1 */
	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_CORE | TCR_WIE_DISEN;
	MTSPR(TCR, data32);
	data32 = 0;
	MTSPR(TBL_W, data32);
	
	//clear ENW/WIS
	data32 = 0xc0000000;
	MTSPR(TSR, data32);
	
	while(1)
	{
		MFSPR(data32, TSR);
		if((data32 & TSR_ENW_MASK) == TSR_ENW_MASK)
		{
			MFSPR(tmp1, TBL_R);
			MFSPR(tmp2, TBU_R);
			MFSPR(tmp3, TSR);
			uart_printf(" wdog exception occour ---- ok, TBL:0x%x, TBU:0x%x, TSR:0x%x\n", tmp1, tmp2, tmp3);
			
			/* will goto ROM_Start at 0xffffxxxx */
			while(1);
		}		
	}
}


/*
 * trigger when TBL byte2_bit3 from 0 to 1
 * clear TSR and continue
 */
void wdog_test3()
{
	unsigned int data32 = 0, tmp1, tmp2, tmp3;
	
	/*exception on a transition when TBL byte2_bit3 from 0 to 1 */
	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_SYSTEM | TCR_WIE_DISEN;
	MTSPR(TCR, data32);
	data32 = 0;
	MTSPR(TBL_W, data32);
	
	//clear ENW/WIS
	data32 = 0xc0000000;
	MTSPR(TSR, data32);
	
	while(1)
	{
		MFSPR(data32, TSR);
		if((data32 & TSR_ENW_MASK) == TSR_ENW_MASK)
		{
			MFSPR(tmp1, TBL_R);
			MFSPR(tmp2, TBU_R);
			MFSPR(tmp3, TSR);
			uart_printf(" wdog exception occour ---- ok, TBL:0x%x, TBU:0x%x, TSR:0x%x\n", tmp1, tmp2, tmp3);
			
			/* will trigger system_reset */
			while(1);
		}		
	}
}


/* wdog irq test , when TBL *3, trigger and clear ENW | WIS */
void isr_wdog()
{
	unsigned int tmp1, tmp2, tmp3, data32 = 0;

	uart_printf(" ######## go into wdog isr ######## \n");

	MFSPR(tmp1, TBL_R);
	MFSPR(tmp2, TBU_R);
	MFSPR(tmp3, TSR);
	
	wdog_irq_flag = 1;
	// tmp1 shoulde be 0x400000 *2 *x
	uart_printf(" wdog exception occour ---- ok, TBL:0x%x, TBU:0x%x, TSR:0x%x\n", tmp1, tmp2, tmp3);
	
	//clear ENW WIS and TSR[WRS] status, if not clear, when TCR set WRC, then will trigger reset
//	MTSPR(TSR, (TSR_ENW_MASK | TSR_WIS_MASK | TSR_WRS_SYSTEM | TSR_WRS_CORE));	
	MTSPR(TSR, (TSR_ENW_MASK | TSR_WIS_MASK));	
}


/* wdog irq test for system/core reset, when TBL *3, trigger wdog_int but not clear ENW | WIS, so will trigger system/core reset */
void isr_wdog2()
{
	unsigned int tmp1, tmp2, tmp3, data32 = 0;

	uart_printf(" ######## go into wdog isr ######## \n");

	MFSPR(tmp1, TBL_R);
	MFSPR(tmp2, TBU_R);
	MFSPR(tmp3, TSR);
	
	wdog_irq_flag = 1;
	// tmp1 shoulde be 0x400000 *2 *x
	uart_printf(" wdog exception occour ---- ok, TBL:0x%x, TBU:0x%x, TSR:0x%x\n", tmp1, tmp2, tmp3);
	
	//not clear ENW WIS and TSR[WRS] status, if not clear, when TCR set WRC, then will trigger reset
//	MTSPR(TSR, (TSR_ENW_MASK | TSR_WIS_MASK));	
}

/* MPIC[1] irq test for MPIC_test */
void isr_MPIC()
{
	unsigned int tmp1, tmp2, tmp3, data32 = 0;

	uart_printf(" ######## go into MPIC[1] isr ######## \n");

	MFSPR(tmp1, TBL_R);
	MFSPR(tmp2, TBU_R);
	MFSPR(tmp3, TSR);
	
//	wdog_irq_flag = 1;
	// tmp1 shoulde be 0x400000 *2 *x
//	uart_printf(" wdog exception occour ---- ok, TBL:0x%x, TBU:0x%x, TSR:0x%x\n", tmp1, tmp2, tmp3);
	
	//not clear ENW WIS and TSR[WRS] status, if not clear, when TCR set WRC, then will trigger reset
//	MTSPR(TSR, (TSR_ENW_MASK | TSR_WIS_MASK));	
}
/* 
 * irq test
 * when first Time-out, TSR[ENW,WIS] from [0,0] to [1,0]
 * when second Time-out, TSR[ENW,WIS] from [1,0] to [1,1], and then trigger Interrupt Handler 
 * at Interrupt Handler IRQ, if clear TSR[ENW/WIS], need twice Time-out will trigger Interrupt Handler again
 */
void wdog_irq_test()
{
	/* watchdog timer interrupt enabled need MSR[CE] */	
	unsigned int data32 = 0, data, tmp1, tmp2, tmp3, count;
	wdog_irq_flag = 0;
	
	InstallInterruptHandler0(204, isr_wdog);

	//clear TSR
	data32 = 0xf0000000;
	MTSPR(TSR, data32);
	
	//init TBL
	data32 = 0;
	MTSPR(TBL_W, data32);

//	mpic_unmask_Num(0);
//	mpic_unmask_Num(1);
	
	/*exception on a transition when TBL byte2_bit6 from 0 to 1 */	
//		data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_MPIC | TCR_WIE_MASK;
//		data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_MPIC;
//		data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_CORE | TCR_WIE_MASK;
//		data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_CORE;
//		data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT5 | TCR_WRC_SYSTEM | TCR_WIE_MASK;
//		data32 = TCR_WDOG_BYTE1 | TCR_WDOG_BIT5 | TCR_WRC_SYSTEM;
		data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT5 | TCR_WIE_MASK;
	
	MTSPR(TCR, data32);
	MFSPR(data, TCR);
	
	for(count=0; count<0x10; count++){
		/*first Time-out, now TSR[ENW,WIS] should from [0,0] to [1,0]*/
		MFSPR(tmp1, TSR);
		uart_printf(" ######TSR:0x%x ########\n", tmp1);
		while(1)
		{
			MFSPR(tmp2, TSR);
			if(tmp2 != tmp1){
				uart_printf(" ######TSR:0x%x ########\n", tmp2);
				break;
			}
		}

		/*second Time-out, now TSR[ENW,WIS] should from [1,0] to [1,1], and trigger Interrupt Handler*/
		while(!wdog_irq_flag);
			wdog_irq_flag=0;	
	}
	
	MFSPR(tmp1, TBL_R);
	MFSPR(tmp2, TBU_R);
	MFSPR(tmp3, TSR);

	uart_printf(" ###### wdog_irq_test1 is OK, TBL:0x%x, TBU:0x%x, TSR:0x%x ########\n", tmp1, tmp2, tmp3);
}

/* 
 * irq test2
 * when first Time-out, TSR[ENW,WIS] from [0,0] to [1,0]
 * then clear ENW, so will not trigger irq handler 
 * finally not clear ENW, so will trigger irq handler 
 */
void wdog_irq_test2()
{
	/* watchdog timer interrupt enabled need MSR[CE] */	
	unsigned int data32 = 0, data, tmp1, tmp2, tmp3, count;
	wdog_irq_flag = 0;
	
	InstallInterruptHandler0(204, isr_wdog);

	//clear TSR
	data32 = 0xf0000000;
	MTSPR(TSR, data32);
	
	//init TBL
	data32 = 0;
	MTSPR(TBL_W, data32);

	/*exception on a transition when TBL byte2_bit6 from 0 to 1 */
	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT6 | TCR_WIE_MASK;

	MTSPR(TCR, data32);
	MFSPR(data, TCR);

	count=0x8;
	while(count--)
	{
		/*first Time-out, now TSR[ENW,WIS] should from [0,0] to [1,0], will not go into isr_wdog*/
		while(1){
			MFSPR(data32, TSR);
			if((data32 & TSR_ENW_MASK) == TSR_ENW_MASK)
			{
				MFSPR(tmp3, TSR);
				uart_printf(" #######wdog first timer out occour, TSR:0x%x#########\n", tmp3);
				
				//clear ENW
				MTSPR(TSR, 0x80000000);				
				break;
			}		
		}
	}
	
	MFSPR(tmp1, TBL_R);
	MFSPR(tmp2, TBU_R);
	MFSPR(tmp3, TSR);
	uart_printf(" ###### wdog_irq_test1 is OK, TBL:0x%x, TBU:0x%x, TSR:0x%x ########\n", tmp1, tmp2, tmp3);
	
	/*finally, not clr ENW, will first and second Time-out, now TSR[ENW,WIS] should from [1,0] to [1,1], and trigger Interrupt Handler*/
	while(!wdog_irq_flag);
		wdog_irq_flag=0;
		
}

/* 
 * system_reset test without wdog_int
 * when first Time-out, TSR[ENW,WIS] from [0,0] to [1,0]
 * clear TSR[ENW], so will not trigger system_reset
 * finally, clear TSR[ENW], when second Time-out, TSR[ENW,WIS] from [1,0] to [1,1], and then third Time-out trigger system_reset 
 */
void wdog_system_reset_test1()
{
	/* watchdog timer interrupt enabled need MSR[CE] */	
	unsigned int data32 = 0, data, tmp1, tmp2, tmp3, count;
	wdog_irq_flag = 0;
	
//	InstallInterruptHandler0(204, isr_wdog);

	//clear TSR
	data32 = 0xf0000000;
	MTSPR(TSR, data32);
	
	//init TBL
	data32 = 0;
	MTSPR(TBL_W, data32);

//	mpic_unmask_Num(0);
//	mpic_unmask_Num(1);
	
	/*exception on a transition when TBL byte2_bit6 from 0 to 1 */	
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_MPIC | TCR_WIE_MASK;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_MPIC;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_CORE | TCR_WIE_MASK;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_CORE;
//	data32 = TCR_WDOG_BYTE3 | TCR_WDOG_BIT2 | TCR_WRC_SYSTEM | TCR_WIE_MASK;
	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_SYSTEM;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT6 | TCR_WIE_MASK;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_MASK;

	MTSPR(TCR, data32);
	MFSPR(data, TCR);
	
	for(count=0; count<0x4; count++){
		/*first Time-out, now TSR[ENW,WIS] should from [0,0] to [1,0]*/
		MFSPR(tmp1, TSR);
		uart_printf(" ######TSR:0x%x ########\n", tmp1);
		while(1)
		{
			MFSPR(tmp2, TSR);
			if(tmp2 != tmp1){
				uart_printf(" ######TSR:0x%x ########\n", tmp2);
				break;
			}
		}

		/*clear TSR[ENW]£¬ so will not second Time-out*/
		MTSPR(TSR, (TSR_ENW_MASK));		
	}

	MFSPR(tmp1, TBL_R);
	MFSPR(tmp2, TBU_R);
	MFSPR(tmp3, TSR);
	uart_printf(" ###### system_reset_test1, TBL:0x%x, TBU:0x%x, TSR:0x%x ########\n", tmp1, tmp2, tmp3);
	uart_printf(" if print here, it's right!");
	
	/*first Time-out, now TSR[ENW,WIS] should from [0,0] to [1,0]*/
	MFSPR(tmp1, TSR);
	uart_printf(" ######TSR:0x%x ########\n", tmp1);
	while(1)
	{
		MFSPR(tmp2, TSR);
		if(tmp2 != tmp1){
			uart_printf(" ######TSR:0x%x ########\n", tmp2);
			break;
		}
	}
	
	/*second Time-out, now TSR[ENW,WIS] should from [1,0] to [1,1], and then third Time-out will trigger system_reset*/
	MFSPR(tmp1, TSR);
	uart_printf(" ######TSR:0x%x ########\n", tmp1);
	while(1)
	{
		MFSPR(tmp2, TSR);
		if(tmp2 != tmp1){
			uart_printf(" ######TSR:0x%x ########\n", tmp2);
			break;
		}
	}
	
	while(1)
	{
		/* now should trigger system_reset when next TLB byte2_bit3 is set */
		MFSPR(tmp1, TBL_R);
		uart_printf(" ###### system_reset_test1, TBL:0x%x ########\n", tmp1);
	}
}

/* 
 * system_reset test with wdog_int
 * when first Time-out, TSR[ENW,WIS] from [0,0] to [1,0]
 * clear TSR[ENW], so will not trigger system_reset
 * test2, TSR[ENW,WIS] from [0,0] to [1,0] and form [1,0] to [1,1], and then trigger wdog_int, not clear ENW and WIS, so will trigger system_reset 
 */
void wdog_system_reset_test2()
{
	/* watchdog timer interrupt enabled need MSR[CE] */	
	unsigned int data32 = 0, data, tmp1, tmp2, tmp3, count;
	wdog_irq_flag = 0;
	
	InstallInterruptHandler0(204, isr_wdog);

	//clear TSR
	data32 = 0xf0000000;
	MTSPR(TSR, data32);
	
	//init TBL
	data32 = 0;
	MTSPR(TBL_W, data32);

//	mpic_unmask_Num(0);
//	mpic_unmask_Num(1);
	
	/*exception on a transition when TBL byte2_bit6 from 0 to 1 */	
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_MPIC | TCR_WIE_MASK;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_MPIC;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_CORE | TCR_WIE_MASK;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_CORE;
	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_SYSTEM | TCR_WIE_MASK;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT4 | TCR_WRC_SYSTEM;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT6 | TCR_WIE_MASK;
	
	MTSPR(TCR, data32);
	MFSPR(data, TCR);

	for(count=0; count<0x4; count++){
		/*first Time-out, now TSR[ENW,WIS] should from [0,0] to [1,0]*/
		MFSPR(tmp1, TSR);
		uart_printf(" ######TSR:0x%x ########\n", tmp1);
		while(1)
		{
			MFSPR(tmp2, TSR);
			if(tmp2 != tmp1){
				uart_printf(" ######TSR:0x%x ########\n", tmp2);
				break;
			}
		}

		/*second Time-out, now TSR[ENW,WIS] should from [1,0] to [1,1], and trigger Interrupt Handler*/
		while(!wdog_irq_flag);
			wdog_irq_flag=0;	
	}

	MFSPR(tmp1, TBL_R);
	MFSPR(tmp2, TBU_R);
	MFSPR(tmp3, TSR);
	uart_printf(" ###### system_reset_test2, TBL:0x%x, TBU:0x%x, TSR:0x%x ########\n", tmp1, tmp2, tmp3);
	uart_printf(" if print here, it's right!");
	
	
	/*teset 2, not clear ENW WIS at isr_wdog2, so will trigger reset*/
	InstallInterruptHandler0(204, isr_wdog2);

	//clear TSR
	data32 = 0xf0000000;
	MTSPR(TSR, data32);
	
	//init TBL
	data32 = 0;
	MTSPR(TBL_W, data32);
	
	/* set wdog_int and WRC, so will trigger Interrupt Handler and system reset*/
	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_SYSTEM | TCR_WIE_MASK;
	MTSPR(TCR, data32);
	MFSPR(data, TCR);

	/*first Time-out, now TSR[ENW,WIS] should from [0,0] to [1,0]*/
	MFSPR(tmp1, TSR);
	uart_printf(" ######TSR:0x%x ########\n", tmp1);
	while(1)
	{
		MFSPR(tmp2, TSR);
		if(tmp2 != tmp1){
			uart_printf(" ######TSR:0x%x ########\n", tmp2);
			break;
		}
	}
	
	/*second Time-out, now TSR[ENW,WIS] should from [1,0] to [1,1], and trigger Interrupt Handler*/
	while(!wdog_irq_flag);
		wdog_irq_flag=0;	
		
	while(1)
	{
		/* now should trigger system reset */
		MFSPR(tmp1, TBL_R);
		uart_printf(" ###### system_reset_test2, TBL:0x%x ########\n", tmp1);
	}
}

/* 
 * core_reset test without wdog_int
 * when first Time-out, TSR[ENW,WIS] from [0,0] to [1,0]
 * clear TSR[ENW], so will not trigger core_reset
 * finally, clear TSR[ENW], when second Time-out, TSR[ENW,WIS] from [1,0] to [1,1], and then third Time-out trigger core_reset 
 */
void wdog_core_reset_test1()
{
	/* watchdog timer interrupt enabled need MSR[CE] */	
	unsigned int data32 = 0, data, tmp1, tmp2, tmp3, count;
	wdog_irq_flag = 0;
	
//	InstallInterruptHandler0(204, isr_wdog);

	//clear TSR
	data32 = 0xf0000000;
	MTSPR(TSR, data32);
	
	//init TBL
	data32 = 0;
	MTSPR(TBL_W, data32);

//	mpic_unmask_Num(0);
//	mpic_unmask_Num(1);
	
	/*exception on a transition when TBL byte2_bit6 from 0 to 1 */	
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_MPIC | TCR_WIE_MASK;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_MPIC;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_CORE | TCR_WIE_MASK;
	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_CORE;
//	data32 = TCR_WDOG_BYTE3 | TCR_WDOG_BIT2 | TCR_WRC_SYSTEM | TCR_WIE_MASK;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_SYSTEM;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT6 | TCR_WIE_MASK;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_MASK;

	MTSPR(TCR, data32);
	MFSPR(data, TCR);
	
	for(count=0; count<0x4; count++){
		/*first Time-out, now TSR[ENW,WIS] should from [0,0] to [1,0]*/
		MFSPR(tmp1, TSR);
		uart_printf(" ######TSR:0x%x ########\n", tmp1);
		while(1)
		{
			MFSPR(tmp2, TSR);
			if(tmp2 != tmp1){
				uart_printf(" ######TSR:0x%x ########\n", tmp2);
				break;
			}
		}

		/*clear TSR[ENW]£¬ so will not second Time-out*/
		MTSPR(TSR, (TSR_ENW_MASK));		
	}

	MFSPR(tmp1, TBL_R);
	MFSPR(tmp2, TBU_R);
	MFSPR(tmp3, TSR);
	uart_printf(" ###### core_reset_test1, TBL:0x%x, TBU:0x%x, TSR:0x%x ########\n", tmp1, tmp2, tmp3);
	uart_printf(" if print here, it's right!");
	
	/*first Time-out, now TSR[ENW,WIS] should from [0,0] to [1,0]*/
	MFSPR(tmp1, TSR);
	uart_printf(" ######TSR:0x%x ########\n", tmp1);
	while(1)
	{
		MFSPR(tmp2, TSR);
		if(tmp2 != tmp1){
			uart_printf(" ######TSR:0x%x ########\n", tmp2);
			break;
		}
	}
	
	/*second Time-out, now TSR[ENW,WIS] should from [1,0] to [1,1], and then third Time-out will trigger system_reset*/
	MFSPR(tmp1, TSR);
	uart_printf(" ######TSR:0x%x ########\n", tmp1);
	while(1)
	{
		MFSPR(tmp2, TSR);
		if(tmp2 != tmp1){
			uart_printf(" ######TSR:0x%x ########\n", tmp2);
			break;
		}
	}
	
	while(1)
	{
		/* now should trigger core_reset when next TLB byte2_bit3 is set */
		MFSPR(tmp1, TBL_R);
		uart_printf(" ###### core_reset_test1, TBL:0x%x ########\n", tmp1);
	}
}

/* 
 * core_reset test with wdog_int
 * when first Time-out, TSR[ENW,WIS] from [0,0] to [1,0]
 * clear TSR[ENW], so will not trigger core_reset
 * test2, TSR[ENW,WIS] from [0,0] to [1,0] and form [1,0] to [1,1], and then trigger wdog_int, not clear ENW and WIS, so will trigger core_reset 
 */
void wdog_core_reset_test2()
{
	/* watchdog timer interrupt enabled need MSR[CE] */	
	unsigned int data32 = 0, data, tmp1, tmp2, tmp3, count;
	wdog_irq_flag = 0;
	
	InstallInterruptHandler0(204, isr_wdog);

	//clear TSR
	data32 = 0xf0000000;
	MTSPR(TSR, data32);
	
	//init TBL
	data32 = 0;
	MTSPR(TBL_W, data32);

//	mpic_unmask_Num(0);
//	mpic_unmask_Num(1);
	
	/*exception on a transition when TBL byte2_bit6 from 0 to 1 */	
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_MPIC | TCR_WIE_MASK;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_MPIC;
	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_CORE | TCR_WIE_MASK;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_CORE;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_SYSTEM | TCR_WIE_MASK;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT4 | TCR_WRC_SYSTEM;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT6 | TCR_WIE_MASK;
	
	MTSPR(TCR, data32);
	MFSPR(data, TCR);

	for(count=0; count<0x4; count++){
		/*first Time-out, now TSR[ENW,WIS] should from [0,0] to [1,0]*/
		MFSPR(tmp1, TSR);
		uart_printf(" ######TSR:0x%x ########\n", tmp1);
		while(1)
		{
			MFSPR(tmp2, TSR);
			if(tmp2 != tmp1){
				uart_printf(" ######TSR:0x%x ########\n", tmp2);
				break;
			}
		}

		/*second Time-out, now TSR[ENW,WIS] should from [1,0] to [1,1], and trigger Interrupt Handler*/
		while(!wdog_irq_flag);
			wdog_irq_flag=0;	
	}

	MFSPR(tmp1, TBL_R);
	MFSPR(tmp2, TBU_R);
	MFSPR(tmp3, TSR);
	uart_printf(" ###### core_reset_test2, TBL:0x%x, TBU:0x%x, TSR:0x%x ########\n", tmp1, tmp2, tmp3);
	uart_printf(" if print here, it's right!");
	
	
	/*teset 2, not clear ENW WIS at isr_wdog2, so will trigger reset*/
	InstallInterruptHandler0(204, isr_wdog2);

	//clear TSR
	data32 = 0xf0000000;
	MTSPR(TSR, data32);
	
	//init TBL
	data32 = 0;
	MTSPR(TBL_W, data32);
	
	/* set wdog_int and WRC, so will trigger Interrupt Handler and system reset*/
	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_CORE | TCR_WIE_MASK;
	MTSPR(TCR, data32);
	MFSPR(data, TCR);

	/*first Time-out, now TSR[ENW,WIS] should from [0,0] to [1,0]*/
	MFSPR(tmp1, TSR);
	uart_printf(" ######TSR:0x%x ########\n", tmp1);
	while(1)
	{
		MFSPR(tmp2, TSR);
		if(tmp2 != tmp1){
			uart_printf(" ######TSR:0x%x ########\n", tmp2);
			break;
		}
	}
	
	/*second Time-out, now TSR[ENW,WIS] should from [1,0] to [1,1], and trigger Interrupt Handler*/
	while(!wdog_irq_flag);
		wdog_irq_flag=0;	
		
	while(1)
	{
		/* now should trigger core reset */
		MFSPR(tmp1, TBL_R);
		uart_printf(" ###### core_reset_test2, TBL:0x%x ########\n", tmp1);
	}
	
	/*finally, PC should to 0xFFFFFFFC*/
}


/* 
 * MPIC test without wdog_int
 * when first Time-out, TSR[ENW,WIS] from [0,0] to [1,0]
 * clear TSR[ENW], so will not trigger MPIC[1]
 * finally, clear TSR[ENW], when second Time-out, TSR[ENW,WIS] from [1,0] to [1,1], and then third Time-out trigger MPIC[1]
 */
void wdog_MPIC_test1()
{
	/* watchdog timer interrupt enabled need MSR[CE] */	
	unsigned int data32 = 0, data, tmp1, tmp2, tmp3, count;
	wdog_irq_flag = 0;
	
//	InstallInterruptHandler0(204, isr_wdog);		//204 is wdog int

	//clear TSR
	data32 = 0xf0000000;
	MTSPR(TSR, data32);
	
	//init TBL
	data32 = 0;
	MTSPR(TBL_W, data32);

	mpic_unmask_Num(1);
	InstallInterruptHandler0(1, isr_MPIC);			//1 is MPIC int
	
	/*exception on a transition when TBL byte2_bit6 from 0 to 1 */	
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_MPIC | TCR_WIE_MASK;
	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT4 | TCR_WRC_MPIC;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_CORE | TCR_WIE_MASK;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_CORE;
//	data32 = TCR_WDOG_BYTE3 | TCR_WDOG_BIT2 | TCR_WRC_SYSTEM | TCR_WIE_MASK;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_SYSTEM;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT6 | TCR_WIE_MASK;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_MASK;

	MTSPR(TCR, data32);
	MFSPR(data, TCR);
	
	for(count=0; count<0x4; count++){
		/*first Time-out, now TSR[ENW,WIS] should from [0,0] to [1,0]*/
		MFSPR(tmp1, TSR);
		uart_printf(" ######TSR:0x%x ########\n", tmp1);
		while(1)
		{
			MFSPR(tmp2, TSR);
			if(tmp2 != tmp1){
				uart_printf(" ######TSR:0x%x ########\n", tmp2);
				break;
			}
		}

		/*clear TSR[ENW]£¬ so will not second Time-out*/
		MTSPR(TSR, (TSR_ENW_MASK));		
	}

	MFSPR(tmp1, TBL_R);
	MFSPR(tmp2, TBU_R);
	MFSPR(tmp3, TSR);
	uart_printf(" ###### wdog_mpic_test1, TBL:0x%x, TBU:0x%x, TSR:0x%x ########\n", tmp1, tmp2, tmp3);
	uart_printf(" if print here, it's right!");
	
	/*first Time-out, now TSR[ENW,WIS] should from [0,0] to [1,0]*/
	MFSPR(tmp1, TSR);
	uart_printf(" ######TSR:0x%x ########\n", tmp1);
	while(1)
	{
		MFSPR(tmp2, TSR);
		if(tmp2 != tmp1){
			uart_printf(" ######TSR:0x%x ########\n", tmp2);
			break;
		}
	}
	
	/*second Time-out, now TSR[ENW,WIS] should from [1,0] to [1,1], and then third Time-out will trigger MPIC*/
	MFSPR(tmp1, TSR);
	uart_printf(" ######TSR:0x%x ########\n", tmp1);
	while(1)
	{
		MFSPR(tmp2, TSR);
		if(tmp2 != tmp1){
			uart_printf(" ######TSR:0x%x ########\n", tmp2);
			break;
		}
	}
	
	while(1)
	{
		/* now should trigger MPIC when next TLB byte2_bit4 is set */
		MFSPR(tmp1, TBL_R);
		uart_printf(" ###### wdog_mpic_test1, TBL:0x%x ########\n", tmp1);
	}
}

/* 
 * MPIC test with wdog_int
 * when first Time-out, TSR[ENW,WIS] from [0,0] to [1,0]
 * clear TSR[ENW], so will not trigger MPIC[1]
 * test2, TSR[ENW,WIS] from [0,0] to [1,0] and form [1,0] to [1,1], and then trigger wdog_int, not clear ENW and WIS, so will trigger MPIC[1] 
 * in addition, isr_wdog will not trigger isr_MPIC
 */
void wdog_MPIC_test2()
{
	/* watchdog timer interrupt enabled need MSR[CE] */	
	unsigned int data32 = 0, data, tmp1, tmp2, tmp3, count;
	wdog_irq_flag = 0;
	
	InstallInterruptHandler0(204, isr_wdog);			//204 is MPIC int

	//clear TSR
	data32 = 0xf0000000;
	MTSPR(TSR, data32);
	
	//init TBL
	data32 = 0;
	MTSPR(TBL_W, data32);

	mpic_unmask_Num(1);
	InstallInterruptHandler0(1, isr_MPIC);				//1 is MPIC int

	/*exception on a transition when TBL byte2_bit4 from 0 to 1 */	
	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT4 | TCR_WRC_MPIC | TCR_WIE_MASK;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_MPIC;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_CORE | TCR_WIE_MASK;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_CORE;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT3 | TCR_WRC_SYSTEM | TCR_WIE_MASK;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT4 | TCR_WRC_SYSTEM;
//	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT6 | TCR_WIE_MASK;
	
	MTSPR(TCR, data32);
	MFSPR(data, TCR);

	for(count=0; count<0x4; count++){
		/*first Time-out, now TSR[ENW,WIS] should from [0,0] to [1,0]*/
		MFSPR(tmp1, TSR);
		uart_printf(" ######TSR:0x%x ########\n", tmp1);
		while(1)
		{
			MFSPR(tmp2, TSR);
			if(tmp2 != tmp1){
				uart_printf(" ######TSR:0x%x ########\n", tmp2);
				break;
			}
		}

		/*second Time-out, now TSR[ENW,WIS] should from [1,0] to [1,1], and trigger WDOG Interrupt Handler*/
		while(!wdog_irq_flag);
			wdog_irq_flag=0;	
	}

	MFSPR(tmp1, TBL_R);
	MFSPR(tmp2, TBU_R);
	MFSPR(tmp3, TSR);
	uart_printf(" ###### core_reset_test2, TBL:0x%x, TBU:0x%x, TSR:0x%x ########\n", tmp1, tmp2, tmp3);
	uart_printf(" if print here, it's right!");
	
	/*teset 2, not clear ENW WIS at isr_wdog2, so will trigger MPIC*/
	InstallInterruptHandler0(204, isr_wdog2);

	//clear TSR
	data32 = 0xf0000000;
	MTSPR(TSR, data32);
	
	//init TBL
	data32 = 0;
	MTSPR(TBL_W, data32);
	
	/* set wdog_int and WRC, so will trigger Interrupt Handler and MPIC*/
	data32 = TCR_WDOG_BYTE2 | TCR_WDOG_BIT4 | TCR_WRC_MPIC | TCR_WIE_MASK;
	MTSPR(TCR, data32);
	MFSPR(data, TCR);

	/*first Time-out, now TSR[ENW,WIS] should from [0,0] to [1,0]*/
	MFSPR(tmp1, TSR);
	uart_printf(" ######TSR:0x%x ########\n", tmp1);
	while(1)
	{
		MFSPR(tmp2, TSR);
		if(tmp2 != tmp1){
			uart_printf(" ######TSR:0x%x ########\n", tmp2);
			break;
		}
	}
	
	/*second Time-out, now TSR[ENW,WIS] should from [1,0] to [1,1], and trigger Interrupt Handler*/
	while(!wdog_irq_flag);
		wdog_irq_flag=0;	
		
	while(1)
	{
		/* now should trigger core reset */
		MFSPR(tmp1, TBL_R);
		uart_printf(" ###### MPIC_test2, TBL:0x%x ########\n", tmp1);
	}
	
	/*finally, should trigger MPIC[1]*/
}

void timer_test()
{
//	mpic_unmask_Num(0);
//	mpic_unmask_Num(1);
	
	timer_register_test();
//	tbl_test();
//	tbu_test();
//	atb_test();
	dec_test();
//	decar_test();
	
	dec_irq_test();
//	decar_irq1_test();
//	decar_irq2_test();

//	fit_test_test1();
//	fit_test_test2();
//	fit_test_test3();
//	fit_test_irq();
	
//	wdog_test1();
//	wdog_test2();
//	wdog_irq_test();				//wdog int test1
//	wdog_irq_test2();				//wdog int test1
//	wdog_system_reset_test1();		//system_reset without wdog int
//	wdog_system_reset_test2();		//system_reset with wdog int
//	wdog_core_reset_test1();		//core_reset without wdog int
//	wdog_core_reset_test2();		//core_reset with wdog int
//	wdog_MPIC_test1();				//MPIC_test without wdog int

//	tb_count_test();
	
}
