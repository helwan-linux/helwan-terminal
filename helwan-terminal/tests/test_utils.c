#include <glib.h>
#include "../include/utils.h"

static void
test_safe_parse_int_valid (void)
{
    g_assert_cmpint(helwan_safe_parse_int("800",  0), ==, 800);
    g_assert_cmpint(helwan_safe_parse_int("1",    0), ==, 1);
    g_assert_cmpint(helwan_safe_parse_int("9999", 0), ==, 9999);
}

static void
test_safe_parse_int_invalid (void)
{
    g_assert_cmpint(helwan_safe_parse_int("abc",  42), ==, 42);
    g_assert_cmpint(helwan_safe_parse_int("",     42), ==, 42);
    g_assert_cmpint(helwan_safe_parse_int(NULL,   42), ==, 42);
    g_assert_cmpint(helwan_safe_parse_int("0",    42), ==, 42);
    g_assert_cmpint(helwan_safe_parse_int("-1",   42), ==, 42);
    g_assert_cmpint(helwan_safe_parse_int("12ab", 42), ==, 42);
}

int
main (int argc, char *argv[])
{
    g_test_init(&argc, &argv, NULL);
    g_test_add_func("/utils/safe_parse_int/valid",   test_safe_parse_int_valid);
    g_test_add_func("/utils/safe_parse_int/invalid", test_safe_parse_int_invalid);
    return g_test_run();
}
