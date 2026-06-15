#include <gtk/gtk.h>
#include "terminal_window.h"

int
main (int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkWidget *window = create_terminal_window(argc, (char * const *)argv);
    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}
