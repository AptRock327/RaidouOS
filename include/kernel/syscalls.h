#pragma once
#include <kernel/IO.h>
#include <kernel/multitasking.h>
#include <drivers/mouse.h>
#include <drivers/graphics.h>
#include <programs/terminal.h>
#include <kernel/filesystem.h>

extern "C" void isr128_handler(Context state);