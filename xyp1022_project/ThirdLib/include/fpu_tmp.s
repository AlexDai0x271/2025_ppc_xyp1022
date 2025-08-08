/*
 * fpu_tmp.s
 *
 *  Created on: 2020-11-25
 *      Author: ccore
 */

.include "macros.i"
.include "e500mc.i"

.section .text, "ax"

.align 4 
.global .fpu_tmp_test
.fpu_tmp_test:
  mfmsr r5
  xor r4,r4,r4
  ori r4,r5,0x2000
  mtmsr r4
  isync
  xor r4,r4,r4
  stw r4,0(r3)
  stw r4,4(r3)
  lfd fr0,0(r3)
  fmr fr1,fr0
  fmr fr2,fr0
  fmr fr3,fr0
  fmr fr4,fr0
  fmr fr5,fr0
  fmr fr6,fr0
  fmr fr7,fr0
  fmr fr8,fr0
  fmr fr9,fr0
  fmr fr10,fr0
  fmr fr11,fr0
  fmr fr12,fr0
  fmr fr13,fr0
  fmr fr14,fr0
  fmr fr15,fr0
  fmr fr16,fr0
  fmr fr17,fr0
  fmr fr18,fr0
  fmr fr19,fr0
  fmr fr20,fr0
  fmr fr21,fr0
  fmr fr22,fr0
  fmr fr23,fr0
  fmr fr24,fr0
  fmr fr25,fr0
  fmr fr26,fr0
  fmr fr27,fr0
  fmr fr28,fr0
  fmr fr29,fr0
  fmr fr30,fr0
  fmr fr31,fr0

  isync
blr
