//basic function for string access
#include "stdarg.h"
#include "stddef.h"
//#include "uart.h"
#include "stdlib.h"
//#include "common.h"


char *myitoa(int value, char* string, int radix)
{
		char tmp[33];
		char* tp = tmp;
		int i;
		unsigned v;
		int sign;
		char* sp;

		if (radix > 36 || radix <= 1)
		{
			return 0;
		}
		//SerialOutString("aaaaa\n");
		sign = (radix == 10 && value < 0);
		if (sign)
		v = -value;
		else
		v = (unsigned)value;
		//SerialOutString("bbbb\n");
		while (v || tp == tmp)
		{
			i = v % radix;
			v = v / radix;
			if (i < 10)
			*tp++ = i+'0';
			else
			*tp++ = i + 'a' - 10;
		}
		//SerialOutString("ccccc\n");
		//if (string == 0)
		//string = (char*)malloc((tp-tmp)+sign+1);
		sp = string;

		if (sign)
		*sp++ = '-';
		while (tp > tmp)
		*sp++ = *--tp;
		*sp = 0;
		return string;
}

//direct to uart

int prints(const char *fmt, ...)
{
    const char *s;
    int d;
	float f;
    char buf[16];
    va_list ap;
    va_start(ap, fmt);


    while (*fmt) {
    		//delay_nms(100);//temp solution ?
        if (*fmt != '%') {
            //putchar(*fmt++);
            //SerialOutChar(*fmt++);
        	if(*fmt == '\n')
        	{
        		uart0_16750_TX('\r');
        	}
        	uart0_16750_TX(*fmt);
        	fmt++;
            continue;
        }
        switch (*++fmt) {
            case 's':
                s = va_arg(ap, const char *);
                for ( ; *s; s++) {
                   // putchar(*s);
                 //SerialOutChar(*s);
                	uart0_16750_TX(*s);
                }
                break;
            case 'd':
                d = va_arg(ap, int);
                myitoa(d, buf, 10);
                for (s = buf; *s; s++) {
                   // putchar(*s);
                 //SerialOutChar(*s);
                	uart0_16750_TX(*s);
                }
                break;
			/*
			case 'f':
            case 'F':
            	//serial_putc_dev(0,'a');
            	f = va_arg(ap, double);
            	//serial_putc_dev(0,'b');
            	gcvt(f,15,buf);
            	//serial_putc_dev(0,'c');
                for (s = buf; *s; s++) {
                   // putchar(*s);
                	SerialOutChar(*s);
                }
                break;
            */
            case 'x':
            case 'X':
                d = va_arg(ap, int);
                myitoa(d, buf, 16);
                for (s = buf; *s; s++) {
                   // putchar(*s);
                 //SerialOutChar(*s);
                	uart0_16750_TX(*s);
                }
                break;

            /* Add other specifiers here... */
            default:
                //putchar(*fmt);
                //SerialOutChar(*fmt);
            	uart0_16750_TX(*fmt);
                break;
        }
        fmt++;
    }
    va_end(ap);
    return 1;   /* Dummy return value */
}
