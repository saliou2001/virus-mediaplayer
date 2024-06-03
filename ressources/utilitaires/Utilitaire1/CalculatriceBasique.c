#include <gtk/gtk.h>

float calculate(float num1, float num2, char op) {
    switch(op) {
        case '+':
            return num1 + num2;
        case '-':
            return num1 - num2;
        case '*':
            return num1 * num2;
        case '/':
            return num1 / num2;
        default:
            return 0;
    }
}

// Fonction de rappel pour effectuer le calcul
void calculate_callback(GtkWidget *widget, GtkWidget *entry) {
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
    float num1, num2;
    char op;

    sscanf(text, "%f %c %f", &num1, &op, &num2);

    float result = calculate(num1, num2, op);

    gchar *result_text = g_strdup_printf("Le résultat de %.2f %c %.2f est : %.2f", num1, op, num2, result);
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", result_text);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    g_free(result_text);
}

// Fonction pour afficher la fenêtre
void show_window(GtkWidget *window) {
    gtk_widget_show_all(window);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Calculatrice");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 300, 100);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Création de l'entrée pour la saisie des nombres et de l'opérateur
    GtkWidget *entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 2, 1); // Ajout d'un commentaire ici : Informer l'utilisateur de l'opération de calcul souhaitée
    // Définition du texte d'indication dans l'entrée
    const gchar *placeholder_text = "Entrez le premier nombre, l'opérateur (+, -, *, /) et le deuxième nombre";
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), placeholder_text);

    GtkWidget *calculate_button = gtk_button_new_with_label("Calculer");
    // Connexion du signal "clicked" du bouton à la fonction de calcul
    g_signal_connect(G_OBJECT(calculate_button), "clicked", G_CALLBACK(calculate_callback), entry);
    gtk_grid_attach(GTK_GRID(grid), calculate_button, 1, 1, 1, 1);

    // Connexion du signal "destroy" de la fenêtre à la fonction de terminaison de GTK
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Ajout d'un délai avant d'afficher la fenêtre pour permettre le rafraîchissement de l'interface graphique
    g_timeout_add(100, (GSourceFunc)show_window, window);

    // Lancement de la boucle principale de GTK
    gtk_main();

    return 0;
}
