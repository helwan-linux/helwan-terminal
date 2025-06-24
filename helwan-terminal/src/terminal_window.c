#include "terminal_window.h" // يجب أن يكون هذا التضمين أولاً لضمان تعريف G_DECLARE_FINAL_TYPE
#include <vte/vte.h>
#include <gtk/gtk.h>
#include <pango/pangocairo.h>
#include <stdlib.h> // For strtol, g_strfreev
#include <stdio.h>  // For snprintf
#include <string.h> // For strcmp
#include <glib/gstdio.h> // For g_strtod (safer than atof)
#include <gio/gio.h> // For GSettings

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
    if (!font_desc) { // Add NULL check
        g_warning("Failed to create font description from family: %s", font_family);
        return;
    }
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
        vte_terminal_copy_clipboard_format(VTE_TERMINAL(widget), VTE_FORMAT_TEXT);
        return TRUE;
    }
    // Check for Ctrl+Shift+V (Paste)
    else if ((event->state & (GDK_CONTROL_MASK | GDK_SHIFT_MASK)) == (GDK_CONTROL_MASK | GDK_SHIFT_MASK) &&
             event->keyval == GDK_KEY_V) {
        GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
        gchar *text = gtk_clipboard_wait_for_text(clipboard);
        if (text) {
            vte_terminal_paste_text(VTE_TERMINAL(widget), text);
            g_free(text);
        }
        return TRUE;
    }
    // Check for Ctrl++ (Zoom In) - Try both GDK_KEY_equal (for '+') and GDK_KEY_plus
    else if ((event->state & GDK_CONTROL_MASK) && (event->keyval == GDK_KEY_plus || event->keyval == GDK_KEY_equal)) {
        PangoFontDescription *temp_font_desc = pango_font_description_from_string(cached_font_string);
        if (!temp_font_desc) return FALSE; // Add NULL check
        double current_font_size = (double)pango_font_description_get_size(temp_font_desc) / PANGO_SCALE;
        gchar *current_font_family = g_strdup(pango_font_description_get_family(temp_font_desc));
        pango_font_description_free(temp_font_desc);

        current_font_size += 1.0;

        g_free(cached_font_string);
        cached_font_string = g_strdup_printf("%s %g", current_font_family, current_font_size);
        
        apply_font_settings(VTE_TERMINAL(widget), current_font_family, current_font_size);
        g_free(current_font_family);

        return TRUE;
    }
    // Check for Ctrl+- (Zoom Out) - Try both GDK_KEY_minus and GDK_KEY_underscore (for '-')
    else if ((event->state & GDK_CONTROL_MASK) && (event->keyval == GDK_KEY_minus || event->keyval == GDK_KEY_underscore)) {
        PangoFontDescription *temp_font_desc = pango_font_description_from_string(cached_font_string);
        if (!temp_font_desc) return FALSE; // Add NULL check
        double current_font_size = (double)pango_font_description_get_size(temp_font_desc) / PANGO_SCALE;
        gchar *current_font_family = g_strdup(pango_font_description_get_family(temp_font_desc));
        pango_font_description_free(temp_font_desc);

        if (current_font_size > 1.0) {
            current_font_size -= 1.0;
            g_free(cached_font_string);
            cached_font_string = g_strdup_printf("%s %g", current_font_family, current_font_size);
            
            apply_font_settings(VTE_TERMINAL(widget), current_font_family, current_font_size);
        }
        g_free(current_font_family);
        return TRUE;
    }
    // Check for Ctrl+0 (Reset Zoom)
    else if ((event->state & GDK_CONTROL_MASK) && event->keyval == GDK_KEY_0) {
        g_free(cached_font_string);
        cached_font_string = g_strdup("monospace 10"); // Reset to default string
        
        PangoFontDescription *temp_font_desc = pango_font_description_from_string(cached_font_string);
        if (!temp_font_desc) return FALSE; // Add NULL check
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
    vte_terminal_copy_clipboard_format(terminal, VTE_FORMAT_TEXT);
}

