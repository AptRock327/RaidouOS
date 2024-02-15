#define NULL 0
#include "graphics.h"

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
unsigned int* Sbralimg;
unsigned int* Sbrarimg;

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
    Sbralimg = tga_parse((unsigned char*)read("/font/sbral.tga"), 1);
    Sbrarimg = tga_parse((unsigned char*)read("/font/sbrar.tga"), 1);
}

void print_char(char c, unsigned short x, unsigned short y)
{
    switch(c)
    {
        case 'A':
            display_image(Aaimg, x, y);   
            break;
        case 'a':
            display_image(Abimg, x, y);
            break;
        case 'B':
            display_image(Baimg, x, y);   
            break;
        case 'b':
            display_image(Bbimg, x, y);
            break;
        case 'C':
            display_image(Caimg, x, y);   
            break;
        case 'c':
            display_image(Cbimg, x, y);
            break;
        case 'D':
            display_image(Daimg, x, y);   
            break;
        case 'd':
            display_image(Dbimg, x, y);
            break;
        case 'E':
            display_image(Eaimg, x, y);   
            break;
        case 'e':
            display_image(Ebimg, x, y);
            break;
        case 'F':
            display_image(Faimg, x, y);   
            break;
        case 'f':
            display_image(Fbimg, x, y);
            break;
        case 'G':
            display_image(Gaimg, x, y);   
            break;
        case 'g':
            display_image(Gbimg, x, y);
            break;
        case 'H':
            display_image(Haimg, x, y);   
            break;
        case 'h':
            display_image(Hbimg, x, y);
            break;
        case 'I':
            display_image(Iaimg, x, y);   
            break;
        case 'i':
            display_image(Ibimg, x, y);
            break;
        case 'J':
            display_image(Jaimg, x, y);   
            break;
        case 'j':
            display_image(Jbimg, x, y);
            break;
        case 'K':
            display_image(Kaimg, x, y);   
            break;
        case 'k':
            display_image(Kbimg, x, y);
            break;
        case 'L':
            display_image(Laimg, x, y);
            break; 
        case 'l':
            display_image(Lbimg, x, y);
            break;
        case 'M':
            display_image(Maimg, x, y);
            break;
        case 'm':
            display_image(Mbimg, x, y);
            break;
        case 'N':
            display_image(Naimg, x, y);
            break;
        case 'n':
            display_image(Nbimg, x, y);
            break;
        case 'O':
            display_image(Oaimg, x, y);
            break;
        case 'o':
            display_image(Obimg, x, y);
            break;
        case 'P':
            display_image(Paimg, x, y);
            break;
        case 'p':
            display_image(Pbimg, x, y);
            break;
        case 'Q':
            display_image(Qaimg, x, y);
            break;
        case 'q':
            display_image(Qbimg, x, y);
            break;
        case 'R':
            display_image(Raimg, x, y);   
            break;
        case 'r':
            display_image(Rbimg, x, y);
            break;
        case 'S':
            display_image(Saimg, x, y);
            break;
        case 's':
            display_image(Sbimg, x, y);
            break;
        case 'T':
            display_image(Taimg, x, y);
            break;
        case 't':
            display_image(Tbimg, x, y);
            break;
        case 'U':
            display_image(Uaimg, x, y);   
            break;
        case 'u':
            display_image(Ubimg, x, y);
            break;
        case 'V':
            display_image(Vaimg, x, y);   
            break;
        case 'v':
            display_image(Vbimg, x, y);
            break;
        case 'W':
            display_image(Waimg, x, y);   
            break;
        case 'w':
            display_image(Wbimg, x, y);
            break;
        case 'X':
            display_image(Xaimg, x, y);   
            break;
        case 'x':
            display_image(Xbimg, x, y);
            break;
        case 'Y':
            display_image(Yaimg, x, y);   
            break;
        case 'y':
            display_image(Ybimg, x, y);
            break;
        case 'Z':
            display_image(Zaimg, x, y);   
            break;
        case 'z':
            display_image(Zbimg, x, y);
            break;
        case '1':
            display_image(Oneimg, x, y);   
            break;
        case '2':
            display_image(Twoimg, x, y);
            break;
        case '3':
            display_image(Threeimg, x, y);
            break;
        case '4':
            display_image(Fourimg, x, y);
            break;
        case '5':
            display_image(Fiveimg, x, y);   
            break;
        case '6':
            display_image(Siximg, x, y);
            break;
        case '7':
            display_image(Sevenimg, x, y);   
            break;
        case '8':
            display_image(Eightimg, x, y);
            break;
        case '9':
            display_image(Nineimg, x, y);   
            break;
        case '0':
            display_image(Zeroimg, x, y);
            break;
        case ':':
            display_image(Colonimg, x, y);
            break;
        case '/':
            display_image(Slashimg, x, y);
            break;
        case '.':
            display_image(Dotimg, x, y);
            break;
        case '[':
            display_image(Sbralimg, x, y);
            break;
        case ']':
            display_image(Sbrarimg, x, y);
            break;
        default:
            break;
    }
}

void print(const char* str, unsigned short x, unsigned short y)
{
    unsigned char* current = (unsigned char*)str;
	while(*current != 0)
	{
        print_char(*current, x, y);
        current++;
        x+=20;
	}
}