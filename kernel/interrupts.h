#pragma once

struct IDT
{
    unsigned short offset_low;
    unsigned short selector;
    unsigned char ist;
    unsigned char types_attr;
    unsigned short offset_mid;
    unsigned int offset_high;
    unsigned int zero;
};

void clear_mask(unsigned char IRQ);

void set_mask(unsigned char IRQ);

void InitIDT();