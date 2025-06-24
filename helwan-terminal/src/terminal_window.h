#ifndef HELWAN_TERMINAL_WINDOW_H
#define HELWAN_TERMINAL_WINDOW_H

#include <gtk/gtk.h>
#include <vte/vte.h> // Include VTE header for VteTerminal type

G_BEGIN_DECLS

// 1. Define the instance structure
// This structure holds the data specific to each instance of HelwanTerminalWindow.
// It must inherit from GtkWindow by having GtkWindow parent_instance; as its first member.
typedef struct _HelwanTerminalWindow {
    GtkWindow parent_instance; // Must be the first member, inherited from GtkWindow

    // Add your custom members here
    GtkWidget *notebook; // The GtkNotebook widget to manage tabs
} HelwanTerminalWindow;

// 2. G_DECLARE_FINAL_TYPE will handle the declaration of HelwanTerminalWindowClass for us.
// We DO NOT need to define the class struct manually here.
G_DECLARE_FINAL_TYPE(HelwanTerminalWindow, helwan_terminal_window, HELWAN_TERMINAL, WINDOW, GtkWindow)


// Public function to create a new terminal window
GtkWidget *create_terminal_window();

// Public function to add a new tab to the terminal window
GtkWidget *helwan_terminal_window_new_tab(HelwanTerminalWindow *self);


G_END_DECLS

#endif // HELWAN_TERMINAL_WINDOW_H
