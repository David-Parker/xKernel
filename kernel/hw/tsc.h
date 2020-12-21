#pragma once

#include <kernel/util/stddef.h>

_u64 read_tscp();
_u64 read_tsc();
_u64 get_tsc_freq();
int rdtscp_supported();