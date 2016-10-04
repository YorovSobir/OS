#include "interrupt.h"
#include "memory.h"
#include "serial_port.h"
#include "desc.h"
#define INTS_COUNT 33


extern uint64_t table[INTS_COUNT]; 

struct idt_entry idt_entries[INTS_COUNT];
struct desc_table_ptr idt_ptr;

void set_int_descriptor(int i, uint64_t entry) {
    uint64_t offset = (uint64_t)entry;
    
    idt_entries[i].offset_low = (offset & 0xFFFF); // взяли последние 16 бит
    idt_entries[i].selector = KERNEL_CS;
    idt_entries[i].IST = 0;
    idt_entries[i].flags = (1 << 7) | (0 << 5) | (14); // P=1, DPL = 0, TYPE = 14
    idt_entries[i].offset_hi = ((offset & 0xFFFF0000) >> 16); // [31:16]
    idt_entries[i].offset_32 = (offset >> 32); // [63:32]
    idt_entries[i].other_field = 0;

}


void init_idt(){
    for(int i = 0; i < INTS_COUNT; ++i){
        set_int_descriptor(i, table[i]);
    }

    idt_ptr.size = sizeof(struct idt_entry)*256 - 1;
    idt_ptr.addr = (uint64_t)idt_entries;

    write_idtr(&idt_ptr);
}   


void int_handler(struct frame const *frame) {
    
    print_serial("interrupt with id = "); 
    print_inter(frame->interrupt_id);

    if(frame->interrupt_id ==32){
        out8(0x20, 32); //отправить EOI ненаправленный (5-й бит установлен)
    }
    
    
}

void init_icontr(){
    out8(0x20,0x11); // 10001 в командный порт Master
    out8(0xA0,0x11); // 10001 в командный порт Slave

    out8(0x21, 0x20); // Master: 32
    out8(0xA1, 0x28); // Slave: 40

    out8(0x21, 0x04); // Slave подключен к 2-й ноге(100)
    out8(0xA1, 2); // Slave ко 2-й ноге

    out8(0x21, 0x01);
    out8(0xA1, 0x01); // нам нужен только последний установленный бит

    out8(0x21, 0xFE); // замаскировали все кроме 0 ноги
    out8(0xA1, 0xFF); // замаскировали все.   
}
