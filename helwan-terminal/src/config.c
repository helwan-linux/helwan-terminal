#include "config.h"
#include <pango/pangocairo.h>
#include <string.h>

HelwanConfig *
helwan_config_new (void)
{
    HelwanConfig *cfg = g_new0(HelwanConfig, 1);

    cfg->settings = g_settings_new(HELWAN_SCHEMA_ID);

    cfg->cached_font_string = g_settings_get_string(cfg->settings, HELWAN_KEY_FONT);
    if (!cfg->cached_font_string || strlen(cfg->cached_font_string) == 0) {
        g_free(cfg->cached_font_string);
        cfg->cached_font_string = g_strdup(HELWAN_DEFAULT_FONT);
    }

    return cfg;
}

void
helwan_config_free (HelwanConfig *cfg)
{
    if (!cfg) return;
    g_clear_object(&cfg->settings);
    g_clear_pointer(&cfg->cached_font_string, g_free);
    g_free(cfg);
}

void
helwan_config_apply_font (HelwanConfig *cfg, VteTerminal *vte)
{
    g_return_if_fail(cfg != NULL);
    g_return_if_fail(VTE_IS_TERMINAL(vte));

    PangoFontDescription *fd = pango_font_description_from_string(cfg->cached_font_string);
    vte_terminal_set_font(vte, fd);
    pango_font_description_free(fd);
}

void
helwan_config_change_font_size (HelwanConfig *cfg, VteTerminal *vte, double delta)
{
    g_return_if_fail(cfg != NULL);
    g_return_if_fail(VTE_IS_TERMINAL(vte));

    PangoFontDescription *fd = pango_font_description_from_string(cfg->cached_font_string);
    double size = (double)pango_font_description_get_size(fd) / PANGO_SCALE;
    const gchar *family = pango_font_description_get_family(fd);

    size = MAX(1.0, size + delta);

    g_free(cfg->cached_font_string);
    cfg->cached_font_string = g_strdup_printf("%s %g", family, size);
    pango_font_description_free(fd);

    helwan_config_apply_font(cfg, vte);
}

void
helwan_config_reset_font (HelwanConfig *cfg, VteTerminal *vte)
{
    g_return_if_fail(cfg != NULL);
    g_return_if_fail(VTE_IS_TERMINAL(vte));

    g_free(cfg->cached_font_string);
    cfg->cached_font_string = g_strdup(HELWAN_DEFAULT_FONT);
    helwan_config_apply_font(cfg, vte);
}

gchar *
helwan_config_get_font (HelwanConfig *cfg)
{
    g_return_val_if_fail(cfg != NULL, g_strdup(HELWAN_DEFAULT_FONT));
    return g_strdup(cfg->cached_font_string);
}

gint
helwan_config_get_width (HelwanConfig *cfg)
{
    g_return_val_if_fail(cfg != NULL, HELWAN_DEFAULT_WIDTH);
    return g_settings_get_int(cfg->settings, HELWAN_KEY_WIDTH);
}

gint
helwan_config_get_height (HelwanConfig *cfg)
{
    g_return_val_if_fail(cfg != NULL, HELWAN_DEFAULT_HEIGHT);
    return g_settings_get_int(cfg->settings, HELWAN_KEY_HEIGHT);
}

gdouble
helwan_config_get_opacity (HelwanConfig *cfg)
{
    g_return_val_if_fail(cfg != NULL, HELWAN_DEFAULT_OPACITY);
    return g_settings_get_double(cfg->settings, HELWAN_KEY_OPACITY);
}

void
helwan_config_set_font (HelwanConfig *cfg, const gchar *font_string)
{
    g_return_if_fail(cfg != NULL);
    g_return_if_fail(font_string != NULL);

    g_free(cfg->cached_font_string);
    cfg->cached_font_string = g_strdup(font_string);
    g_settings_set_string(cfg->settings, HELWAN_KEY_FONT, font_string);
}

void
helwan_config_set_width (HelwanConfig *cfg, gint width)
{
    g_return_if_fail(cfg != NULL);
    g_settings_set_int(cfg->settings, HELWAN_KEY_WIDTH, width);
}

void
helwan_config_set_height (HelwanConfig *cfg, gint height)
{
    g_return_if_fail(cfg != NULL);
    g_settings_set_int(cfg->settings, HELWAN_KEY_HEIGHT, height);
}

void
helwan_config_set_opacity (HelwanConfig *cfg, gdouble opacity)
{
    g_return_if_fail(cfg != NULL);
    g_settings_set_double(cfg->settings, HELWAN_KEY_OPACITY, opacity);
}
