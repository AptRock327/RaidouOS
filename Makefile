
OBJS=kernel/kernel.o \
     kernel/IO.o \
     kernel/interrupts.o \
     kernel/pci.o \
     kernel/memory.o \
     kernel/filesystem.o \
     kernel/multitasking.o \
     kernel/syscalls.o \
     programs/terminal.o \
     programs/desktop.o \
     drivers/pcspk.o \
     drivers/graphics.o \
     drivers/rtc.o \
     drivers/mouse.o \
     drivers/keyboard.o \
     bootloader/kernel_loader.o

CXX:=x86_64-elf-g++
LD:=x86_64-elf-ld

CXXFLAGS += -ffreestanding -fpermissive -fno-rtti -fno-exceptions -mno-red-zone -m64

.PHONY: all clean
.SUFFIXES: .cpp .asm .o

all: raidou.img

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.asm.o:
	nasm -f elf64 $< -o $@

# boot.asm needs to be bin format
boot.bin: bootloader/boot.asm
	nasm -f bin $< -o $@

raidou_no_boot.bin: $(OBJS)
	$(LD) $(LDFLAGS) -T linker.ld

raidou.img: boot.bin raidou_no_boot.bin
	cat boot.bin raidou_no_boot.bin > raidou.img

clean:
	rm -f $(OBJS) boot.bin raidou.bin raidou.img
