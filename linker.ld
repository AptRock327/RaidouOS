OUTPUT_FORMAT(binary)

ENTRY(main)

INPUT
(
tmp/kernel_loader.o
tmp/kernel.o
tmp/IO.o
tmp/interrupts.o
tmp/memory.o
tmp/filesystem.o
tmp/multitasking.o
tmp/graphics.o
tmp/mouse.o
tmp/rtc.o
tmp/pcspk.o
tmp/keyboard.o
tmp/pci.o
tmp/terminal.o
tmp/desktop.o
tmp/syscalls.o
)

OUTPUT
(
tmp/raidou_no_boot.bin
)

SECTIONS
{
	. = 0x8000;
	
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