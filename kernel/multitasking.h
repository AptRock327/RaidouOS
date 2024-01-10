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

bool task_state;

Task desktop_task;
Task terminal_task;

extern MouseState MouseStateGlobal;

void InitMultitasking()
{
    desktop_task.rip = (unsigned long long)desktop;
    desktop_task.cs = 0x8;
    desktop_task.rflags = 0x202;
    terminal_task.rip = (unsigned long long)terminal;
    terminal_task.cs = 0x8;
    terminal_task.rflags = 0x202;
}

bool turned_on;

extern "C" void isr0_handler(Task current_task)
{
    if(task_state)
    {
        desktop_task = current_task;
        current_task = terminal_task;
    }
    else
    {
        if(turned_on) terminal_task = current_task;
        else turned_on = true;
        current_task = desktop_task;
    }
    task_state = !task_state;
    outb(0x20, 0x20);
    outb(0xa0, 0x20);
}