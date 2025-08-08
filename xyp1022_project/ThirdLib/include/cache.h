/*
 * cache.h
 *
 *  Created on: 2020-10-26
 *      Author: ccore
 */

#ifndef CACHE_H_
#define CACHE_H_

#define CACHE_DEBUG		uart_printf

#define L1CFG0			515
#define L1CFG1			516
#define L1CSR0			1010
#define L1CSR1			1011
#define L1CSR2			606
#define L2CFG0			519
#define L2CSR0			1017

#define ICFI			(1<<0x1)
#define ICE				(1<<0x0)

#define DCFI			(1<<0x1)
#define DCE				(1<<0x0)

#endif /* CACHE_H_ */
