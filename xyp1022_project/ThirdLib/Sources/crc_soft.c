/*
 * crc_soft.c
 *
 *  Created on: 2017Äê5ÔÂ31ÈÕ
 *      Author: ccore
 */

static unsigned int CRC32[256];
static char init = 0;

static void init_table(unsigned int type)
{
	int i,j;
	unsigned int crc;
	for(i=0;i<256;i++)
	{
		crc=i;
		for(j=0;j<8;j++)
		{
			if(crc & 1)
			{
				crc = (crc >> 1)^type;
			}
			else
			{
				crc = crc >> 1;
			}
		}
		CRC32[i]=crc;
	}
}

static unsigned int crc_x(unsigned char *buf,int len,unsigned int type,unsigned int iv)
{
	unsigned int ret = iv;
	int i;
	if(!init)
	{
		init_table(type);
		init = 1;
	}
	for(i=0;i<len;i++)
	{
		ret = CRC32[((ret&0xff) ^ buf[i])]^(ret>>8);
	}
	ret = ~ret;
	return ret;
}

unsigned char *bswap(unsigned char *buf,int bit_len);

void set_sinit(void)
{
	init =0;
}

unsigned int crc_x_e(unsigned char *buf,int len,unsigned int type,unsigned int type_len,\
		unsigned int iv,unsigned int is_ibs,unsigned int is_obs,unsigned int is_c)		//enhanced crc_x,can custom init val
																						//in case the buf len byte cacu
{
	unsigned int ret=0;
	int i;

	if(type_len==0)
		return 0;

	for(i=0;i<type_len;i++)
	{
		ret|=(1<<i);
	}

	ret &=iv;

	if(!init)
	{
		init_table(type);
		init = 1;
	}
	if(is_ibs)				//0 -> hard swap,while soft not
	{
		for(i=0;i<len;i++)
		{
			bswap(&(buf[i]),8);
		}
	}

	for(i=0;i<len;i++)
	{
		ret = CRC32[((ret&0xff) ^ buf[i])]^(ret>>8);
	}
	if(!is_c)				//1 -> hard not c,and soft not c
	{
		ret = ~ret;
	}
	if(is_obs)				//0 -> hard swap,while soft not
	{
		ret <<=(32-type_len);
		bswap(&ret,type_len);
		ret >>=(32-type_len);
	}
	return ret;
}

unsigned int crc_32_ieee802(unsigned char *buf,int len)
{
	unsigned int ret;
	set_sinit();
	ret = crc_x(buf,len,0xedb88320,0xffffffff);
	l2b(&ret,4);	//fact is big to little exchange
	return ret;
}

unsigned int crc_32_ietf3385(unsigned char *buf,int len)
{
	unsigned int ret;
	set_sinit();
	ret = crc_x(buf,len,0x82f63b78,0xffffffff);
	l2b(&ret,4);	//fact is big to little exchange
	return ret;
}

unsigned int crc_32_custom(unsigned char *buf,int len,unsigned int poly)
{
	unsigned int ret;
	set_sinit();
	ret = crc_x(buf,len,poly,0xffffffff);
	l2b(&ret,4);	//fact is big to little exchange
	return ret;
}

unsigned int crc_16_custom(unsigned char *buf,int len,unsigned int poly)
{
	unsigned int ret;
	set_sinit();
	ret = crc_x(buf,len,poly,0xffff);
	l2b(&ret,4);	//fact is big to little exchange
	return ret;
}


unsigned char *bswap(unsigned char *buf,int bit_len)
{
	int i;
	unsigned char *btmp;
	unsigned char tmp;
	btmp = malloc(bit_len+0x1000);
	btmp +=0x1000;
	for(i=0;i<bit_len;i++)
	{
		btmp[i]=(unsigned char)((buf[i/8])>>(7-(i%8)));
		btmp[i]&=1;
	}
	for(i=0;i<bit_len/2;i++)
	{
		tmp = btmp[i];
		btmp[i]=btmp[bit_len-i-1];
		btmp[bit_len-i-1] = tmp;
	}
	for(i=0;i<bit_len;i++)
	{
		if(!(i%8))
		{
			tmp=0;
		}
		tmp |= (btmp[i]<<(7-(i%8)));
		buf[i/8]=tmp;
	}
	free(btmp-0x1000);
	return buf;
}

