#include "IO.h"
void outb(unsigned short port, unsigned char val)
{
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

unsigned char inb(unsigned short port)
{
    unsigned char returnval;
    asm volatile("inb %1, %0" : "=a"(returnval) : "Nd"(port));
    return returnval;
}

void outw(unsigned short port, unsigned short val)
{
    asm volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

unsigned short inw(unsigned short port)
{
    unsigned short returnval;
    asm volatile("inw %1, %0" : "=a"(returnval) : "Nd"(port));
    return returnval;
}

void outl(unsigned short port, unsigned int val)
{
    asm volatile ("outl %0, %1" : : "a"(val), "Nd"(port));
}

unsigned int inl(unsigned short port)
{
    unsigned int returnval;
    asm volatile("inl %1, %0" : "=a"(returnval) : "Nd"(port));
    return returnval;
}