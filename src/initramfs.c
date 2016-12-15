#include <initramfs.h>
#include <string.h>
#include <stdlib.h>
#include <alloc.h>
#include <print.h>
#include <file_system.h>
#include <memory.h>


//=========================

/*
* copy paste (from this line to line № 72)
* from https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
* changed only - typedef unsigned int  multiboot_uint32_t;
*/

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

struct mboot_info{
    /* Multiboot info version number */
    uint32_t flags;

    /* Available memory from BIOS */
    // fields in lines (24, 25, 28, 31) are not used inside this file
    uint32_t mem_lower;
    uint32_t mem_upper;

    /* "root" partition */
    uint32_t boot_device;

    /* Kernel command line */
    uint32_t cmdline;

    /* Boot-Module list */
    uint32_t mods_count;
    uint32_t mods_addr;

} __attribute__((packed));


struct multiboot_mod_list{
   /* the memory used goes from bytes 'mod_start' to 'mod_end-1' inclusive */
   uint32_t mod_start;
   uint32_t mod_end;

   /* Module command line */
   uint32_t cmdline;

   /* padding to take it to 16 bytes (must be zero) */
   uint32_t pad;
};

typedef struct multiboot_mod_list multiboot_module_t;

void add_initramfs_range(const struct mboot_info *mbi){
    printf("%llx\n", mbi);
    if (CHECK_FLAG (mbi->flags, 3))
         {
           multiboot_module_t *mod;
           size_t i;

           printf ("mods_count = %d, mods_addr = 0x%x\n",
                   (int) mbi->mods_count, (int) mbi->mods_addr);
           for (i = 0, mod = (multiboot_module_t *) (uint64_t) mbi->mods_addr;
                i < mbi->mods_count;
                i++, mod++){

                printf (" mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n",
                         (unsigned) mod->mod_start,
                         (unsigned) mod->mod_end,
                         (char *) (uint64_t) mod->cmdline);

                struct cpio_header *cpio = (struct cpio_header*)(uint64_t)mod->mod_start;

                // printf("magic = %s\n", cpio->magic);

                if (strncmp(cpio->magic, "070701", 6) == 0){
                    fs_addr.start = (uintptr_t) mod->mod_start;
                    fs_addr.end = (uintptr_t) mod->mod_end;
                    // printf("======= %llx %llx\n", fs_addr.start, fs_addr.end);
                }
            }
        }
}


static void align(void** ptr){
    uint64_t temp = (uint64_t) *ptr;
    if (temp % 4 != 0){
         temp = temp + 4 - (temp % 4);
    }
    *ptr = (void*) temp;
}

void init_initramfs(){
    void *ptr = va(fs_addr.start);
    void *end = va(fs_addr.end);
    printf("%llx %llx\n", ptr, end);

    while(1){

        if (strncmp((char*)ptr, END_OF_ARCHIVE, strlen(END_OF_ARCHIVE)) == 0){
            printf("end of initramfs\n");
            break;
        }

        struct cpio_header *cpio = (struct cpio_header*) ptr;

        size_t name_size = atoi(cpio->namesize, 8);
        size_t file_size = atoi(cpio->filesize, 8);

        char *name = (char*) mem_alloc((sizeof(char) * (name_size + 3)));
        name[0] = '.';
        name[1] = '/';
        memcpy(name + 2, (char*)ptr + sizeof(struct cpio_header), name_size);
        name[name_size + 2] = '\0';

        if (strncmp(name + 2, END_OF_ARCHIVE, strlen(END_OF_ARCHIVE)) == 0){
            mem_free(name);
            return;
        }

        void* data = (char*)ptr + sizeof( struct cpio_header) + name_size;

        align(&data);

        // check out of bounds
        if ((char*)data + file_size > (char*)end){
            mem_free(name);
            return;
        }

        if (S_ISREG(atoi(cpio->mode, 8))){
            int fd = open(name, F_WRITE);
            write(fd, data, file_size);
            close(fd);
        }
        else {
            if (S_ISDIR(atoi(cpio->mode, 8))) {
                mkdir(name);
            }
        }

        mem_free(name);

        ptr = (char*)data + file_size;
        align(&ptr);

    }

}
