#pragma once
#define NULL 0
#include "../kernel/memory.h"
#include "../kernel/filesystem.h"

const char* backbuffer;

void InitDoubleBuffering()
{
    backbuffer = (const char*)calloc(80000000);
}

void putpixel(unsigned short x, unsigned short y, unsigned char red, unsigned char green, unsigned char blue)
{
    char* vidmem = (char*)backbuffer;
    *(vidmem + (y * 1280 * 3) + (x * 3)) = blue;
    *(vidmem + (y * 1280 * 3) + (x * 3) + 1) = green;
    *(vidmem + (y * 1280 * 3) + (x * 3) + 2) = red;
}

unsigned int *tga_parse(unsigned char *ptr, int size)
{
    unsigned int *data;
    int i, j, k, x, y, w = (ptr[13] << 8) + ptr[12], h = (ptr[15] << 8) + ptr[14], o = (ptr[11] << 8) + ptr[10];
    int m = ((ptr[1]? (ptr[7]>>3)*ptr[5] : 0) + 18);
 
    if(w<1 || h<1) return NULL;
 
    data = (unsigned int*)malloc((w*h+2)*sizeof(unsigned int));
    if(!data) return NULL;
 
    switch(ptr[2]) {
        case 1:
            if(ptr[6]!=0 || ptr[4]!=0 || ptr[3]!=0 || (ptr[7]!=24 && ptr[7]!=32)) { free(data); return NULL; }
            for(y=i=0; y<h; y++) {
                k = ((!o?h-y-1:y)*w);
                for(x=0; x<w; x++) {
                    j = ptr[m + k++]*(ptr[7]>>3) + 18;
                    data[2 + i++] = ((ptr[7]==32?ptr[j+3]:0xFF) << 24) | (ptr[j+2] << 16) | (ptr[j+1] << 8) | ptr[j];
                }
            }
            break;
        case 2:
            if(ptr[5]!=0 || ptr[6]!=0 || ptr[1]!=0 || (ptr[16]!=24 && ptr[16]!=32)) { free(data); return NULL; }
            for(y=i=0; y<h; y++) {
                j = ((!o?h-y-1:y)*w*(ptr[16]>>3));
                for(x=0; x<w; x++) {
                    data[2 + i++] = ((ptr[16]==32?ptr[j+3]:0xFF) << 24) | (ptr[j+2] << 16) | (ptr[j+1] << 8) | ptr[j];
                    j += ptr[16]>>3;
                }
            }
            break;
        case 9:
            if(ptr[6]!=0 || ptr[4]!=0 || ptr[3]!=0 || (ptr[7]!=24 && ptr[7]!=32)) { free(data); return NULL; }
            y = i = 0;
            for(x=0; x<w*h && m<size;) {
                k = ptr[m++];
                if(k > 127) {
                    k -= 127; x += k;
                    j = ptr[m++]*(ptr[7]>>3) + 18;
                    while(k--) {
                        if(!(i%w)) { i=((!o?h-y-1:y)*w); y++; }
                        data[2 + i++] = ((ptr[7]==32?ptr[j+3]:0xFF) << 24) | (ptr[j+2] << 16) | (ptr[j+1] << 8) | ptr[j];
                    }
                } else {
                    k++; x += k;
                    while(k--) {
                        j = ptr[m++]*(ptr[7]>>3) + 18;
                        if(!(i%w)) { i=((!o?h-y-1:y)*w); y++; }
                        data[2 + i++] = ((ptr[7]==32?ptr[j+3]:0xFF) << 24) | (ptr[j+2] << 16) | (ptr[j+1] << 8) | ptr[j];
                    }
                }
            }
            break;
        case 10:
            if(ptr[5]!=0 || ptr[6]!=0 || ptr[1]!=0 || (ptr[16]!=24 && ptr[16]!=32)) { free(data); return NULL; }
            y = i = 0;
            for(x=0; x<w*h && m<size;) {
                k = ptr[m++];
                if(k > 127) {
                    k -= 127; x += k;
                    while(k--) {
                        if(!(i%w)) { i=((!o?h-y-1:y)*w); y++; }
                        data[2 + i++] = ((ptr[16]==32?ptr[m+3]:0xFF) << 24) | (ptr[m+2] << 16) | (ptr[m+1] << 8) | ptr[m];
                    }
                    m += ptr[16]>>3;
                } else {
                    k++; x += k;
                    while(k--) {
                        if(!(i%w)) { i=((!o?h-y-1:y)*w); y++; }
                        data[2 + i++] = ((ptr[16]==32?ptr[m+3]:0xFF) << 24) | (ptr[m+2] << 16) | (ptr[m+1] << 8) | ptr[m];
                        m += ptr[16]>>3;
                    }
                }
            }
            break;
        default:
            free(data); return NULL;
    }
    data[0] = w;
    data[1] = h;
    return data;
}

