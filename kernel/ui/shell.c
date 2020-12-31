#include <kernel/ui/shell.h>
#include <kernel/util/util.h>
#include <kernel/hw/keyboard.h>
#include <kernel/hw/console.h>
#include <kernel/lib/iolib.h>

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
            shell_parse_user_input(user_input);
            break;
        case KEY_PAGE_UP:
            console_scroll_n(VGA_MAX_ROWS);
            break;
        case KEY_PAGE_DOWN:
            console_scroll_n(-VGA_MAX_ROWS);
            break;
        default:
            console_putc(keyboard_map[key]);
            console_flush();
            break;
    }
}

void shell_parse_user_input(ring_buffer_t* user_input)
{
    console_putc('\n');

    char input[512 + 1];
    int input_i = 0;
    int read_idx = user_input->idx_start;

    while (read_idx != -1)
    {
        size_t keycode = ring_buffer_get(user_input, read_idx);
        read_idx = ring_buffer_next(user_input, read_idx);

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
    //kprintf("cmd: %s\n", cmd);

    if (strcmp(cmd, "help"))
    {

    }
    else if (strcmp(cmd, "cls"))
    {
        console_clear();
    }
        else if (strcmp(cmd, "sig"))
    {
        shell_print_sig();
    }
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