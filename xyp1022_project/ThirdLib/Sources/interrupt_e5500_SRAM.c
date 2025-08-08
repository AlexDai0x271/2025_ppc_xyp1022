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
	/* recover computation mode */
	__init_hardware();
}
