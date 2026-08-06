#include "terminal_window.h"
#include <gtk/gtk.h>

// Callback لزر "About"
void on_about_button_clicked(GtkButton *button, HelwanTerminalWindow *window) {
    (void)button; // مش مستخدم

    gtk_show_about_dialog(GTK_WINDOW(window),
        "program-name", "Helwan Terminal",
        "version", "0.1.5",
        "copyright", "© 2025 Helwan Linux",
        "comments", "A powerful terminal emulator developed at Helwan Linux.\n\n"
                    "• Copy/Paste: Ctrl + Shift + C / V\n"
                    "• Zoom: Ctrl + + / - / 0\n"
                    "• Settings: Click the settings icon in the top bar\n"
                    "• New Tab: Click the plus icon to start a new session",
        "website", "https://github.com/helwan-linux/helwan-terminal",
        "authors", (const char *[]){"Saeed Badreldin", NULL},
        "license-type", GTK_LICENSE_GPL_3_0,
        "logo-icon-name", "helwan-terminal",
        NULL);
}

// Callback لزر "Help"
void on_help_button_clicked(GtkButton *button, HelwanTerminalWindow *window) {
    (void)button;

    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Helwan Terminal Help",
        GTK_WINDOW(window),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "OK",
        GTK_RESPONSE_OK,
        NULL
    );

    GtkWidget *box = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    GtkWidget *label = gtk_label_new(
        "لإظهار قائمة الأوامر، اكتب: مساعدة\n"
        "To show the commands list, type: help\n"
        "Para mostrar la lista de comandos, escribe: ayuda\n"
        "要显示命令列表，请输入：帮助"
    );

    GtkWidget *link = gtk_link_button_new_with_label(
        "https://github.com/helwan-linux/helwan-terminal/blob/main/arch_terminal_commands.md",
        "Open Commands Guide"
    );

    gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(box), link, TRUE, TRUE, 10);

    gtk_widget_show_all(dialog);

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
