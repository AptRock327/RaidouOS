#include <drivers/pit.h>

extern unsigned long long UNIVERSAL_ELAPSED_TIME;

void InitPIT(unsigned int hz)
{
    unsigned int divisor = (unsigned int)((unsigned int)1193180 / (unsigned int)hz); //calculate divisor based on PIT frequency
    outb(0x43, 0x36); //set command to frequency setting
    outb(0x40, divisor & 0xFF); //set low byte
    outb(0x40, divisor >> 8); //set high byte
}

void sleep(unsigned long long ms)
{
    unsigned long long tmp = UNIVERSAL_ELAPSED_TIME;
    while((UNIVERSAL_ELAPSED_TIME-tmp) < ms);
}