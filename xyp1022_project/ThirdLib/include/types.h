//-----------------------------------------------------------------------------
//  date: 2012/06/05
//  file: types.h
//-----------------------------------------------------------------------------
//
//  Copyright (c) 2012 CCore, Inc.  All rights reserved.
//
//  This file contains definitions for basic types of variables.
//-----------------------------------------------------------------------------

#ifndef TYPES_H
#define TYPES_H

/************************** Constant Definitions *****************************/

#ifndef TRUE
#  define TRUE		1
#endif

#ifndef FALSE
#  define FALSE		0
#endif

#ifndef NULL
#define NULL		0
#endif


/**************************** Type Definitions *******************************/

typedef unsigned char		Uint8;
typedef unsigned char		UINT8;
typedef unsigned char		uint8;
typedef char			int8;
typedef unsigned short		Uint16;
typedef unsigned short		UINT16;
typedef unsigned short		uint16;
typedef short			int16;
typedef unsigned long		Uint32;
typedef unsigned long		UINT32;
typedef unsigned long		uint32;
typedef long			int32;
typedef float			float32;
typedef double			float64;
typedef unsigned long		boolean;

typedef struct
{
	Uint32 Upper;
	Uint32 Lower;
} Uint64;

#ifndef __KERNEL__
typedef Uint8          U8;
typedef Uint16         U16;
typedef Uint32         U32;
#else
#include <linux/types.h>
#endif


/***************** Macros (In-line Functions) Definitions *********************/

#define UINT64_MSW(x) ((x).Upper)

#define UINT64_LSW(x) ((x).Lower)


#endif  // TYPES_H
