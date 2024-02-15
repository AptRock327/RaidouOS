#include <programs/terminal.h>

extern bool ClearPermit;
extern MouseState MouseStateGlobal;
extern bool keyboard_input;
extern unsigned char KBState;
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

char terminalbuffer[248] = "RaidouOS terminal [type help]                                 root:                                                                                                                                                                                   ";

char* openedfile;

void terminal()
{
    unsigned int* TerminalLogoData = tga_parse((unsigned char*)read("/img/terminal.tga"), 1);
    unsigned short charpos = 68;
    openedfile = (char*)calloc(21);
    char last = 0;
    char tmp;
    bool nonewline;
    while(1)
    {
        if(default_mode)
        {
            char output[248] = "RaidouOS terminal [type help]                                 root:                                                                                                                                                                                   ";
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
                        const char* output = "Running RaidouOS beta";
                        for(char i = 0; i < 24; i++) terminalbuffer[charpos+i] = output[i];
                    }
                    else if(terminalbuffer[charpos-25] == 'c' && terminalbuffer[charpos-24] == 'l' && terminalbuffer[charpos-23] == 'e' && terminalbuffer[charpos-22] == 'a' && terminalbuffer[charpos-21] == 'r')
                    {
                        default_mode = true;
                    }
                    else if(terminalbuffer[charpos-25] == 'h' && terminalbuffer[charpos-24] == 'e' && terminalbuffer[charpos-23] == 'l' && terminalbuffer[charpos-22] == 'p')
                    {
                        charpos=0;
                        const char* output = "help: shows the help screen";
                        for(char i = 0; i < 30; i++) terminalbuffer[charpos+i] = output[i];
                        charpos+=31-(charpos%31);
                        const char* output1 = "id: identifies the OS";
                        for(char i = 0; i < 30; i++) terminalbuffer[charpos+i] = output1[i];
                        charpos+=31-(charpos%31);
                        const char* output2 = "echo [msg]: prints a message";
                        for(char i = 0; i < 30; i++) terminalbuffer[charpos+i] = output2[i];
                        charpos+=31-(charpos%31);
                        const char* output3 = "clear: clears the terminal";
                        for(char i = 0; i < 30; i++) terminalbuffer[charpos+i] = output3[i];
                        charpos+=31-(charpos%31);
                        const char* output4 = "open [path]: opens a file";
                        for(char i = 0; i < 30; i++) terminalbuffer[charpos+i] = output4[i];
                        charpos+=31-(charpos%31);
                        const char* output5 = "write [msg]: writes to a file";
                        for(char i = 0; i < 30; i++) terminalbuffer[charpos+i] = output5[i];
                        charpos+=31-(charpos%31);
                        const char* output6 = "od [msg]: overrides a file";
                        for(char i = 0; i < 30; i++) terminalbuffer[charpos+i] = output6[i];
                        charpos+=31-(charpos%31);
                        const char* output7 = "bf [path]: runs a bf program";
                        for(char i = 0; i < 30; i++) terminalbuffer[charpos+i] = output7[i];
                        charpos+=31-(charpos%31);
                    }
                    else if(terminalbuffer[charpos-25] == 'o' && terminalbuffer[charpos-24] == 'p' && terminalbuffer[charpos-23] == 'e' && terminalbuffer[charpos-22] == 'n')
                    {
                        for(char i = 0; i < 20; i++)
                        {
                            if(terminalbuffer[charpos-20+i] != ' ') openedfile[i] = terminalbuffer[charpos-20+i];
                        }
                        const char* outie = "Invalid path: NOT.IN.FS";
                        const char* OPENTEXT = "OPEN";
                        if(!fschk((const char*)openedfile))
                        {
                            for(char i = 0; i < 23; i++) terminalbuffer[charpos+i] = outie[i];
                        }
                        else
                        {
                            for(char i = 0; i < 20; i++) terminalbuffer[charpos+i] = openedfile[i];
                            terminalbuffer[charpos+strlen(openedfile)] = ' ';
                            terminalbuffer[charpos+strlen(openedfile)+1] = 'o';
                            terminalbuffer[charpos+strlen(openedfile)+2] = 'p';
                            terminalbuffer[charpos+strlen(openedfile)+3] = 'e';
                            terminalbuffer[charpos+strlen(openedfile)+4] = 'n';
                            terminalbuffer[charpos+strlen(openedfile)+5] = 'e';
                            terminalbuffer[charpos+strlen(openedfile)+6] = 'd';
                        }
                    }
                    else if(terminalbuffer[charpos-25] == 'w' && terminalbuffer[charpos-24] == 'r' && terminalbuffer[charpos-23] == 'i' && terminalbuffer[charpos-22] == 't' && terminalbuffer[charpos-21] == 'e')
                    {
                        const char* outie = "Invalid write: NO.OPEN.FILE";
                        char* output = (char*)calloc(20);
                        if(!fschk((const char*)openedfile))
                        {
                            for(char i = 0; i < 30; i++) terminalbuffer[charpos+i] = outie[i];
                        }
                        else
                        {
                            for(char i = 0; i < 19; i++) output[i] = terminalbuffer[charpos-19+i];
                            asm("int $0x80" : : "a" (6), "b" (output));
                            nonewline = true;
                        }
                        free(output);
                    }
                    else if(terminalbuffer[charpos-25] == 'o' && terminalbuffer[charpos-24] == 'd')
                    {
                        const char* outie = "Invalid write: NO.OPEN.FILE";
                        char* output = (char*)calloc(23);
                        if(!fschk((const char*)openedfile))
                        {
                            for(char i = 0; i < 30; i++) terminalbuffer[charpos+i] = outie[i];
                        }
                        else
                        {
                            for(char i = 0; i < 24; i++) output[i] = terminalbuffer[charpos-23+i];
                            asm("int $0x80" : : "a" (7), "b" (output));
                            nonewline = true;
                        }
                    }
                    else if(terminalbuffer[charpos-25] == 'b' && terminalbuffer[charpos-24] == 'f')
                    {
                        char* p = (char*)calloc(30000);
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
                                case 'n':
                                    ++p;
                                    break;
                                case '<':
                                    --p;
                                    break;
                                case 'p':
                                    --p;
                                    break;
                                case '+':
                                    ++(*p);
                                    break;
                                case 'i':
                                    ++(*p);
                                    break;
                                case '-':
                                    --(*p);
                                    break;
                                case 'd':
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
                                            case 'n':
                                                ++p;
                                                break;
                                            case '<':
                                                --p;
                                                break;
                                            case 'p':
                                                --p;
                                                break;
                                            case '+':
                                                ++(*p);
                                                break;
                                            case 'i':
                                                ++(*p);
                                                break;
                                            case '-':
                                                --(*p);
                                                break;
                                            case 'd':
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
                                                        case 'n':
                                                            ++p;
                                                            break;
                                                        case '<':
                                                            --p;
                                                            break;
                                                        case 'p':
                                                            --p;
                                                            break;
                                                        case '+':
                                                            ++(*p);
                                                            break;
                                                        case 'i':
                                                            ++(*p);
                                                            break;
                                                        case '-':
                                                            --(*p);
                                                            break;
                                                        case 'd':
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
                        free(inputpath);
                    }
                    else
                    {
                        const char* output = "Illegal command";
                        for(char i = 0; i < 15; i++) terminalbuffer[charpos+i] = output[i];
                    }
                    if(!nonewline) charpos+=31-(charpos%31);
                    if(nonewline) nonewline = false;
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