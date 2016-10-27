#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "ioport.h"
#include <stdint.h>

void init_serial();
void print_serial(const char *);

void print_inter(uint64_t );
void serial_write(const char *buf, uint64_t size);


#endif /* __SERIAL_H__ */
