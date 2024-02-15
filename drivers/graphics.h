#pragma once
#define NULL 0
#include "../kernel/memory.h"
#include "../kernel/filesystem.h"

void InitDoubleBuffering();

void putpixel(unsigned short x, unsigned short y, unsigned char red, unsigned char green, unsigned char blue);

unsigned int *tga_parse(unsigned char *ptr, int size);

void display_image(unsigned int* image_data, int x, int y);

void clear_screen(char red, char green, char blue);

void SwapBuffers();

void InitFont();

void print_char(char c, unsigned short x, unsigned short y);

void print(const char* str, unsigned short x, unsigned short y);