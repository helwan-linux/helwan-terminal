#include "terminal_window.h"
#include <vte/vte.h>
#include <gtk/gtk.h>
#include <pango/pangocairo.h>
#include <stdlib.h> // For atoi
#include <gio/gio.h> // For GSettings
#include <string.h> // For strcmp

// Define our custom window type
G_DEFINE_TYPE(HelwanTerminalWindow, helwan_terminal_window, GTK_TYPE_WINDOW);

// Private function to initialize the HelwanTerminalWindow
static void helwan_terminal_window_init(HelwanTerminalWindow *self) {
    (void)self;
}

// Private function to set up the HelwanTerminalWindow's class
static void helwan_terminal_window_class_init(HelwanTerminalWindowClass *klass) {
    (void)klass;
}

// Global variable for GSettings and cached font string
static GSettings *settings = NULL;
static gchar *cached_font_string = NULL; // To store "Font Family Size" string from GSettings

// Function to apply font settings to a VTE terminal
static void apply_font_settings(VteTerminal *terminal, const char *font_family, double font_size) {
    PangoFontDescription *font_desc = pango_font_description_from_string(font_family);
    pango_font_description_set_size(font_desc, font_size * PANGO_SCALE);
    vte_terminal_set_font(terminal, font_desc);
    pango_font_description_free(font_desc);
}

// Callback for key press events on the VTE terminal
static gboolean on_terminal_key_press(GtkWidget *widget, GdkEventKey *event, HelwanTerminalWindow *window) {
    (void)window;

    // Check for Ctrl+Shift+C (Copy)
    if ((event->state & (GDK_CONTROL_MASK | GDK_SHIFT_MASK)) == (GDK_CONTROL_MASK | GDK_SHIFT_MASK) &&
        event->keyval == GDK_KEY_C) {
        vte_terminal_copy_clipboard(VTE_TERMINAL(widget));
        return TRUE;
    }
    // Check for Ctrl+Shift+V (Paste)
    else if ((event->state & (GDK_CONTROL_MASK | GDK_SHIFT_MASK)) == (GDK_CONTROL_MASK | GDK_SHIFT_MASK) &&
             event->keyval == GDK_KEY_V) {
        vte_terminal_paste_clipboard(VTE_TERMINAL(widget));
        return TRUE;
    }
    // Check for Ctrl++ (Zoom In) - Try both GDK_KEY_equal (for '+') and GDK_KEY_plus
    else if ((event->state & GDK_CONTROL_MASK) && (event->keyval == GDK_KEY_plus || event->keyval == GDK_KEY_equal)) {
        PangoFontDescription *temp_font_desc = pango_font_description_from_string(cached_font_string);
        double current_font_size = (double)pango_font_description_get_size(temp_font_desc) / PANGO_SCALE;
        gchar *current_font_family = g_strdup(pango_font_description_get_family(temp_font_desc)); // Get current family
        pango_font_description_free(temp_font_desc);

        current_font_size += 1.0;

        g_free(cached_font_string); // Free old string
        // Create new font string from extracted family and new size
        cached_font_string = g_strdup_printf("%s %g", current_font_family, current_font_size);
        g_free(current_font_family); // Free family string

        // Apply font settings to the current terminal
        PangoFontDescription *new_font_desc_for_apply = pango_font_description_from_string(cached_font_string);
        double new_applied_size = (double)pango_font_description_get_size(new_font_desc_for_apply) / PANGO_SCALE;
        gchar *new_applied_family = g_strdup(pango_font_description_get_family(new_font_desc_for_apply));
        pango_font_description_free(new_font_desc_for_apply);
        apply_font_settings(VTE_TERMINAL(widget), new_applied_family, new_applied_size);
        g_free(new_applied_family);

        return TRUE;
    }
    // Check for Ctrl+- (Zoom Out) - Try both GDK_KEY_minus and GDK_KEY_underscore (for '-')
    else if ((event->state & GDK_CONTROL_MASK) && (event->keyval == GDK_KEY_minus || event->keyval == GDK_KEY_underscore)) {
        PangoFontDescription *temp_font_desc = pango_font_description_from_string(cached_font_string);
        double current_font_size = (double)pango_font_description_get_size(temp_font_desc) / PANGO_SCALE;
        gchar *current_font_family = g_strdup(pango_font_description_get_family(temp_font_desc)); // Get current family
        pango_font_description_free(temp_font_desc);

        if (current_font_size > 1.0) {
            current_font_size -= 1.0;
            g_free(cached_font_string); // Free old string
            // Create new font string from extracted family and new size
            cached_font_string = g_strdup_printf("%s %g", current_font_family, current_font_size);
            
            // Apply font settings to the current terminal
            PangoFontDescription *new_font_desc_for_apply = pango_font_description_from_string(cached_font_string);
            double new_applied_size = (double)pango_font_description_get_size(new_font_desc_for_apply) / PANGO_SCALE;
            gchar *new_applied_family = g_strdup(pango_font_description_get_family(new_font_desc_for_apply));
            pango_font_description_free(new_font_desc_for_apply);
            apply_font_settings(VTE_TERMINAL(widget), new_applied_family, new_applied_size);
            g_free(new_applied_family);
        }
        g_free(current_font_family); // Free family string
        return TRUE;
    }
    // Check for Ctrl+0 (Reset Zoom)
    else if ((event->state & GDK_CONTROL_MASK) && event->keyval == GDK_KEY_0) {
        g_free(cached_font_string);
        cached_font_string = g_strdup("monospace 10"); // Reset to default string
        PangoFontDescription *temp_font_desc = pango_font_description_from_string(cached_font_string);
        double reset_font_size = (double)pango_font_description_get_size(temp_font_desc) / PANGO_SCALE;
        gchar *reset_font_family = g_strdup(pango_font_description_get_family(temp_font_desc));
        pango_font_description_free(temp_font_desc);
        apply_font_settings(VTE_TERMINAL(widget), reset_font_family, reset_font_size);
        g_free(reset_font_family);
        return TRUE;
    }

    return FALSE;
}

