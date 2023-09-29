#include <gtk/gtk.h>
#include "fs_helper.h"
enum {
    COLUMN_TITLE,
    COLUMN_TYPE,
    NUM_COLUMNS
};

void add_to_tree_store(GtkTreeStore *treestore, const gchar *parent, const gchar *title, const gchar *type);


void print_finder(struct Directory* directory, GtkTreeStore *treestore, const char* depth_level, int el_count, int hack, const char* root){
    int i = 0;
    int dir_count = 0;
    char buff[50];

    if (strcmp(depth_level, "n") == 0){
        while (directory->filenames[i] != NULL){
            add_to_tree_store(treestore, NULL, "File", directory->filenames[i]);
            el_count++;
            i++;
        }
        while (directory->child_directory[dir_count] != NULL){
            sprintf(buff, "%d", el_count);
            char* new_level = strdup(buff);
            add_to_tree_store(treestore, NULL, "Directory", directory->name);
            el_count++;
            print_finder(directory->child_directory[dir_count], treestore, new_level, el_count, hack, root);
            dir_count++;
            free(new_level);
        }
    } else {
        while (directory->filenames[i] != NULL) {
            if (hack < 2 && i < el_count) {
                add_to_tree_store(treestore, depth_level, "File", directory->filenames[i]);
                i++;
            } else {
                sprintf(buff, "%d", el_count);
                char* new_root = malloc(strlen(root) + strlen(buff) + 2);
                strcpy(new_root, root);
                strcat(new_root, ":");
                strcat(new_root, buff);
                add_to_tree_store(treestore, new_root, "File", directory->filenames[i]);
                i++;
                el_count++;
                free(new_root);
            }
        }
    }

    while (directory->child_directory[dir_count] != NULL){
        if (hack < 1) {
            sprintf(buff, "%d", el_count);
            char *new_level = strdup(buff);
            char* new_root = malloc(strlen(root) + strlen(buff) + 2);
            strcpy(new_root, root);
            strcat(new_root, ":");
            strcat(new_root, buff);
            add_to_tree_store(treestore, new_level, "Directory", directory->name);
            hack++;
            print_finder(directory->child_directory[dir_count], treestore, depth_level, el_count, hack, new_root);
            dir_count++;
            free(new_level);
            free(new_root);
        } else {
            sprintf(buff, "%d", el_count);
            char* new_root = malloc(strlen(root) + strlen(buff) + 2);
            strcpy(new_root, root);
            strcat(new_root, ":");
            strcat(new_root, buff);
            add_to_tree_store(treestore, new_root, "Directory", directory->name);
            el_count++;
            print_finder(directory->child_directory[dir_count], treestore, depth_level, el_count, hack, new_root);
            dir_count++;
            free(new_root);
        }
    }
}

void add_to_tree_store(GtkTreeStore *treestore, const gchar *parent, const gchar *title, const gchar *type) {
    GtkTreeIter iter;
    GtkTreeIter parent_iter;

    if (parent == NULL) {
        gtk_tree_store_append(treestore, &iter, NULL);
        gtk_tree_store_set(treestore, &iter, COLUMN_TITLE, title, COLUMN_TYPE, type, -1);
    } else {
        gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(treestore), &parent_iter, parent);
        gtk_tree_store_append(treestore, &iter, &parent_iter);
        gtk_tree_store_set(treestore, &iter, COLUMN_TITLE, title, COLUMN_TYPE, type, -1);
    }
}

int main(int argc, char *argv[]) {
    char cwd[PATH_MAX];
    Directory* directory = get_files_ind_nested_dirs_from_directory(getcwd(cwd, sizeof(cwd)));
    GtkWidget *window;
    GtkWidget *treeview;
    GtkTreeStore *treestore;
    GtkTreeViewColumn *column;
    GtkCellRenderer *renderer;
    GtkTreeIter topLevel, child;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTK Simple Tree");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    treestore = gtk_tree_store_new(NUM_COLUMNS, G_TYPE_STRING, G_TYPE_STRING);
    char buff[50] = {'n'};
    buff[1] = '\0';
    int el_counter = -1;
    int hack = 0;
    char root[50] = {'\0'};
    print_finder(directory, treestore, buff, el_counter, hack, root);
    treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(treestore));

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Filetype", renderer, "text", COLUMN_TITLE, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Name", renderer, "text", COLUMN_TYPE, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    gtk_tree_view_expand_all(GTK_TREE_VIEW(treeview));

    gtk_container_add(GTK_CONTAINER(window), treeview);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}