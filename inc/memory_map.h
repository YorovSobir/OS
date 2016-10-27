#ifndef __MEMORY_MAP_H__
#define __MEMORY_MAP_H__

#include <stdint.h>

void memory_info(void);

struct free_memory{
	uint64_t start;
	uint64_t end;
};
typedef struct free_memory free_memory_t;

#endif /*__MEMORY_MAP_H__*/
