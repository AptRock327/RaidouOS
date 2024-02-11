#pragma once
#include "../drivers/graphics.h"
#include "../kernel/filesystem.h"
#include "../drivers/keyboard.h"
#include "../drivers/mouse.h"
#include "../kernel/pci.h"
#include "../drivers/rtc.h"
#include "../kernel/memory.h"

extern bool ClearPermit;
bool terminal_on;

unsigned short terminal_xs = 320;
unsigned short terminal_xe = 960;
unsigned short terminal_ys = 300;
unsigned short terminal_ye = 700;

bool LeftShiftHeld = false;
bool RightShiftHeld = false;
bool capslock = false;

const char KeyboardTable[] ={
    0, 0, '1', '2',
    '3', '4', '5', '6',
    '7', '8', '9', '0',
    '-', '=', 0, 0,
    'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i',
    'o', 'p', '[', ']',
    0, 0, 'a', 's',
    'd', 'f', 'g', 'h',
    'j', 'k', 'l', ';',
    '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',',
    '.', '/', 0, '*',
    0, ' '
};

bool default_mode;

char terminalbuffer[248] = "RaidouOS terminal                                             root:                                                                                                                                                                                   ";

void terminal()
{
    unsigned int* TerminalLogoData = tga_parse((unsigned char*)read("/img/terminal.tga"), 1);
    unsigned short charpos = 68;
    char last = 0;
    char tmp;
    while(1)
    {
        if(default_mode)
        {
            char output[248] = "RaidouOS terminal                                             root:                                                                                                                                                                                   ";
            for(unsigned char i = 0; i < 249; i++) terminalbuffer[i] = output[i];
            charpos = 68;
            default_mode = false;
        }
        asm("int $0x80" : : "a" (2), "b" (TerminalLogoData), "c" (20), "d" (20));
        if(MouseStateGlobal.left_held && (MouseStateGlobal.absolute_x < 70) && (MouseStateGlobal.absolute_y < 70)) terminal_on = !terminal_on;
        ClearPermit = true;
        if(terminal_on)
        {
            asm("int $0x80" : : "a" (2), "b" (TerminalLogoData), "c" (terminal_xs), "d" (terminal_ys));
            asm("int $0x80" : : "a" (3), "b" ("Terminal"), "c" (terminal_xs + 80), "d" (terminal_ys + 10));
            tmp = terminalbuffer[31];
            terminalbuffer[31] = 0;
            asm("int $0x80" : : "a" (3), "b" (terminalbuffer), "c" (terminal_xs), "d" (terminal_ys + 70));
            terminalbuffer[31] = tmp;
            tmp = terminalbuffer[62];
            terminalbuffer[62] = 0;
            asm("int $0x80" : : "a" (3), "b" (terminalbuffer+31), "c" (terminal_xs), "d" (terminal_ys + 110));
            terminalbuffer[62] = tmp;
            tmp = terminalbuffer[93];
            terminalbuffer[93] = 0;
            asm("int $0x80" : : "a" (3), "b" (terminalbuffer+62), "c" (terminal_xs), "d" (terminal_ys + 150));
            terminalbuffer[93] = tmp;
            tmp = terminalbuffer[124];
            terminalbuffer[124] = 0;
            asm("int $0x80" : : "a" (3), "b" (terminalbuffer+93), "c" (terminal_xs), "d" (terminal_ys + 190));
            terminalbuffer[124] = tmp;
            tmp = terminalbuffer[155];
            terminalbuffer[155] = 0;
            asm("int $0x80" : : "a" (3), "b" (terminalbuffer+124), "c" (terminal_xs), "d" (terminal_ys + 230));
            terminalbuffer[155] = tmp;
            tmp = terminalbuffer[186];
            terminalbuffer[186] = 0;
            asm("int $0x80" : : "a" (3), "b" (terminalbuffer+155), "c" (terminal_xs), "d" (terminal_ys + 270));
            terminalbuffer[186] = tmp;
            tmp = terminalbuffer[217];
            terminalbuffer[217] = 0;
            asm("int $0x80" : : "a" (3), "b" (terminalbuffer+186), "c" (terminal_xs), "d" (terminal_ys + 310));
            terminalbuffer[217] = tmp;
            terminalbuffer[248] = 0;
            asm("int $0x80" : : "a" (3), "b" (terminalbuffer+217), "c" (terminal_xs), "d" (terminal_ys + 350));
            if(keyboard_input)
            {
                keyboard_input = false;
                if(KBState == 0x0E)
                {
                    if((charpos % 31) != 0)
                    {
                        charpos-=1;
                        terminalbuffer[charpos] = ' ';
                    }
                    else
                    {
                        charpos-=1;
                        terminalbuffer[charpos] = 0;
                    }
                }
                if(KBState == 0x2A) LeftShiftHeld = true;
                if(KBState == 0xAA) LeftShiftHeld = false;
                if(KBState == 0x36) RightShiftHeld = true;
                if(KBState == 0xB6) RightShiftHeld = false;
                if(KBState == 0x3A) capslock = !capslock;
                if(KBState == 0x1C)
                {
                    charpos+=31-(charpos%31);
                    if(terminalbuffer[charpos-25] == 'i' && terminalbuffer[charpos-24] == 'd')
                    {
                        const char* output = "Running RaidouOS alpha";
                        for(char i = 0; i < 24; i++) terminalbuffer[charpos+i] = output[i];
                    }
                    else if(terminalbuffer[charpos-25] == 'e' && terminalbuffer[charpos-24] == 'c' && terminalbuffer[charpos-23] == 'h' && terminalbuffer[charpos-22] == 'o')
                    {
                        for(char i = 0; i < 20; i++) terminalbuffer[charpos+i] = terminalbuffer[charpos-20+i];
                    }
                    else if(terminalbuffer[charpos-25] == 'b' && terminalbuffer[charpos-24] == 'f')
                    {
                        char *p=(char*)calloc(30000);
                        unsigned short bfindex = 0;
                        unsigned short bracket_tmp;
                        unsigned short bracket_tmp2;
                        char* inputpath = (char*)calloc(22);
                        for(char i = 0; i < 22; i++)
                        {
                            if(terminalbuffer[charpos-22+i] != ' ') inputpath[i] = terminalbuffer[charpos-22+i];
                        }
                        const char* output = read((const char*)inputpath);
                        const char* outie = "Invalid path: NOT.IN.FS";
                        if(output == 0)
                        {
                            for(char i = 0; i < 23; i++) terminalbuffer[charpos+i] = outie[i];
                        }
                        bool endloop;
                        while(output[bfindex])
                        {
                            switch(output[bfindex])
                            {
                                case '>':
                                    ++p;
                                    break;
                                case '<':
                                    --p;
                                    break;
                                case '+':
                                    ++(*p);
                                    break;
                                case '-':
                                    --(*p);
                                    break;
                                case '.':
                                    terminalbuffer[charpos] = *p;
                                    charpos++;
                                    break;
                                case '[':
                                    bracket_tmp = bfindex;
                                    endloop = false;
                                    while(!endloop)
                                    {
                                        bfindex++;
                                        switch(output[bfindex])
                                        {
                                            case '>':
                                                ++p;
                                                break;
                                            case '<':
                                                --p;
                                                break;
                                            case '+':
                                                ++(*p);
                                                break;
                                            case '-':
                                                --(*p);
                                                break;
                                            case '.':
                                                terminalbuffer[charpos] = *p;
                                                charpos++;
                                                break;
                                            case '[':
                                                bracket_tmp2 = bfindex;
                                                while(!((*p == 0) && (output[bfindex] == ']')))
                                                {
                                                    bfindex++;
                                                    switch(output[bfindex])
                                                    {
                                                        case '>':
                                                            ++p;
                                                            break;
                                                        case '<':
                                                            --p;
                                                            break;
                                                        case '+':
                                                            ++(*p);
                                                            break;
                                                        case '-':
                                                            --(*p);
                                                            break;
                                                        case '.':
                                                            terminalbuffer[charpos] = *p;
                                                            charpos++;
                                                            break;
                                                        case ']':
                                                            if(*p != 0) bfindex=bracket_tmp2;
                                                            break;
                                                        default:
                                                            break;
                                                    }
                                                }
                                                break;
                                            case ']':
                                                if(*p != 0) bfindex=bracket_tmp;
                                                if(*p == 0) endloop = true;
                                                break;
                                            default:
                                                break;
                                        }
                                    }
                                    break;
                                default:
                                    break;
                            }
                            bfindex++;
                        }
                    }
                    else
                    {
                        const char* output = "Illegal command";
                        for(char i = 0; i < 15; i++) terminalbuffer[charpos+i] = output[i];
                    }
                    charpos+=31-(charpos%31);
                    terminalbuffer[charpos] = 'r';
                    charpos++;
                    terminalbuffer[charpos] = 'o';
                    charpos++;
                    terminalbuffer[charpos] = 'o';
                    charpos++;
                    terminalbuffer[charpos] = 't';
                    charpos++;
                    terminalbuffer[charpos] = ':';
                    charpos++;
                    terminalbuffer[charpos] = ' ';
                    charpos++;
                }
                if((KBState < 0x3A) && !(KBState == 0x2A) && !(KBState == 0xAA) && !(KBState == 0x36) && !(KBState == 0xB6) && !(KBState == 0x0E) && !(KBState == 0x1C))
                {
                    if((LeftShiftHeld | RightShiftHeld) != capslock) terminalbuffer[charpos] = KeyboardTable[KBState] - 32;
                    else terminalbuffer[charpos] = KeyboardTable[KBState];
                    if(((charpos+1) % 31) != 0) charpos++;
                }
            }
            if(MouseStateGlobal.left_held && (MouseStateGlobal.absolute_x > terminal_xs) && (MouseStateGlobal.absolute_x < terminal_xe-70) && (MouseStateGlobal.absolute_y > terminal_ys) && (MouseStateGlobal.absolute_y < terminal_ye))
            {
                if((terminal_xs+MouseStateGlobal.deltax > 0) && (terminal_xe+MouseStateGlobal.deltax < 1280))
                {
                    terminal_xs+=MouseStateGlobal.deltax;
                    terminal_xe+=MouseStateGlobal.deltax;
                }
                if((terminal_ys+MouseStateGlobal.deltay > 0) && (terminal_ye+MouseStateGlobal.deltay < 1024))
                {
                    terminal_ys+=MouseStateGlobal.deltay;
                    terminal_ye+=MouseStateGlobal.deltay;
                }
            }
            ClearPermit = false;
        }
    }
}