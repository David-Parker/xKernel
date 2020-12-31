#ifdef UNIT_TEST 
#include <kernel/test/unit_test.h>
#include <kernel/util/stddef.h>
#include <kernel/util/debug.h>
#include <kernel/util/util.h>
#include <kernel/lib/iolib.h>
#include <kernel/lib/ds.h>
#include <kernel/hw/console.h>
#include <kernel/mem/malloc.h>

#pragma region strcmp

bool strcmp_nulls()
{
    char* lhs = NULL;
    char* rhs = NULL;

    tassert(strcmp(lhs, rhs) == true);

    return true;
}

bool strcmp_one_null()
{
    char* lhs = "hey";
    char* rhs = NULL;

    tassert(strlen(lhs) == 3);
    tassert(strcmp(lhs, rhs) == false);

    return true;
}

bool strcmp_same()
{
    char* lhs = "hey";
    char* rhs = "hey";

    tassert(strlen(lhs) == 3);
    tassert(strcmp(lhs, rhs) == true);

    return true;
}

bool strcmp_different()
{
    char* lhs = "hey";
    char* rhs = "hi";

    tassert(strcmp(lhs, rhs) == false);

    return true;
}

bool strcmp_different_substring()
{
    char* lhs = "hey";
    char* rhs = "heyh";

    tassert(strcmp(lhs, rhs) == false);

    return true;
}
#pragma endregion
#pragma region iolib
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverflow"

bool TestString(char* expected, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buf[4096];
    vkprintf(fmt, buf, ARRSIZE(buf), args);
    va_end(args);
    tassert(strcmp(expected, buf));
}

