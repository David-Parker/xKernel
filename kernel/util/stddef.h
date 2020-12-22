#pragma once

/**** CONSTANTS ****/
#define true 1
#define false 0
#define bool _Bool
#define NULL 0
#define NANOS_PER_SEC 1000000000
#define MICROS_PER_SEC 1000000
#define MILLIS_PER_SEC 1000

/**** CALLING CONVENTIONS ****/
#define _cdecl __attribute__((__cdecl__))
#define _stdcall __attribute__((__stdcall__))
#define _fastcall __attribute__((__fastcall__))

/**** TYPEDEFS ****/
typedef unsigned long long _u64;
typedef unsigned int _u32;
typedef unsigned short _u16;
typedef unsigned char _u8;
typedef long long _s64;
typedef int _s32;
typedef short _s16;
typedef char _s8;