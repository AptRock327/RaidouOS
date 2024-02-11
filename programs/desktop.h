#pragma once
#include "../drivers/graphics.h"
#include "../kernel/filesystem.h"
#include "../drivers/mouse.h"
#include "../kernel/pci.h"
#include "../drivers/rtc.h"
#include "terminal.h"

extern unsigned char CursorImage[];

bool ClearPermit;
extern bool terminal_on;

extern char terminalbuffer[248];

extern unsigned short terminal_xs;
extern unsigned short terminal_xe;
extern unsigned short terminal_ys;
extern unsigned short terminal_ye;

extern bool default_mode;

char integerToStringOutput[128];
template<typename T>
const char* IntegerToString(T value)
{
    unsigned char isNegative = 0;

    if(value < 0)
    {
        isNegative = 1;
        value *= -1;
        integerToStringOutput[0] = '-';
    }

    unsigned char size = 0;
    unsigned long long sizeTester = (unsigned long long)value;
    while(sizeTester / 10 > 0)
    {
        sizeTester /= 10;
        size++;
    }

    unsigned char index = 0;
    unsigned long long newValue = (unsigned long long)value;
    while(newValue / 10 > 0)
    {
        unsigned char remainder = newValue % 10;
        newValue /= 10;
        integerToStringOutput[isNegative + size - index] = remainder + 48;
        index++;
    }
    unsigned char remainder = newValue % 10;
    integerToStringOutput[isNegative + size - index] = remainder + 48;
    integerToStringOutput[isNegative + size + 1] = 0;
    if(value >= 10) return integerToStringOutput;
    if(value < 10)
    {
        integerToStringOutput[1] = integerToStringOutput[0];
        integerToStringOutput[0] = '0';
        return integerToStringOutput;
    }

    return (const char*)"";
}

void desktop()
{
    unsigned int* CursorData = tga_parse((unsigned char*)read("/img/cursor.tga"), 1);
    unsigned int* LogoData = tga_parse((unsigned char*)read("/img/logo.tga"), 1);
    unsigned int* MiniLogoData = tga_parse((unsigned char*)read("/img/mini_logo.tga"), 1);
    unsigned int* CloseData = tga_parse((unsigned char*)read("/img/close.tga"), 1);
    for(int i = 0; i < 1280; i++ )
    {
        for(int j = 0; j < 960; j++) putpixel(i, j, 0, 0, 0);
        for(int j = 960; j < 1024; j++) putpixel(i, j, 95, 0, 160);
    }
    while(1)
    {
        ClearPermit = false;
        MouseStateGlobal.mouse_input = false;
        /*
        print("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz123456789:", 0, 0);
        print("The quick brown fox jumps over the lazy dog", 100, 700);
        */
        asm("int $0x80" : : "a" (2), "b" (LogoData), "c" (490), "d" (362));
        asm("int $0x80" : : "a" (2), "b" (MiniLogoData), "c" (10), "d" (965));
        print(IntegerToString<unsigned char>(rtc_correct_hour()), 1170, 970);
        print(":",  1210, 970);
        print(IntegerToString<unsigned char>(rtc_correct_minutes()), 1230, 970);

        //Manage the terminal window
        if(terminal_on)
        {
            for(int i = terminal_xs; i < terminal_xe; i++ )
            {
                for(int j = terminal_ys; j < terminal_ys+70; j++) putpixel(i, j, 95, 0, 160);
                for(int j = terminal_ys+70; j < terminal_ye; j++) putpixel(i, j, 128, 128, 128);
            }
            asm("int $0x80" : : "a" (2), "b" (CloseData), "c" (terminal_xe-70), "d" (terminal_ys));
        }

        while(!ClearPermit)
        {
            if(MouseStateGlobal.left_held && (MouseStateGlobal.absolute_x < 70) && (MouseStateGlobal.absolute_y < 70)) terminal_on = !terminal_on;
            if(terminal_on && MouseStateGlobal.left_held && (MouseStateGlobal.absolute_x > terminal_xe-70) && (MouseStateGlobal.absolute_x < terminal_xe) && (MouseStateGlobal.absolute_y > terminal_ys) && (MouseStateGlobal.absolute_y < terminal_ye))
            {
                terminal_on = false;
                default_mode = true;
            }
        }
        asm("int $0x80" : : "a" (2), "b" (CursorData), "c" (MouseStateGlobal.absolute_x), "d" (MouseStateGlobal.absolute_y));
        SwapBuffers();
        for(int i = 0; i < 1280; i++ )
        {
            for(int j = 0; j < 960; j++) putpixel(i, j, 0, 0, 0);
            for(int j = 960; j < 1024; j++) putpixel(i, j, 95, 0, 160);
        }
    }
}