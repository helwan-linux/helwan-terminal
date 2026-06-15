#ifndef HELWAN_TERMINAL_TAB_H
#define HELWAN_TERMINAL_TAB_H

#include <gtk/gtk.h>
#include <vte/vte.h>
#include "config.h"

GtkWidget *helwan_tab_new (GtkNotebook  *notebook,
                           HelwanConfig *cfg,
                           char * const *command);

#endif /* HELWAN_TERMINAL_TAB_H */
