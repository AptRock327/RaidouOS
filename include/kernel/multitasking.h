#pragma once
#include <kernel/IO.h>
#include <drivers/graphics.h>
#include <drivers/mouse.h>
#include <kernel/filesystem.h>
#include <programs/desktop.h>
#include <programs/terminal.h>
#include <programs/editor.h>

struct Context
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

class Process
{
    public:
        unsigned long long PID;
        Context pcontext; //process context
        unsigned long long* threads; //thread entry points
        unsigned char cthread; //current thread

        Process(unsigned long long DPID, unsigned long long* entries); //desired PID and entry points for the code
};

void InitMultitasking();

extern "C" void isr0_handler(Context current_context);