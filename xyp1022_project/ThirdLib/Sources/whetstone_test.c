/*
 * whetstone_test.c
 *
 *  Created on: 2020-11-26
 *      Author: ccore
 */

#include <stdio.h>
#include "gpio.h"
#include "iomacros.h"
#include "e500mc_asm.h"
#include "common.h"
	
void float_point_test()
{
	unsigned int val, val_addr;
	val_addr = &val;

	#if 1
	#if 0
		MFMSR(read_value);
		read_value |= 0x2000;
		MTMSR(read_value);
		
		float X1,X2,X3,X4,X,Y,Z;
		float T;
		long I;
		T  = .499975;
		X1  =  1.0;
		X2  =  1.0;
	
	
		for (I = 1; I <= 2; I++) {
			X3 = (X1 + X2);
		}
		uart_printf_buf("############ Y #############!\n");
	#else
		
		extern void fpu_tmp_test(unsigned int val);
		fpu_tmp_test(val_addr);
	
		float T;
		long I;
		float X1,X2,X3,X4;
		double Y1,Y2,Y3,Y4;
		val_addr = &X1;
		T  = .499975;
		X1  =  1.1;
		X2  =  2.2;
		X3  =  6.6;
		X4  =  13.2;
		for (I = 1; I <= 2; I++) {
			X3 += (X1 + X2);
		}	
		if(X3 != X4)
			while(1);
		
		val_addr = &Y1;
		Y1 = 2.2;
		Y2 = 3.1;
		Y3 = Y1*Y2;
		Y4 = 6.82;
		
	#endif
	#endif
}

void whetstone_test()
{
	whetstone_main();
}
