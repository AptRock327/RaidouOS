OUTPUT_FORMAT(binary)

ENTRY(main)

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
