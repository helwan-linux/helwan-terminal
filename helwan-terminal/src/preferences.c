#include "terminal_window.h"
#include <gtk/gtk.h>
#include <vte/vte.h>
#include <pango/pangocairo.h>
#include <gio/gio.h>
#include <string.h>
#include <stdlib.h>

// المتغيرات العامة
extern GSettings *settings;
extern gchar *cached_font_string;

// دالة لتطبيق الشفافية مباشرة عند تحريك المزلاج
static void on_opacity_scale_value_changed(GtkRange *range, HelwanTerminalWindow *window) {
    double new_opacity = gtk_range_get_value(range);
    gtk_widget_set_opacity(GTK_WIDGET(window), new_opacity);
}

// Callback لتطبيق الإعدادات
static void on_apply_preferences_clicked(GtkButton *button, gpointer user_data) {
    (void)button;

    GtkWidget *dialog = GTK_WIDGET(user_data);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GList *children = gtk_container_get_children(GTK_CONTAINER(content_area));
    GtkWidget *grid = NULL;
    if (children) {
        grid = GTK_WIDGET(children->data);
        g_list_free(children);
    }

    if (!grid || !GTK_IS_GRID(grid)) {
        g_warning("Preferences dialog grid not found.");
        return;
    }

    // Font
    GtkWidget *font_chooser_widget = gtk_grid_get_child_at(GTK_GRID(grid), 1, 0);
    if (font_chooser_widget && GTK_IS_FONT_CHOOSER(font_chooser_widget)) {
        PangoFontDescription *font_desc = gtk_font_chooser_get_font_desc(GTK_FONT_CHOOSER(font_chooser_widget));
        if (font_desc) {
            gchar *new_font_string = pango_font_description_to_string(font_desc);
            pango_font_description_free(font_desc);
            if (settings) {
                g_settings_set_string(settings, "font-family", new_font_string);
            }
            g_free(cached_font_string);
            cached_font_string = g_strdup(new_font_string);
            g_free(new_font_string);
        }
    }

    // Window Width
    GtkWidget *width_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 1);
    int new_width = atoi(gtk_entry_get_text(GTK_ENTRY(width_entry)));
    if (settings) {
        g_settings_set_int(settings, "window-width", new_width);
    }

    // Window Height
    GtkWidget *height_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 2);
    int new_height = atoi(gtk_entry_get_text(GTK_ENTRY(height_entry)));
    if (settings) {
        g_settings_set_int(settings, "window-height", new_height);
    }

    // Opacity
    GtkWidget *opacity_scale = gtk_grid_get_child_at(GTK_GRID(grid), 1, 3);
    double new_opacity = gtk_range_get_value(GTK_RANGE(opacity_scale));
    if (settings) {
        g_settings_set_double(settings, "opacity", new_opacity);
    }

    HelwanTerminalWindow *window = HELWAN_TERMINAL_WINDOW(gtk_window_get_transient_for(GTK_WINDOW(dialog)));
    if (window) {
        // Apply font to all VTE instances
        gint n_pages = gtk_notebook_get_n_pages(GTK_NOTEBOOK(window->notebook));
        for (gint i = 0; i < n_pages; i++) {
            GtkWidget *vte_widget = gtk_notebook_get_nth_page(GTK_NOTEBOOK(window->notebook), i);
            if (VTE_IS_TERMINAL(vte_widget)) {
                PangoFontDescription *temp_font_desc = pango_font_description_from_string(cached_font_string);
                double applied_font_size = (double)pango_font_description_get_size(temp_font_desc) / PANGO_SCALE;
                gchar *applied_font_family = g_strdup(pango_font_description_get_family(temp_font_desc));
                pango_font_description_free(temp_font_desc);

                apply_font_settings(VTE_TERMINAL(vte_widget), applied_font_family, applied_font_size);
                g_free(applied_font_family);
            }
        }

        // Apply window size
        if (new_width > 0 && new_height > 0) {
            gtk_window_resize(GTK_WINDOW(window), new_width, new_height);
        }

        gtk_widget_set_opacity(GTK_WIDGET(window), new_opacity);
    }
}