bool kprintf_int()
{
    char* expected = "0";
    char buf[2];

    ksprintf("%d", buf, ARRSIZE(buf), 0);
    tassert(strcmp(expected, buf));
    ksprintf("%i", buf, ARRSIZE(buf), 0);
    tassert(strcmp(expected, buf));

    return true;
}
bool kprintf_int_zero()
{
    char expected[] = "0";
    TestString(expected, "%d", 0);
    TestString(expected, "%i", 0);
    return true;
}
bool kprintf_int_positive_small()
{
    char expected[] = "1";
    TestString(expected, "%d", 1);
    TestString(expected, "%i", 1);
    return true;
}
bool kprintf_int_positive_max()
{
    char expected[] = "2147483647";
    TestString(expected, "%d", INT32_MAX);
    TestString(expected, "%i", INT32_MAX);
    return true;
}
bool kprintf_int_positive_max_plusone()
{
    char expected[] = "-2147483648";
    TestString(expected, "%d", INT32_MAX + 1);
    TestString(expected, "%i", INT32_MAX + 1);
    return true;
}
bool kprintf_int_negative_small()
{
    char expected[] = "-1";
    TestString(expected, "%d", -1);
    TestString(expected, "%i", -1);
    return true;
}
bool kprintf_int_negative_max()
{
    char expected[] = "-2147483648";
    TestString(expected, "%d", INT32_MIN);
    TestString(expected, "%i", INT32_MIN);
    return true;
}
bool kprintf_int_negative_max_plusone()
{
    char expected[] = "2147483647";
    TestString(expected, "%d", INT32_MIN - 1);
    TestString(expected, "%i", INT32_MIN - 1);
    return true;
}
bool kprintf_unsigned_int_zero()
{
    char expected[] = "0";
    TestString(expected, "%u", 0);
    return true;
}
bool kprintf_unsigned_int_small()
{
    char expected[] = "1";
    TestString(expected, "%u", 1);
    return true;
}
bool kprintf_unsigned_int_max()
{
    char expected[] = "4294967295";
    TestString(expected, "%u", UINT32_MAX);
    return true;
}
bool kprintf_unsigned_int_max_plusone()
{
    char expected[] = "0";
    TestString(expected, "%u", UINT32_MAX + 1);
    return true;
}
bool kprintf_long_zero()
{
    char expected[] = "0";
    TestString(expected, "%ld", 0l);
    return true;
}
bool kprintf_long_positive_small()
{
    char expected[] = "1";
    TestString(expected, "%ld", 1l);
    return true;
}
bool kprintf_long_positive_max()
{
    char buffer[64];
    char* expected = "-1";
    TestString(expected, "%ld", INT64_MAX);
    return true;
}
bool kprintf_long_positive_max_plusone()
{
    char buffer[64];
    char* expected = "0";
    TestString(expected, "%ld", INT64_MAX + 1);
    return true;
}
bool kprintf_long_negative_small()
{
    char expected[] = "-1";
    TestString(expected, "%ld", -1l);
    return true;
}
bool kprintf_long_negative_max()
{
    char buffer[64];
    char* expected = "0";
    TestString(expected, "%ld", INT64_MIN);
    return true;
}
bool kprintf_long_negative_max_plusone()
{
    char buffer[64];
    char* expected = "-1";
    TestString(expected, "%ld", INT64_MIN - 1);
    return true;
}
bool kprintf_unsigned_long_zero()
{
    char expected[] = "0";
    TestString(expected, "%lu", 0ul);
    return true;
}
bool kprintf_unsigned_long_small()
{
    char expected[] = "1";
    TestString(expected, "%lu", 1ul);
    return true;
}
bool kprintf_unsigned_long_max()
{
    char buffer[64];
    char* expected = "18446744073709551615";
    TestString(expected, "%llu", UINT64_MAX);
    return true;
}
bool kprintf_unsigned_longmax_plusone()
{
    char buffer[64];
    char* expected = "0";
    TestString(expected, "%lu", UINT64_MAX + 1);
    return true;
}
bool kprintf_longlong_zero()
{
    char expected[] = "0";
    TestString(expected, "%lld", 0ll);
    return true;
}
bool kprintf_longlong_positive_small()
{
    char expected[] = "1";
    TestString(expected, "%lld", 1ll);
    return true;
}
bool kprintf_longlong_positive_max()
{
    char expected[] = "9223372036854775807";
    TestString(expected, "%lld", INT64_MAX);
    return true;
}
bool kprintf_longlong_positive_max_plusone()
{
    char expected[] = "-9223372036854775808";
    TestString(expected, "%lld", INT64_MAX + 1);
    return true;
}
bool kprintf_longlong_negative_small()
{
    char expected[] = "-1";
    TestString(expected, "%lld", -1ll);
    return true;
}
bool kprintf_longlong_negative_max()
{
    char expected[] = "-9223372036854775808";
    TestString(expected, "%lld", INT64_MIN);
    return true;
}
bool kprintf_longlong_negative_max_plusone()
{
    char expected[] = "9223372036854775807";
    TestString(expected, "%lld", INT64_MIN - 1);
    return true;
}
bool kprintf_unsigned_longlong_zero()
{
    char expected[] = "0";
    TestString(expected, "%llu", 0ull);
    return true;
}
bool kprintf_unsigned_longlong_small()
{
    char expected[] = "1";
    TestString(expected, "%llu", 1ull);
    return true;
}
bool kprintf_unsigned_longlong_max()
{
    char expected[] = "18446744073709551615";
    TestString(expected, "%llu", UINT64_MAX);
    return true;
}
bool kprintf_unsigned_longlongmax_plusone()
{
    char expected[] = "0";
    TestString(expected, "%llu", UINT64_MAX + 1);
    return true;
}
bool kprintf_string_empty()
{
    char expected[] = "";
    TestString(expected, "");
    return true;
}
bool kprintf_string_small()
{
    char expected[] = "test";
    TestString(expected, "%s", "test");
    return true;
}
bool kprintf_string_multi()
{
    char expected[] = "foo bar";
    TestString(expected, "%s %s", "foo", "bar");
    return true;
}
bool kprintf_hex()
{
    char expected[] = "f";
    TestString(expected, "%x", 15);
    return true;
}
bool kprintf_hex_max()
{
    char expected[] = "7fffffff";
    TestString(expected, "%x", INT32_MAX);
    return true;
}
bool kprintf_mixed()
{
    char expected[] = "% 123 is foo 10 12345 %";
    TestString(expected, "%% %d is %s %x %llu %%", 123, "foo", 16, 12345ull);
    return true;
}
bool sscanf_none()
{
    char* str = "bar";
    int res = 0;

    int scanned = sscanf(str, "foo %d", &res);

    tassert(scanned == 0);
    tassert(res == 0);

    return true;
}
bool sscanf_int_zero()
{
    char* str = "foo";
    int res = 0;

    int scanned = sscanf(str, "foo %d", &res);

    tassert(scanned == 0);
    tassert(res == 0);

    return true;
}
bool sscanf_int_one()
{
    char* str = "foo 42";
    int res = 0;

    int scanned = sscanf(str, "foo %d", &res);

    tassert(scanned == 1);
    tassert(res == 42);

    return true;
}
bool sscanf_int_two()
{
    char* str = "foo 42 69";
    int res1 = 0;
    int res2 = 0;

    int scanned = sscanf(str, "foo %d %d", &res1, &res2);

    tassert(scanned == 2);
    tassert(res1 == 42);
    tassert(res2 == 69);

    return true;
}
#pragma GCC diagnostic pop
#pragma endregion iolib
#pragma region linked_list
bool linkedlist_empty()
{
    linked_list_t list;
    linked_list_init(&list);

    tassert(list.head == NULL);
    tassert(list.count == 0);

    return true;
}

