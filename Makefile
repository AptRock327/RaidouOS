OBJS=src/bootloader/kernel_loader.o \
     src/kernel/kernel.o \
     src/kernel/IO.o \
     src/kernel/interrupts.o \
     src/kernel/pci.o \
     src/kernel/memory.o \
     src/kernel/filesystem.o \
     src/kernel/multitasking.o \
     src/kernel/syscalls.o \
     src/kernel/time.o \
     src/drivers/pcspk.o \
     src/drivers/graphics.o \
     src/drivers/rtc.o \
     src/drivers/mouse.o \
     src/drivers/keyboard.o \
     src/drivers/pit.o \
     src/programs/terminal.o \
     src/programs/desktop.o \
     src/programs/editor.o

CXX:=x86_64-elf-g++
LD:=x86_64-elf-ld

CXXFLAGS += -I include -ffreestanding -fpermissive -fno-rtti -fno-exceptions -mno-red-zone -m64

.PHONY: all clean
.SUFFIXES: .cpp .asm .o

all: raidou.img

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.asm.o:
	nasm -f elf64 $< -o $@

# boot.asm needs to be bin format
boot.bin: src/bootloader/boot.asm
	nasm -f bin $< -o $@

raidou_no_boot.bin: $(OBJS)
	$(LD) $(LDFLAGS) -T linker.ld -o $@ $(OBJS)

raidou.img: boot.bin raidou_no_boot.bin
	cat boot.bin raidou_no_boot.bin > raidou.img

clean:
	rm -f $(OBJS) boot.bin raidou_no_boot.bin raidou.img