// Callback for "Copy" menu item
static void on_copy_menu_item_activated(GtkMenuItem *menu_item, VteTerminal *terminal) {
    (void)menu_item;
    vte_terminal_copy_clipboard(terminal);
}

// Callback for "Paste" menu item
static void on_paste_menu_item_activated(GtkMenuItem *menu_item, VteTerminal *terminal) {
    (void)menu_item;
    vte_terminal_paste_clipboard(terminal);
}

// Callback for mouse button press event on the VTE terminal
static gboolean on_terminal_button_press(GtkWidget *widget, GdkEventButton *event, VteTerminal *terminal) {
    (void)widget;

    // Check for right-click (button 3)
    if (event->button == GDK_BUTTON_SECONDARY) {
        GtkWidget *menu = gtk_menu_new();
        GtkWidget *copy_item = gtk_menu_item_new_with_label("Copy");
        GtkWidget *paste_item = gtk_menu_item_new_with_label("Paste");

        g_signal_connect(copy_item, "activate", G_CALLBACK(on_copy_menu_item_activated), terminal);
        g_signal_connect(paste_item, "activate", G_CALLBACK(on_paste_menu_item_activated), terminal);

        gtk_menu_shell_append(GTK_MENU_SHELL(menu), copy_item);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), paste_item);

        if (!vte_terminal_get_has_selection(terminal)) {
            gtk_widget_set_sensitive(copy_item, FALSE);
        }
        GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
        if (!gtk_clipboard_wait_for_text(clipboard)) {
             gtk_widget_set_sensitive(paste_item, FALSE);
        }

        gtk_widget_show_all(menu);
        gtk_menu_popup_at_pointer(GTK_MENU(menu), (GdkEvent*)event);

        return TRUE;
    }
    return FALSE;
}

// Callback for closing a tab
static void on_tab_close_button_clicked(GtkButton *button, GtkWidget *vte_widget) {
    (void)button;
    GtkWidget *notebook = gtk_widget_get_parent(gtk_widget_get_parent(vte_widget));
    gint page_num = gtk_notebook_page_num(GTK_NOTEBOOK(notebook), vte_widget);
    if (page_num != -1) {
        gtk_notebook_remove_page(GTK_NOTEBOOK(notebook), page_num);
    }

    if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) == 0) {
        gtk_main_quit();
    }
}