bool linkedlist_add_one()
{
    struct list_int
    {
        linked_list_node_t elem;
        int x;
    };

    struct list_int e;
    e.x = 42;

    linked_list_t list;
    linked_list_init(&list);

    linked_list_add_back(&list, &e.elem);

    tassert(list.head != NULL);
    tassert(list.count == 1);
    tassert(((list_entry(list.head, struct list_int, elem)))->x == 42);

    return true;
}

bool linkedlist_add_multi()
{
    struct list_int
    {
        linked_list_node_t elem;
        int x;
    };

    struct list_int e1,e2,e3;

    e1.x = 1;
    e2.x = 2;
    e3.x = 3;

    linked_list_t list;
    linked_list_init(&list);

    linked_list_add_back(&list, &e1.elem);
    linked_list_add_back(&list, &e2.elem);
    linked_list_add_back(&list, &e3.elem);

    tassert(list.head != NULL);
    tassert(list.count == 3);

    linked_list_node_t* p = list.head;

    int x = 1;
    while (p != list.head->prev)
    {
        tassert(list_entry(p, struct list_int, elem)->x == x++);
        p = p->next;
    }

    return true;
}

bool linkedlist_remove_one()
{
    struct list_int
    {
        linked_list_node_t elem;
        int x;
    };

    struct list_int e;
    e.x = 42;

    linked_list_t list;
    linked_list_init(&list);

    linked_list_add_back(&list, &e.elem);
    linked_list_remove(&list, &e.elem);

    tassert(list.head == NULL);
    tassert(list.count == 0);

    return true;
}

bool linkedlist_remove_multi()
{
    struct list_int
    {
        linked_list_node_t elem;
        int x;
    };

    struct list_int e1,e2,e3;

    e1.x = 1;
    e2.x = 2;
    e3.x = 3;

    linked_list_t list;
    linked_list_init(&list);

    linked_list_add_back(&list, &e1.elem);
    linked_list_add_back(&list, &e2.elem);
    linked_list_add_back(&list, &e3.elem);

    linked_list_remove(&list, &e2.elem);

    tassert(list.head != NULL);
    tassert(list.count == 2);

    linked_list_node_t* p = list.head;

    tassert(list_entry(p, struct list_int, elem)->x == 1);
    tassert(list_entry(p->next, struct list_int, elem)->x == 3);

    return true;
}
#pragma endregion
#pragma region malloc
bool malloc_aligned_one_byte()
{
    void* mem = kmalloc(1);

    tassert((size_t)mem % 8 == 0);

    return true;
}

bool malloc_aligned_large()
{
    int* mem = (int*)kmalloc(4096 * sizeof(int));

    tassert((size_t)mem % 8 == 0);

    return true;
}

bool malloc_aligned_multi()
{
    for (int i = 0; i < 10; ++i)
    {
        void* mem = kmalloc(3);
        tassert((size_t)mem % 8 == 0);
    }

    return true;
}

bool malloc_mem_corruption()
{
    int* buf1 = (int*)kmalloc(4096 * sizeof(int));

    for (int i = 0; i < 4096; ++i)
    {
        *(buf1+i) = 0xdeadbeef;
    }

    int* buf2 = (int*)kmalloc(4096 * sizeof(int));

    for (int i = 0; i < 4096; ++i)
    {
        *(buf2+i) = 0xf00df33d;
    }

    for (int i = 0; i < 4096; ++i)
    {
        tassert(*(buf1+i) == 0xdeadbeef);
        tassert(*(buf2+i) == 0xf00df33d);
    }

    return true;
}