// إنشاء نافذة الإعدادات
void create_preferences_dialog(HelwanTerminalWindow *window) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Preferences",
                                                    GTK_WINDOW(window),
                                                    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                    "Cancel", GTK_RESPONSE_CANCEL,
                                                    "Apply", GTK_RESPONSE_APPLY,
                                                    "Ok", GTK_RESPONSE_OK,
                                                    NULL);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    gtk_box_pack_start(GTK_BOX(content_area), grid, TRUE, TRUE, 0);

    // Font chooser
    GtkWidget *font_label = gtk_label_new("Font:");
    gtk_grid_attach(GTK_GRID(grid), font_label, 0, 0, 1, 1);
    GtkWidget *font_chooser_widget = gtk_font_chooser_widget_new();
    gtk_grid_attach(GTK_GRID(grid), font_chooser_widget, 1, 0, 1, 1);

    gchar *font_from_settings = settings ? g_settings_get_string(settings, "font-family") : NULL;
    if (!font_from_settings) {
        font_from_settings = g_strdup("monospace 10");
    }
    PangoFontDescription *initial_font_desc = pango_font_description_from_string(font_from_settings);
    gtk_font_chooser_set_font_desc(GTK_FONT_CHOOSER(font_chooser_widget), initial_font_desc);
    pango_font_description_free(initial_font_desc);
    g_free(font_from_settings);

    // Window Width
    GtkWidget *width_label = gtk_label_new("Window Width:");
    gtk_grid_attach(GTK_GRID(grid), width_label, 0, 1, 1, 1);
    GtkWidget *width_entry = gtk_entry_new();
    gint initial_width = settings ? g_settings_get_int(settings, "window-width") : 800;
    gchar *width_str = g_strdup_printf("%d", initial_width);
    gtk_entry_set_text(GTK_ENTRY(width_entry), width_str);
    g_free(width_str);
    gtk_grid_attach(GTK_GRID(grid), width_entry, 1, 1, 1, 1);

    // Window Height
    GtkWidget *height_label = gtk_label_new("Window Height:");
    gtk_grid_attach(GTK_GRID(grid), height_label, 0, 2, 1, 1);
    GtkWidget *height_entry = gtk_entry_new();
    gint initial_height = settings ? g_settings_get_int(settings, "window-height") : 600;
    gchar *height_str = g_strdup_printf("%d", initial_height);
    gtk_entry_set_text(GTK_ENTRY(height_entry), height_str);
    g_free(height_str);
    gtk_grid_attach(GTK_GRID(grid), height_entry, 1, 2, 1, 1);

    // Opacity
    GtkWidget *opacity_label = gtk_label_new("Opacity:");
    gtk_grid_attach(GTK_GRID(grid), opacity_label, 0, 3, 1, 1);
    GtkAdjustment *adjustment = gtk_adjustment_new(0.85, 0.0, 1.0, 0.01, 0.1, 0.0);
    GtkWidget *opacity_scale = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, adjustment);
    gtk_scale_set_digits(GTK_SCALE(opacity_scale), 2);
    if (settings) {
        gtk_adjustment_set_value(adjustment, g_settings_get_double(settings, "opacity"));
    }
    gtk_grid_attach(GTK_GRID(grid), opacity_scale, 1, 3, 1, 1);

    g_signal_connect(opacity_scale, "value-changed", G_CALLBACK(on_opacity_scale_value_changed), window);

    gtk_widget_show_all(dialog);

    gint response;
    do {
        response = gtk_dialog_run(GTK_DIALOG(dialog));
        if (response == GTK_RESPONSE_APPLY) {
            on_apply_preferences_clicked(NULL, dialog);
        } else if (response == GTK_RESPONSE_OK) {
            on_apply_preferences_clicked(NULL, dialog);
            break;
        } else if (response == GTK_RESPONSE_CANCEL) {
            break;
        }
    } while (TRUE);

    gtk_widget_destroy(dialog);
}


// Callback لزر Preferences
void on_preferences_button_clicked(GtkButton *button, HelwanTerminalWindow *window) {
    (void)button;
    create_preferences_dialog(window);
}
