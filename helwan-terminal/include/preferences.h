#ifndef HELWAN_PREFERENCES_H
#define HELWAN_PREFERENCES_H

#include <gtk/gtk.h>
#include "config.h"

void helwan_show_preferences (GtkWindow    *parent,
                              GtkNotebook  *notebook,
                              HelwanConfig *cfg);

#endif /* HELWAN_PREFERENCES_H */
