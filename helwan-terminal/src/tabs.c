#include "terminal_window.h"
#include <gtk/gtk.h>
#include <vte/vte.h>
#include <string.h>
#include <stdlib.h>

// تأكد من تعريف هذا المتغير في ملف الإعدادات الخاص بك أو استيراده هنا
extern const char *cached_font_string;

// Callback لإغلاق التبويب
static void on_tab_close_button_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    GtkWidget *vte_widget = GTK_WIDGET(user_data);

    GtkWidget *notebook = gtk_widget_get_parent(vte_widget);
    while (notebook && !GTK_IS_NOTEBOOK(notebook)) {
        notebook = gtk_widget_get_parent(notebook);
    }

    if (notebook && GTK_IS_NOTEBOOK(notebook)) {
        gint page_num = gtk_notebook_page_num(GTK_NOTEBOOK(notebook), vte_widget);
        if (page_num != -1) {
            gtk_notebook_remove_page(GTK_NOTEBOOK(notebook), page_num);
        }

        if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) == 0) {
            gtk_main_quit();
        }
    }
}

// دالة لإنشاء تبويب جديد
GtkWidget *helwan_terminal_window_new_tab(HelwanTerminalWindow *self, char * const *command_to_execute) {
    GtkWidget *vte = vte_terminal_new();

    g_signal_connect(vte, "key-press-event", G_CALLBACK(on_terminal_key_press), self);
    g_signal_connect(vte, "button-press-event", G_CALLBACK(on_terminal_button_press), vte);

    // تشغيل الأمر مع عمل Cast لتجنب تحذير الـ const
    if (command_to_execute != NULL && command_to_execute[0] != NULL) {
        vte_terminal_spawn_async(VTE_TERMINAL(vte),
                                 VTE_PTY_DEFAULT,
                                 NULL,
                                 (char **)command_to_execute,
                                 NULL,
                                 G_SPAWN_SEARCH_PATH,
                                 NULL, NULL, NULL, -1, NULL, NULL, NULL);
    } else {
        char *default_cmd[] = {"/bin/bash", NULL};
        vte_terminal_spawn_async(VTE_TERMINAL(vte),
                                 VTE_PTY_DEFAULT,
                                 NULL,
                                 default_cmd,
                                 NULL,
                                 G_SPAWN_SEARCH_PATH,
                                 NULL, NULL, NULL, -1, NULL, NULL, NULL);
    }

    GtkWidget *label_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *tab_label = gtk_label_new("Terminal");
    GtkWidget *close_button = gtk_button_new_from_icon_name("window-close-symbolic", GTK_ICON_SIZE_MENU);

    gtk_button_set_relief(GTK_BUTTON(close_button), GTK_RELIEF_NONE);
    gtk_box_pack_start(GTK_BOX(label_box), tab_label, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(label_box), close_button, FALSE, FALSE, 0);

    g_signal_connect(close_button, "clicked", G_CALLBACK(on_tab_close_button_clicked), vte);

    gtk_notebook_append_page(GTK_NOTEBOOK(self->notebook), vte, label_box);
    gtk_widget_show_all(label_box);
    gtk_widget_show(vte);

    // تطبيق إعدادات الخط مع التحقق من تعريف المتغير
    if (cached_font_string != NULL) {
        PangoFontDescription *temp_font_desc = pango_font_description_from_string(cached_font_string);
        double applied_font_size = (double)pango_font_description_get_size(temp_font_desc) / PANGO_SCALE;
        gchar *applied_font_family = g_strdup(pango_font_description_get_family(temp_font_desc));
        pango_font_description_free(temp_font_desc);

        apply_font_settings(VTE_TERMINAL(vte), applied_font_family, applied_font_size);
        g_free(applied_font_family);
    }

    return vte;
}

void on_new_tab_button_clicked(GtkButton *button, HelwanTerminalWindow *window) {
    (void)button;
    helwan_terminal_window_new_tab(window, NULL);
    gtk_notebook_set_current_page(GTK_NOTEBOOK(window->notebook),
                                  gtk_notebook_get_n_pages(GTK_NOTEBOOK(window->notebook)) - 1);
}
