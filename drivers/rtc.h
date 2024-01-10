#pragma once
#include "../kernel/IO.h"

unsigned char rtc_seconds()
{
    outb(0x70, 1 << 7);
    return inb(0x71);
}

unsigned char rtc_minutes()
{
    outb(0x70, 0b10000010);
    return inb(0x71);
}

unsigned char rtc_hour()
{
    outb(0x70, 0b10000100);
    return inb(0x71);
}

unsigned char rtc_correct_seconds()
{
    unsigned char sec = rtc_seconds();
    if(rtc_seconds() >= 16) sec-=6;
    if(rtc_seconds() >= 32) sec-=6;
    if(rtc_seconds() >= 48) sec-=6;
    if(rtc_seconds() >= 64) sec-=6;
    if(rtc_seconds() >= 80) sec-=6;
    return sec;
}

unsigned char rtc_correct_minutes()
{
    unsigned char min = rtc_minutes();
    if(rtc_minutes() >= 16) min-=6;
    if(rtc_minutes() >= 32) min-=6;
    if(rtc_minutes() >= 48) min-=6;
    if(rtc_minutes() >= 64) min-=6;
    if(rtc_minutes() >= 80) min-=6;
    return min;
}

unsigned char rtc_correct_hour()
{
    unsigned char h = rtc_hour();
    if(rtc_hour() >= 16) h-=6;
    if(rtc_hour() >= 32) h-=6;
    return h;
}

void rtc_wait_second()
{
    unsigned char seconds = rtc_seconds();
    while(seconds == rtc_seconds());
    return;
}