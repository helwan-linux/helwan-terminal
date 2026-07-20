#include "terminal_window.h"
#include <gtk/gtk.h>

// Callback لزر "About"
void on_about_button_clicked(GtkButton *button, HelwanTerminalWindow *window) {
    (void)button; // مش مستخدم

    gtk_show_about_dialog(GTK_WINDOW(window),
                          "program-name", "Helwan Terminal",
                          "version", "0.1.4", // رقم الإصدار
                          "copyright", "© 2025 Helwan Linux",
                          "comments", "A powerful terminal emulator developed at Helwan Linux.",
                          "website", "https://github.com/helwan-linux/helwan-terminal",
                          "authors", (const char *[]){"Saeed Badrelden", NULL},
                          "license-type", GTK_LICENSE_GPL_3_0,
                          "logo-icon-name", "helwan-terminal",
                          NULL);
}

// Callback لزر "Help"
void on_help_button_clicked(GtkButton *button, HelwanTerminalWindow *window) {
    (void)button;

    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "================ Helwan Linux Terminal Help ================\n\n"
        "1. System Version:\n"
        "   الإصدار | version | versión | 版本\n\n"
        "2. Repository Sync:\n"
        "   مزامنة | sync | sincronizar | 同步\n\n"
        "3. Install Packages:\n"
        "   تثبيت | install | instalar | 安装\n\n"
        "4. Update System:\n"
        "   تحديث | update | actualizar | 更新\n\n"
        "5. Search Packages (Online):\n"
        "   بحث | search | buscar | 搜索\n\n"
        "6. Search Packages (Local):\n"
        "   بحث_محلي | search-local | buscar-local | 本地搜索\n\n"
        "7. Package Info:\n"
        "   معلومات_الحزمة | pkg-info | info-paquete | 软件包信息\n\n"
        "8. Local Install:\n"
        "   تثبيت_محلي | local-install | instalar-local | 本地安装\n\n"
        "9. Clear Cache:\n"
        "   تفريغ_التخزين_المؤقت | clr-cache | limpiar-caché | 清理缓存\n\n"
        "10. Unlock Database:\n"
        "   فك_القفل | unlock | desbloquear | 解锁\n\n"
        "11. Remove Package:\n"
        "   حذف | remove | eliminar | 删除\n\n"
        "12. Autoremove Orphans:\n"
        "   تنظيف_المخلفات | autoremove | limpiar-automatico | 自动清理\n\n"
        "13. System Info:\n"
        "   حلوان | helwan | helwán | 赫尔万\n\n"
        "============================================================");

    gtk_window_set_title(GTK_WINDOW(dialog), "Helwan Terminal Help");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
