#include <gtk/gtk.h>

void calculate_percentage_callback(GtkWidget *widget, GtkWidget *entry) {
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
    float number, percentage;
    gchar **tokens = g_strsplit(text, " ", -1);

    if (g_strv_length(tokens) >= 2) {
        sscanf(tokens[0], "%f", &number);
        sscanf(tokens[1], "%f", &percentage);

        float result = (number * percentage) / 100;

        gchar *result_text = g_strdup_printf("%.2f est %.2f%% de %.2f", result, percentage, number);
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", result_text);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        g_free(result_text);
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Le texte doit être au format 'nombre pourcentage'.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }

    g_strfreev(tokens);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Calculatrice de pourcentage");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 300, 150);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_container_add(GTK_CONTAINER(window), grid);

    GtkWidget *instruction_label = gtk_label_new("Entrez le nombre suivi du pourcentage (ex: 100 20) :");
    gtk_grid_attach(GTK_GRID(grid), instruction_label, 0, 0, 2, 1);

    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Entrez le nombre suivi du pourcentage (ex: 100 20)");
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 1, 2, 1);

    GtkWidget *calculate_button = gtk_button_new_with_label("Calculer");
    g_signal_connect(G_OBJECT(calculate_button), "clicked", G_CALLBACK(calculate_percentage_callback), entry);
    gtk_grid_attach(GTK_GRID(grid), calculate_button, 1, 2, 1, 1);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
