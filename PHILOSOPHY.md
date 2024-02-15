# The philosophy of RaidouOS

If you have just randomly stumbled into this project, you might notice that this operating system is strange even on a conceptual level. Hence, I want to clear up some things related to its development philosophy.

## Purpose

RaidouOS is **not** an OS meant to be used by the average user. It's primarily a personal project that I enjoy working on. If it will be used by others, it will be used for *fun*. This either means playing around with it in a virtual machine or creating programs. In short, this OS is a ***toy***.

## Running in kernel mode

Every single line of code is meant to run in ring 0. Think TempleOS. Because of the purpose of the OS, protecting everything using x86's ring 3 is pointless. The programs **should** be able to access all of the OS and hardware resources.

## System call usage

Programs can choose whether they want to communicate directly through kernel headers or through system calls. Using system calls it not necessary as everything is running in kernel mode, but I think it spices up some things.

## Japanese GUI
GUI functionality should include Japanese kanji. This is more of a stylistic, rather than philosophical decision, but it's worth mentioning.

## Inside jokes

As this OS is not a fully serious project, a lot of inside jokes are found inside it, even the name itself ;). These might cause seemingly strange parts of code. For example, the SYS_WTF syscall (5), which displays a purple rectangle for a split second and opens/minimizes the terminal if the left mouse button was clicked.