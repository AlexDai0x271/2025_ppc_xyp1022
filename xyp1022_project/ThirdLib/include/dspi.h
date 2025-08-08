/*
 * dspi.h
 *
 *  Created on: 2021
 *      Author: ccore
 */
#ifndef DSPI_H_
#define DSPI_H_

//#include "types.h"
#include "common.h"

/***************** regs defines ************************************************/
/*******************************************************************************/
#define SPI_TLB_BASE		(0xE9600000)

#define SPI_TCF_INT_NUM 		(48)
#define SPI_EOQF_INT_NUM 		(47)
#define SPI_TFFF_INT_NUM 		(46)
#define SPI_RFDF_INT_NUM 		(45)
#define SPI_TFUF_RFOF_INT_NUM	(44)

#define SPI_DMA_DST_PER 	(1 <<12) //CH1_CFG_H[DST_PER]	dma_spi0_tx_ack
#define SPI_DMA_SRC_PER  	(0 <<7)  //CH1_CFG_H[SRC_PER]  	dma_spi0_rx_ack
#define SPI_DMA_ADDR_H		0x00000004 
/*******************************************************************/

 struct DSPI_tag {
        union {
            UINT32 R;
            struct {
                UINT32 MSTR:1;
                UINT32 CONT_SCKE:1;
                UINT32 DCONF:2;
                UINT32 FRZ:1;
                UINT32 MTFE:1;
                UINT32 PCSSE:1;
                UINT32 ROOE:1;
                  UINT32:2;
                UINT32 PCSIS5:1;
                UINT32 PCSIS4:1;
                UINT32 PCSIS3:1;
                UINT32 PCSIS2:1;
                UINT32 PCSIS1:1;
                UINT32 PCSIS0:1;
                UINT32 DOZE:1;
                UINT32 MDIS:1;
                UINT32 DIS_TXF:1;
                UINT32 DIS_RXF:1;
                UINT32 CLR_TXF:1;
                UINT32 CLR_RXF:1;
                UINT32 SMPL_PT:2;
                  UINT32:7;
                UINT32 HALT:1;
            } B;
        } MCR;                  /*0x00 Module Configuration Register */
        
        UINT32 dspi_reserved1;

            union {
                UINT32 R;
                struct {
                    UINT32 TCNT:16;
                      UINT32:16;
                } B;
            } TCR;				//0x08

            union {
                UINT32 R;
                struct {
                    UINT32 DBR:1;
                    UINT32 FMSZ:4;
                    UINT32 CPOL:1;
                    UINT32 CPHA:1;
                    UINT32 LSBFE:1;
                    UINT32 PCSSCK:2;
                    UINT32 PASC:2;
                    UINT32 PDT:2;
                    UINT32 PBR:2;
                    UINT32 CSSCK:4;
                    UINT32 ASC:4;
                    UINT32 DT:4;
                    UINT32 BR:4;
                } B;
            } CTAR[6];              /*0x0C-0x23 Clock and Transfer Attributes Registers */

            
            UINT32 dspi_ctar_reserved[2];
            
            union {
                UINT32 R;
                struct {
                    UINT32 TCF:1;
                    UINT32 TXRXS:1;
                      UINT32:1;
                    UINT32 EOQF:1;
                    UINT32 TFUF:1;
                      UINT32:1;
                    UINT32 TFFF:1;
                      UINT32:5;
                    UINT32 RFOF:1;
                      UINT32:1;
                    UINT32 RFDF:1;
                      UINT32:1;
                    UINT32 TXCTR:4;
                    UINT32 TXNXTPTR:4;
                    UINT32 RXCTR:4;
                    UINT32 POPNXTPTR:4;
                } B;
            } SR;                   /*0x2C Status Register */

            union {
                UINT32 R;
                struct {
                    UINT32 TCFRE:1;
                      UINT32:2;
                    UINT32 EOQFRE:1;
                    UINT32 TFUFRE:1;
                      UINT32:1;
                    UINT32 TFFFRE:1;
                    UINT32 TFFFDIRS:1;
                      UINT32:4;
                    UINT32 RFOFRE:1;
                      UINT32:1;
                    UINT32 RFDFRE:1;
                    UINT32 RFDFDIRS:1;
                      UINT32:16;
                } B;
            } RSER;                 /*0x30 DMA/Interrupt Request Select and Enable Register */

            union {
                UINT32 R;
                struct {
                    UINT32 CONT:1;
                    UINT32 CTAS:3;
                    UINT32 EOQ:1;
                    UINT32 CTCNT:1;
                      UINT32:4;
                    UINT32 PCS5:1;
                    UINT32 PCS4:1;
                    UINT32 PCS3:1;
                    UINT32 PCS2:1;
                    UINT32 PCS1:1;
                    UINT32 PCS0:1;
                    UINT32 TXDATA:16;
                } B;
            } PUSHR;                /*0x34 PUSH TX FIFO Register */

            union {
                UINT32 R;
                struct {
                    UINT32:16;
                    UINT32 RXDATA:16;
                } B;
            } POPR;                 /*0x38 POP RX FIFO Register */

            union {
                UINT32 R;
                struct {
                    UINT32 TXCMD:16;
                    UINT32 TXDATA:16;
                } B;
            } TXFR[4];              /*0x3C-0x4B Transmit FIFO Registers */

            UINT32 DSPI_reserved_txf[12];

            union {
                UINT32 R;
                struct {
                    UINT32:16;
                    UINT32 RXDATA:16;
                } B;
            } RXFR[4];              /*0x7C Transmit FIFO Registers */
            
        };        

