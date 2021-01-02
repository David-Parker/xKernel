#pragma once

#include <stddef.h>
#include <ds.h>

void shell_handle_user_input(ring_buffer_t* user_input);
void shell_parse_user_input(ring_buffer_t* user_input);
void shell_handle_command(char* cmd);

void shell_print_help();
void shell_print_sig();