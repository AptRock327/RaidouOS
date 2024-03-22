#include <kernel/multitasking.h>

#define MAX_SYS_PROCESSES 128 //modifies the maximum amount of processes RaidouOS can run simultaneously
#define MAX_SYS_THREADS 32 //modifies the maximum amount of threads every process can have

unsigned long long UNIVERSAL_ELAPSED_TIME = 0;

unsigned long long CPID; //current PID
unsigned long long lastCPID;

Process* process_list;

bool turned_on;
bool first_task;

Process::Process(unsigned long long DPID, unsigned long long* entries)
{
    this->PID = DPID;
    this->threads = (unsigned long long*)calloc(MAX_SYS_THREADS * sizeof(unsigned long long));
    unsigned long long i = 0;
    while(entries[i] != 0)
    {
        this->threads[i] = entries[i];
        i++;
    }
    this->cthread = 0;
    this->pcontext.rip = threads[0];
    this->pcontext.cs = 0x8;
    this->pcontext.rflags = 0x202;
}

void InitMultitasking()
{
    process_list = (Process*)calloc(MAX_SYS_PROCESSES*sizeof(Process));
    CPID = 1;
    first_task = true;
    unsigned long long* DesktopEntries = (unsigned long long*)calloc(16);
    DesktopEntries[0] = (unsigned long long)desktop;
    Process pdesktop(1, DesktopEntries);
    process_list[pdesktop.PID] = pdesktop;
    unsigned long long* TerminalEntries = (unsigned long long*)calloc(24);
    TerminalEntries[0] = (unsigned long long)terminal;
    TerminalEntries[1] = (unsigned long long)terminal_audio;
    Process pterminal(2, TerminalEntries);
    process_list[pterminal.PID] = pterminal;
    unsigned long long* EditorEntries = (unsigned long long*)calloc(16);
    EditorEntries[0] = (unsigned long long)editor;
    Process peditor(3, EditorEntries);
    process_list[peditor.PID] = peditor;
}

//SCHEDULER
extern "C" void isr0_handler(Context current_context)
{
    //SCHEDULE PROCESSES
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

    //SCHEDULE THREADS
    if(process_list[CPID].threads[process_list[CPID].cthread + 1])
    {
        process_list[CPID].threads[process_list[CPID].cthread] = process_list[CPID].pcontext.rip;
        process_list[CPID].cthread++;
    }
    else
    {
        process_list[CPID].threads[process_list[CPID].cthread] = process_list[CPID].pcontext.rip;
        process_list[CPID].cthread = 0;
    }
    process_list[CPID].pcontext.rip = process_list[CPID].threads[process_list[CPID].cthread];

    //FINALIZATION
    UNIVERSAL_ELAPSED_TIME+=40;
    outb(0x20, 0x20);
    outb(0xa0, 0x20);
}