void display_image(unsigned int* image_data, int x, int y)
{
    int n = (image_data[0]*image_data[1])+8;
    char* vidmem = (char*)backbuffer + y*1280*3 + x*3;
    for(unsigned int i = 0; i < (image_data[0]*image_data[1]*3); i++)
    {
        if(i != 0 && i % (image_data[0]*3) == 0) vidmem+=((1280-image_data[0])*3);
        if(i % 3 == 0) n--;
        if(((image_data[n] & 0xFF000000) >> 24) > 0)
        {
            if(i % 3 == 0) *vidmem = (char)(image_data[n] & 0xFF); //blue
            if((i+1) % 3 == 0) *vidmem = (char)((image_data[n] & 0xFF0000) >> 16); //red
            if((i+2) % 3 == 0) *vidmem = (char)((image_data[n] & 0xFF00) >> 8); //green
        }
        vidmem++;
    }
}

void clear_screen(char red, char green, char blue)
{
    char* vidmem = (char*)backbuffer;
    for(unsigned int i = 0; i < 4000000; i++)
    {
        if(i % 3 == 0) *vidmem = blue;
        if((i+1) % 3 == 0) *vidmem = red;
        if((i+2) % 3 == 0) *vidmem = green;
        vidmem++;
    }
}

void SwapBuffers()
{
    char* buffermem = (char*)backbuffer;
    char* vidmem = (char*)0x20000000;
    for(unsigned int i = 0; i < 10000000; i++)
    {
        if(i % 3 == 0) *vidmem = *buffermem;
        if((i+1) % 3 == 0) *vidmem = *buffermem;
        if((i+2) % 3 == 0) *vidmem = *buffermem;
        vidmem++;
        buffermem++;
    }
}

unsigned int* Aaimg;
unsigned int* Abimg;
unsigned int* Baimg;
unsigned int* Bbimg;
unsigned int* Caimg;
unsigned int* Cbimg;
unsigned int* Daimg;
unsigned int* Dbimg;
unsigned int* Eaimg;
unsigned int* Ebimg;
unsigned int* Faimg;
unsigned int* Fbimg;
unsigned int* Gaimg;
unsigned int* Gbimg;
unsigned int* Haimg;
unsigned int* Hbimg;
unsigned int* Iaimg;
unsigned int* Ibimg;
unsigned int* Jaimg;
unsigned int* Jbimg;
unsigned int* Kaimg;
unsigned int* Kbimg;
unsigned int* Laimg;
unsigned int* Lbimg;
unsigned int* Maimg;
unsigned int* Mbimg;
unsigned int* Naimg;
unsigned int* Nbimg;
unsigned int* Oaimg;
unsigned int* Obimg;
unsigned int* Paimg;
unsigned int* Pbimg;
unsigned int* Qaimg;
unsigned int* Qbimg;
unsigned int* Raimg;
unsigned int* Rbimg;
unsigned int* Saimg;
unsigned int* Sbimg;
unsigned int* Taimg;
unsigned int* Tbimg;
unsigned int* Uaimg;
unsigned int* Ubimg;
unsigned int* Vaimg;
unsigned int* Vbimg;
unsigned int* Waimg;
unsigned int* Wbimg;
unsigned int* Xaimg;
unsigned int* Xbimg;
unsigned int* Yaimg;
unsigned int* Ybimg;
unsigned int* Zaimg;
unsigned int* Zbimg;
unsigned int* Oneimg;
unsigned int* Twoimg;
unsigned int* Threeimg;
unsigned int* Fourimg;
unsigned int* Fiveimg;
unsigned int* Siximg;
unsigned int* Sevenimg;
unsigned int* Eightimg;
unsigned int* Nineimg;
unsigned int* Colonimg;
unsigned int* Zeroimg;
unsigned int* Slashimg;
unsigned int* Dotimg;

