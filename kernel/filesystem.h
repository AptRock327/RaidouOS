#pragma once
#include "memory.h"

extern unsigned char CursorImage[];
extern unsigned char logo[];
extern unsigned char MiniLogo[];
extern unsigned char TerminalLogo[];
extern unsigned char Close[];
extern unsigned char HelloWorld[];
extern unsigned char Dot[];

//font start
extern unsigned char Aa[];
extern unsigned char Ab[];
extern unsigned char Ba[];
extern unsigned char Bb[];
extern unsigned char Ca[];
extern unsigned char Cb[];
extern unsigned char Da[];
extern unsigned char Dbb[];
extern unsigned char Ea[];
extern unsigned char Eb[];
extern unsigned char Fa[];
extern unsigned char Fb[];
extern unsigned char Ga[];
extern unsigned char Gb[];
extern unsigned char Ha[];
extern unsigned char Hb[];
extern unsigned char Ia[];
extern unsigned char Ib[];
extern unsigned char Jaa[];
extern unsigned char Jbb[];
extern unsigned char Ka[];
extern unsigned char Kb[];
extern unsigned char La[];
extern unsigned char Lb[];
extern unsigned char Ma[];
extern unsigned char Mb[];
extern unsigned char Na[];
extern unsigned char Nb[];
extern unsigned char Oa[];
extern unsigned char Ob[];
extern unsigned char Pa[];
extern unsigned char Pb[];
extern unsigned char Qa[];
extern unsigned char Qb[];
extern unsigned char Ra[];
extern unsigned char Rb[];
extern unsigned char Sa[];
extern unsigned char Sb[];
extern unsigned char Ta[];
extern unsigned char Tb[];
extern unsigned char Ua[];
extern unsigned char Ub[];
extern unsigned char Va[];
extern unsigned char Vb[];
extern unsigned char Wa[];
extern unsigned char Wb[];
extern unsigned char Xa[];
extern unsigned char Xb[];
extern unsigned char Ya[];
extern unsigned char Yb[];
extern unsigned char Za[];
extern unsigned char Zb[];
extern unsigned char one[];
extern unsigned char two[];
extern unsigned char three[];
extern unsigned char four[];
extern unsigned char five[];
extern unsigned char six[];
extern unsigned char seven[];
extern unsigned char eight[];
extern unsigned char nine[];
extern unsigned char colon[];
extern unsigned char zero[];
extern unsigned char slash[];
//font end

class File
{
    public:
        const char* path;
        char* address;

        File(const char* pa, char* ad)
        {
            this->path = pa;
            this->address = ad;
        }

        char* readFile()
        {
            return this->address;
        }


};

File* FileTable = (File*)calloc(70000);

unsigned long long strlen(const char* str)
{
    unsigned long long len;
    for (len = 0; str[len] != 0; len++);
    return len;
}

bool strcmp(const char* str1, const char* str2)
{
    unsigned long long len = strlen(str1);

    if (strlen(str2) != len)
        return false;

    for (unsigned long long i = 0; i < len; i++)
        if (str1[i] != str2[i])
            return false;

    return true;
}

char* read(const char* ph)
{
    for(int i = 0; i < 71; i++)
    {
        if(strcmp(FileTable[i].path, ph))
        {
            return FileTable[i].readFile();
        }
    }
    return 0;
}

