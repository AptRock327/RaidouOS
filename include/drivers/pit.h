#include <kernel/IO.h>
#include <kernel/multitasking.h>
#include <drivers/graphics.h>

void InitPIT(unsigned int hz);

void sleep(unsigned long long ms);