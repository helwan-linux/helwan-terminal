#include <gtk/gtk.h>
#include "terminal_window.h" // عشان يتعرف على create_terminal_window

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv); // لازم تعمل init للـ GTK وتمررله argc و argv

    GtkWidget *window = create_terminal_window(argc, argv); // ****** لازم تعدل السطر ده ******

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
