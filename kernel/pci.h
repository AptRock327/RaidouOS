#pragma once
#include "IO.h"
#include "../drivers/graphics.h"
#include "../drivers/rtc.h"

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

unsigned int read_pci(unsigned short bus, unsigned short device, unsigned short function, unsigned int registeroffset)
{
    unsigned int id =
        0x1 << 31
        | ((bus & 0xFF) << 16)
        | ((device & 0x1F) << 11)
        | ((function & 0x07) << 8)
        | (registeroffset & 0xFC);
    outl(0xCF8, id);
    unsigned int result = inl(0xCFC);
    return result >> (8* (registeroffset % 4));
}

void write_pci(unsigned short bus, unsigned short device, unsigned short function, unsigned int registeroffset, unsigned int value)
{
    unsigned int id =
        0x1 << 31
        | ((bus & 0xFF) << 16)
        | ((device & 0x1F) << 11)
        | ((function & 0x07) << 8)
        | (registeroffset & 0xFC);
    outl(0xCF8, id);
    outl(0xCFC, value);
}

bool multifunction(unsigned short bus, unsigned short device)
{
    return read_pci(bus, device, 0, 0x0E) & (1<<7);
}

device_descriptor get_device_descriptor(unsigned short bus, unsigned short device, unsigned short function)
{
    device_descriptor result;

    result.bus = bus;
    result.device = device;
    result.function = function;

    result.vendor_id = read_pci(bus, device, function, 0x00);
    result.device_id = read_pci(bus, device, function, 0x02);

    result.class_id = read_pci(bus, device, function, 0x0b);
    result.subclass_id = read_pci(bus, device, function, 0x0a);
    result.interface_id = read_pci(bus, device, function, 0x09);

    result.revision = read_pci(bus, device, function, 0x08);
    result.interrupt = read_pci(bus, device, function, 0x3c);

    return result;
}

char hexToStringOutput[128];
template<typename T>
const char* HexToString(T value)
{
    T* valPtr = &value;
    unsigned char* ptr;
    unsigned char temp;
    unsigned char size = (sizeof(T)) * 2 - 1;
    unsigned char i;
    for(i = 0; i < size; i++)
    {
    ptr = ((unsigned char*)valPtr + i);
    temp = ((*ptr & 0xF0) >> 4);
    hexToStringOutput[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : 48);
    temp = ((*ptr & 0x0F));
    hexToStringOutput[size - (i * 2 + 0)] = temp + (temp > 9 ? 55 : 48);
    }
    hexToStringOutput[size + 1] = 0;
    return hexToStringOutput;
}

void output_device_list()
{
    unsigned char function_index = 1;
    print("LISTING AVAILABLE PCI/PCIe DEVICES AND THEIR FUNCTIONS", 0, 0);
    #ifdef cool_sequence
        SwapBuffers();
        rtc_wait_second();
        rtc_wait_second();
        rtc_wait_second();
    #endif
    for(int bus = 0; bus < 8; bus++)
    {
        for(int device = 0; device < 32; device++)
        {
            int numFunctions = multifunction(bus, device) ? 8 : 1;
            for(int function = 0; function < numFunctions; function++)
            {
                device_descriptor dev = get_device_descriptor(bus, device, function);

                if(dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF)
                    continue;

                print("PCI bus: ", 0, function_index*50);
                print(HexToString<unsigned char>((char)(bus & 0xFF)), 180, function_index*50);

                print(" Device: ", 220, function_index*50);
                print(HexToString((char)(device & 0xFF)), 400, function_index*50);

                print(" ID: ", 440, function_index*50);
                print(HexToString((char)((dev.device_id & 0xFF00) >> 8)), 540, function_index*50);
                print(HexToString((char)(dev.device_id & 0xFF)), 580, function_index*50);

                print(" Function: ", 620, function_index*50);
                print(HexToString((char)((function & 0xFF))), 840, function_index*50);
                function_index++;
            }
        }
    }
}