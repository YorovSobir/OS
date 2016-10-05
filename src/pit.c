#include "pit.h"

void pit(uint16_t init){
     
    out8(0x43, 0x34); 

    out8(0x40, (init & 0xFF));
    out8(0x40, ((init & 0xFF00) >> 8));
    
}