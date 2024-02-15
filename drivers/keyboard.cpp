#include "keyboard.h"

bool keyboard_input;
unsigned char KBState;

extern "C" void isr1_handler()
{
    keyboard_input = true;
    KBState = inb(0x60);
    outb(0x20, 0x20);
    outb(0xa0, 0x20);
}