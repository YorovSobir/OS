#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include <stdint.h>

void init_idt();

struct frame{
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
    uint64_t rbp;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    uint64_t rsp;
	uint64_t interrupt_id;
	uint64_t error_code;
} __attribute__((packed));

struct idt_entry {
	uint16_t offset_low;
	uint16_t selector;
	uint8_t IST;
 	uint8_t flags;
	uint16_t offset_hi;
	uint32_t offset_32;
	uint32_t other_field;
} __attribute__((packed));


void init_icontr();

#endif /*__INTERRUPT_H__*/