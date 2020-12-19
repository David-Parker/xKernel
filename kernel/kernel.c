  /* This will force us to create a kernel entry function instead of jumping to kernel.c:0x00 */
void dummy_test_entrypoint() {
}

void kprint(char* str)
{
    char* video_memory = (char*) 0xb8000;

    while (*str != 0)
    {
        *(video_memory) = *str++;
        video_memory+=2;
    }
}

void main() {
    kprint("Kernel has been loaded into memory.\n");
}