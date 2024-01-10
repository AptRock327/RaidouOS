#pragma once
#include "IO.h"
#include "multitasking.h"
#include "../drivers/mouse.h"
#include "../drivers/graphics.h"
#include "../programs/terminal.h"

extern bool terminal_on;

extern "C" void isr128_handler(Task state)
{
    switch(state.rax)
    {
        case 1:
            putpixel((unsigned short)state.rbx, (unsigned short)state.rcx, 95, 0, 160);
            break;

        case 2:
            display_image((unsigned int*)state.rbx, (unsigned short)state.rcx, (unsigned short)state.rdx);
            break;
        
        case 3:
            print((char*)state.rbx, (unsigned short)state.rcx, (unsigned short)state.rdx);
            break;
        
        case 4:
            SwapBuffers();
            break;
        
        case 5:
            for(int i = 320; i < 960; i++)
            {
                for(int j = 200; j < 400; j++) { putpixel(i, j, 95, 0, 160); if(MouseStateGlobal.left_held) terminal_on = !terminal_on;}
            }
            break;

        default:
            break;
    }
    outb(0x20, 0x20);
    outb(0xa0, 0x20);
}