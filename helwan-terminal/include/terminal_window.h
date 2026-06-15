#ifndef HELWAN_TERMINAL_WINDOW_H
#define HELWAN_TERMINAL_WINDOW_H

#include <gtk/gtk.h>
#include <gobject/gobject.h>
#include <vte/vte.h>
#include "config.h"

G_BEGIN_DECLS

#define HELWAN_TYPE_TERMINAL_WINDOW (helwan_terminal_window_get_type())

G_DECLARE_FINAL_TYPE(HelwanTerminalWindow,
                     helwan_terminal_window,
                     HELWAN, TERMINAL_WINDOW,
                     GtkWindow)

struct _HelwanTerminalWindow {
    GtkWindow     parent_instance;
    GtkWidget    *notebook;
    HelwanConfig *config;
};

GtkWidget *create_terminal_window        (gint argc, char * const *argv);
GtkWidget *helwan_terminal_window_new_tab(HelwanTerminalWindow *self,
                                          char * const         *command);

G_END_DECLS

#endif /* HELWAN_TERMINAL_WINDOW_H */
