#include <gtk/gtk.h>

// Fonction pour calculer le PGCD
int compute_gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Fonction de rappel pour le bouton "Calculer"
void calculate_gcd(GtkWidget *widget, gpointer data) {
    // Récupération des entrées des deux nombres
    GtkWidget **entries = (GtkWidget **)data;

    // Récupération des valeurs des entrées
    const gchar *text1 = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *text2 = gtk_entry_get_text(GTK_ENTRY(entries[1]));

    // Conversion des valeurs en entiers
    int number1, number2;
    if (sscanf(text1, "%d", &number1) != 1 || sscanf(text2, "%d", &number2) != 1) {
        // Affichage d'un message d'erreur si les entrées ne sont pas des nombres valides
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Veuillez entrer des nombres valides.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Calcul du PGCD
    int gcd = compute_gcd(number1, number2);

    // Affichage du résultat dans une boîte de dialogue
    gchar *result_text = g_strdup_printf("Le PGCD de %d et %d est : %d", number1, number2, gcd);
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", result_text);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    g_free(result_text);
}

int main(int argc, char *argv[]) {
    // Initialisation de GTK
    gtk_init(&argc, &argv);

    // Création de la fenêtre principale
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Calcul du PGCD");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 300, 150);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Création d'une grille pour organiser les widgets
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Création des étiquettes et des entrées pour les deux nombres
    GtkWidget *labels[2];
    GtkWidget *entries[2];
    const char *label_texts[2] = {"Nombre 1 :", "Nombre 2 :"};
    for (int i = 0; i < 2; ++i) {
        // Création de l'étiquette
        labels[i] = gtk_label_new(label_texts[i]);
        gtk_grid_attach(GTK_GRID(grid), labels[i], 0, i, 1, 1);

        // Création de l'entrée
        entries[i] = gtk_entry_new();
        gtk_grid_attach(GTK_GRID(grid), entries[i], 1, i, 1, 1);
    }

    // Création du bouton "Calculer"
    GtkWidget *calculate_button = gtk_button_new_with_label("Calculer PGCD");
    gtk_grid_attach(GTK_GRID(grid), calculate_button, 0, 2, 2, 1);
    g_signal_connect(calculate_button, "clicked", G_CALLBACK(calculate_gcd), entries);

    // Affichage de la fenêtre principale et de tous ses widgets
    gtk_widget_show_all(window);
    // Démarrage de la boucle principale de GTK
    gtk_main();

    return 0;
}