// Function to create a new terminal tab
// تم تعديل هذه الدالة لتستقبل أمر للتنفيذ (أو NULL للـ shell العادي)
GtkWidget *helwan_terminal_window_new_tab(HelwanTerminalWindow *self, char * const *command_to_execute) {
    GtkWidget *vte = vte_terminal_new();

    char * const *spawn_command;
    if (command_to_execute && command_to_execute[0] != NULL) {
        // إذا تم تمرير أمر، استخدمه
        spawn_command = command_to_execute;
    } else {
        // وإلا، استخدم bash افتراضيًا
        spawn_command = (char * const []){"/bin/bash", NULL};
    }

    vte_terminal_spawn_sync(VTE_TERMINAL(vte),
                            VTE_PTY_DEFAULT,
                            NULL, // working directory
                            spawn_command, // الأمر الذي سيتم تنفيذه
                            NULL, // environment
                            0, // spawn flags
                            NULL, NULL, NULL, NULL, NULL);

    // Apply the current font settings to the new terminal from cached string
    PangoFontDescription *temp_font_desc = pango_font_description_from_string(cached_font_string);
    double applied_font_size = (double)pango_font_description_get_size(temp_font_desc) / PANGO_SCALE;
    gchar *applied_font_family = g_strdup(pango_font_description_get_family(temp_font_desc));
    pango_font_description_free(temp_font_desc);

    apply_font_settings(VTE_TERMINAL(vte), applied_font_family, applied_font_size);
    g_free(applied_font_family);

    g_signal_connect(vte, "key-press-event", G_CALLBACK(on_terminal_key_press), self);
    g_signal_connect(vte, "button-press-event", G_CALLBACK(on_terminal_button_press), vte);
    gtk_widget_add_events(vte, GDK_BUTTON_PRESS_MASK);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *tab_label = gtk_label_new("Terminal");
    GtkWidget *close_button = gtk_button_new_from_icon_name("window-close-symbolic", GTK_ICON_SIZE_SMALL_TOOLBAR);
    gtk_button_set_relief(GTK_BUTTON(close_button), GTK_RELIEF_NONE);

    g_signal_connect(close_button, "clicked", G_CALLBACK(on_tab_close_button_clicked), vte);

    gtk_box_pack_start(GTK_BOX(hbox), tab_label, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), close_button, FALSE, FALSE, 0);
    gtk_widget_show_all(hbox);

    gtk_notebook_append_page(GTK_NOTEBOOK(self->notebook), vte, hbox);
    gtk_widget_show_all(vte);

    return vte;
}

// دالة مساعدة لتطبيق الشفافية مباشرة عند تحريك المزلاج
static void on_opacity_scale_value_changed(GtkRange *range, HelwanTerminalWindow *window) {
    double new_opacity = gtk_range_get_value(range);
    gtk_window_set_opacity(GTK_WINDOW(window), new_opacity);
}

