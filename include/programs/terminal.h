#pragma once
#include <drivers/graphics.h>
#include <kernel/filesystem.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <kernel/pci.h>
#include <drivers/rtc.h>
#include <drivers/pit.h>
#include <drivers/pcspk.h>
#include <kernel/memory.h>
#include <string.h>

void terminal_audio();
void terminal();