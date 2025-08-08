//------------------------------------------------------------------
// Description   : This is the header file of the rio
// Module Name   : env/include/rio.h
// First Edit    : 2020-09-11
// Last Modified : 2020-09-11
//------------------------------------------------------------------
// 
#ifndef RIO_H
#define RIO_H

#define RIO_AXI_DATA_PATH

#ifdef RIO1_TEST
  #define RIO_SLV_BASE_ADDRESS                 0xB0000000
  #define RIO_BASE_ADDRESS                     0xEA000000
  #define RIO_MISC_ADDRESS                     0xEB001000
  #define RIO_PHY_ADDRESS                      0xEB736000

  #define RIO_INT_NUM                          21
#else
  #define RIO_SLV_BASE_ADDRESS                 0xA0000000
  #define RIO_BASE_ADDRESS                     0xE9000000
  #define RIO_MISC_ADDRESS                     0xEB000000
  #define RIO_PHY_ADDRESS                      0xEB731000

  #define RIO_INT_NUM                          22
#endif

#define RAB_BRIDGE_REG_ADDR_BASE_APB           RIO_BASE_ADDRESS + 0x0
#define RAB_BRIDGE_REG_AXI_PIO_BASE_APB        RIO_BASE_ADDRESS + 0x180
#define RAB_BRIDGE_REG_AXI_ADDR_MAP_BASE_APB   RIO_BASE_ADDRESS + 0x200
 
#define IO_READ32_RIO(p,v)   (v = (*(volatile unsigned int *)(p)))
#define IO_READ16_RIO(p,v)   (v = (*(volatile unsigned short *)(p)))
#define IO_READ8_RIO(p,v)    (v = (*(volatile unsigned char *)(p)))

#define A_AXI_LDW(data, addr) IO_READ32_RIO(addr, data) 
#define A_AXI_LDH(data, addr) IO_READ16_RIO(addr, data) 
#define A_AXI_LDB(data, addr) IO_READ8_RIO(addr, data) 
#define A_AXI_STW(data, addr) IO_WRITE32(addr, data) 
#define A_AXI_STH(data, addr) IO_WRITE16(addr, data) 
#define A_AXI_STB(data, addr) IO_WRITE8(addr, data) 
 
#define A_APB_LDW(data, addr) IO_READ32_RIO(addr, data) 
#define A_APB_STW(data, addr) IO_WRITE32(addr, data) 
 
#define A_MISC_LDW(data, addr) IO_READ32_RIO(addr, data) 
#define A_MISC_STW(data, addr) IO_WRITE32(addr, data) 

#define A_MEM_LDW(data, addr) IO_READ32_RIO(addr, data) 
#define A_MEM_STW(data, addr) IO_WRITE32(addr, data) 

void check_rdata(unsigned int expect_data, unsigned int rdata);
void apb_enum_a();
void rio_pio_map_a();
int a_link_done();
void a_intr_enable();
void a_clr_rio_intr();
void Intr_init();
void RIO_int();
void a_dma_init(int reg_base, int dma_wr, int trans_size, int src_addr, int dest_addr, int desc_chain_size, int desc_num, int desc_ptr);
void a_start_dma(int dma_wr);
void a_data_init(int start_addr, int data_size, int init_data);

#endif  //End define RIO_H
