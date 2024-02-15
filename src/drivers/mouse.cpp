#include <drivers/mouse.h>

unsigned char buffer[3];
unsigned char offset = 0;
unsigned char buttons = 0;

MouseState MouseStateGlobal;

void InitMouse()
{
    outb(0x64, 0xA8);
    outb(0x64 ,0x20);
    unsigned char status = inb(0x60) | 2;
    outb(0x64, 0x60);
    outb(0x60, status);
    outb(0x64, 0xD4);
    outb(0x60, 0xF4);
    inb(0x60);
}

extern "C" void isr12_handler()
{
    MouseStateGlobal.mouse_input = true;
    buffer[offset] = inb(0x60);

    offset = (offset + 1) % 3;

    if(offset == 0)
    {
        if(buffer[1] != 0 || buffer[2] != 0)
        {
            MouseStateGlobal.deltax = (char)buffer[1];
            MouseStateGlobal.deltay = -((char)buffer[2]);

            MouseStateGlobal.absolute_x += MouseStateGlobal.deltax;
            if(MouseStateGlobal.absolute_x >= 1280) MouseStateGlobal.absolute_x = 1279;
            if(MouseStateGlobal.absolute_x < 0) MouseStateGlobal.absolute_x = 0;
            MouseStateGlobal.absolute_y += MouseStateGlobal.deltay;
            if(MouseStateGlobal.absolute_y >= 1024) MouseStateGlobal.absolute_y = 1023;
            if(MouseStateGlobal.absolute_y < 0) MouseStateGlobal.absolute_y = 0;
        }

        for(unsigned char i = 0; i < 2; i++)
        {
            if((buffer[0] & (0x1<<i)) != (buttons & (0x1<<i)))
            {
                if(buttons & (0x1<<i) && i==0)
                {
                    MouseStateGlobal.left_released = true;
                }
                else MouseStateGlobal.left_released = false;

                if(!(buttons & (0x1<<i)) && i==0)
                {
                    MouseStateGlobal.left_held = true;
                }
                else MouseStateGlobal.left_held = false;

                if(buttons & (0x1<<i) && i==1)
                {
                    MouseStateGlobal.right_released = true;
                }
                else MouseStateGlobal.right_released = false;
                
                if(!(buttons & (0x1<<i)) && i==1)
                {
                    MouseStateGlobal.right_held = true;
                }
                else MouseStateGlobal.right_held = false;
            }
        }
        buttons = buffer[0];
    }

    outb(0x20, 0x20);
    outb(0xa0, 0x20);
}