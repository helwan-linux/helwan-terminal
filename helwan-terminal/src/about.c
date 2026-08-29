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

    gtk_window_set_default_size(GTK_WINDOW(dialog), 450, 320);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);

    // الحصول على صندوق المحتوى الرئيسي وإضافة مسافة حوله
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_container_set_border_width(GTK_CONTAINER(content_area), 12);

    // إنشاء VBox لتنظيم العناصر فوق بعضها بمسافات منظمة
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_box_pack_start(GTK_BOX(content_area), box, TRUE, TRUE, 0);

    // نص الإرشادات باللغات المختلفة
    GtkWidget *label = gtk_label_new(
        "لإظهار قائمة الأوامر، اكتب: مساعدة\n"
        "To show the commands list, type: help\n"
        "Para mostrar la lista de comandos, escribe: ayuda\n"
        "要显示命令列表，请输入：帮助"
    );
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    // فاصل جمالي بين النص والأزرار
    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(box), separator, FALSE, FALSE, 5);

    // زرار دليل الأوامر (Markdown)
    GtkWidget *btn_md = gtk_link_button_new_with_label(
        "https://github.com/helwan-linux/helwan-terminal/blob/main/arch_terminal_commands.md",
        "Open Commands Guide MD"
    );
    gtk_box_pack_start(GTK_BOX(box), btn_md, FALSE, FALSE, 0);

    // زرار دليل الأوامر (PDF - رابط التحميل المباشر)
    GtkWidget *btn_pdf = gtk_link_button_new_with_label(
        "https://raw.githubusercontent.com/helwan-linux/helwan-terminal/main/arch_terminal_commands.pdf",
        "Download Commands Guide PDF"
    );
    gtk_box_pack_start(GTK_BOX(box), btn_pdf, FALSE, FALSE, 0);

    // إظهار جميع العناصر داخل النافذة
    gtk_widget_show_all(dialog);

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
