#ifndef HELWAN_SHORTCUTS_H
#define HELWAN_SHORTCUTS_H

#include <gtk/gtk.h>
#include <vte/vte.h>
#include "config.h"

gboolean helwan_on_key_press (GtkWidget *widget, GdkEventKey *event, HelwanConfig *cfg);

#endif /* HELWAN_SHORTCUTS_H */
