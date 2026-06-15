#include "shortcuts.h"

static void
paste_received_cb (GtkClipboard *clipboard, const gchar *text, gpointer user_data)
{
    (void)clipboard;
    if (text)
        vte_terminal_paste_text(VTE_TERMINAL(user_data), text);
}

gboolean
helwan_on_key_press (GtkWidget *widget, GdkEventKey *event, HelwanConfig *cfg)
{
    VteTerminal *vte = VTE_TERMINAL(widget);
    GdkModifierType ctrl_shift = GDK_CONTROL_MASK | GDK_SHIFT_MASK;

    /* Ctrl+Shift+C — Copy */
    if ((event->state & ctrl_shift) == ctrl_shift && event->keyval == GDK_KEY_C) {
        vte_terminal_copy_clipboard_format(vte, VTE_FORMAT_TEXT);
        return TRUE;
    }

    /* Ctrl+Shift+V — Paste (async) */
    if ((event->state & ctrl_shift) == ctrl_shift && event->keyval == GDK_KEY_V) {
        GtkClipboard *cb = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
        gtk_clipboard_request_text(cb, paste_received_cb, vte);
        return TRUE;
    }

    /* Ctrl++ — Zoom In */
    if ((event->state & GDK_CONTROL_MASK) &&
        (event->keyval == GDK_KEY_plus || event->keyval == GDK_KEY_equal)) {
        helwan_config_change_font_size(cfg, vte, +1.0);
        return TRUE;
    }

    /* Ctrl+- — Zoom Out */
    if ((event->state & GDK_CONTROL_MASK) &&
        (event->keyval == GDK_KEY_minus || event->keyval == GDK_KEY_underscore)) {
        helwan_config_change_font_size(cfg, vte, -1.0);
        return TRUE;
    }

    /* Ctrl+0 — Reset Zoom */
    if ((event->state & GDK_CONTROL_MASK) && event->keyval == GDK_KEY_0) {
        helwan_config_reset_font(cfg, vte);
        return TRUE;
    }

    return FALSE;
}