// Callback for applying preferences
static void on_apply_preferences_clicked(GtkButton *button, gpointer user_data) {
    (void)button;

    GtkWidget *dialog = GTK_WIDGET(user_data);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GList *children = gtk_container_get_children(GTK_CONTAINER(content_area));
    GtkWidget *grid = NULL;
    if (children) {
        grid = GTK_WIDGET(children->data);
        g_list_free(children);
    }

    if (!grid || !GTK_IS_GRID(grid)) {
        g_warning("Could not find the GtkGrid in preferences dialog content area. Dialog might be structured differently.");
        return;
    }

    // Font
    GtkWidget *font_chooser_widget = gtk_grid_get_child_at(GTK_GRID(grid), 1, 0);
    if (!font_chooser_widget || !GTK_IS_FONT_CHOOSER(font_chooser_widget)) {
        g_warning("Could not get font chooser widget or it's not a GtkFontChooser.");
        return;
    }
    PangoFontDescription *font_desc = gtk_font_chooser_get_font_desc(GTK_FONT_CHOOSER(font_chooser_widget));

    gchar *new_font_string = NULL;
    if (font_desc) {
        new_font_string = pango_font_description_to_string(font_desc);
        pango_font_description_free(font_desc);
        if (settings) {
            g_settings_set_string(settings, "font-family", new_font_string);
        }
        g_free(cached_font_string);
        cached_font_string = g_strdup(new_font_string);
        g_free(new_font_string);
    }

    // Window Width
    GtkWidget *width_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 1);
    if (!width_entry || !GTK_IS_ENTRY(width_entry)) {
        g_warning("Could not get width entry widget or it's not a GtkEntry.");
        return;
    }
    int new_width = atoi(gtk_entry_get_text(GTK_ENTRY(width_entry)));
    if (settings) {
        g_settings_set_int(settings, "window-width", new_width);
    }

    // Window Height
    GtkWidget *height_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 2);
    if (!height_entry || !GTK_IS_ENTRY(height_entry)) {
        g_warning("Could not get height entry widget or it's not a GtkEntry.");
        return;
    }
    int new_height = atoi(gtk_entry_get_text(GTK_ENTRY(height_entry)));
    if (settings) {
        g_settings_set_int(settings, "window-height", new_height);
    }

    // Opacity
    GtkWidget *opacity_scale = gtk_grid_get_child_at(GTK_GRID(grid), 1, 3); // الصف الجديد للشفافية
    if (!opacity_scale || !GTK_IS_SCALE(opacity_scale)) {
        g_warning("Could not get opacity scale widget or it's not a GtkScale.");
        return;
    }
    double new_opacity = gtk_range_get_value(GTK_RANGE(opacity_scale));
    if (settings) {
        g_settings_set_double(settings, "opacity", new_opacity);
    }

    HelwanTerminalWindow *window = HELWAN_TERMINAL_WINDOW(gtk_window_get_transient_for(GTK_WINDOW(dialog)));
    if (window) {
        // Apply font to all VTE instances
        gint n_pages = gtk_notebook_get_n_pages(GTK_NOTEBOOK(window->notebook));
        for (gint i = 0; i < n_pages; i++) {
            GtkWidget *vte_widget = gtk_notebook_get_nth_page(GTK_NOTEBOOK(window->notebook), i);
            if (VTE_IS_TERMINAL(vte_widget)) {
                PangoFontDescription *temp_font_desc = pango_font_description_from_string(cached_font_string);
                double applied_font_size = (double)pango_font_description_get_size(temp_font_desc) / PANGO_SCALE;
                gchar *applied_font_family = g_strdup(pango_font_description_get_family(temp_font_desc));
                pango_font_description_free(temp_font_desc);

                apply_font_settings(VTE_TERMINAL(vte_widget), applied_font_family, applied_font_size);
                g_free(applied_font_family);
            }
        }

        // Apply window size
        if (new_width > 0 && new_height > 0) {
            gtk_window_resize(GTK_WINDOW(window), new_width, new_height);
        }

        // Apply opacity (يتم تطبيقه هنا أيضًا عند الضغط على Apply/OK لحفظ القيمة)
        gtk_window_set_opacity(GTK_WINDOW(window), new_opacity);
    }
}

