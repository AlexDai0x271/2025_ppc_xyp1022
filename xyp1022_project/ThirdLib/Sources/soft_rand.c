/*
 * soft_rand.c
 *
 *  Created on: Apr 19, 2017
 *      Author: jzj
 */
#include <stdlib.h>
#include "sec_common.h"
alloc_rand_dp(void **data_raw,void **data,void **data_cmp,unsigned int *size)
{
	unsigned int i;
	unsigned int *dp;
	*size = ( 0x10|(unsigned)(rand()) )&RANDSIZE_MASK;	//>16byte < 32K byte & alian to 16byte
	//*size = 0x610;		//zjjin 18-3-30
	*data_raw = malloc(*size + DATA_ALIGN);
	*data = (struct std_data *)(((unsigned long)(*data_raw)+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));
	*data_cmp = malloc(*size);
	dp = (unsigned int *)(*data);
	for(i=0;i<(*size)/4;i++)
	{
		dp[i] = (unsigned int)rand();
	}
	memcpy_c(*data_cmp,*data,*size);
	return 0;
}

alloc_rand_dp2(void **data_raw,void **data,void **data_cmp,unsigned int *size)
{
	unsigned int i;
	unsigned int *dp;
	*size = ( 0x10|(unsigned)(rand()) )&(RANDSIZE_MASK>>2);	//>16byte < 32K byte & alian to 16byte
	//*size = 0x610;		//zjjin 18-3-30
	*data_raw = malloc(*size + DATA_ALIGN);
	*data = (struct std_data *)(((unsigned long)(*data_raw)+(DATA_ALIGN-1))&(~(DATA_ALIGN-1)));
	*data_cmp = malloc(*size);
	dp = (unsigned int *)(*data);
	for(i=0;i<(*size)/4;i++)
	{
		dp[i] = (unsigned int)rand();
	}
	memcpy_c(*data_cmp,*data,*size);
	return 0;
}

free_rand_dp(unsigned char **data_raw,unsigned char **data_cmp)
{
	free(*data_raw);
	free(*data_cmp);
}

