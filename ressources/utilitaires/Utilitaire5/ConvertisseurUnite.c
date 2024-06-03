#include <gtk/gtk.h>

void convert_units_callback(GtkWidget *widget, GtkWidget *entry, GtkWidget *direction_combo) {
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
    float quantity;
    sscanf(text, "%f", &quantity);
    float converted_quantity;
    const gchar *direction = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(direction_combo));
    GtkWidget *dialog;

    // Affichage pour déboguer la direction de conversion
    g_print("Direction sélectionnée : %s\n", direction);

    // Vérifier la direction et appliquer la logique de conversion appropriée
    if (g_strcmp0(direction, "Kilogrammes vers Tonnes") == 0) {
        converted_quantity = quantity * 0.001; // Conversion de kg en tonnes
        dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%.2f Kilogrammes équivaut à %.3f Tonnes", quantity, converted_quantity);
    } else {
        converted_quantity = quantity * 1000; // Conversion de tonnes en kg
        dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%.2f Tonnes équivalent à %.3f Kilogrammes", quantity, converted_quantity);
    }

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Fonction de callback pour le signal "clicked" du bouton de conversion
void on_convert_button_clicked(GtkWidget *widget, gpointer data) {
    // Obtention des widgets nécessaires
    GtkWidget *grid = gtk_widget_get_parent(widget);
    GtkWidget *entry = gtk_grid_get_child_at(GTK_GRID(grid), 0, 1);
    GtkWidget *direction_combo = gtk_grid_get_child_at(GTK_GRID(grid), 1, 1);

    // Appel de la fonction de conversion avec les widgets appropriés
    convert_units_callback(widget, entry, direction_combo);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Convertisseur de Kilogrammes en Tonnes et vice versa");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 400, 200);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Ajout d'une étiquette pour guider l'utilisateur
    GtkWidget *label = gtk_label_new("Entrez le nombre à convertir :");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 2, 1);

    // Ajout d'une case de saisie pour la quantité
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Entrez le nombre");
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 1, 1, 1);

    // Ajout d'une liste déroulante pour choisir la direction de la conversion
    GtkWidget *direction_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(direction_combo), "Kilogrammes vers Tonnes");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(direction_combo), "Tonnes vers Kilogrammes");
    gtk_combo_box_set_active(GTK_COMBO_BOX(direction_combo), 1);
    gtk_grid_attach(GTK_GRID(grid), direction_combo, 1, 1, 1, 1);

    // Ajout d'un bouton de conversion
    GtkWidget *convert_button = gtk_button_new_with_label("Convertir");
    gtk_grid_attach(GTK_GRID(grid), convert_button, 0, 2, 2, 1);

    // Connecter le signal clicked du bouton à la fonction de conversion
    g_signal_connect(G_OBJECT(convert_button), "clicked", G_CALLBACK(on_convert_button_clicked), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