// Function to create the preferences dialog
static void create_preferences_dialog(HelwanTerminalWindow *window) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Preferences",
                                                    GTK_WINDOW(window),
                                                    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                    "Cancel", GTK_RESPONSE_CANCEL,
                                                    "Apply", GTK_RESPONSE_APPLY,
                                                    "Ok", GTK_RESPONSE_OK,
                                                    NULL);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    gtk_box_pack_start(GTK_BOX(content_area), grid, TRUE, TRUE, 0);

    // Font selection (Row 0)
    GtkWidget *font_label = gtk_label_new("Font:");
    gtk_grid_attach(GTK_GRID(grid), font_label, 0, 0, 1, 1);
    GtkWidget *font_chooser_widget = gtk_font_chooser_widget_new();
    gtk_grid_attach(GTK_GRID(grid), font_chooser_widget, 1, 0, 1, 1);

    gchar *font_from_settings = NULL;
    if (settings) {
        font_from_settings = g_settings_get_string(settings, "font-family");
    }
    if (!font_from_settings) {
        font_from_settings = g_strdup("monospace 10");
    }
    PangoFontDescription *initial_font_desc = pango_font_description_from_string(font_from_settings);
    gtk_font_chooser_set_font_desc(GTK_FONT_CHOOSER(font_chooser_widget), initial_font_desc);
    pango_font_description_free(initial_font_desc);
    g_free(font_from_settings);

    // Window width (Row 1)
    GtkWidget *width_label = gtk_label_new("Window Width:");
    gtk_grid_attach(GTK_GRID(grid), width_label, 0, 1, 1, 1);
    GtkWidget *width_entry = gtk_entry_new();
    gint initial_width = 800;
    if (settings) {
        initial_width = g_settings_get_int(settings, "window-width");
    }
    gtk_entry_set_text(GTK_ENTRY(width_entry), g_strdup_printf("%d", initial_width));
    gtk_grid_attach(GTK_GRID(grid), width_entry, 1, 1, 1, 1);

    // Window height (Row 2)
    GtkWidget *height_label = gtk_label_new("Window Height:");
    gtk_grid_attach(GTK_GRID(grid), height_label, 0, 2, 1, 1);
    GtkWidget *height_entry = gtk_entry_new();
    gint initial_height = 600;
    if (settings) {
        initial_height = g_settings_get_int(settings, "window-height");
    }
    gtk_entry_set_text(GTK_ENTRY(height_entry), g_strdup_printf("%d", initial_height));
    gtk_grid_attach(GTK_GRID(grid), height_entry, 1, 2, 1, 1);

    // Opacity control (Row 3)
    GtkWidget *opacity_label = gtk_label_new("Opacity:");
    gtk_grid_attach(GTK_GRID(grid), opacity_label, 0, 3, 1, 1);
    
    GtkAdjustment *adjustment = gtk_adjustment_new(0.85, 0.0, 1.0, 0.01, 0.1, 0.0);
    GtkWidget *opacity_scale = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, adjustment);
    gtk_range_set_fill_level(GTK_RANGE(opacity_scale), 1.0); // Show fill up to 1.0
    gtk_scale_set_digits(GTK_SCALE(opacity_scale), 2); // Show 2 decimal places

    if (settings) {
        gtk_adjustment_set_value(adjustment, g_settings_get_double(settings, "opacity"));
    }
    gtk_grid_attach(GTK_GRID(grid), opacity_scale, 1, 3, 1, 1);

    // ربط المزلاج بدالة تحديث الشفافية الفورية
    g_signal_connect(opacity_scale, "value-changed", G_CALLBACK(on_opacity_scale_value_changed), window);

    gtk_widget_show_all(dialog);

    gint response;
    do {
        response = gtk_dialog_run(GTK_DIALOG(dialog));

        if (response == GTK_RESPONSE_APPLY) {
            on_apply_preferences_clicked(NULL, dialog);
        } else if (response == GTK_RESPONSE_OK) {
            on_apply_preferences_clicked(NULL, dialog);
            break;
        } else if (response == GTK_RESPONSE_CANCEL) {
            break;
        }
    } while (TRUE);

    gtk_widget_destroy(dialog);
}

// Function to show the About Dialog (as a separate button action)
static void on_about_button_clicked(GtkButton *button, HelwanTerminalWindow *window) {
    (void)button; // Unused parameter

    gtk_show_about_dialog(GTK_WINDOW(window),
                          "program-name", "Helwan Terminal",
                          "version", "0.1.0", // رقم الإصدارة
                          "copyright", "© 2025 Helwan University", // حقوق الملكية
                          "comments", "A powerful terminal emulator developed at Helwan University.", // معلومات إضافية
                          "website", "https://github.com/YourGitHubUser/helwan-terminal", // رابط المشروع (يمكنك تغييره)
                          "authors", (const char *[]){"Your Name", "Another Author (optional)", NULL}, // أسماء المطورين
                          "license-type", GTK_LICENSE_GPL_3_0,
                          "logo-icon-name", "helwan-terminal", // اسم الأيقونة التي سيتم تثبيتها
                          NULL);
}


// Callback for "Preferences" button
static void on_preferences_button_clicked(GtkButton *button, HelwanTerminalWindow *window) {
    (void)button;
    create_preferences_dialog(window);
}

// Callback for the "New Tab" button
static void on_new_tab_button_clicked(GtkButton *button, HelwanTerminalWindow *window) {
    (void)button;
    // عند فتح tab جديد من زر "New Tab"، لا يوجد أمر خارجي، لذا نمرر NULL
    helwan_terminal_window_new_tab(window, NULL);
    gtk_notebook_set_current_page(GTK_NOTEBOOK(window->notebook), gtk_notebook_get_n_pages(GTK_NOTEBOOK(window->notebook)) - 1);
}

