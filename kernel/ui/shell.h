#pragma once

#include <kernel/util/stddef.h>
#include <kernel/lib/ds.h>

void shell_handle_user_input(ring_buffer_t* user_input);
void shell_parse_user_input(ring_buffer_t* user_input);
void shell_handle_command(char* cmd);
void shell_print_sig();