#include "multitasking.h"

bool task_state;

Task desktop_task;
Task terminal_task;

bool turned_on;

void InitMultitasking()
{
    desktop_task.rip = (unsigned long long)desktop;
    desktop_task.cs = 0x8;
    desktop_task.rflags = 0x202;
    terminal_task.rip = (unsigned long long)terminal;
    terminal_task.cs = 0x8;
    terminal_task.rflags = 0x202;
}

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