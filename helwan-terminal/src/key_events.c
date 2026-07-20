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

// دالة التعامل مع ضغطات لوحة المفاتيح
gboolean on_terminal_key_press(GtkWidget *widget, GdkEventKey *event, HelwanTerminalWindow *window) {
    (void)window;

    // Ctrl+Shift+C (Copy)
    if ((event->state & (GDK_CONTROL_MASK | GDK_SHIFT_MASK)) == (GDK_CONTROL_MASK | GDK_SHIFT_MASK) &&
        event->keyval == GDK_KEY_C) {
        vte_terminal_copy_clipboard_format(VTE_TERMINAL(widget), VTE_FORMAT_TEXT);
        return TRUE;
    }
    // Ctrl+Shift+V (Paste)
    else if ((event->state & (GDK_CONTROL_MASK | GDK_SHIFT_MASK)) == (GDK_CONTROL_MASK | GDK_SHIFT_MASK) &&
             event->keyval == GDK_KEY_V) {
        GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
        gchar *text = gtk_clipboard_wait_for_text(clipboard);
        if (text) {
            vte_terminal_paste_text(VTE_TERMINAL(widget), text);
            g_free(text);
        }
        return TRUE;
    }
    // Ctrl++ أو Ctrl+= (Zoom In)
    else if ((event->state & GDK_CONTROL_MASK) && (event->keyval == GDK_KEY_plus || event->keyval == GDK_KEY_equal)) {
        increase_font_size(VTE_TERMINAL(widget));
        return TRUE;
    }
    // Ctrl+- أو Ctrl+_ (Zoom Out)
    else if ((event->state & GDK_CONTROL_MASK) && (event->keyval == GDK_KEY_minus || event->keyval == GDK_KEY_underscore)) {
        decrease_font_size(VTE_TERMINAL(widget));
        return TRUE;
    }
    // Ctrl+0 (Reset Zoom)
    else if ((event->state & GDK_CONTROL_MASK) && event->keyval == GDK_KEY_0) {
        reset_font_size(VTE_TERMINAL(widget));
        return TRUE;
    }

    return FALSE;
}
