#include "utils.h"

gint
helwan_safe_parse_int (const gchar *str, gint fallback)
{
    if (!str || *str == '\0') return fallback;

    gchar *end = NULL;
    gint64 val = g_ascii_strtoll(str, &end, 10);

    if (end == str || *end != '\0') return fallback;
    if (val <= 0) return fallback;

    return (gint)val;
}