void InitFs()
{
    File cursor((const char*)"/img/cursor.tga", (char*)CursorImage);
    FileTable[0] = cursor;
    File Aafile((const char*)"/font/Aa.tga", (char*)Aa);
    FileTable[1] = Aafile;
    File Abfile((const char*)"/font/Ab.tga", (char*)Ab);
    FileTable[2] = Abfile;
    File Bafile((const char*)"/font/Ba.tga", (char*)Ba);
    FileTable[3] = Bafile;
    File Bbfile((const char*)"/font/Bb.tga", (char*)Bb);
    FileTable[4] = Bbfile;
    File Cafile((const char*)"/font/Ca.tga", (char*)Ca);
    FileTable[5] = Cafile;
    File Cbfile((const char*)"/font/Cb.tga", (char*)Cb);
    FileTable[6] = Cbfile;
    File Dafile((const char*)"/font/Da.tga", (char*)Da);
    FileTable[7] = Dafile;
    File Dbfile((const char*)"/font/Db.tga", (char*)Dbb);
    FileTable[8] = Dbfile;
    File Eafile((const char*)"/font/Ea.tga", (char*)Ea);
    FileTable[9] = Eafile;
    File Ebfile((const char*)"/font/Eb.tga", (char*)Eb);
    FileTable[10] = Ebfile;
    File Fafile((const char*)"/font/Fa.tga", (char*)Fa);
    FileTable[11] = Fafile;
    File Fbfile((const char*)"/font/Fb.tga", (char*)Fb);
    FileTable[12] = Fbfile;
    File Gafile((const char*)"/font/Ga.tga", (char*)Ga);
    FileTable[13] = Gafile;
    File Gbfile((const char*)"/font/Gb.tga", (char*)Gb);
    FileTable[14] = Gbfile;
    File Hafile((const char*)"/font/Ha.tga", (char*)Ha);
    FileTable[15] = Hafile;
    File Hbfile((const char*)"/font/Hb.tga", (char*)Hb);
    FileTable[16] = Hbfile;
    File Iafile((const char*)"/font/Ia.tga", (char*)Ia);
    FileTable[17] = Iafile;
    File Ibfile((const char*)"/font/Ib.tga", (char*)Ib);
    FileTable[18] = Ibfile;
    File Jafile((const char*)"/font/Ja.tga", (char*)Jaa);
    FileTable[19] = Jafile;
    File Jbfile((const char*)"/font/Jb.tga", (char*)Jbb);
    FileTable[20] = Jbfile;
    File Kafile((const char*)"/font/Ka.tga", (char*)Ka);
    FileTable[21] = Kafile;
    File Kbfile((const char*)"/font/Kb.tga", (char*)Kb);
    FileTable[22] = Kbfile;
    File Lafile((const char*)"/font/La.tga", (char*)La);
    FileTable[23] = Lafile;
    File Lbfile((const char*)"/font/Lb.tga", (char*)Lb);
    FileTable[24] = Lbfile;
    File Mafile((const char*)"/font/Ma.tga", (char*)Ma);
    FileTable[25] = Mafile;
    File Mbfile((const char*)"/font/Mb.tga", (char*)Mb);
    FileTable[26] = Mbfile;
    File Nafile((const char*)"/font/Na.tga", (char*)Na);
    FileTable[27] = Nafile;
    File Nbfile((const char*)"/font/Nb.tga", (char*)Nb);
    FileTable[28] = Nbfile;
    File Oafile((const char*)"/font/Oa.tga", (char*)Oa);
    FileTable[29] = Oafile;
    File Obfile((const char*)"/font/Ob.tga", (char*)Ob);
    FileTable[30] = Obfile;
    File Pafile((const char*)"/font/Pa.tga", (char*)Pa);
    FileTable[31] = Pafile;
    File Pbfile((const char*)"/font/Pb.tga", (char*)Pb);
    FileTable[32] = Pbfile;
    File Qafile((const char*)"/font/Qa.tga", (char*)Qa);
    FileTable[33] = Qafile;
    File Qbfile((const char*)"/font/Qb.tga", (char*)Qb);
    FileTable[34] = Qbfile;
    File Rafile((const char*)"/font/Ra.tga", (char*)Ra);
    FileTable[35] = Rafile;
    File Rbfile((const char*)"/font/Rb.tga", (char*)Rb);
    FileTable[36] = Rbfile;
    File Safile((const char*)"/font/Sa.tga", (char*)Sa);
    FileTable[37] = Safile;
    File Sbfile((const char*)"/font/Sb.tga", (char*)Sb);
    FileTable[38] = Sbfile;
    File Tafile((const char*)"/font/Ta.tga", (char*)Ta);
    FileTable[39] = Tafile;
    File Tbfile((const char*)"/font/Tb.tga", (char*)Tb);
    FileTable[40] = Tbfile;
    File Uafile((const char*)"/font/Ua.tga", (char*)Ua);
    FileTable[41] = Uafile;
    File Ubfile((const char*)"/font/Ub.tga", (char*)Ub);
    FileTable[42] = Ubfile;
    File Vafile((const char*)"/font/Va.tga", (char*)Va);
    FileTable[43] = Vafile;
    File Vbfile((const char*)"/font/Vb.tga", (char*)Vb);
    FileTable[44] = Vbfile;
    File Wafile((const char*)"/font/Wa.tga", (char*)Wa);
    FileTable[45] = Wafile;
    File Wbfile((const char*)"/font/Wb.tga", (char*)Wb);
    FileTable[46] = Wbfile;
    File Xafile((const char*)"/font/Xa.tga", (char*)Xa);
    FileTable[47] = Xafile;
    File Xbfile((const char*)"/font/Xb.tga", (char*)Xb);
    FileTable[48] = Xbfile;
    File Yafile((const char*)"/font/Ya.tga", (char*)Ya);
    FileTable[49] = Yafile;
    File Ybfile((const char*)"/font/Yb.tga", (char*)Yb);
    FileTable[50] = Ybfile;
    File Zafile((const char*)"/font/Za.tga", (char*)Za);
    FileTable[51] = Zafile;
    File Zbfile((const char*)"/font/Zb.tga", (char*)Zb);
    FileTable[52] = Zbfile;
    File Onefile((const char*)"/font/one.tga", (char*)one);
    FileTable[53] = Onefile;
    File Twofile((const char*)"/font/two.tga", (char*)two);
    FileTable[54] = Twofile;
    File Threefile((const char*)"/font/three.tga", (char*)three);
    FileTable[55] = Threefile;
    File Fourfile((const char*)"/font/four.tga", (char*)four);
    FileTable[56] = Fourfile;
    File Fivefile((const char*)"/font/five.tga", (char*)five);
    FileTable[57] = Fivefile;
    File Sixfile((const char*)"/font/six.tga", (char*)six);
    FileTable[58] = Sixfile;
    File Sevenfile((const char*)"/font/seven.tga", (char*)seven);
    FileTable[59] = Sevenfile;
    File Eightfile((const char*)"/font/eight.tga", (char*)eight);
    FileTable[60] = Eightfile;
    File Ninefile((const char*)"/font/nine.tga", (char*)nine);
    FileTable[61] = Ninefile;
    File Colonfile((const char*)"/font/colon.tga", (char*)colon);
    FileTable[62] = Colonfile;
    File Zerofile((const char*)"/font/zero.tga", (char*)zero);
    FileTable[63] = Zerofile;
    File Slashfile((const char*)"/font/slash.tga", (char*)slash);
    FileTable[64] = Slashfile;
    File Logofile((const char*)"/img/logo.tga", (char*)logo);
    FileTable[65] = Logofile;
    File MiniLogofile((const char*)"/img/mini_logo.tga", (char*)MiniLogo);
    FileTable[66] = MiniLogofile;
    File Terminalfile((const char*)"/img/terminal.tga", (char*)TerminalLogo);
    FileTable[67] = Terminalfile;
    File Closefile((const char*)"/img/close.tga", (char*)Close);
    FileTable[68] = Closefile;
    File HelloWorldfile((const char*)"/programs/hw.bf", (char*)HelloWorld);
    FileTable[69] = HelloWorldfile;
    File Dotfile((const char*)"/font/dot.tga", (char*)Dot);
    FileTable[70] = Dotfile;
}