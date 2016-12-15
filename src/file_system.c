#include <file_system.h>
#include <print.h>
#include <string.h>
#include <stdlib.h>
#include <alloc.h>
#include <spinlock.h>

struct file_desc{
    node_t* file_node;
    size_t current_pos;
    int empty;
};

typedef struct file_desc file_desc_t;

static file_desc_t desc[MAX_DESC_COUNT];
static node_t* root;
static struct spinlock lock;

void init_fs() {
    int enable = spin_lock_irqsave(&lock);
    root = (node_t*) mem_alloc(sizeof(node_t));
    root->name = (char*) mem_alloc((sizeof(char) * 2));
    memcpy(root->name, ".", 2);
    root->type = DIRECTORY;
    root->next = NULL;
    root->child = NULL;
    root->addr = NULL;
    root->size = 0;

    for (int i = 0; i < MAX_DESC_COUNT; i++) {
        desc[i].empty = 1;
    }
    spin_unlock_irqrestore(&lock, enable);
}

static void print_dir(node_t *root, char *tab){
    while(root != NULL){
        printf("%s%s\n", tab, root->name);
        if (root->type == DIRECTORY){
            char *childtab = (char*) mem_alloc((sizeof(char)) * (strlen(tab) + 5));
            memset(childtab, 0, strlen(tab) + 5);
            if (strlen(tab) != 0){
                memcpy(childtab, tab, strlen(tab));
                memcpy(childtab + strlen(tab), "----", 4);
            }
            else{
                memcpy(childtab, "----", 4);
            }
            print_dir(root->child, childtab);
            mem_free(childtab);
        }
        root = root->next;
    }
}


void print_fs(){
    int enable = spin_lock_irqsave(&lock);
    node_t* temp = root;
    print_dir(temp, "");
    spin_unlock_irqrestore(&lock, enable);
}

static void remove_dir(node_t* root){
    while (root != NULL){
        if (root->type == DIRECTORY){
            remove_dir(root->child);
            node_t *temp = root->next;
            mem_free(root->addr);
            mem_free(root->name);
            mem_free(root);
            root = temp;
            continue;

        }
        if (root->type == FILE_){
            node_t *temp = root->next;
            mem_free(root->addr);
            mem_free(root->name);
            mem_free(root);
            root = temp;
            continue;
        }
    }
}

void remove_fs(){
    int enable = spin_lock_irqsave(&lock);
    node_t *temp = root;
    remove_dir(temp);
    spin_unlock_irqrestore(&lock, enable);
}

// split path by "/"
static int split_path(const char* pathname, char*** res){

    if (strlen(pathname) == 0){
        return -1;
    }

    const char* temp = pathname;
    const char* temp1 = pathname;
    int count = 1;
    int maxlen = 0;

    while (*temp){
        if (*temp == '/'){
            ++count;
            if (temp - temp1 > maxlen){
                maxlen = temp - temp1;
                temp1 = temp + 1;
            }
        }
        ++temp;
    }

    if (temp - temp1 > maxlen){
        maxlen = temp - temp1;
    }

    char **res_temp = (char**) mem_alloc((sizeof (char*)) * count);

    for (int i = 0; i < count; ++i){
        res_temp[i] = (char*) mem_alloc((sizeof (char)) * (maxlen + 1));
    }

    temp = pathname;
    int i = 0;
    int j = 0;

    while (*temp){
        if (*temp == '/'){
            res_temp[i][j] = '\0';
            ++i;
            ++temp;
            j = 0;
            continue;
        }

        res_temp[i][j] = *temp;
        ++j;
        ++temp;
    }
    res_temp[i][j] = '\0';

    *res = res_temp;
    return count;

}



static node_t* get_node(char** paths, int count, enum obj_type objtype){
    node_t * cur_node = root;
    int i = 0;
    while (i < count && cur_node != NULL){
        if (strcmp(cur_node->name, paths[i]) == 0){
            switch(cur_node->type){
                case DIRECTORY:{
                    if (objtype == DIRECTORY){
                        if (i == count - 1){
                            return cur_node;
                        }
                        // в одной директории нет двух одинаковых по названию поддиректорий
                        cur_node = cur_node->child;
                        ++i;
                        break;
                    }
                    // objtype == FILE_
                    if (i == count - 1){
                        printf("%s is DIRECTORY not FILE\n", paths[i]);
                        return NULL;
                    }
                    // одинаковых имен на одном уровне не должно быть
                    cur_node = cur_node->child;
                    ++i;
                    break;
                }

                case FILE_:{
                    if (objtype == DIRECTORY){
                        // нет одинаковых по имени директорий и файлов на одном уровне!
                        printf("%s doesn't exist\n", paths[i]);
                        return NULL;
                    }
                    if (i == count - 1){
                        return cur_node;
                    }

                    printf("%s is FILE not DIRECTORY\n", paths[i]);
                    return NULL;
                }
            } //switch
        } //if

        else{
            cur_node = cur_node->next;
        }
    }

//    printf("%s doesn't exist!\n", paths[count - 1]);
    return cur_node;
}

