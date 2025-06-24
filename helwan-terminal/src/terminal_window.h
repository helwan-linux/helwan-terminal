#ifndef HELWAN_TERMINAL_WINDOW_H
#define HELWAN_TERMINAL_WINDOW_H

#include <gtk/gtk.h>
#include <gobject/gobject.h> // Explicitly include gobject for G_DECLARE_FINAL_TYPE
#include <vte/vte.h>         // Include VTE header for VteTerminal type

// G_DECLARE_FINAL_TYPE generates the necessary type registration functions
// and casting macros (like HELWAN_TERMINAL_WINDOW) automatically.
// This macro also handles the forward declaration and basic structure definition.
G_DECLARE_FINAL_TYPE(HelwanTerminalWindow, helwan_terminal_window, HELWAN, TERMINAL_WINDOW, GTK_TYPE_WINDOW)

// Function to create a new instance of your custom window
GtkWidget *create_terminal_window(gint argc, char * const *argv);

// Function to create a new terminal tab
GtkWidget *helwan_terminal_window_new_tab(HelwanTerminalWindow *self, char * const *command_to_execute);


#endif /* HELWAN_TERMINAL_WINDOW_H */
