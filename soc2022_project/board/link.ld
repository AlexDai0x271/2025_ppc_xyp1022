/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

MEMORY
{
    I-RAM : ORIGIN = 0x0        , LENGTH = 0x40000  /* I-RAM    256KB */
    D-RAM : ORIGIN = 0x10000000 , LENGTH = 0x80000  /* D-RAM    512KB */
}

__min_heap_size = 0x200;

REGION_ALIAS("REGION_TEXT",    I-RAM);
REGION_ALIAS("REGION_RODATA",  I-RAM);
REGION_ALIAS("REGION_DATA",    D-RAM);
REGION_ALIAS("REGION_BSS",     D-RAM);

ENTRY(Reset_Handler)
SECTIONS
{
    .text : {
        . = ALIGN(0x4);
        __stext = .;
        KEEP (*startup.o(*.text*))
        KEEP (*startup.o(*.vectors*))
        *(.text)
        *(.text*)
        *(.text.*)
        *(.gnu.warning)
        *(.stub)
        *(.gnu.linkonce.t*)
        *(.glue_7t)
        *(.glue_7)
        *(.jcr)
        KEEP (*(.init))
        KEEP (*(.fini))
        . = ALIGN(0x4);
        PROVIDE (__ctbp = .);
        *(.call_table_data)
        *(.call_table_text)
        . = ALIGN(0x10);
        __etext = .;
        . = ALIGN(0x4);
    } > REGION_TEXT

    .rodata : {
        . = ALIGN(0x4);
        __srodata = .;
        *(.srodata*)
        __rodata_start__ = .;
        *(.rdata)
        *(.rdata*)
        *(.rdata1)
        *(.rdata.*)
        *(.rodata)
        *(.rodata1)
        *(.rodata*)
        *(.rodata.*)
        *(.rodata.str1.4)
        . = ALIGN(0x4);
        __ctor_start__ = .;
        KEEP (*(SORT(.ctors.*)))
        KEEP (*(.ctors))
        __ctor_end__ = .;
        __dtor_start__ = .;
        KEEP (*(SORT(.dtors.*)))
        KEEP (*(.dtors))
        __dtor_end__ = .;
        . = ALIGN(0x4);
        __erodata = .;
        __rodata_end__ = .;
        . = ALIGN(0x4);
    } > REGION_RODATA

    .data : {
        . = ALIGN(0x4);
        __sdata = .;
        __data_start__ = .;
        *(.got.plt)
        *(.got)
        *(.gnu.linkonce.r*)
        *(.data)
        *(.data*)
        *(.data1)
        *(.data.*)
        *(.gnu.linkonce.d*)
        *(.data1)
        *(.gcc_except_table)
        *(.gcc_except_table*)
        __start_init_call = .;
        *(.initcall.init)
        __stop_init_call = .;
        __start_cmd = .;
        *(.bootloaddata.cmd)
        . = ALIGN(4);
        __stop_cmd = .;
        __global_pointer$ = .;
        *(.sdata)
        *(.sdata.*)
        *(.sdata2.*)
        *(.gnu.linkonce.s.*)
        *(__libc_atexit)
        *(__libc_subinit)
        *(__libc_subfreeres)
        *(.note.ABI-tag)
        __edata = .;
        __data_end__ = .;
        . = ALIGN(0x4);
    } > REGION_DATA AT > REGION_RODATA

    .bss : {
        . = ALIGN(0x4);
        __sbss = ALIGN(0x4);
        __bss_start__ = .;
        *(.dynsbss)
        *(.sbss)
        *(.sbss.*)
        *(.scommon)
        *(.dynbss)
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(0x4);
        __ebss = .;
        __bss_end__ = .;
        . = ALIGN(0x4);
    } > REGION_BSS

    ._user_heap : {
        . = ALIGN(0x4);
        __heap_start = .;
        . += __min_heap_size;
        . = ALIGN(0x4);
    } > REGION_BSS
}
