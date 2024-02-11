mkdir tmp
nasm bootloader/boot.asm -f bin -o tmp/boot.bin
nasm bootloader/kernel_loader.asm -f elf64 -o tmp/kernel_loader.o
x86_64-elf-gcc -ffreestanding -fpermissive -fno-rtti -fno-exceptions -mno-red-zone -m64 -c "kernel/kernel.cpp" -o "tmp/kernel.o"
x86_64-elf-ld -T "linker.ld"
cat tmp/boot.bin tmp/raidou_no_boot.bin > raidou.img
rm -rf tmp
