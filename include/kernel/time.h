struct TimeAndDate
{
    unsigned short year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
};

TimeAndDate translate_unix_timestamp(unsigned long long timestamp);