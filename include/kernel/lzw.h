#pragma once
#include <kernel/memory.h>
#include <drivers/graphics.h>
#include <drivers/pit.h>
#include <string.h>

unsigned char* lzw_decompress(unsigned short* input);