bool calloc_all_zero()
{
    int* mem = (int*)kcalloc(4096 * sizeof(int));

    //kprintf("%x\n", mem);

    for (int i = 0; i < 4096; ++i)
    {
        tassert(*(mem+i) == 0);
    }

    tassert((size_t)mem % 8 == 0);

    return true;
}
#pragma endregion
#pragma region ring_buffer
bool ring_buffer_spin()
{
    size_t buffer[4];

    ring_buffer_t ring;
    ring_buffer_init(&ring, buffer, ARRSIZE(buffer), ARRSIZE(buffer));

    for (int i = 0; i < 8; ++i)
    {
        ring_buffer_push(&ring, i % 4);
    }

    for (int i = 0; i < 8; ++i)
    {
        size_t elem = ring_buffer_get(&ring, i);
        tassert(elem == i % 4);
    }

    return true;
}

bool ring_buffer_window()
{
    size_t buffer[8];

    ring_buffer_t ring;
    ring_buffer_init(&ring, buffer, ARRSIZE(buffer), ARRSIZE(buffer) / 2);

    for (int i = 0; i < 8; ++i)
    {
        ring_buffer_push(&ring, i);
    }

    // [0,1,2,3,(4,5,6,7)]

    int x = 4;
    int i = ring.idx_start;

    while (i != -1)
    {
        size_t elem = ring_buffer_get(&ring, i);
        tassert(elem == x++);
        i = ring_buffer_next(&ring, i);
    }

    return true;
}

bool ring_buffer_window_reverse()
{
    size_t buffer[8];

    ring_buffer_t ring;
    ring_buffer_init(&ring, buffer, ARRSIZE(buffer), ARRSIZE(buffer) / 2);

    for (int i = 0; i < 8; ++i)
    {
        ring_buffer_push(&ring, i);
    }

    // [0,1,2,3,(4,5,6,7)]

    int x = 4;
    int i = ring.idx_start;

    while (i != -1)
    {
        size_t elem = ring_buffer_get(&ring, i);
        //kprintf("%d s:%d e:%d i:%d\n", elem, ring.idx_start, ring.idx_end, i);
        tassert(elem == x--);
        i = ring_buffer_prev(&ring, i);
    }

    return true;
}

bool ring_buffer_window_indirect()
{
    size_t buffer[8];

    ring_buffer_t ring;
    ring_buffer_init(&ring, buffer, ARRSIZE(buffer), ARRSIZE(buffer) / 2);

    for (int i = 0; i < 8; ++i)
    {
        size_t* mem = (size_t*)kmalloc(sizeof(size_t));
        *mem = i;

        ring_buffer_push(&ring, (size_t)mem);
    }

    // [0,1,2,3,(4,5,6,7)]

    int x = 4;
    int i = ring.idx_start;

    while (i != -1)
    {
        size_t* elem = (size_t*)ring_buffer_get(&ring, i);
        tassert(*elem == x++);
        i = ring_buffer_next(&ring, i);
    }

    return true;
}

bool ring_buffer_rewind()
{
    size_t buffer[8] = {0};

    ring_buffer_t ring;
    ring_buffer_init(&ring, buffer, ARRSIZE(buffer), ARRSIZE(buffer) / 2);

    for (int i = 0; i < 5; ++i)
    {
        size_t* mem = (size_t*)kmalloc(sizeof(size_t));
        *mem = i;

        ring_buffer_push(&ring, (size_t)mem);
    }

    size_t* elem = (size_t*)ring_buffer_get(&ring, ring.idx_start);
    tassert(*elem == 1);

    // rewind
    int prev_start = ring_buffer_prev(&ring, ring.idx_start);
    int prev_end = ring_buffer_prev(&ring, ring.idx_end);

    ring.idx_start = prev_start;
    ring.idx_end = prev_end;

    elem = (size_t*)ring_buffer_get(&ring, ring.idx_start);
    tassert(*elem == 0);

    // rewind
    prev_start = ring_buffer_prev(&ring, ring.idx_start);
    prev_end = ring_buffer_prev(&ring, ring.idx_end);

    ring.idx_start = prev_start;
    ring.idx_end = prev_end;

    elem = (size_t*)ring_buffer_get(&ring, ring.idx_start);
    tassert(elem == NULL);

    for (int i = 0; i < 16; ++i)
    {
        prev_start = ring_buffer_prev(&ring, ring.idx_start);
        prev_end = ring_buffer_prev(&ring, ring.idx_end);

        ring.idx_start = prev_start;
        ring.idx_end = prev_end;
    }

    return true;
}
#pragma endregion

