#include "memory_map.h"
#include "multiboot.h"
#include "print.h"
#include <stdint.h>
#include "serial_port.h"

free_memory_t free[32];
uint32_t free_size = 0;

extern char text_phys_begin[];
extern char bss_phys_end[];

extern  uint32_t mb_info_asm;

void add_to_free(uint64_t start, uint64_t end)
{
	if (start >= end) return;

	free[free_size].start = start;
	free[free_size].end = end;
	++free_size; 

}

void reserve_memory_for_kernel(multiboot_memory_map_t *memory_map)
{
	
    if (memory_map->addr > (uint64_t)bss_phys_end || memory_map->addr + memory_map->len < (uint64_t)text_phys_begin)
    {
    	add_to_free(memory_map->addr, memory_map->addr + memory_map->len);
    	return;
    }

    else 
    {
    	if ((uint64_t)bss_phys_end > memory_map->addr + memory_map->len)
    	{
    		add_to_free(memory_map->addr, (uint64_t)text_phys_begin - 1);
    		return;
    	}
    	
    	if (memory_map->addr > (uint64_t)text_phys_begin)
    	{
    		add_to_free((uint64_t)bss_phys_end + 1, memory_map->addr + memory_map->len);
    		return;
    	}
		
		add_to_free(memory_map->addr, (uint64_t)text_phys_begin - 1);
		add_to_free((uint64_t)bss_phys_end + 1, memory_map->addr + memory_map->len);
    }

}

void memory_info()
{

	multiboot_info_t * mb_info = (multiboot_info_t*)(uint64_t)mb_info_asm;

    if (mb_info->flags & 1){
    	printf("amount of lower = %d and upper = %d\n", mb_info->mem_lower, mb_info->mem_upper);	
    }

    if (!(mb_info->flags & (1 << 6))){
    	printf("mmap_* fields are invalid\n");
    	__asm__("hlt");
    }

    multiboot_memory_map_t *memory_map;

    for (memory_map = (multiboot_memory_map_t*)(uint64_t)mb_info->mmap_addr; 

		((uint64_t)memory_map) < mb_info->mmap_addr + mb_info->mmap_length;

		memory_map = (multiboot_memory_map_t*)((uint64_t)memory_map + memory_map->size + sizeof(memory_map->size)))
    {

    	printf("addr = 0x%x, len = 0x%x, type = %s \n", memory_map->addr,
    		memory_map->addr + memory_map->len, memory_map->type == MULTIBOOT_MEMORY_AVAILABLE ? "available" : "reserved");
    
    	if (memory_map->type == MULTIBOOT_MEMORY_AVAILABLE)
    	{
    		reserve_memory_for_kernel(memory_map);
    	}
    }

    printf("\nkernel: start_addr = %x, end_addr = %x \n", (uint64_t)text_phys_begin, (uint64_t)bss_phys_end);
    
    printf("\nfree memory after reserve memory for kernel:\n");

    for (uint32_t i = 0; i < free_size; ++i)
    {	
    	printf("start = 0x%x, end = 0x%x\n", free[i].start, free[i].end);
    }
}

