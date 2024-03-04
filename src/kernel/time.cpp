#include <kernel/time.h>

TimeAndDate translate_unix_timestamp(unsigned long long timestamp) //not entirely precise
{
    TimeAndDate result;
    unsigned long long rest = timestamp;
    result.year = (unsigned long long)(timestamp/31556926)+1970;
    rest -= (unsigned long long)(timestamp/31556926)*31556926;
    result.month = (unsigned long long)(timestamp/2629743);
    rest -= (unsigned long long)(timestamp/2629743)*2629743;
    result.day = (unsigned long long)(timestamp/86400);
    rest -= (unsigned long long)(timestamp/86400)*86400;
    result.hour = (unsigned long long)(timestamp/3600);
    rest -= (unsigned long long)(timestamp/3600)*3600;
    result.minute = (unsigned long long)(timestamp/60);
    rest -= (unsigned long long)(timestamp/60)*60;
    result.second = rest;
    return result;
}