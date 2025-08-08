#ifndef FD_H
#define FD_H

#define FD_STW(addr,data)  STW(data,addr)

#define FD_CR_MASK         0x0F000001
#define FD_IER_MASK        0x00000003
#define FD_SR_MASK         0x00000003
#define FD_TIMER_MASK      0xFFFFFFFF

#define FD_BASE_ADDR         0xEB003000

#define FD_CR                (FD_BASE_ADDR + 0x00)
#define FD_IER               (FD_BASE_ADDR + 0x04)
#define FD_SR                (FD_BASE_ADDR + 0x08)    //Read only
#define FD_TIMER             (FD_BASE_ADDR + 0x0C)
#define FD_RESERVED          (FD_BASE_ADDR + 0x10)

//Control register
#define FD_ENABLE            (1<<0)
#define FD_TRIM_LOW(n)       (n<<8)
#define FD_TRIM_HIGH(n)      (n<<16)
//Defult 5:input clk is divided by 32 
//0000:input clk               0001: input clk divided by 2
//0010:input clk divided by 2  0011:input clk divided by 8
//0100:input clk divided by 16 0101:input clk divided by 32
#define FD_CLK_SEL(n)        (n<<24)  

//Interrupt Enable register
//#define FD_HRST_EN           (1<<0)
//#define FD_LRST_EN           (1<<1)
#define FD_HINT_EN           (1<<1)
#define FD_LINT_EN           (1<<0)

//State register
#define FD_HFD_STAT          (1<<1)
#define FD_LFD_STAT          (1<<0)

#endif