void InitFont()
{
    Aaimg = tga_parse((unsigned char*)read("/font/Aa.tga"), 1);
    Abimg = tga_parse((unsigned char*)read("/font/Ab.tga"), 1);
    Baimg = tga_parse((unsigned char*)read("/font/Ba.tga"), 1);
    Bbimg = tga_parse((unsigned char*)read("/font/Bb.tga"), 1);
    Caimg = tga_parse((unsigned char*)read("/font/Ca.tga"), 1);
    Cbimg = tga_parse((unsigned char*)read("/font/Cb.tga"), 1);
    Daimg = tga_parse((unsigned char*)read("/font/Da.tga"), 1);
    Dbimg = tga_parse((unsigned char*)read("/font/Db.tga"), 1);
    Eaimg = tga_parse((unsigned char*)read("/font/Ea.tga"), 1);
    Ebimg = tga_parse((unsigned char*)read("/font/Eb.tga"), 1);
    Faimg = tga_parse((unsigned char*)read("/font/Fa.tga"), 1);
    Fbimg = tga_parse((unsigned char*)read("/font/Fb.tga"), 1);
    Gaimg = tga_parse((unsigned char*)read("/font/Ga.tga"), 1);
    Gbimg = tga_parse((unsigned char*)read("/font/Gb.tga"), 1);
    Haimg = tga_parse((unsigned char*)read("/font/Ha.tga"), 1);
    Hbimg = tga_parse((unsigned char*)read("/font/Hb.tga"), 1);
    Iaimg = tga_parse((unsigned char*)read("/font/Ia.tga"), 1);
    Ibimg = tga_parse((unsigned char*)read("/font/Ib.tga"), 1);
    Jaimg = tga_parse((unsigned char*)read("/font/Ja.tga"), 1);
    Jbimg = tga_parse((unsigned char*)read("/font/Jb.tga"), 1);
    Kaimg = tga_parse((unsigned char*)read("/font/Ka.tga"), 1);
    Kbimg = tga_parse((unsigned char*)read("/font/Kb.tga"), 1);
    Laimg = tga_parse((unsigned char*)read("/font/La.tga"), 1);
    Lbimg = tga_parse((unsigned char*)read("/font/Lb.tga"), 1);
    Maimg = tga_parse((unsigned char*)read("/font/Ma.tga"), 1);
    Mbimg = tga_parse((unsigned char*)read("/font/Mb.tga"), 1);
    Naimg = tga_parse((unsigned char*)read("/font/Na.tga"), 1);
    Nbimg = tga_parse((unsigned char*)read("/font/Nb.tga"), 1);
    Oaimg = tga_parse((unsigned char*)read("/font/Oa.tga"), 1);
    Obimg = tga_parse((unsigned char*)read("/font/Ob.tga"), 1);
    Paimg = tga_parse((unsigned char*)read("/font/Pa.tga"), 1);
    Pbimg = tga_parse((unsigned char*)read("/font/Pb.tga"), 1);
    Qaimg = tga_parse((unsigned char*)read("/font/Qa.tga"), 1);
    Qbimg = tga_parse((unsigned char*)read("/font/Qb.tga"), 1);
    Raimg = tga_parse((unsigned char*)read("/font/Ra.tga"), 1);
    Rbimg = tga_parse((unsigned char*)read("/font/Rb.tga"), 1);
    Saimg = tga_parse((unsigned char*)read("/font/Sa.tga"), 1);
    Sbimg = tga_parse((unsigned char*)read("/font/Sb.tga"), 1);
    Taimg = tga_parse((unsigned char*)read("/font/Ta.tga"), 1);
    Tbimg = tga_parse((unsigned char*)read("/font/Tb.tga"), 1);
    Uaimg = tga_parse((unsigned char*)read("/font/Ua.tga"), 1);
    Ubimg = tga_parse((unsigned char*)read("/font/Ub.tga"), 1);
    Vaimg = tga_parse((unsigned char*)read("/font/Va.tga"), 1);
    Vbimg = tga_parse((unsigned char*)read("/font/Vb.tga"), 1);
    Waimg = tga_parse((unsigned char*)read("/font/Wa.tga"), 1);
    Wbimg = tga_parse((unsigned char*)read("/font/Wb.tga"), 1);
    Xaimg = tga_parse((unsigned char*)read("/font/Xa.tga"), 1);
    Xbimg = tga_parse((unsigned char*)read("/font/Xb.tga"), 1);
    Yaimg = tga_parse((unsigned char*)read("/font/Ya.tga"), 1);
    Ybimg = tga_parse((unsigned char*)read("/font/Yb.tga"), 1);
    Zaimg = tga_parse((unsigned char*)read("/font/Za.tga"), 1);
    Zbimg = tga_parse((unsigned char*)read("/font/Zb.tga"), 1);
    Oneimg = tga_parse((unsigned char*)read("/font/one.tga"), 1);
    Twoimg = tga_parse((unsigned char*)read("/font/two.tga"), 1);
    Threeimg = tga_parse((unsigned char*)read("/font/three.tga"), 1);
    Fourimg = tga_parse((unsigned char*)read("/font/four.tga"), 1);
    Fiveimg = tga_parse((unsigned char*)read("/font/five.tga"), 1);
    Siximg = tga_parse((unsigned char*)read("/font/six.tga"), 1);
    Sevenimg = tga_parse((unsigned char*)read("/font/seven.tga"), 1);
    Eightimg = tga_parse((unsigned char*)read("/font/eight.tga"), 1);
    Nineimg = tga_parse((unsigned char*)read("/font/nine.tga"), 1);
    Colonimg = tga_parse((unsigned char*)read("/font/colon.tga"), 1);
    Zeroimg = tga_parse((unsigned char*)read("/font/zero.tga"), 1);
    Slashimg = tga_parse((unsigned char*)read("/font/slash.tga"), 1);
    Dotimg = tga_parse((unsigned char*)read("/font/dot.tga"), 1);
}

