#include <gtk/gtk.h>
#include <vte/vte.h>
#include "terminal_window.h"

int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create a new terminal window
    GtkWidget *window = create_terminal_window();

    // Show all widgets
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}
