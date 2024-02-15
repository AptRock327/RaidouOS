OUTPUT_FORMAT(binary)

ENTRY(main)

INPUT
(
	src/kernel/kernel.o
	src/kernel/IO.o
	src/kernel/interrupts.o
	src/kernel/memory.o
	src/kernel/filesystem.o
	src/kernel/multitasking.o
	src/kernel/pci.o
	src/drivers/rtc.o
	src/drivers/graphics.o
	src/drivers/mouse.o
	src/drivers/pcspk.o
	src/drivers/keyboard.o
	src/programs/terminal.o
	src/programs/desktop.o
	src/kernel/syscalls.o
	src/bootloader/kernel_loader.o
)

OUTPUT
(
	raidou_no_boot.bin
)

SECTIONS
{
	. = 0x8000;

	.boot : ALIGN(0x1000)
	{
	*(.boot)
	}
	
	.text : ALIGN(0x1000)
	{
	*(.text)
	}

	.idt BLOCK(0x1000) : ALIGN(0x1000)
	{
	_idt = .;
	. = . + 0x1000;
	}

	.data : ALIGN(0x1000)
	{
	*(.data)
	}

	.rodata : ALIGN(0x1000)
	{
	*(.rodata)
	}
	
	.bss : ALIGN(0x1000)
	{
	*(COMMON)
	*(.bss)
	}
}
