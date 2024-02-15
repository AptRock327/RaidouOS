#pragma once
#include <kernel/IO.h>
#include <drivers/graphics.h>
#include <drivers/rtc.h>

class device_descriptor
{
public:
    unsigned int portBase;
    unsigned int interrupt;

    unsigned short bus;
    unsigned short device;
    unsigned short function;

    unsigned short vendor_id;
    unsigned short device_id;

    unsigned char class_id;
    unsigned char subclass_id;
    unsigned char interface_id;

    unsigned char revision;
};

unsigned int read_pci(unsigned short bus, unsigned short device, unsigned short function, unsigned int registeroffset);

void write_pci(unsigned short bus, unsigned short device, unsigned short function, unsigned int registeroffset, unsigned int value);

bool multifunction(unsigned short bus, unsigned short device);

device_descriptor get_device_descriptor(unsigned short bus, unsigned short device, unsigned short function);

void output_device_list();