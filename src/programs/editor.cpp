#include <programs/editor.h>

unsigned short editor_xs = 320;
unsigned short editor_xe = 960;
unsigned short editor_ys = 300;
unsigned short editor_ye = 700;

extern bool ClearPermit;
extern bool keyboard_input;
extern unsigned char KBState;
extern MouseState MouseStateGlobal;
extern char* openedfile;

bool editor_on;

bool editor_focus;

bool edLeftShiftHeld = false;
bool edRightShiftHeld = false;
bool edcapslock = false;

char editorbuffer[248] = "Lorem ipsum                                                                                                                                                                                                                                           ";

void editor()
{
    const char KeyboardTable[] =
    {
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
    char tmp;
    unsigned char editorpos = 0;
    unsigned int* EditorLogoData = tga_parse((unsigned char*)read("/img/editor.tga"), 1);
    const char* file_contents;
    while(true)
    {
        asm("int $0x80" : : "a" (2), "b" (EditorLogoData), "c" (20), "d" (110));
        ClearPermit = true;
        if(editor_on)
        {
            asm("int $0x80" : : "a" (2), "b" (EditorLogoData), "c" (editor_xs), "d" (editor_ys));
            asm("int $0x80" : : "a" (3), "b" ("Text Editor"), "c" (editor_xs + 80), "d" (editor_ys + 10));
            tmp = editorbuffer[31];
            editorbuffer[31] = 0;
            asm("int $0x80" : : "a" (3), "b" (editorbuffer), "c" (editor_xs), "d" (editor_ys + 70));
            editorbuffer[31] = tmp;
            tmp = editorbuffer[62];
            editorbuffer[62] = 0;
            asm("int $0x80" : : "a" (3), "b" (editorbuffer+31), "c" (editor_xs), "d" (editor_ys + 110));
            editorbuffer[62] = tmp;
            tmp = editorbuffer[93];
            editorbuffer[93] = 0;
            asm("int $0x80" : : "a" (3), "b" (editorbuffer+62), "c" (editor_xs), "d" (editor_ys + 150));
            editorbuffer[93] = tmp;
            tmp = editorbuffer[124];
            editorbuffer[124] = 0;
            asm("int $0x80" : : "a" (3), "b" (editorbuffer+93), "c" (editor_xs), "d" (editor_ys + 190));
            editorbuffer[124] = tmp;
            tmp = editorbuffer[155];
            editorbuffer[155] = 0;
            asm("int $0x80" : : "a" (3), "b" (editorbuffer+124), "c" (editor_xs), "d" (editor_ys + 230));
            editorbuffer[155] = tmp;
            tmp = editorbuffer[186];
            editorbuffer[186] = 0;
            asm("int $0x80" : : "a" (3), "b" (editorbuffer+155), "c" (editor_xs), "d" (editor_ys + 270));
            editorbuffer[186] = tmp;
            tmp = editorbuffer[217];
            editorbuffer[217] = 0;
            asm("int $0x80" : : "a" (3), "b" (editorbuffer+186), "c" (editor_xs), "d" (editor_ys + 310));
            editorbuffer[217] = tmp;
            asm("int $0x80" : : "a" (3), "b" (editorbuffer+217), "c" (editor_xs), "d" (editor_ys + 350));

            if(keyboard_input && editor_focus)
            {
                keyboard_input = false;
                if(KBState == 0x0E)
                {
                    if((editorpos % 31) != 0)
                    {
                        editorpos-=1;
                        editorbuffer[editorpos] = ' ';
                    }
                    else
                    {
                        editorpos-=1;
                        editorbuffer[editorpos] = 0;
                    }
                }
                if(KBState == 0x2A) edLeftShiftHeld = true;
                if(KBState == 0xAA) edLeftShiftHeld = false;
                if(KBState == 0x36) edRightShiftHeld = true;
                if(KBState == 0xB6) edRightShiftHeld = false;
                if(KBState == 0x3A) edcapslock = !edcapslock;
                if(KBState == 0x1C)
                {
                    editorpos+=31-(editorpos%31);
                }
                //load file
                if(KBState == 0x3B)
                {
                    file_contents = read(openedfile);
                    for(unsigned char i = 0; i < strlen(file_contents); i++)
                    {
                        editorbuffer[i] = file_contents[i];
                    }
                }
                //save file
                if(KBState == 0x3C)
                {
                    asm("int $0x80" : : "a" (7), "b" (editorbuffer));
                }
                if((KBState < 0x3A) && !(KBState == 0x2A) && !(KBState == 0xAA) && !(KBState == 0x36) && !(KBState == 0xB6) && !(KBState == 0x0E) && !(KBState == 0x1C))
                {
                    if((edLeftShiftHeld | edRightShiftHeld) != edcapslock) editorbuffer[editorpos] = KeyboardTable[KBState] - 32;
                    else editorbuffer[editorpos] = KeyboardTable[KBState];
                    if(((editorpos+1) % 31) != 0) editorpos++;
                }
            }

            if(MouseStateGlobal.left_held && (MouseStateGlobal.absolute_x > editor_xs) && (MouseStateGlobal.absolute_x < editor_xe-70) && (MouseStateGlobal.absolute_y > editor_ys) && (MouseStateGlobal.absolute_y < editor_ye))
            {
                if((editor_xs+MouseStateGlobal.deltax > 0) && (editor_xe+MouseStateGlobal.deltax < 1280))
                {
                    editor_xs+=MouseStateGlobal.deltax;
                    editor_xe+=MouseStateGlobal.deltax;
                }
                if((editor_ys+MouseStateGlobal.deltay > 0) && (editor_ye+MouseStateGlobal.deltay < 1024))
                {
                    editor_ys+=MouseStateGlobal.deltay;
                    editor_ye+=MouseStateGlobal.deltay;
                }
            }
            ClearPermit = false;
        }
    }
}