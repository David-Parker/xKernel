#pragma once

/**** CONSTANTS ****/
#define true 1
#define false 0
#define bool _Bool
#define NULL 0
#define NANOS_PER_SEC 1000000000
#define MICROS_PER_SEC 1000000
#define MILLIS_PER_SEC 1000

/**** NUMERIC LIMITS ****/
#define INT8_MIN    (-0x7f - 1)
#define INT16_MIN   (-0x7fff - 1)
#define INT32_MIN   (-0x7fffffff - 1)
#define INT64_MIN   (-0x7fffffffffffffff - 1)
#define INT8_MAX    0x7f
#define INT16_MAX   0x7fff
#define INT32_MAX   0x7fffffff
#define INT64_MAX   0x7fffffffffffffff
#define UINT8_MAX   0xff
#define UINT16_MAX  0xffff
#define UINT32_MAX  0xffffffff
#define UINT64_MAX  0xffffffffffffffff

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
typedef __SIZE_TYPE__ size_t;
typedef __PTRDIFF_TYPE__ ptrdiff_t;

/**** ARRAYS ****/
#define ARRSIZE(a) (sizeof(a) / sizeof(a[0]))
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *) 0)->MEMBER)