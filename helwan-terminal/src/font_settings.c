#include "terminal_window.h"
#include <gtk/gtk.h>
#include <vte/vte.h>
#include <pango/pangocairo.h>
#include <gio/gio.h>
#include <string.h>
#include <stdlib.h>

// المتغيرات العامة (موجودة في helwan_terminal_window.c)
extern GSettings *settings;
extern gchar *cached_font_string;

// دالة لتطبيق إعدادات الخط على VTE
void apply_font_settings(VteTerminal *terminal, const char *font_family, double font_size) {
    PangoFontDescription *font_desc = pango_font_description_from_string(font_family);
    pango_font_description_set_size(font_desc, font_size * PANGO_SCALE);
    vte_terminal_set_font(terminal, font_desc);
    pango_font_description_free(font_desc);
}

// دالة لتكبير الخط
void increase_font_size(VteTerminal *terminal) {
    PangoFontDescription *temp_font_desc = pango_font_description_from_string(cached_font_string);
    double current_font_size = (double)pango_font_description_get_size(temp_font_desc) / PANGO_SCALE;
    gchar *current_font_family = g_strdup(pango_font_description_get_family(temp_font_desc));
    pango_font_description_free(temp_font_desc);

    current_font_size += 1.0;

    g_free(cached_font_string);
    cached_font_string = g_strdup_printf("%s %g", current_font_family, current_font_size);

    apply_font_settings(terminal, current_font_family, current_font_size);
    g_free(current_font_family);

    if (settings) {
        g_settings_set_string(settings, "font-family", cached_font_string);
    }
}

// دالة لتصغير الخط
void decrease_font_size(VteTerminal *terminal) {
    PangoFontDescription *temp_font_desc = pango_font_description_from_string(cached_font_string);
    double current_font_size = (double)pango_font_description_get_size(temp_font_desc) / PANGO_SCALE;
    gchar *current_font_family = g_strdup(pango_font_description_get_family(temp_font_desc));
    pango_font_description_free(temp_font_desc);

    if (current_font_size > 1.0) {
        current_font_size -= 1.0;
        g_free(cached_font_string);
        cached_font_string = g_strdup_printf("%s %g", current_font_family, current_font_size);

        apply_font_settings(terminal, current_font_family, current_font_size);

        if (settings) {
            g_settings_set_string(settings, "font-family", cached_font_string);
        }
    }
    g_free(current_font_family);
}

// دالة لإعادة الخط للوضع الافتراضي
void reset_font_size(VteTerminal *terminal) {
    g_free(cached_font_string);
    cached_font_string = g_strdup("monospace 10");

    PangoFontDescription *temp_font_desc = pango_font_description_from_string(cached_font_string);
    double reset_font_size = (double)pango_font_description_get_size(temp_font_desc) / PANGO_SCALE;
    gchar *reset_font_family = g_strdup(pango_font_description_get_family(temp_font_desc));
    pango_font_description_free(temp_font_desc);

    apply_font_settings(terminal, reset_font_family, reset_font_size);
    g_free(reset_font_family);

    if (settings) {
        g_settings_set_string(settings, "font-family", cached_font_string);
    }
}
