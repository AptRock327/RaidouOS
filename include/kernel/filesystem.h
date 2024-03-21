#pragma once
#include <kernel/memory.h>
#include <string.h>

class File
{
    public:
        const char* path;
        char* address;

        File(const char* pa, char* ad);

        char* readFile();
};

char* read(const char* ph);

bool fschk(const char* ph); //checks for the presence of a file in the filesystem

void foverride(const char* ph, const char* filebuffer);

void write(const char* ph, const char* filebuffer);

void InitFs();