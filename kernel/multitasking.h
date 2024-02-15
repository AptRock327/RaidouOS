#pragma once
#include "IO.h"
#include "../drivers/graphics.h"
#include "../drivers/mouse.h"
#include "filesystem.h"
#include "../programs/desktop.h"
#include "../programs/terminal.h"

struct Task
{
    unsigned long long r15;
    unsigned long long r14;
    unsigned long long r13;
    unsigned long long r12;
    unsigned long long r11;
    unsigned long long r10;
    unsigned long long r9;
    unsigned long long r8;
    unsigned long long rdi;
    unsigned long long rsi;
    unsigned long long rbp;
    unsigned long long rdx;
    unsigned long long rcx;
    unsigned long long rbx;
    unsigned long long rax;
    unsigned long long rip;
    unsigned long long cs;
    unsigned long long rflags;
};

void InitMultitasking();

extern "C" void isr0_handler(Task current_task);