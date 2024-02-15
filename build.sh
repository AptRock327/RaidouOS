mkdir tmp
nasm bootloader/boot.asm -f bin -o tmp/boot.bin
nasm bootloader/kernel_loader.asm -f elf64 -o tmp/kernel_loader.o
x86_64-elf-gcc -ffreestanding -fpermissive -fno-rtti -fno-exceptions -mno-red-zone -m64 -c "kernel/kernel.cpp" -o "tmp/kernel.o"
x86_64-elf-gcc -ffreestanding -fpermissive -fno-rtti -fno-exceptions -mno-red-zone -m64 -c "kernel/IO.cpp" -o "tmp/IO.o"
x86_64-elf-gcc -ffreestanding -fpermissive -fno-rtti -fno-exceptions -mno-red-zone -m64 -c "kernel/interrupts.cpp" -o "tmp/interrupts.o"
x86_64-elf-gcc -ffreestanding -fpermissive -fno-rtti -fno-exceptions -mno-red-zone -m64 -c "kernel/memory.cpp" -o "tmp/memory.o"
x86_64-elf-gcc -ffreestanding -fpermissive -fno-rtti -fno-exceptions -mno-red-zone -m64 -c "kernel/filesystem.cpp" -o "tmp/filesystem.o"
x86_64-elf-gcc -ffreestanding -fpermissive -fno-rtti -fno-exceptions -mno-red-zone -m64 -c "kernel/multitasking.cpp" -o "tmp/multitasking.o"
x86_64-elf-gcc -ffreestanding -fpermissive -fno-rtti -fno-exceptions -mno-red-zone -m64 -c "drivers/graphics.cpp" -o "tmp/graphics.o"
x86_64-elf-gcc -ffreestanding -fpermissive -fno-rtti -fno-exceptions -mno-red-zone -m64 -c "drivers/mouse.cpp" -o "tmp/mouse.o"
x86_64-elf-gcc -ffreestanding -fpermissive -fno-rtti -fno-exceptions -mno-red-zone -m64 -c "drivers/rtc.cpp" -o "tmp/rtc.o"
x86_64-elf-gcc -ffreestanding -fpermissive -fno-rtti -fno-exceptions -mno-red-zone -m64 -c "drivers/keyboard.cpp" -o "tmp/keyboard.o"
x86_64-elf-gcc -ffreestanding -fpermissive -fno-rtti -fno-exceptions -mno-red-zone -m64 -c "kernel/pci.cpp" -o "tmp/pci.o"
x86_64-elf-gcc -ffreestanding -fpermissive -fno-rtti -fno-exceptions -mno-red-zone -m64 -c "programs/terminal.cpp" -o "tmp/terminal.o"
x86_64-elf-gcc -ffreestanding -fpermissive -fno-rtti -fno-exceptions -mno-red-zone -m64 -c "programs/desktop.cpp" -o "tmp/desktop.o"
x86_64-elf-gcc -ffreestanding -fpermissive -fno-rtti -fno-exceptions -mno-red-zone -m64 -c "drivers/pcspk.cpp" -o "tmp/pcspk.o"
x86_64-elf-gcc -ffreestanding -fpermissive -fno-rtti -fno-exceptions -mno-red-zone -m64 -c "kernel/syscalls.cpp" -o "tmp/syscalls.o"
x86_64-elf-ld -T "linker.ld"
cat tmp/boot.bin tmp/raidou_no_boot.bin > raidou.img
rm -rf tmp
