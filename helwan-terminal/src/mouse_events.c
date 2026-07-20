#include "terminal_window.h"
#include <gtk/gtk.h>
#include <vte/vte.h>
#include <string.h>
#include <stdlib.h>

// Callback لنسخ النص من القائمة
static void on_copy_menu_item_activated(GtkMenuItem *menu_item, VteTerminal *terminal) {
    (void)menu_item;
    vte_terminal_copy_clipboard_format(terminal, VTE_FORMAT_TEXT);
}

// Callback للصق النص من القائمة
static void on_paste_menu_item_activated(GtkMenuItem *menu_item, VteTerminal *terminal) {
    (void)menu_item;
    GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gchar *text = gtk_clipboard_wait_for_text(clipboard);
    if (text) {
        vte_terminal_paste_text(terminal, text);
        g_free(text);
    }
}

// Callback للتعامل مع ضغطات الماوس على الـ VTE
gboolean on_terminal_button_press(GtkWidget *widget, GdkEventButton *event, VteTerminal *terminal) {
    (void)widget;

    // Right-click (زر الماوس الأيمن)
    if (event->button == GDK_BUTTON_SECONDARY) {
        GtkWidget *menu = gtk_menu_new();
        GtkWidget *copy_item = gtk_menu_item_new_with_label("Copy");
        GtkWidget *paste_item = gtk_menu_item_new_with_label("Paste");

        g_signal_connect(copy_item, "activate", G_CALLBACK(on_copy_menu_item_activated), terminal);
        g_signal_connect(paste_item, "activate", G_CALLBACK(on_paste_menu_item_activated), terminal);

        gtk_menu_shell_append(GTK_MENU_SHELL(menu), copy_item);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), paste_item);

        // تعطيل زر النسخ لو مفيش تحديد
        if (!vte_terminal_get_has_selection(terminal)) {
            gtk_widget_set_sensitive(copy_item, FALSE);
        }

        // تعطيل زر اللصق لو الكليب بورد فاضي
        GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
        gchar *clipboard_text = gtk_clipboard_wait_for_text(clipboard);
        if (!clipboard_text) {
            gtk_widget_set_sensitive(paste_item, FALSE);
        } else {
            g_free(clipboard_text);
        }

        gtk_widget_show_all(menu);
        gtk_menu_popup_at_pointer(GTK_MENU(menu), (GdkEvent*)event);

        return TRUE;
    }
    return FALSE;
}