// Function to create the main terminal window
// ****** تم تعديل هذه الدالة لاستقبال argc و argv ******
GtkWidget *create_terminal_window(gint argc, char * const *argv) {
    // Initialize GSettings for our schema
    if (!settings) {
        settings = g_settings_new("org.helwan_terminal.gschema");
    }

    // Load initial font settings from GSettings
    if (!cached_font_string) {
        cached_font_string = g_settings_get_string(settings, "font-family");
        if (!cached_font_string) { // Fallback if GSettings doesn't have it (e.g., first run)
            cached_font_string = g_strdup("monospace 10");
        }
    }

    // Get initial opacity from GSettings
    double initial_opacity = 0.85; // Default fallback
    if (settings) {
        initial_opacity = g_settings_get_double(settings, "opacity");
    }

    // Create an instance of our custom window type
    gint initial_window_width = g_settings_get_int(settings, "window-width");
    gint initial_window_height = g_settings_get_int(settings, "window-height");

    HelwanTerminalWindow *window = g_object_new(helwan_terminal_window_get_type(),
                                                "title", "Helwan Terminal",
                                                "default-width", initial_window_width,
                                                "default-height", initial_window_height,
                                                NULL);

    gtk_widget_set_app_paintable(GTK_WIDGET(window), TRUE);
    gtk_window_set_opacity(GTK_WINDOW(window), initial_opacity);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *header_bar = gtk_header_bar_new();
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header_bar), TRUE);
    gtk_header_bar_set_title(GTK_HEADER_BAR(header_bar), "Helwan Terminal"); // عنوان النافذة الرئيسي
    gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);

    // زر "New Tab"
    GtkWidget *new_tab_button = gtk_button_new_from_icon_name("list-add-symbolic", GTK_ICON_SIZE_SMALL_TOOLBAR);
    gtk_button_set_relief(GTK_BUTTON(new_tab_button), GTK_RELIEF_NONE);
    g_signal_connect(new_tab_button, "clicked", G_CALLBACK(on_new_tab_button_clicked), window);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), new_tab_button);

    // زر "Preferences"
    GtkWidget *settings_button = gtk_button_new_from_icon_name("preferences-system-symbolic", GTK_ICON_SIZE_SMALL_TOOLBAR);
    gtk_button_set_relief(GTK_BUTTON(settings_button), GTK_RELIEF_NONE);
    g_signal_connect(settings_button, "clicked", G_CALLBACK(on_preferences_button_clicked), window);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), settings_button);

    // زر "About" الجديد والمستقل
    GtkWidget *about_button = gtk_button_new_from_icon_name("help-about-symbolic", GTK_ICON_SIZE_SMALL_TOOLBAR);
    gtk_button_set_relief(GTK_BUTTON(about_button), GTK_RELIEF_NONE);
    g_signal_connect(about_button, "clicked", G_CALLBACK(on_about_button_clicked), window);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), about_button);


    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    window->notebook = gtk_notebook_new();
    gtk_notebook_set_scrollable(GTK_NOTEBOOK(window->notebook), TRUE);
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(window->notebook), GTK_POS_TOP);

    gtk_box_pack_start(GTK_BOX(vbox), window->notebook, TRUE, TRUE, 0);

    // ****** هنا الجزء الجديد والمهم ******
    // نحتاج نمرر الأمر لو موجود، أو NULL لو مش موجود.
    // هندور على "-e" في الـ arguments عشان نعرف لو فيه أمر عايز يتنفذ.
    char * const *command_for_first_tab = NULL;
    if (argc > 1) { // لو فيه أي arguments بعد اسم البرنامج
        if (strcmp(argv[1], "-e") == 0) { // لو الـ argument التاني هو "-e"
            if (argc > 2) { // ولو فيه أمر بعد "-e"
                // يبقى الأمر اللي عايز يتنفذ بيبدأ من argv[2]
                command_for_first_tab = &argv[2];
            } else {
                // لو "-e" موجود ومفيش أمر بعده، نفتح bash عادي
                command_for_first_tab = NULL;
            }
        } else {
            // لو أول argument مش "-e"، يبقى بنعتبر كل الـ arguments أمر واحد
            command_for_first_tab = &argv[1];
        }
    }
    
    // نفتح أول tab بالـ command اللي لقيناه (أو بـ NULL لو مفيش)
    helwan_terminal_window_new_tab(window, command_for_first_tab);
    // *************************************************************
    
    return GTK_WIDGET(window);
}
