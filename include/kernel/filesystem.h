#pragma once
#include <kernel/memory.h>

class File
{
    public:
        const char* path;
        char* address;

        File(const char* pa, char* ad);

        char* readFile();
};

unsigned long long strlen(const char* str);

bool strcmp(const char* str1, const char* str2);

char* read(const char* ph);

bool fschk(const char* ph); //checks for the presence of a file in the filesystem

void foverride(const char* ph, const char* filebuffer);

void write(const char* ph, const char* filebuffer);

void InitFs();