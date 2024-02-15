#include "IO.h"
#include "interrupts.h"

extern IDT _idt[256];
extern unsigned long long isr0;
extern unsigned long long isr1;
extern unsigned long long isr12;
extern unsigned long long isr128;
extern "C" void LoadIDT();

void clear_mask(unsigned char IRQ)
{
    unsigned short port;
    unsigned char value;
 
    if(IRQ < 8) port = 0x21;
    else
    {
        port = 0xA1;
        IRQ -= 8;
    }
    value = inb(port) & ~(1 << IRQ);
    outb(port, value);        
}

void set_mask(unsigned char IRQ)
{
    unsigned short port;
    unsigned char value;
 
    if(IRQ < 8) port = 0x21;
    else
    {
        port = 0xA1;
        IRQ -= 8;
    }
    value = inb(port) | (1 << IRQ);
    outb(port, value);        
}

void InitIDT()
{
    _idt[0].zero = 0;
    _idt[0].offset_low = (unsigned short)(((unsigned long long)&isr0 & 0x000000000000ffff));
    _idt[0].offset_mid = (unsigned short)(((unsigned long long)&isr0 & 0x00000000ffff0000) >> 16);
    _idt[0].offset_high = (unsigned int)(((unsigned long long)&isr0 & 0xffffffff00000000) >> 32);
    _idt[0].ist = 0;
    _idt[0].selector = 0x08;
    _idt[0].types_attr = 0x8e;

    _idt[1].zero = 0;
    _idt[1].offset_low = (unsigned short)(((unsigned long long)&isr1 & 0x000000000000ffff));
    _idt[1].offset_mid = (unsigned short)(((unsigned long long)&isr1 & 0x00000000ffff0000) >> 16);
    _idt[1].offset_high = (unsigned int)(((unsigned long long)&isr1 & 0xffffffff00000000) >> 32);
    _idt[1].ist = 0;
    _idt[1].selector = 0x08;
    _idt[1].types_attr = 0x8e;

    _idt[12].zero = 0;
    _idt[12].offset_low = (unsigned short)(((unsigned long long)&isr12 & 0x000000000000ffff));
    _idt[12].offset_mid = (unsigned short)(((unsigned long long)&isr12 & 0x00000000ffff0000) >> 16);
    _idt[12].offset_high = (unsigned int)(((unsigned long long)&isr12 & 0xffffffff00000000) >> 32);
    _idt[12].ist = 0;
    _idt[12].selector = 0x08;
    _idt[12].types_attr = 0x8e;

    _idt[128].zero = 0;
    _idt[128].offset_low = (unsigned short)(((unsigned long long)&isr128 & 0x000000000000ffff));
    _idt[128].offset_mid = (unsigned short)(((unsigned long long)&isr128 & 0x00000000ffff0000) >> 16);
    _idt[128].offset_high = (unsigned int)(((unsigned long long)&isr128 & 0xffffffff00000000) >> 32);
    _idt[128].ist = 0;
    _idt[128].selector = 0x08;
    _idt[128].types_attr = 0x8e;

	unsigned char a1, a2;

	a1 = inb(0x21);
	a2 = inb(0xA1);

	outb(0x20, 0x11);
	outb(0xA0, 0x11);

	outb(0x21, 0);
	outb(0xA1, 8);

	outb(0x21, 4);
	outb(0xA1, 2);

	outb(0x21, 0x01);
	outb(0xA1, 0x01);

	outb(0x21, a1);
	outb(0xA1, a2);

    outb(0x21, 0xff);
    outb(0xa1, 0xff);
    clear_mask(0);
    clear_mask(1);
    clear_mask(2);
    clear_mask(12);
    LoadIDT();
}