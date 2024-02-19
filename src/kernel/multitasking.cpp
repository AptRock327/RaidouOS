#include <kernel/multitasking.h>

#define MAX_SYS_PROCESSES 128 //modifies the maximum amount of processes RaidouOS can run simultaneously

unsigned long long CPID; //current PID
unsigned long long lastCPID;

Process* process_list;

bool turned_on;
bool first_task;

Process::Process(unsigned long long DPID, unsigned long long entry)
{
    this->PID = DPID;
    this->pcontext.rip = entry;
    this->pcontext.cs = 0x8;
    this->pcontext.rflags = 0x202;
}

void InitMultitasking()
{
    process_list = (Process*)calloc(MAX_SYS_PROCESSES*sizeof(Process));
    CPID = 1;
    first_task = true;
    Process pdesktop(1, (unsigned long long)desktop);
    process_list[pdesktop.PID] = pdesktop;
    Process pterminal(2, (unsigned long long)terminal);
    process_list[pterminal.PID] = pterminal;
}

extern "C" void isr0_handler(Context current_context)
{
    if(!first_task)
    {
        process_list[CPID-1].pcontext = current_context;
    }
    else
    {
        if(turned_on) process_list[lastCPID].pcontext = current_context;
        else turned_on = true;
        first_task = false;
    }
    current_context = process_list[CPID].pcontext;
    if(process_list[CPID+1].PID)
    {
        CPID++;
    }
    else
    {
        lastCPID = CPID;
        CPID = 1;
        first_task = true;
    }
    outb(0x20, 0x20);
    outb(0xa0, 0x20);
}