void test_init()
{
    TEST_FUNC(strcmp_nulls);
    TEST_FUNC(strcmp_one_null);
    TEST_FUNC(strcmp_same);
    TEST_FUNC(strcmp_different);
    TEST_FUNC(strcmp_different_substring);
    TEST_FUNC(kprintf_int);
    TEST_FUNC(kprintf_int_zero);
    TEST_FUNC(kprintf_int_positive_small);
    TEST_FUNC(kprintf_int_positive_max);
    TEST_FUNC(kprintf_int_positive_max_plusone);
    TEST_FUNC(kprintf_int_negative_small);
    TEST_FUNC(kprintf_int_negative_max);
    TEST_FUNC(kprintf_int_negative_max_plusone);
    TEST_FUNC(kprintf_unsigned_int_zero);
    TEST_FUNC(kprintf_unsigned_int_small);
    TEST_FUNC(kprintf_unsigned_int_max);
    TEST_FUNC(kprintf_unsigned_int_max_plusone);
    TEST_FUNC(kprintf_long_zero);
    TEST_FUNC(kprintf_long_positive_small);
    TEST_FUNC(kprintf_long_positive_max);
    TEST_FUNC(kprintf_long_positive_max_plusone);
    TEST_FUNC(kprintf_long_negative_small);
    TEST_FUNC(kprintf_long_negative_max);
    TEST_FUNC(kprintf_long_negative_max_plusone);
    TEST_FUNC(kprintf_unsigned_long_zero);
    TEST_FUNC(kprintf_unsigned_long_small);
    TEST_FUNC(kprintf_unsigned_long_max);
    TEST_FUNC(kprintf_unsigned_longmax_plusone);
    TEST_FUNC(kprintf_longlong_zero);
    TEST_FUNC(kprintf_longlong_positive_small);
    TEST_FUNC(kprintf_longlong_positive_max);
    TEST_FUNC(kprintf_longlong_positive_max_plusone);
    TEST_FUNC(kprintf_longlong_negative_small);
    TEST_FUNC(kprintf_longlong_negative_max);
    TEST_FUNC(kprintf_longlong_negative_max_plusone);
    TEST_FUNC(kprintf_unsigned_longlong_zero);
    TEST_FUNC(kprintf_unsigned_longlong_small);
    TEST_FUNC(kprintf_unsigned_longlong_max);
    TEST_FUNC(kprintf_unsigned_longlongmax_plusone);
    TEST_FUNC(kprintf_string_empty);
    TEST_FUNC(kprintf_string_small);
    TEST_FUNC(kprintf_string_multi);
    TEST_FUNC(kprintf_hex);
    TEST_FUNC(kprintf_hex_max);
    TEST_FUNC(kprintf_mixed);
    TEST_FUNC(sscanf_none);
    TEST_FUNC(sscanf_int_zero);
    TEST_FUNC(sscanf_int_one);
    TEST_FUNC(sscanf_int_two);
    TEST_FUNC(linkedlist_empty);
    TEST_FUNC(linkedlist_add_one);
    TEST_FUNC(linkedlist_add_multi);
    TEST_FUNC(linkedlist_remove_one);
    TEST_FUNC(linkedlist_remove_multi);
    TEST_FUNC(malloc_aligned_one_byte);
    TEST_FUNC(malloc_aligned_large);
    TEST_FUNC(malloc_aligned_multi);
    TEST_FUNC(malloc_mem_corruption);
    TEST_FUNC(calloc_all_zero);
    TEST_FUNC(ring_buffer_spin);
    TEST_FUNC(ring_buffer_window);
    TEST_FUNC(ring_buffer_window_reverse);
    TEST_FUNC(ring_buffer_window_indirect);
    TEST_FUNC(ring_buffer_rewind);
}

void test_driver()
{
    test_init();

    int passed = 0;

    kprintf("Starting kernel unit tests...\n");

    for (int i = 0; i < test_no; ++i)
    {
        kprintf("[%d/%d] ", i+1, test_no+1);
        
        if (unit_tests[i].func() == true)
        {
            passed++;
            console_set_colors(VGA_COLOR_BLACK, VGA_COLOR_GREEN);
            kprintf("PASSED ");
            console_set_colors(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
            kprintf("%s()\n", unit_tests[i].str);
        }
        else
        {
            console_set_colors(VGA_COLOR_BLACK, VGA_COLOR_RED);
            kprintf("FAILED ");
            console_set_colors(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
            kprintf("%s()\n", unit_tests[i].str);
        }
    }

    kprintf("%d out of %d tests passed.\n", passed, test_no);
}
#endif