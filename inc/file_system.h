#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <stddef.h>

#define MAX_DESC_COUNT  255

enum FLAGS{
    F_READ,
    F_WRITE
};

enum obj_type{
    DIRECTORY,
    FILE_
};

struct node{
    struct node* next;
    struct node* child;

    enum obj_type type;
    char* name;
    int size;
    void* addr;
};

typedef struct node node_t;

void init_fs();
void remove_fs();
void print_fs();
// http://man7.org/linux/man-pages/man2/write.2.html

int open(const char *pathname, int flags);
int close(int fd);
int read(int fd, void *buf, size_t count);
int write(int fd, const void *buf, size_t count);
int mkdir(const char *pathname);
//===============================================
node_t *get_root();
node_t *readdir(node_t *cur_node);


#endif // FILE_SYSTEM_H
