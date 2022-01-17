#include "mvk.h"

#include <stdcommon.h>

#include <cpuid.h>

#include "multiboot.h"

#include "vga.h"
#include "terminal.h"
#include "util.h"
#include "gdt.h"
#include "idt.h"
#include "paging.h"
#include "mmap.h"
#include "io.h"
#include "cpu.h"
#include "pci.h"
#include "rtc.h"
#include "fpu.h"

void kernel_main(unsigned long magic, multiboot_info_t* mbi) 
{
	paging_initialize();

	fpu_load_control_word(0x37A); // Division By Zero and Invalid Operands will cause exceptions in the FPU.

	stdio_initialize(mbi);

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
	{
		printf("kernel_main: Magic value isn't correct! 0x%x\n", (unsigned int)magic);
		abort();
	}

	gdt_initialize();
	idt_initialize();
	mmap_initialize(mbi);

	pci_initialize();

	rtc_state timestamp;
	read_rtc(&timestamp);

	printf("Welcome to MVK (Minimum Viable Kernel) Version %u.%u.%u %u/%u/%u %u:%u.%u\n", MVK_VERSION_MAJOR, MVK_VERSION_MINOR, MVK_VERSION_PATCH, timestamp.month, timestamp.day, timestamp.year, timestamp.hour, timestamp.minute, timestamp.second);
	puts("CPU information:");
	printf("  Vendor: %s\n", get_cpu_vendor());

	bool sse, sse2, sse3, sse41, sse42, ssse3, avx, avx512bf16, aes, fl6c, rdrnd;
	get_cpu_features(&avx512bf16, &sse3, &ssse3, &sse41, &sse42, &aes, &avx, &fl6c, &rdrnd, &sse, &sse2);
	printf("  CPU Features: SSE(%u) SSE2(%u) SSE3(%u) SSE4.1(%u) SSE4.2(%u)\n  SSSE3(%u) AVX(%u) AVX512bf16(%u) AES(%u) FL6C(%u) RDRND(%u)\n", sse, sse2, sse3, sse41, sse42, ssse3, avx, avx512bf16, aes, fl6c, rdrnd);

	puts("Memory Information:");
	printf("  Available Memory: %u Bytes\n", (uint32_t)mmap_free_memory_length);

	puts("PCI Bus Information:");
    if (pci_connected_devices)
    {
        for (size_t i = 0; i < pci_connected_devices_length; ++i)
        {
            pci_device device = pci_connected_devices[i];

            printf("  %x:%x.%x [%x:%x] [%x:%x] (rev %x) (prog-if %x)\n", 
                device.bus, device.device, device.function, 
                pci_get_class(device.bus, device.device, device.function),
                pci_get_subclass(device.bus, device.device, device.function),
                pci_get_vendor(device.bus, device.device, device.function),
                pci_get_device(device.bus, device.device, device.function),
                pci_get_revision(device.bus, device.device, device.function),
                pci_get_progif(device.bus, device.device, device.function));
        }
    }
    printf("  %u connected devices.\n", pci_connected_devices_length);

    while (true);
}
