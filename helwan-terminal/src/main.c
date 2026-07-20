#include <gtk/gtk.h>
#include "terminal_window.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // مرر argv كما هي إلى الدالة، وتأكد أن منطق التحليل داخل create_terminal_window
    GtkWidget *window = create_terminal_window(argc, argv);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
