#include "preferences.h"
#include "utils.h"
#include <pango/pangocairo.h>

typedef struct {
    GtkWidget    *font_chooser;
    GtkWidget    *width_entry;
    GtkWidget    *height_entry;
    GtkWidget    *opacity_scale;
} HelwanPrefsWidgets;

static void
on_opacity_changed (GtkRange *range, GtkWindow *parent)
{
    gtk_widget_set_opacity(GTK_WIDGET(parent), gtk_range_get_value(range));
}

static void
apply_prefs (HelwanPrefsWidgets *w,
             GtkWindow          *parent,
             GtkNotebook        *notebook,
             HelwanConfig       *cfg)
{
    /* Font */
    PangoFontDescription *fd =
        gtk_font_chooser_get_font_desc(GTK_FONT_CHOOSER(w->font_chooser));
    if (fd) {
        gchar *font_str = pango_font_description_to_string(fd);
        helwan_config_set_font(cfg, font_str);
        pango_font_description_free(fd);
        g_free(font_str);

        /* Apply to all tabs */
        gint n = gtk_notebook_get_n_pages(notebook);
        for (gint i = 0; i < n; i++) {
            GtkWidget *page = gtk_notebook_get_nth_page(notebook, i);
            if (VTE_IS_TERMINAL(page))
                helwan_config_apply_font(cfg, VTE_TERMINAL(page));
        }
    }

    /* Width & Height */
    gint w_val = helwan_safe_parse_int(
        gtk_entry_get_text(GTK_ENTRY(w->width_entry)),
        helwan_config_get_width(cfg));
    gint h_val = helwan_safe_parse_int(
        gtk_entry_get_text(GTK_ENTRY(w->height_entry)),
        helwan_config_get_height(cfg));

    if (w_val > 0 && h_val > 0) {
        helwan_config_set_width(cfg, w_val);
        helwan_config_set_height(cfg, h_val);
        gtk_window_resize(parent, w_val, h_val);
    }

    /* Opacity */
    gdouble opacity = gtk_range_get_value(GTK_RANGE(w->opacity_scale));
    helwan_config_set_opacity(cfg, opacity);
    gtk_widget_set_opacity(GTK_WIDGET(parent), opacity);
}

void
helwan_show_preferences (GtkWindow    *parent,
                         GtkNotebook  *notebook,
                         HelwanConfig *cfg)
{
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Preferences", parent,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "Cancel", GTK_RESPONSE_CANCEL,
        "Apply",  GTK_RESPONSE_APPLY,
        "OK",     GTK_RESPONSE_OK,
        NULL);

    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *grid    = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    gtk_box_pack_start(GTK_BOX(content), grid, TRUE, TRUE, 0);

    HelwanPrefsWidgets w = {0};

    /* Row 0 — Font */
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Font:"), 0, 0, 1, 1);
    w.font_chooser = gtk_font_chooser_widget_new();
    gchar *current_font = helwan_config_get_font(cfg);
    PangoFontDescription *fd = pango_font_description_from_string(current_font);
    gtk_font_chooser_set_font_desc(GTK_FONT_CHOOSER(w.font_chooser), fd);
    pango_font_description_free(fd);
    g_free(current_font);
    gtk_grid_attach(GTK_GRID(grid), w.font_chooser, 1, 0, 1, 1);

    /* Row 1 — Width */
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Window Width:"), 0, 1, 1, 1);
    w.width_entry = gtk_entry_new();
    gchar *w_str = g_strdup_printf("%d", helwan_config_get_width(cfg));
    gtk_entry_set_text(GTK_ENTRY(w.width_entry), w_str);
    g_free(w_str);
    gtk_grid_attach(GTK_GRID(grid), w.width_entry, 1, 1, 1, 1);

    /* Row 2 — Height */
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Window Height:"), 0, 2, 1, 1);
    w.height_entry = gtk_entry_new();
    gchar *h_str = g_strdup_printf("%d", helwan_config_get_height(cfg));
    gtk_entry_set_text(GTK_ENTRY(w.height_entry), h_str);
    g_free(h_str);
    gtk_grid_attach(GTK_GRID(grid), w.height_entry, 1, 2, 1, 1);

    /* Row 3 — Opacity */
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Opacity:"), 0, 3, 1, 1);
    GtkAdjustment *adj = gtk_adjustment_new(
        helwan_config_get_opacity(cfg), 0.0, 1.0, 0.01, 0.1, 0.0);
    w.opacity_scale = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, adj);
    gtk_scale_set_digits(GTK_SCALE(w.opacity_scale), 2);
    g_signal_connect(w.opacity_scale, "value-changed",
                     G_CALLBACK(on_opacity_changed), parent);
    gtk_grid_attach(GTK_GRID(grid), w.opacity_scale, 1, 3, 1, 1);

    gtk_widget_show_all(dialog);

    gint response;
    do {
        response = gtk_dialog_run(GTK_DIALOG(dialog));
        if (response == GTK_RESPONSE_APPLY || response == GTK_RESPONSE_OK)
            apply_prefs(&w, parent, notebook, cfg);
    } while (response == GTK_RESPONSE_APPLY);

    gtk_widget_destroy(dialog);
}