// Callback for "Paste" menu item
static void on_paste_menu_item_activated(GtkMenuItem *menu_item, VteTerminal *terminal) {
    (void)menu_item;
    GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gchar *text = gtk_clipboard_wait_for_text(clipboard);
    if (text) {
        vte_terminal_paste_text(VTE_TERMINAL(terminal), text); // Changed widget to terminal for consistency
        g_free(text);
    }
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
        gchar *clipboard_text = gtk_clipboard_wait_for_text(clipboard);
        if (!clipboard_text) {
             gtk_widget_set_sensitive(paste_item, FALSE);
        } else {
            g_free(clipboard_text);
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
GtkWidget *helwan_terminal_window_new_tab(HelwanTerminalWindow *self, char * const *command_to_execute) {
    GtkWidget *vte = vte_terminal_new();

    char **envp = g_get_environ(); // Get current environment variables

    char * const *final_argv;
    gchar *full_command_str = NULL; // To manage memory for the combined command string

    if (command_to_execute && command_to_execute[0] != NULL) {
        // Construct the command string: "exec your_program_path args ; exec /bin/bash"
        // g_strjoinv safely joins array elements with spaces
        gchar *joined_command_parts = g_strjoinv(" ", (char**)command_to_execute);
        full_command_str = g_strdup_printf("exec %s ; exec /bin/bash", joined_command_parts);
        g_free(joined_command_parts); // Free the temporary joined string

        // Prepare argv for sh -c
        // This is the standard way to execute a full command string via a shell
        // Note: argv_sh_c must be a static or globally scoped array for vte_terminal_spawn_async
        // or allocated dynamically and freed later. Using a local const char* array is safe here.
        static char *argv_sh_c[4]; // Needs to be static or dynamically allocated for spawn_async
        argv_sh_c[0] = "/bin/sh";
        argv_sh_c[1] = "-c";
        argv_sh_c[2] = full_command_str;
        argv_sh_c[3] = NULL;
        final_argv = argv_sh_c;
    } else {
        // If no command is passed, just run /bin/bash
        static char *default_shell[] = {"/bin/bash", NULL}; // Static for safety with spawn_async
        final_argv = default_shell;
    }

    vte_terminal_spawn_async(VTE_TERMINAL(vte),        // 1: VteTerminal *terminal
                             VTE_PTY_DEFAULT,          // 2: VtePtyFlags pty_flags
                             NULL,                     // 3: const char *working_directory
                             final_argv,               // 4: char **argv - use final_argv here
                             envp,                     // 5: char **envv
                             G_SPAWN_SEARCH_PATH | G_SPAWN_CHILD_INHERITS_STDIN, // 6: GSpawnFlags spawn_flags
                             NULL,                     // 7: GSpawnChildSetupFunc child_setup
                             NULL,                     // 8: gpointer child_setup_data
                             (GDestroyNotify)g_strfreev, // 9: GDestroyNotify child_setup_data_destroy
                             -1,                       // 10: int timeout
                             NULL,                     // 11: GCancellable *cancellable
                             (VteTerminalSpawnAsyncCallback)NULL, // 12: VteTerminalSpawnAsyncCallback callback
                             (gpointer)NULL);          // 13: gpointer user_data

    // Free the memory allocated for full_command_str if it was used
    if (full_command_str) {
        g_free(full_command_str);
    }

    // Add a reasonable initial size to the VTE terminal
    vte_terminal_set_size(VTE_TERMINAL(vte), 80, 24); // 80 columns, 24 rows - standard default

    // Apply the current font settings to the new terminal from cached string
    PangoFontDescription *temp_font_desc = pango_font_description_from_string(cached_font_string);
    if (temp_font_desc) { // Add NULL check
        double applied_font_size = (double)pango_font_description_get_size(temp_font_desc) / PANGO_SCALE;
        gchar *applied_font_family = g_strdup(pango_font_description_get_family(temp_font_desc));
        pango_font_description_free(temp_font_desc);

        apply_font_settings(VTE_TERMINAL(vte), applied_font_family, applied_font_size);
        g_free(applied_font_family);
    } else {
        g_warning("Failed to create font description for new terminal from cached string: %s", cached_font_string);
        // Fallback to a default font if cached_font_string is invalid
        apply_font_settings(VTE_TERMINAL(vte), "monospace", 10.0);
    }

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

// Helper function to apply opacity directly when scale value changes
static void on_opacity_scale_value_changed(GtkRange *range, HelwanTerminalWindow *window) {
    double new_opacity = gtk_range_get_value(range);
    gtk_widget_set_opacity(GTK_WIDGET(window), new_opacity);
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
    if (font_chooser_widget && GTK_IS_FONT_CHOOSER(font_chooser_widget)) {
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
    } else {
        g_warning("Could not get font chooser widget or it's not a GtkFontChooser.");
    }

    // Window Width
    GtkWidget *width_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 1);
    if (width_entry && GTK_IS_ENTRY(width_entry)) {
        const char *width_text = gtk_entry_get_text(GTK_ENTRY(width_entry));
        char *endptr;
        long new_width_long = strtol(width_text, &endptr, 10);
        if (endptr == width_text || *endptr != '\0' || new_width_long <= 0) {
            g_warning("Invalid width entered: %s. Keeping previous value.", width_text);
        } else {
            int new_width = (int)new_width_long;
            if (settings) {
                g_settings_set_int(settings, "window-width", new_width);
            }
        }
    } else {
        g_warning("Could not get width entry widget or it's not a GtkEntry.");
    }

    // Window Height
    GtkWidget *height_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 2);
    if (height_entry && GTK_IS_ENTRY(height_entry)) {
        const char *height_text = gtk_entry_get_text(GTK_ENTRY(height_entry));
        char *endptr;
        long new_height_long = strtol(height_text, &endptr, 10);
        if (endptr == height_text || *endptr != '\0' || new_height_long <= 0) {
            g_warning("Invalid height entered: %s. Keeping previous value.", height_text);
        } else {
            int new_height = (int)new_height_long;
            if (settings) {
                g_settings_set_int(settings, "window-height", new_height);
            }
        }
    } else {
        g_warning("Could not get height entry widget or it's not a GtkEntry.");
    }

    // Opacity
    GtkWidget *opacity_scale = gtk_grid_get_child_at(GTK_GRID(grid), 1, 3);
    if (opacity_scale && GTK_IS_SCALE(opacity_scale)) {
        double new_opacity = gtk_range_get_value(GTK_RANGE(opacity_scale));
        if (settings) {
            g_settings_set_double(settings, "opacity", new_opacity);
        }
    } else {
        g_warning("Could not get opacity scale widget or it's not a GtkScale.");
    }

    HelwanTerminalWindow *window = HELWAN_TERMINAL_WINDOW(gtk_window_get_transient_for(GTK_WINDOW(dialog)));
    if (window) {
        // Apply font to all VTE instances
        gint n_pages = gtk_notebook_get_n_pages(GTK_NOTEBOOK(window->notebook));
        for (gint i = 0; i < n_pages; i++) {
            GtkWidget *vte_widget = gtk_notebook_get_nth_page(GTK_NOTEBOOK(window->notebook), i);
            if (VTE_IS_TERMINAL(vte_widget)) {
                PangoFontDescription *temp_font_desc = pango_font_description_from_string(cached_font_string);
                if (temp_font_desc) { // Add NULL check
                    double applied_font_size = (double)pango_font_description_get_size(temp_font_desc) / PANGO_SCALE;
                    gchar *applied_font_family = g_strdup(pango_font_description_get_family(temp_font_desc));
                    pango_font_description_free(temp_font_desc);

                    apply_font_settings(VTE_TERMINAL(vte_widget), applied_font_family, applied_font_size);
                    g_free(applied_font_family);
                } else {
                    g_warning("Failed to create font description for VTE instance from cached string: %s", cached_font_string);
                }
            }
        }

        // Apply window size
        int current_width = g_settings_get_int(settings, "window-width");
        int current_height = g_settings_get_int(settings, "window-height");
        if (current_width > 0 && current_height > 0) {
            gtk_window_resize(GTK_WINDOW(window), current_width, current_height);
        }

        // Apply opacity
        double current_opacity = g_settings_get_double(settings, "opacity");
        gtk_widget_set_opacity(GTK_WIDGET(window), current_opacity);
    }
}

// Function to show the About Dialog (as a separate button action)
static void on_about_button_clicked(GtkButton *button, HelwanTerminalWindow *window) {
    (void)button;

    gtk_show_about_dialog(GTK_WINDOW(window),
                          "program-name", "Helwan Terminal",
                          "version", "0.1.0",
                          "copyright", "© 2025 Helwan University",
                          "comments", "A powerful terminal emulator developed at Helwan University.",
                          "website", "https://github.com/YourGitHubUser/helwan-terminal",
                          "authors", (const char *[]){"Your Name", "Another Author (optional)", NULL},
                          "license-type", GTK_LICENSE_GPL_3_0,
                          "logo-icon-name", "helwan-terminal",
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
    helwan_terminal_window_new_tab(window, NULL);
    gtk_notebook_set_current_page(GTK_NOTEBOOK(window->notebook), gtk_notebook_get_n_pages(GTK_NOTEBOOK(window->notebook)) - 1);
}

// Function to create the main terminal window
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
    gint initial_window_width = 800; // Default value
    gint initial_window_height = 600; // Default value
    if (settings) {
        initial_window_width = g_settings_get_int(settings, "window-width");
        initial_window_height = g_settings_get_int(settings, "window-height");
        // Ensure values are valid (greater than zero)
        if (initial_window_width <= 0) initial_window_width = 800;
        if (initial_window_height <= 0) initial_window_height = 600;
    }

    HelwanTerminalWindow *window = g_object_new(helwan_terminal_window_get_type(),
                                                "title", "Helwan Terminal",
                                                "default-width", initial_window_width,
                                                "default-height", initial_window_height,
                                                NULL);

    gtk_widget_set_app_paintable(GTK_WIDGET(window), TRUE);
    gtk_widget_set_opacity(GTK_WIDGET(window), initial_opacity);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *header_bar = gtk_header_bar_new();
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header_bar), TRUE);
    gtk_header_bar_set_title(GTK_HEADER_BAR(header_bar), "Helwan Terminal");
    gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);

    // "New Tab" button
    GtkWidget *new_tab_button = gtk_button_new_from_icon_name("list-add-symbolic", GTK_ICON_SIZE_SMALL_TOOLBAR);
    gtk_button_set_relief(GTK_BUTTON(new_tab_button), GTK_RELIEF_NONE);
    g_signal_connect(new_tab_button, "clicked", G_CALLBACK(on_new_tab_button_clicked), window);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), new_tab_button);

    // "Preferences" button
    GtkWidget *settings_button = gtk_button_new_from_icon_name("preferences-system-symbolic", GTK_ICON_SIZE_SMALL_TOOLBAR);
    gtk_button_set_relief(GTK_BUTTON(settings_button), GTK_RELIEF_NONE);
    g_signal_connect(settings_button, "clicked", G_CALLBACK(on_preferences_button_clicked), window);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), settings_button);

    // "About" button
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

    char * const *command_for_first_tab = NULL;
    if (argc > 1) { // If there are arguments after the program name
        if (strcmp(argv[1], "-e") == 0) { // If the second argument is "-e"
            if (argc > 2) { // And there's a command after "-e"
                // The command to execute starts from argv[2]
                command_for_first_tab = &argv[2];
            } else {
                // If "-e" exists but no command after it, open regular bash
                command_for_first_tab = NULL;
            }
        } else {
            // If the first argument is not "-e", consider all arguments as a single command
            command_for_first_tab = &argv[1];
        }
    }
    
    // Open the first tab with the command found (or NULL if none)
    helwan_terminal_window_new_tab(window, command_for_first_tab);
    
    return GTK_WIDGET(window);
}
