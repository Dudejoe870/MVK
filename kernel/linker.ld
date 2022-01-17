ENTRY(_start)

SECTIONS
{
	. = 0xC0000000;
	_kernel_start = .;
	.text ALIGN(4K) : AT(ADDR(.text) - 0xC0000000)
	{
		KEEP(*(.multiboot.data))
		KEEP(*(.multiboot.text))
		*(.text)
	}
 
	.rodata ALIGN(4K) : AT(ADDR(.rodata) - 0xC0000000)
	{
		*(.rodata)
	}
 
	.data ALIGN(4K) : AT(ADDR(.data) - 0xC0000000)
	{
		*(.data)
	}
 
	.bss ALIGN(4K) : AT(ADDR(.bss) - 0xC0000000)
	{
		_sbss = .;
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
		_ebss = .;
	}
	_kernel_end = .;
}