/*******************************************************************/

/*******************************************************************/


/*************** bit defines **************************************************/
/******************************************************************************/
 /* DSPIx_MCR */
#define Slave_Mode		(0<< 31)			
#define Master_Mode		(1<< 31)

#define Not_Cont_SCK		(0<< 30)			
#define Continuous_SCK	(1<< 30)

 //DCONF
#define SPI_Config		(0<< 28)			
//#define DSI_Config	(1<< 28)
//#define CSI_Config	(2<< 28)
 
#define FRZ_DBUG		(1<< 27)
#define HALT_DSPI		(1<< 0)
 
#define PCSn(n)			(1<< (16+n) )// //1 The inactive state of CS0_x is high
 
/* DSPIx_SR */
#define SR_TCF			(1<< 31)
#define SR_TXRXS		(1<< 30)
#define SR_EOQF			(1<< 28)
#define SR_TFUF			(1<< 27)
#define SR_TFFF			(1<< 25)
#define SR_RFOF			(1<< 19)
#define SR_RFDF			(1<< 17)
 
/* DSPIx_RSER */
#define IE_TCF			(1<< 31)
#define IE_EOQF			(1<< 28)
#define IE_TFUF			(1<< 27)
#define IE_TFFF			(1<< 25)
#define IE_RFOF			(1<< 19)
#define IE_RFDF			(1<< 17)
#define IE_TFFF_DIRS	(1<< 24)
#define IE_RFDF_DIRS	(1<< 16)
 
 
/* DSPIx_CTARn */
#define CTARn_DBR_0		(0<< 31)
#define CTARn_DBR_1		(1<< 31)
 
#define CTARn_FMSZ(x)	((x-1)<< 27)
#define CTARn_CPOL_0	(0<< 26)
#define CTARn_CPOL_1	(1<< 26)
#define CTARn_CPHA_0	(0<< 25)
#define CTARn_CPHA_1	(1<< 25)
#define CTARn_MSB		(0<< 24)
#define CTARn_LSB		(1<< 24) 
 
#define CTARn_PBR_2		(0<< 16) 
#define CTARn_PBR_3		(1<< 16)
#define CTARn_PBR_5		(2<< 16)
#define CTARn_PBR_7		(3<< 16)
 
#define CTARn_BR_2		(0<< 0) 
#define CTARn_BR_4		(1<< 0)
#define CTARn_BR_6		(2<< 0)
#define CTARn_BR_8		(3<< 0)
#define CTARn_BR_16		(4<< 0)
#define CTARn_BR_32		(5<< 0)
#define CTARn_BR_64		(6<< 0)
#define CTARn_BR_128	(7<< 0)
 
/* DSPIx_PUSHR */
#define Master_CONT		(1<< 31)
#define Master_not_CONT	(0<< 31)
#define Master_CTASn(n)	(n<< 28 )
#define Master_EOQ		(1<< 27)
#define Master_CTCNT	(1<< 26)
#define Master_PCSn(n)	(1<< (16+n) ) 

 
/******************************************************************/
// REG DEFINITION

/******************************************************************/


/******************************************************************/
#define DSPI_A    (*( struct DSPI_tag *) SPI_TLB_BASE)
//#define DSPI_B    (*( struct DSPI_tag *) SPI_TLB_BASE2)
//#define DSPI_C    (*( struct DSPI_tag *) SPI_TLB_BASE3)


/*************** fun defines **************************************************/
/******************************************************************************/


#endif /* DSPI_H_ */
