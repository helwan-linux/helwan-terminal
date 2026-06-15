#include "terminal_tab.h"
#include "shortcuts.h"

static void
paste_cb (GtkClipboard *cb, const gchar *text, gpointer user_data)
{
    (void)cb;
    if (text)
        vte_terminal_paste_text(VTE_TERMINAL(user_data), text);
}

static void
copy_cb (GtkMenuItem *item, VteTerminal *vte)
{
    (void)item;
    vte_terminal_copy_clipboard_format(vte, VTE_FORMAT_TEXT);
}

static gboolean
on_button_press (GtkWidget *widget, GdkEventButton *event, VteTerminal *vte)
{
    (void)widget;
    if (event->button != GDK_BUTTON_SECONDARY) return FALSE;

    GtkWidget *menu       = gtk_menu_new();
    GtkWidget *copy_item  = gtk_menu_item_new_with_label("Copy");
    GtkWidget *paste_item = gtk_menu_item_new_with_label("Paste");

    gtk_widget_set_sensitive(copy_item, vte_terminal_get_has_selection(vte));

    g_signal_connect(copy_item,  "activate", G_CALLBACK(copy_cb),  vte);
    g_signal_connect(paste_item, "activate", G_CALLBACK(paste_cb), vte);

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), copy_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), paste_item);
    gtk_widget_show_all(menu);
    gtk_menu_popup_at_pointer(GTK_MENU(menu), (GdkEvent *)event);

    return TRUE;
}

static void
on_close_clicked (GtkButton *btn, GtkWidget *vte)
{
    (void)btn;
    GtkWidget *notebook = gtk_widget_get_parent(vte);
    if (!GTK_IS_NOTEBOOK(notebook)) return;
    gint page = gtk_notebook_page_num(GTK_NOTEBOOK(notebook), vte);
    if (page != -1)
        gtk_notebook_remove_page(GTK_NOTEBOOK(notebook), page);
    if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) == 0)
        gtk_main_quit();
}

static void
on_title_changed (VteTerminal *vte, GtkLabel *label)
{
    const gchar *title = vte_terminal_get_window_title(vte);
    gtk_label_set_text(label, title ? title : "Terminal");
}

GtkWidget *
helwan_tab_new (GtkNotebook  *notebook,
                HelwanConfig *cfg,
                char * const *command)
{
    GtkWidget *vte = vte_terminal_new();

    char *default_cmd[] = { "/bin/bash", NULL };
    char **spawn_cmd = (command && command[0]) ? (char **)command : default_cmd;

    vte_terminal_spawn_async(VTE_TERMINAL(vte),
                             VTE_PTY_DEFAULT, NULL,
                             spawn_cmd, NULL,
                             G_SPAWN_DEFAULT,
                             NULL, NULL, NULL,
                             -1, NULL, NULL, NULL);

    vte_terminal_set_size(VTE_TERMINAL(vte), 80, 24);
    helwan_config_apply_font(cfg, VTE_TERMINAL(vte));

    g_signal_connect(vte, "key-press-event",    G_CALLBACK(helwan_on_key_press), cfg);
    g_signal_connect(vte, "button-press-event", G_CALLBACK(on_button_press),     vte);
    gtk_widget_add_events(vte, GDK_BUTTON_PRESS_MASK);

    GtkWidget *hbox      = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
    GtkWidget *label     = gtk_label_new("Terminal");
    GtkWidget *close_btn = gtk_button_new_from_icon_name("window-close-symbolic",
                                                          GTK_ICON_SIZE_SMALL_TOOLBAR);
    gtk_button_set_relief(GTK_BUTTON(close_btn), GTK_RELIEF_NONE);

    g_signal_connect(vte,       "window-title-changed", G_CALLBACK(on_title_changed), label);
    g_signal_connect(close_btn, "clicked",               G_CALLBACK(on_close_clicked), vte);

    gtk_box_pack_start(GTK_BOX(hbox), label,     TRUE,  TRUE,  0);
    gtk_box_pack_start(GTK_BOX(hbox), close_btn, FALSE, FALSE, 0);
    gtk_widget_show_all(hbox);

    gtk_notebook_append_page(notebook, vte, hbox);
    gtk_widget_show_all(vte);

    return vte;
}