static void mem_free_memory(char** path, int count){
    for (int i = 0; i < count; ++i){
        mem_free(path[i]);
    }
    mem_free(path);
}

static node_t* create_node(char** paths, int count, enum obj_type type){

    if (get_node(paths, count, type) != NULL){
        printf("%s already exists\n", paths[count - 1]);
        return NULL;
    }

    node_t* parent_ = get_node(paths, count - 1, DIRECTORY);
    if (parent_ == NULL){
        return NULL;
    }

    node_t* child_ = (node_t*) mem_alloc(sizeof(node_t));
    memset(child_, 0, sizeof(node_t));
    node_t* temp = parent_->child;
    parent_->child = child_;
    child_->next = temp;

    child_->addr = 0;
    child_->size = 0;
    child_->type = type;
    child_->name = (char*) mem_alloc((sizeof (char)) * (strlen(paths[count - 1]) + 2));
    memcpy(child_->name, paths[count - 1], strlen(paths[count - 1]) + 1);
    return child_;

}

int open(const char *pathname, int flags){

    int enable = spin_lock_irqsave(&lock);
    if (strlen(pathname) == 0){
        printf("Invalid path!\n");
        spin_unlock_irqrestore(&lock, enable);
        return -1;
    }

    int fd = -1;

    for (size_t i = 0; i < MAX_DESC_COUNT; ++i){
        if (desc[i].empty == 1){
            desc[i].empty = 0;
            fd = i;
            break;
        }
    }

    if (fd == -1){
        printf("Cannot open file, no mem_free descriptor\n");
        spin_unlock_irqrestore(&lock, enable);
        return -1;
    }

    file_desc_t* fdesk = &desc[fd];

    char** paths;
    int count = split_path(pathname, &paths);

    node_t *fnode;
    if (flags == F_READ){
        fnode = get_node(paths, count, FILE_);
    }
    if (flags == F_WRITE){
        fnode = create_node(paths, count, FILE_);
    }

    mem_free_memory(paths, count);

    if (fnode == NULL){
        fdesk->empty = 1;
        spin_unlock_irqrestore(&lock, enable);
        return -1;
    }

    fdesk->file_node = fnode;
    fdesk->current_pos = 0;
    spin_unlock_irqrestore(&lock, enable);
    return fd;

}

int close(int fd){
    int enable = spin_lock_irqsave(&lock);
    desc[fd].empty = 1;
    desc[fd].current_pos = 0;
    desc[fd].file_node = NULL;
    spin_unlock_irqrestore(&lock, enable);
    return 0;
}

int read(int fd, void *buf, size_t count){
    int enable = spin_lock_irqsave(&lock);
    node_t* fnode = desc[fd].file_node;

    if (fnode == NULL){
        printf("Cannot read from file\n");
        spin_unlock_irqrestore(&lock, enable);
        return -1;
    }

    char* addr = fnode->addr;
    size_t start = desc[fd].current_pos;
    size_t cur_pos = start;
    size_t size = fnode->size;
    char* dst = (char*) buf;

    size_t i = 0;
    while (cur_pos < size && i < count){
        dst[i] = addr[i];
        ++cur_pos;
        ++i;
    }
    dst[i] = '\0';
    spin_unlock_irqrestore(&lock, enable);
    return cur_pos - start;
}

int write(int fd, const void *buf, size_t count){
    int enable = spin_lock_irqsave(&lock);
    node_t* fnode = desc[fd].file_node;

    if (fnode == NULL){
        printf("Cannot write file\n");
        spin_unlock_irqrestore(&lock, enable);
        return -1;
    }

    size_t start = desc[fd].current_pos;
    size_t cur_pos = start;
    size_t size = fnode->size;

    size_t i = size;
    while (cur_pos + count >= i) {
        ++i;
    }

    if (i > size){
        size = i;
        fnode->addr = (char*) mem_realloc(fnode->addr, i);
        fnode->size = i;
    }

    char* addr = fnode->addr;
    const char* dst = (char*)buf;

    i = 0;
    while (i < count){
        addr[cur_pos + i] = dst[i];
        ++i;
    }
    spin_unlock_irqrestore(&lock, enable);
    return count;
}

int mkdir(const char *pathname){
    int enable = spin_lock_irqsave(&lock);
    char** paths;
    int count = split_path(pathname, &paths);

    node_t* node_ = create_node(paths, count, DIRECTORY);

    mem_free_memory(paths, count);

    if (node_ == NULL){
        spin_unlock_irqrestore(&lock, enable);
        return -1;
    }

    spin_unlock_irqrestore(&lock, enable);
    return 0;
}

node_t * get_root(){
    return root;
}

node_t* readdir(node_t* cur_node){
    int enable = spin_lock_irqsave(&lock);
    if (cur_node == NULL){
        spin_unlock_irqrestore(&lock, enable);
        return NULL;
    }
    printf("%s\n", cur_node->name);
    node_t* next = cur_node->next;
    spin_unlock_irqrestore(&lock, enable);
    return next;
}

