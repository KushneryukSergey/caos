#include <stdio.h>

int directory_up(const char* path, int last, int next) {
    if (((last == 2) || ((last > 2) && path[last - 3] == '/')) &&
        path[last - 2] == '.' &&
        path[last - 1] == '.' &&
        path[next] == '/')
        return 1;
    return 0;
}

int current_dir(const char* path, int last, int next) {
    if (((last == 1) || ((last > 1) && path[last - 2] == '/')) &&
        path[last - 1] == '.' &&
        path[next] == '/')
        return 1;
    return 0;
}

int extra_slash(const char* path, int last, int next) {
    //printf("es [%d %d]\n", last, next);
    if (last &&
        path[last - 1] == '/' &&
        path[next] == '/')
        return 1;
    return 0;
}

void reduce_dir_up(char* path, int* last) {
    if (!(*last))
        return;
    --(*last);
    while ((*last)) {
        --(*last);
        if (path[*last] == '/'){
            ++(*last);
            break;
        }
    }
}

extern void normalize_path(char* path) {
    int last = 0;  // index of element lying after last correct
    int next = 0;  // index of next element to be checked
    
    while (path[next] != '\0') {
        //printf("np %d %d\n", last, next);
        if (directory_up(path, last, next)) {
            //printf("[1]\n");
            last -= 2;
            reduce_dir_up(path, &last);
            ++next;
        } else if (current_dir(path, last, next)) {
            //printf("[2]\n");
            last -= 1;
            ++next;
        } else if(extra_slash(path, last, next)) {
            //printf("[3]\n");
            ++next;
        } else {
            //printf("[4]\n");
            path[last++] = path[next++];
        }
    }
    path[last] = path[next];
}