#pragma once
#include "../kernel/IO.h"
#include "graphics.h"

struct MouseState
{
    bool mouse_input;
    bool left_released;
    bool left_held;
    bool right_released;
    bool right_held;
    char deltax;
    char deltay;
    unsigned short absolute_x = 0;
    unsigned short absolute_y = 0;
};

void InitMouse();

extern "C" void isr12_handler();