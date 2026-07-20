#include "terminal_window.h"
#include <gtk/gtk.h>
#include <vte/vte.h>
#include <pango/pangocairo.h>
#include <gio/gio.h>
#include <string.h>
#include <stdlib.h>

// تعريف النوع الأساسي
G_DEFINE_TYPE(HelwanTerminalWindow, helwan_terminal_window, GTK_TYPE_WINDOW)

// متغيرات عامة للإعدادات والخط
GSettings *settings = NULL;
gchar *cached_font_string = NULL;

// دالة init
static void helwan_terminal_window_init(HelwanTerminalWindow *self) {
    (void)self;
}

// دالة class_init
static void helwan_terminal_window_class_init(HelwanTerminalWindowClass *klass) {
    (void)klass;
}

// إنشاء النافذة الرئيسية
GtkWidget *create_terminal_window(gint argc, char * const *argv) {
    if (!settings) {
        settings = g_settings_new("org.helwan_terminal.gschema");
    }

    if (!cached_font_string) {
        cached_font_string = g_settings_get_string(settings, "font-family");
        if (!cached_font_string) {
            cached_font_string = g_strdup("monospace 10");
        }
    }

    double initial_opacity = 0.85;
    if (settings) {
        initial_opacity = g_settings_get_double(settings, "opacity");
    }

    gint initial_window_width = g_settings_get_int(settings, "window-width");
    gint initial_window_height = g_settings_get_int(settings, "window-height");

    HelwanTerminalWindow *window = g_object_new(helwan_terminal_window_get_type(),
                                                "title", "Helwan Terminal",
                                                "default-width", initial_window_width,
                                                "default-height", initial_window_height,
                                                NULL);

    gtk_widget_set_app_paintable(GTK_WIDGET(window), TRUE);
    gtk_widget_set_opacity(GTK_WIDGET(window), initial_opacity);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Header bar
    GtkWidget *header_bar = gtk_header_bar_new();
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header_bar), TRUE);
    gtk_header_bar_set_title(GTK_HEADER_BAR(header_bar), "Helwan Terminal");
    gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);

    // زر تبويب جديد
    GtkWidget *new_tab_button = gtk_button_new_from_icon_name("list-add-symbolic", GTK_ICON_SIZE_SMALL_TOOLBAR);
    gtk_button_set_relief(GTK_BUTTON(new_tab_button), GTK_RELIEF_NONE);
    g_signal_connect(new_tab_button, "clicked", G_CALLBACK(on_new_tab_button_clicked), window);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), new_tab_button);

    // زر الإعدادات
    GtkWidget *settings_button = gtk_button_new_from_icon_name("preferences-system-symbolic", GTK_ICON_SIZE_SMALL_TOOLBAR);
    gtk_button_set_relief(GTK_BUTTON(settings_button), GTK_RELIEF_NONE);
    g_signal_connect(settings_button, "clicked", G_CALLBACK(on_preferences_button_clicked), window);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), settings_button);

    // زر حول البرنامج
    GtkWidget *about_button = gtk_button_new_from_icon_name("help-about-symbolic", GTK_ICON_SIZE_SMALL_TOOLBAR);
    gtk_button_set_relief(GTK_BUTTON(about_button), GTK_RELIEF_NONE);
    g_signal_connect(about_button, "clicked", G_CALLBACK(on_about_button_clicked), window);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), about_button);
    
    // زر المساعدة
    GtkWidget *help_button = gtk_button_new_from_icon_name("help-contents-symbolic", GTK_ICON_SIZE_SMALL_TOOLBAR);
    gtk_button_set_relief(GTK_BUTTON(help_button), GTK_RELIEF_NONE);
    g_signal_connect(help_button, "clicked", G_CALLBACK(on_help_button_clicked), window);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), help_button);

    // VBox + Notebook
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    window->notebook = gtk_notebook_new();
    gtk_notebook_set_scrollable(GTK_NOTEBOOK(window->notebook), TRUE);
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(window->notebook), GTK_POS_TOP);

    gtk_box_pack_start(GTK_BOX(vbox), window->notebook, TRUE, TRUE, 0);

    // تشغيل تبويب أولي
    char **spawn_argv = NULL;
    if (argc > 2 && strcmp(argv[1], "-e") == 0) {
        GString *command_string = g_string_new(argv[2]);
        for (int i = 3; i < argc; i++) {
            g_string_append_printf(command_string, " %s", argv[i]);
        }
        
        if (g_shell_parse_argv(command_string->str, NULL, &spawn_argv, NULL)) {
            helwan_terminal_window_new_tab(window, spawn_argv);
            g_strfreev(spawn_argv);
        } else {
            helwan_terminal_window_new_tab(window, NULL);
        }
        g_string_free(command_string, TRUE);
    } else {
        helwan_terminal_window_new_tab(window, NULL);
    }
    
    return GTK_WIDGET(window);
}
