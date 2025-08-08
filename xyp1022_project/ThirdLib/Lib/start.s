	
	.section .text_init,"ax"
	.align	2
	.global __start
	.global __reset
	.global sys_reset
	.type __start,@function
__start:
__reset:
sys_reset:
	nop
	nop
	nop
	b	init_core