void print(const char* str, unsigned short x, unsigned short y)
{
    unsigned char* current = (unsigned char*)str;
	while(*current != 0)
	{
        if(*current == 'A')
        {
            display_image(Aaimg, x, y);   
        }
        if(*current == 'a')
        {
            display_image(Abimg, x, y);
        }
        if(*current == 'B')
        {
            display_image(Baimg, x, y);   
        }
        if(*current == 'b')
        {
            display_image(Bbimg, x, y);
        }
        if(*current == 'C')
        {
            display_image(Caimg, x, y);   
        }
        if(*current == 'c')
        {
            display_image(Cbimg, x, y);
        }
        if(*current == 'D')
        {
            display_image(Daimg, x, y);   
        }
        if(*current == 'd')
        {
            display_image(Dbimg, x, y);
        }
        if(*current == 'E')
        {
            display_image(Eaimg, x, y);   
        }
        if(*current == 'e')
        {
            display_image(Ebimg, x, y);
        }
        if(*current == 'F')
        {
            display_image(Faimg, x, y);   
        }
        if(*current == 'f')
        {
            display_image(Fbimg, x, y);
        }
        if(*current == 'G')
        {
            display_image(Gaimg, x, y);   
        }
        if(*current == 'g')
        {
            display_image(Gbimg, x, y);
        }
        if(*current == 'H')
        {
            display_image(Haimg, x, y);   
        }
        if(*current == 'h')
        {
            display_image(Hbimg, x, y);
        }
        if(*current == 'I')
        {
            display_image(Iaimg, x, y);   
        }
        if(*current == 'i')
        {
            display_image(Ibimg, x, y);
        }
        if(*current == 'J')
        {
            display_image(Jaimg, x, y);   
        }
        if(*current == 'j')
        {
            display_image(Jbimg, x, y);
        }
        if(*current == 'K')
        {
            display_image(Kaimg, x, y);   
        }
        if(*current == 'k')
        {
            display_image(Kbimg, x, y);
        }
        if(*current == 'L')
        {
            display_image(Laimg, x, y);   
        }
        if(*current == 'l')
        {
            display_image(Lbimg, x, y);
        }
        if(*current == 'M')
        {
            display_image(Maimg, x, y);   
        }
        if(*current == 'm')
        {
            display_image(Mbimg, x, y);
        }
        if(*current == 'N')
        {
            display_image(Naimg, x, y);   
        }
        if(*current == 'n')
        {
            display_image(Nbimg, x, y);
        }
        if(*current == 'O')
        {
            display_image(Oaimg, x, y);   
        }
        if(*current == 'o')
        {
            display_image(Obimg, x, y);
        }
        if(*current == 'P')
        {
            display_image(Paimg, x, y);   
        }
        if(*current == 'p')
        {
            display_image(Pbimg, x, y);
        }
        if(*current == 'Q')
        {
            display_image(Qaimg, x, y);   
        }
        if(*current == 'q')
        {
            display_image(Qbimg, x, y);
        }
        if(*current == 'R')
        {
            display_image(Raimg, x, y);   
        }
        if(*current == 'r')
        {
            display_image(Rbimg, x, y);
        }
        if(*current == 'S')
        {
            display_image(Saimg, x, y);   
        }
        if(*current == 's')
        {
            display_image(Sbimg, x, y);
        }
        if(*current == 'T')
        {
            display_image(Taimg, x, y);   
        }
        if(*current == 't')
        {
            display_image(Tbimg, x, y);
        }
        if(*current == 'U')
        {
            display_image(Uaimg, x, y);   
        }
        if(*current == 'u')
        {
            display_image(Ubimg, x, y);
        }
        if(*current == 'V')
        {
            display_image(Vaimg, x, y);   
        }
        if(*current == 'v')
        {
            display_image(Vbimg, x, y);
        }
        if(*current == 'W')
        {
            display_image(Waimg, x, y);   
        }
        if(*current == 'w')
        {
            display_image(Wbimg, x, y);
        }
        if(*current == 'X')
        {
            display_image(Xaimg, x, y);   
        }
        if(*current == 'x')
        {
            display_image(Xbimg, x, y);
        }
        if(*current == 'Y')
        {
            display_image(Yaimg, x, y);   
        }
        if(*current == 'y')
        {
            display_image(Ybimg, x, y);
        }
        if(*current == 'Z')
        {
            display_image(Zaimg, x, y);   
        }
        if(*current == 'z')
        {
            display_image(Zbimg, x, y);
        }
        if(*current == '1')
        {
            display_image(Oneimg, x, y);   
        }
        if(*current == '2')
        {
            display_image(Twoimg, x, y);
        }
        if(*current == '3')
        {
            display_image(Threeimg, x, y);   
        }
        if(*current == '4')
        {
            display_image(Fourimg, x, y);
        }
        if(*current == '5')
        {
            display_image(Fiveimg, x, y);   
        }
        if(*current == '6')
        {
            display_image(Siximg, x, y);
        }
        if(*current == '7')
        {
            display_image(Sevenimg, x, y);   
        }
        if(*current == '8')
        {
            display_image(Eightimg, x, y);
        }
        if(*current == '9')
        {
            display_image(Nineimg, x, y);   
        }
        if(*current == '0')
        {
            display_image(Zeroimg, x, y);
        }
        if(*current == ':')
        {
            display_image(Colonimg, x, y);
        }
        if(*current == '/')
        {
            display_image(Slashimg, x, y);
        }
        if(*current == '.')
        {
            display_image(Dotimg, x, y);
        }
        current++;
        x+=20;
	}
}

