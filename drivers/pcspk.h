#pragma once
#include "../kernel/IO.h"

void sound(unsigned short freq)
{
     outb(0x43, 0xb6);
     outb(0x42, (unsigned char) (1193180 / freq));
     outb(0x42, (unsigned char) ((1193180 / freq) >> 8));
     outb(0x61, inb(0x61) | 3);
}

void nosound()
{
 	outb(0x61, inb(0x61) & 0xFC);
}