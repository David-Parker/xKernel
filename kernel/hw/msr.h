#pragma once

#include <kernel/util/stddef.h>

#define MSR_PLATFORM_INFO   0x000000ce

bool msr_supported();
_u64 read_msr(_u32 msr);