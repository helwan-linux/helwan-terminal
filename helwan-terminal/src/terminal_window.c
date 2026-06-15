#include "terminal_window.h"
#include "terminal_tab.h"
#include "preferences.h"
#include <string.h>

G_DEFINE_TYPE(HelwanTerminalWindow, helwan_terminal_window, GTK_TYPE_WINDOW)

/* ------------------------------------------------------------------ */
/* GObject lifecycle                                                    */
/* ------------------------------------------------------------------ */

static void
helwan_terminal_window_dispose (GObject *obj)
{
    HelwanTerminalWindow *self = HELWAN_TERMINAL_WINDOW(obj);
    helwan_config_free(self->config);
    self->config = NULL;
    G_OBJECT_CLASS(helwan_terminal_window_parent_class)->dispose(obj);
}

static void
helwan_terminal_window_class_init (HelwanTerminalWindowClass *klass)
{
    GObjectClass *obj_class = G_OBJECT_CLASS(klass);
    obj_class->dispose = helwan_terminal_window_dispose;
}

static void
helwan_terminal_window_init (HelwanTerminalWindow *self)
{
    (void)self;
}

/* ------------------------------------------------------------------ */
/* Callbacks                                                            */
/* ------------------------------------------------------------------ */

static void
on_new_tab (GtkButton *btn, HelwanTerminalWindow *self)
{
    (void)btn;
    helwan_terminal_window_new_tab(self, NULL);
    gtk_notebook_set_current_page(
        GTK_NOTEBOOK(self->notebook),
        gtk_notebook_get_n_pages(GTK_NOTEBOOK(self->notebook)) - 1);
}

static void
on_preferences (GtkButton *btn, HelwanTerminalWindow *self)
{
    (void)btn;
    helwan_show_preferences(GTK_WINDOW(self),
                            GTK_NOTEBOOK(self->notebook),
                            self->config);
}

static void
on_about (GtkButton *btn, HelwanTerminalWindow *self)
{
    (void)btn;
    gtk_show_about_dialog(GTK_WINDOW(self),
        "program-name", "Helwan Terminal",
        "version",      "0.1.0",
        "copyright",    "© 2025 Helwan Linux",
        "comments",     "A powerful terminal emulator for Helwan Linux.",
        "website",      "https://github.com/helwan-linux/helwan-terminal",
        "authors",      (const char *[]){"Saeed Badrelden", NULL},
        "license-type", GTK_LICENSE_GPL_3_0,
        "logo-icon-name", "helwan-terminal",
        NULL);
}

/* ------------------------------------------------------------------ */
/* Public API                                                           */
/* ------------------------------------------------------------------ */

GtkWidget *
helwan_terminal_window_new_tab (HelwanTerminalWindow *self, char * const *command)
{
    return helwan_tab_new(GTK_NOTEBOOK(self->notebook), self->config, command);
}

GtkWidget *
create_terminal_window (gint argc, char * const *argv)
{
    HelwanTerminalWindow *self = g_object_new(
        HELWAN_TYPE_TERMINAL_WINDOW,
        "title", "Helwan Terminal",
        NULL);

    /* Config */
    self->config = helwan_config_new();

    /* Window size + opacity */
    gtk_window_set_default_size(GTK_WINDOW(self),
        helwan_config_get_width(self->config),
        helwan_config_get_height(self->config));
    gtk_widget_set_app_paintable(GTK_WIDGET(self), TRUE);
    gtk_widget_set_opacity(GTK_WIDGET(self),
        helwan_config_get_opacity(self->config));

    g_signal_connect(self, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* Header bar */
    GtkWidget *header = gtk_header_bar_new();
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header), TRUE);
    gtk_header_bar_set_title(GTK_HEADER_BAR(header), "Helwan Terminal");
    gtk_window_set_titlebar(GTK_WINDOW(self), header);

    GtkWidget *btn_tab   = gtk_button_new_from_icon_name("list-add-symbolic",           GTK_ICON_SIZE_SMALL_TOOLBAR);
    GtkWidget *btn_pref  = gtk_button_new_from_icon_name("preferences-system-symbolic", GTK_ICON_SIZE_SMALL_TOOLBAR);
    GtkWidget *btn_about = gtk_button_new_from_icon_name("help-about-symbolic",         GTK_ICON_SIZE_SMALL_TOOLBAR);

    gtk_button_set_relief(GTK_BUTTON(btn_tab),   GTK_RELIEF_NONE);
    gtk_button_set_relief(GTK_BUTTON(btn_pref),  GTK_RELIEF_NONE);
    gtk_button_set_relief(GTK_BUTTON(btn_about), GTK_RELIEF_NONE);

    g_signal_connect(btn_tab,   "clicked", G_CALLBACK(on_new_tab),     self);
    g_signal_connect(btn_pref,  "clicked", G_CALLBACK(on_preferences), self);
    g_signal_connect(btn_about, "clicked", G_CALLBACK(on_about),       self);

    gtk_header_bar_pack_end(GTK_HEADER_BAR(header), btn_tab);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header), btn_pref);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header), btn_about);

    /* Layout */
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(self), vbox);

    self->notebook = gtk_notebook_new();
    gtk_notebook_set_scrollable(GTK_NOTEBOOK(self->notebook), TRUE);
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(self->notebook), GTK_POS_TOP);
    gtk_box_pack_start(GTK_BOX(vbox), self->notebook, TRUE, TRUE, 0);

    /* First tab */
    char * const *cmd = NULL;
    if (argc > 1 && strcmp(argv[1], "-e") == 0 && argc > 2)
        cmd = &argv[2];
    else if (argc > 1)
        cmd = &argv[1];

    helwan_terminal_window_new_tab(self, cmd);

    return GTK_WIDGET(self);
}
