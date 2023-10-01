//
// Created by Иван Шкурко on 27.09.2023.
//

#ifndef GTK_DT_FS_HELPER_H
#define GTK_DT_FS_HELPER_H


typedef struct Directory Directory;

struct Directory {
    int counter;
    char name[1024];
    char *filenames[1024];
    Directory *child_directory[1024];
};

Directory* get_files_ind_nested_dirs_from_directory(const char* root_path);

#endif //GTK_DT_FS_HELPER_H
