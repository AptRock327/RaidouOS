#pragma once
void outb(unsigned short port, unsigned char val);

unsigned char inb(unsigned short port);

void outw(unsigned short port, unsigned short val);

unsigned short inw(unsigned short port);

void outl(unsigned short port, unsigned int val);

unsigned int inl(unsigned short port);