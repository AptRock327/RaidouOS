#include <kernel/syscalls.h>

extern bool terminal_on;
extern MouseState MouseStateGlobal;
extern char* openedfile;

extern "C" void isr128_handler(Context state)
{
    switch(state.rax)
    {
        case 1: //SYS_PUTPIXEL
            putpixel((unsigned short)state.rbx, (unsigned short)state.rcx, 95, 0, 160);
            break;

        case 2: //SYS_DISPLAYIMG
            display_image((unsigned int*)state.rbx, (unsigned short)state.rcx, (unsigned short)state.rdx);
            break;
        
        case 3: //SYS_PRINTSTR
            print((char*)state.rbx, (unsigned short)state.rcx, (unsigned short)state.rdx);
            break;
        
        case 4: //SYS_SWAPBUFFER
            SwapBuffers();
            break;
        
        case 5: //SYS_WTF
            for(int i = 320; i < 960; i++)
            {
                for(int j = 200; j < 400; j++) { putpixel(i, j, 95, 0, 160); if(MouseStateGlobal.left_held) terminal_on = !terminal_on;}
            }
            break;

        case 6: //SYS_WRITE
            write(openedfile, (const char*)state.rbx);
            break;

        case 7: //SYS_OVERRIDE
            foverride(openedfile, (const char*)state.rbx);
            break;

        case 8: //SYS_SLEEP
            sleep((unsigned long long)state.rbx);
            break;

        default:
            break;
    }
    outb(0x20, 0x20);
    outb(0xa0, 0x20);
}