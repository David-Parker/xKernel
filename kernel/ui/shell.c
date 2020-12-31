#include <kernel/ui/shell.h>
#include <kernel/util/util.h>
#include <kernel/hw/keyboard.h>
#include <kernel/hw/console.h>
#include <kernel/lib/iolib.h>
#include <kernel/mem/malloc.h>
#include <kernel/test/unit_test.h>
#include <kernel/util/debug.h>

void shell_handle_user_input(ring_buffer_t* user_input)
{
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
            console_popc();
            console_flush();
            break;
        case KEY_ENTER:
            console_putc('\n');
            shell_parse_user_input(user_input);
            break;
        case KEY_PAGE_UP:
            console_scroll_n(VIDEO_RING_WINDOW_SIZE);
            break;
        case KEY_PAGE_DOWN:
            console_scroll_n(-(VIDEO_RING_WINDOW_SIZE));
            break;
        default:
            console_putc(keyboard_map[key]);
            console_flush();
            break;
    }
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

        // skip over next char
        if (keycode == KEY_BACKSPACE)
        {
            if (input_i > 0)
                input[input_i--] = '\0';

            continue;
        }

        if (!keyboard_is_printable(keycode))
        {
            continue;
        }

        char c = keyboard_map[keycode];
        input[input_i++] = c;
    }
    
    input[input_i] = '\0';

    ring_buffer_clear(user_input);
    shell_handle_command(input);
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
        console_clear();
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