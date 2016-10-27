static void qemu_gdb_hang(void)
{
#ifdef DEBUG
	static volatile int wait = 1;

	while (wait);
#endif
}

#include <desc.h>
#include <ints.h>
#include "serial_port.h"
#include "interrupt.h"
#include "pit.h"
#include "memory_map.h"

void main(void)
{


	qemu_gdb_hang();

	disable_ints();
/*
	init_serial();
	print_serial("Serial port init.. OK \n");
    
    init_idt();
    
    __asm__ volatile("int %0" : : "n"(0));
    
    init_icontr();

    enable_ints();
 */  
    memory_info();
    //pit(0xFFFF);
    
    
    while (1);
}
