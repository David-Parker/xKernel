#include <kernel/test/unit_test.h>
#include <kernel/util/stddef.h>
#include <kernel/util/debug.h>
#include <kernel/util/util.h>
#include <kernel/lib/iolib.h>

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

    tassert(strcmp(lhs, rhs) == false);

    return true;
}

bool strcmp_same()
{
    char* lhs = "hey";
    char* rhs = "hey";

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
#pragma GCC diagnostic pop
#pragma endregion iolib

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
}