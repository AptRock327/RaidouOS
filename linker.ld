OUTPUT_FORMAT(binary)

ENTRY(main)

INPUT
(
	bootloader/kernel_loader.o
	kernel/kernel.o
	kernel/IO.o
	kernel/interrupts.o
	kernel/memory.o
	kernel/filesystem.o
	kernel/multitasking.o
	kernel/pci.o
	drivers/rtc.o
	drivers/graphics.o
	drivers/mouse.o
	drivers/pcspk.o
	drivers/keyboard.o
	programs/terminal.o
	programs/desktop.o
	kernel/syscalls.o
)

OUTPUT
(
	raidou_no_boot.bin
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
