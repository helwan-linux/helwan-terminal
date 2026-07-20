#ifndef HELWAN_TERMINAL_WINDOW_H
#define HELWAN_TERMINAL_WINDOW_H

#include <gtk/gtk.h>
#include <vte/vte.h>

// تعريف النوع الأساسي
G_DECLARE_FINAL_TYPE(HelwanTerminalWindow, helwan_terminal_window, HELWAN, TERMINAL_WINDOW, GtkWindow)

struct _HelwanTerminalWindow {
    GtkWindow parent_instance;
    GtkWidget *notebook;
};

struct _HelwanTerminalWindowClass {
    GtkWindowClass parent_class;
};

// دوال عامة
GtkWidget *create_terminal_window(gint argc, char * const *argv);
GtkWidget *helwan_terminal_window_new_tab(HelwanTerminalWindow *self, char * const *command_to_execute);

// دوال الخطوط
void apply_font_settings(VteTerminal *terminal, const char *font_family, double font_size);
void increase_font_size(VteTerminal *terminal);
void decrease_font_size(VteTerminal *terminal);
void reset_font_size(VteTerminal *terminal);

// دوال الكيبورد
gboolean on_terminal_key_press(GtkWidget *widget, GdkEventKey *event, HelwanTerminalWindow *window);

// دوال الماوس
gboolean on_terminal_button_press(GtkWidget *widget, GdkEventButton *event, VteTerminal *terminal);

// دوال التبويبات
void on_new_tab_button_clicked(GtkButton *button, HelwanTerminalWindow *window);

// دوال الإعدادات
void create_preferences_dialog(HelwanTerminalWindow *window);
void on_preferences_button_clicked(GtkButton *button, HelwanTerminalWindow *window);

// دوال حول البرنامج
void on_about_button_clicked(GtkButton *button, HelwanTerminalWindow *window);
void on_help_button_clicked(GtkButton *button, HelwanTerminalWindow *window);

#endif
