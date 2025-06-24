#ifndef HELWAN_TERMINAL_WINDOW_H
#define HELWAN_TERMINAL_WINDOW_H

#include <gtk/gtk.h>
#include <vte/vte.h> // Include VTE header for VteTerminal type

// Forward declaration of our custom window type
typedef struct _HelwanTerminalWindow HelwanTerminalWindow;
typedef struct _HelwanTerminalWindowClass HelwanTerminalWindowClass;

// Define the structure for your custom window
struct _HelwanTerminalWindow {
    GtkWindow parent_instance;
    // Add any private members for your window here
    GtkWidget *notebook; // Added for managing tabs
};

// Define the class structure for your custom window
struct _HelwanTerminalWindowClass {
    GtkWindowClass parent_class;
};

// Function to get the GType for your custom window
GType helwan_terminal_window_get_type(void);

// Function to create a new instance of your custom window
GtkWidget *create_terminal_window(gint argc, char * const *argv);

// Function to create a new terminal tab
// تم تحديث الإعلان ليتطابق مع التعريف في .c
GtkWidget *helwan_terminal_window_new_tab(HelwanTerminalWindow *self, char * const *command_to_execute);


#endif /* HELWAN_TERMINAL_WINDOW_H */
