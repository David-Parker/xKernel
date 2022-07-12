#include <shell.h>
#include <util.h>
#include <keyboard.h>
#include <iolib.h>
#include <malloc.h>
#include <test/unit_test.h>
#include <debug.h>
#include <string.h>
#include <unistd.h>
#include <console.h>
#include <time.h>

// user_input is a stream of raw keycodes
void shell_handle_user_input(ring_buffer_t* user_input)
{
    // TODO: user_input is referenced in the keyboard isr
    // there may be race conditions between this function and the isr
    if (user_input->idx_read == user_input->idx_end)
    {
        return;
    }

    char key = (char)ring_buffer_get(user_input, user_input->idx_read);
    user_input->idx_read = ring_buffer_next(user_input, user_input->idx_read);

    if (user_input->idx_read == -1) user_input->idx_read = user_input->idx_end;

    switch (key)
    {
        case KEY_BACKSPACE:
            write(stdout, -KEY_BACKSPACE);
            break;
        case KEY_ENTER:
            write(stdout, '\n');
            shell_parse_user_input(user_input);
            break;
        case KEY_PAGE_UP:
            write(stdout, -KEY_PAGE_UP);
            break;
        case KEY_PAGE_DOWN:
            write(stdout, -KEY_PAGE_DOWN);
            break;
        default:
            write(stdout, keyboard_map[key]);
            break;
    }

    // TODO: Shell should not talk to console directly
     console_flush();
}

// Parses a stream of keycodes (this is not neccessarily what is displayed on the screen currently)
void shell_parse_user_input(ring_buffer_t* user_input)
{
    char* input = (char*)kmalloc(user_input->buf_len + 1);
    int input_i = 0;
    int read_idx = user_input->idx_start;

    while (read_idx != -1)
    {
        kassert(input_i >= 0);
        kassert(input_i < (user_input->buf_len + 1));

        size_t keycode = ring_buffer_get(user_input, read_idx);
        read_idx = ring_buffer_next(user_input, read_idx);
        
        if (keycode == KEY_BACKSPACE)
        {
            // Skip over next char
            if (input_i > 0)
                input[input_i--] = '\0';
        }
        else if (keycode == KEY_LEFT_SHIFT)
        {
            // Convert character to it's alternate character
        }
        else if (!keyboard_is_printable(keycode))
        {
            continue;
        }
        else
        {
            char c = keyboard_map[keycode];
            input[input_i++] = c;
        }
    }
    
    input[input_i] = '\0';

    ring_buffer_clear(user_input);
    shell_handle_command(input);
    free(input);
}

void shell_handle_command(char* cmd)
{
    int arg1, arg2;

    if (strcmp(cmd, "help"))
    {
        shell_print_help();
    }
    else if (strcmp(cmd, "cls"))
    {
        console_reset();
    }
    else if (strcmp(cmd, "sig"))
    {
        shell_print_sig();
    }
    else if (sscanf(cmd, "clr %d %d", &arg1, &arg2) == 2)
    {
        if ((arg1 == 0 && arg2 == 0) || 
            (arg1 < 0 || arg1 > 15 ||
            (arg2 < 0 || arg2 > 15)))
        {
            kprintf("Invalid color codes.\n");
            return;
        }

        console_set_colors(arg1, arg2);
    }
    else if (strcmp(cmd, "test"))
    {
        test_driver();
    }
    else if (strcmp(cmd, "time"))
    {
        _u64 timestamp = ktime_get_ms();

        _u64 days = timestamp / MILLIS_PER_SEC / 60 / 60 / 24;
        timestamp -= days*24*60*60*MILLIS_PER_SEC;
        _u64 hours = timestamp / MILLIS_PER_SEC / 60 / 60;
        timestamp -= hours*60*60*MILLIS_PER_SEC;
        _u64 mins = timestamp / MILLIS_PER_SEC / 60;
        timestamp -= mins*60*MILLIS_PER_SEC;
        _u64 secs = timestamp / MILLIS_PER_SEC;
        timestamp -= secs*MILLIS_PER_SEC;
        _u64 mils = timestamp;

        kprintf("Time Elapsed: %llud %lluh %llum:%llus %llums\n", days, hours, mins, secs, mils);
    }
}

void shell_print_help()
{
    kprintf("To scroll the console, use pg up and pg down keys.\n");
    kprintf("List of available commands:\n");
    kprintf("    help           - Prints this message.\n");
    kprintf("    cls            - Clears the entire screen.\n");
    kprintf("    clr [bg] [fg]  - Changes the console colors. Color codes: 0 to 15.\n");
    kprintf("    sig            - Prints the xKernel signature.\n");
    kprintf("    test           - Run kernel unit tests.\n");
    kprintf("    time           - Gets the time elapsed since boot.\n");
}

void shell_print_sig()
{
    kprintf(" ______ ______ ______ ______ ______ ______ ______ ______ ______ \n");
    kprintf("|______|______|______|______|______|______|______|______|______|\n");
    kprintf("         _   __                     _         _____ _____ __    \n");
    kprintf("        | | / /                    | |       |  _  |  _  /  |   \n");
    kprintf("   __  _| |/ /  ___ _ __ _ __   ___| | __   _| |/' | |/' `| |   \n");
    kprintf("   \\ \\/ |    \\ / _ | '__| '_ \\ / _ | | \\ \\ / |  /| |  /| || |   \n");
    kprintf("    >  <| |\\  |  __| |  | | | |  __| |  \\ V /\\ |_/ \\ |_/ _| |_  \n");
    kprintf("   /_/\\_\\_| \\_/\\___|_|  |_| |_|\\___|_|   \\_/  \\___(_\\___/\\___/\n");
    kprintf(" ______ ______ ______ ______ ______ ______ ______ ______ ______ \n");
    kprintf("|______|______|______|______|______|______|______|______|______|\n\n");
}