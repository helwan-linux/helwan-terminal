#ifndef HELWAN_CONFIG_H
#define HELWAN_CONFIG_H

#include <gtk/gtk.h>
#include <vte/vte.h>
#include <gio/gio.h>

#define HELWAN_SCHEMA_ID        "org.helwan_terminal.gschema"
#define HELWAN_KEY_FONT         "font-family"
#define HELWAN_KEY_WIDTH        "window-width"
#define HELWAN_KEY_HEIGHT       "window-height"
#define HELWAN_KEY_OPACITY      "opacity"
#define HELWAN_DEFAULT_FONT     "monospace 10"
#define HELWAN_DEFAULT_WIDTH    800
#define HELWAN_DEFAULT_HEIGHT   600
#define HELWAN_DEFAULT_OPACITY  0.85

typedef struct {
    GSettings *settings;
    gchar     *cached_font_string;
} HelwanConfig;

HelwanConfig *helwan_config_new         (void);
void          helwan_config_free        (HelwanConfig *cfg);
void          helwan_config_apply_font  (HelwanConfig *cfg, VteTerminal *vte);
void          helwan_config_change_font_size (HelwanConfig *cfg, VteTerminal *vte, double delta);
void          helwan_config_reset_font  (HelwanConfig *cfg, VteTerminal *vte);
gchar        *helwan_config_get_font    (HelwanConfig *cfg);
gint          helwan_config_get_width   (HelwanConfig *cfg);
gint          helwan_config_get_height  (HelwanConfig *cfg);
gdouble       helwan_config_get_opacity (HelwanConfig *cfg);
void          helwan_config_set_font    (HelwanConfig *cfg, const gchar *font_string);
void          helwan_config_set_width   (HelwanConfig *cfg, gint width);
void          helwan_config_set_height  (HelwanConfig *cfg, gint height);
void          helwan_config_set_opacity (HelwanConfig *cfg, gdouble opacity);

#endif /* HELWAN_CONFIG_H */
