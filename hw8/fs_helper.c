#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Directory Directory;

struct Directory {
    int counter;
    char name[1024];
    char *filenames[1024];
    Directory *child_directory[1024];
};


void recursive_traverse(Directory* cur_dir, const char* root_path) {
    struct dirent* dir;
    int i = 0;
    DIR* d;
    char filepath[1024] = {'\0'};
    snprintf(filepath, sizeof(filepath), "%s/%s", root_path, cur_dir->name);

    d = opendir(filepath);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG) {
                cur_dir->filenames[i] = strdup(dir->d_name); // Дублируем строку, чтобы избежать ошибок
                printf("File: %s\n", dir->d_name);
                i++;
            } else if (dir->d_type == DT_DIR && strcmp(dir->d_name, "..") != 0 && strcmp(dir->d_name, ".") != 0) {
                printf("Folder: %s\n", dir->d_name);
                Directory* child = malloc(sizeof(Directory));
                strcpy(child->name, dir->d_name);
                cur_dir->child_directory[cur_dir->counter] = child;
                cur_dir->counter += 1;
                recursive_traverse(child, filepath);
            }
        }
        closedir(d);
    }
}

Directory* get_files_ind_nested_dirs_from_directory(const char* root_path) {
    DIR* d;
    struct dirent* dir;
    d = opendir(root_path);
    Directory* directory = malloc(sizeof(Directory));
    directory->counter = 0;
    int i = 0;
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_DIR && strcmp(dir->d_name, "..") != 0 && strcmp(dir->d_name, ".") != 0) {
                printf("Folder: %s\n", dir->d_name);
                Directory* child = malloc(sizeof(Directory));
                strcpy(child->name, dir->d_name);
                directory->child_directory[directory->counter] = child;
                directory->counter += 1;
                recursive_traverse(child, root_path);
            } else if (dir->d_type == DT_REG) {
                directory->filenames[i] = strdup(dir->d_name); // Дублируем строку, чтобы избежать ошибок
                printf("File: %s\n", dir->d_name);
                i++;
            }
        }
        closedir(d);
    }
    return directory;
}
