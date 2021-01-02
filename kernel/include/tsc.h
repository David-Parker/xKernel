#pragma once

#include <stddef.h>

_u64 read_tscp();
_u64 read_tsc();
int rdtscp_supported();
_u64 get_tsc_freq();