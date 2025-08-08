#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

void InterruptHandler(long cause);
extern void __init_hardware(void);

#ifdef __cplusplus
}
#endif

void InterruptHandler(long cause)
{
	unsigned long proc_id;
	
	/* recover computation mode */
	__init_hardware();
	
	/* read processor id */
	asm ("mfpir %0" : "=r" (proc_id));

	printf("Core%lu: InterruptHandler: %#lx exception.\r\n", proc_id>>5, cause);
}
