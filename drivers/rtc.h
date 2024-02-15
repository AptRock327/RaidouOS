#pragma once
#include "../kernel/IO.h"

unsigned char rtc_seconds();

unsigned char rtc_minutes();

unsigned char rtc_hour();

unsigned char rtc_correct_seconds();

unsigned char rtc_correct_minutes();

unsigned char rtc_correct_hour();

void rtc_wait_second();