void print_char(char c, unsigned short x, unsigned short y)
{
    if(c == 'A')
    {
        display_image(Aaimg, x, y);   
    }
    if(c == 'a')
    {
        display_image(Abimg, x, y);
    }
    if(c == 'B')
    {
        display_image(Baimg, x, y);   
    }
    if(c == 'b')
    {
        display_image(Bbimg, x, y);
    }
    if(c == 'C')
    {
        display_image(Caimg, x, y);   
    }
    if(c == 'c')
    {
        display_image(Cbimg, x, y);
    }
    if(c == 'D')
    {
        display_image(Daimg, x, y);   
    }
    if(c == 'd')
    {
        display_image(Dbimg, x, y);
    }
    if(c == 'E')
    {
        display_image(Eaimg, x, y);   
    }
    if(c == 'e')
    {
        display_image(Ebimg, x, y);
    }
    if(c == 'F')
    {
        display_image(Faimg, x, y);   
    }
    if(c == 'f')
    {
        display_image(Fbimg, x, y);
    }
    if(c == 'G')
    {
        display_image(Gaimg, x, y);   
    }
    if(c == 'g')
    {
        display_image(Gbimg, x, y);
    }
    if(c == 'H')
    {
        display_image(Haimg, x, y);   
    }
    if(c == 'h')
    {
        display_image(Hbimg, x, y);
    }
    if(c == 'I')
    {
        display_image(Iaimg, x, y);   
    }
    if(c == 'i')
    {
        display_image(Ibimg, x, y);
    }
    if(c == 'J')
    {
        display_image(Jaimg, x, y);   
    }
    if(c == 'j')
    {
        display_image(Jbimg, x, y);
    }
    if(c == 'K')
    {
        display_image(Kaimg, x, y);   
    }
    if(c == 'k')
    {
        display_image(Kbimg, x, y);
    }
    if(c == 'L')
    {
        display_image(Laimg, x, y);   
    }
    if(c == 'l')
    {
        display_image(Lbimg, x, y);
    }
    if(c == 'M')
    {
        display_image(Maimg, x, y);   
    }
    if(c == 'm')
    {
        display_image(Mbimg, x, y);
    }
    if(c == 'N')
    {
        display_image(Naimg, x, y);   
    }
    if(c == 'n')
    {
        display_image(Nbimg, x, y);
    }
    if(c == 'O')
    {
        display_image(Oaimg, x, y);   
    }
    if(c == 'o')
    {
        display_image(Obimg, x, y);
    }
    if(c == 'P')
    {
        display_image(Paimg, x, y);   
    }
    if(c == 'p')
    {
        display_image(Pbimg, x, y);
    }
    if(c == 'Q')
    {
        display_image(Qaimg, x, y);   
    }
    if(c == 'q')
    {
        display_image(Qbimg, x, y);
    }
    if(c == 'R')
    {
        display_image(Raimg, x, y);   
    }
    if(c == 'r')
    {
        display_image(Rbimg, x, y);
    }
    if(c == 'S')
    {
        display_image(Saimg, x, y);   
    }
    if(c == 's')
    {
        display_image(Sbimg, x, y);
    }
    if(c == 'T')
    {
        display_image(Taimg, x, y);   
    }
    if(c == 't')
    {
        display_image(Tbimg, x, y);
    }
    if(c == 'U')
    {
        display_image(Uaimg, x, y);   
    }
    if(c == 'u')
    {
        display_image(Ubimg, x, y);
    }
    if(c == 'V')
    {
        display_image(Vaimg, x, y);   
    }
    if(c == 'v')
    {
        display_image(Vbimg, x, y);
    }
    if(c == 'W')
    {
        display_image(Waimg, x, y);   
    }
    if(c == 'w')
    {
        display_image(Wbimg, x, y);
    }
    if(c == 'X')
    {
        display_image(Xaimg, x, y);   
    }
    if(c == 'x')
    {
        display_image(Xbimg, x, y);
    }
    if(c == 'Y')
    {
        display_image(Yaimg, x, y);   
    }
    if(c == 'y')
    {
        display_image(Ybimg, x, y);
    }
    if(c == 'Z')
    {
        display_image(Zaimg, x, y);   
    }
    if(c == 'z')
    {
        display_image(Zbimg, x, y);
    }
    if(c == '1')
    {
        display_image(Oneimg, x, y);   
    }
    if(c == '2')
    {
        display_image(Twoimg, x, y);
    }
    if(c == '3')
    {
        display_image(Threeimg, x, y);   
    }
    if(c == '4')
    {
        display_image(Fourimg, x, y);
    }
    if(c == '5')
    {
        display_image(Fiveimg, x, y);   
    }
    if(c == '6')
    {
        display_image(Siximg, x, y);
    }
    if(c == '7')
    {
        display_image(Sevenimg, x, y);   
    }
    if(c == '8')
    {
        display_image(Eightimg, x, y);
    }
    if(c == '9')
    {
        display_image(Nineimg, x, y);   
    }
    if(c == '0')
    {
        display_image(Zeroimg, x, y);
    }
    if(c == ':')
    {
        display_image(Colonimg, x, y);
    }
    if(c == '/')
    {
        display_image(Slashimg, x, y);
    }
    if(c == '.')
    {
        display_image(Dotimg, x, y);
    }
}