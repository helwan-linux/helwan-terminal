#ifndef HELWAN_TERMINAL_WINDOW_H
#define HELWAN_TERMINAL_WINDOW_H

#include <gtk/gtk.h>
#include <gobject/gobject.h> // Explicitly include gobject for G_DECLARE_FINAL_TYPE
#include <vte/vte.h>         // Include VTE header for VteTerminal type

// G_DECLARE_FINAL_TYPE generates the necessary type registration functions
// and casting macros (like HELWAN_TERMINAL_WINDOW) automatically.
// The 5th argument should be the C struct name of the parent type, which is 'GtkWindow'.
G_DECLARE_FINAL_TYPE(HelwanTerminalWindow, helwan_terminal_window, HELWAN, TERMINAL_WINDOW, GtkWindow)

// Define the structure for your custom window explicitly
// This completes the 'typedef' definition for the compiler
struct _HelwanTerminalWindow {
    GtkWindow parent_instance; // GtkWindow is the direct parent instance
    GtkWidget *notebook;       // Added for managing tabs
};

// Function to create a new instance of your custom window
GtkWidget *create_terminal_window(gint argc, char * const *argv);

// Function to create a new terminal tab
GtkWidget *helwan_terminal_window_new_tab(HelwanTerminalWindow *self, char * const *command_to_execute);


#endif /* HELWAN_TERMINAL_WINDOW_H */
