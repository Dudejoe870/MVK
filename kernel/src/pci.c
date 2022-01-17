#include "pci.h"

#include "io.h"
#include "terminal.h"

pci_device* pci_connected_devices;
size_t pci_connected_devices_length;

void scan_pci_buses(void);
void check_device(uint8_t, uint8_t);
void check_bus(uint8_t);
void check_function(uint8_t, uint8_t, uint8_t);

void scan_pci_buses(void)
{
    uint8_t header_type = pci_get_header_type(0, 0, 0);
    if ((header_type & 0x80) == 0) check_bus(0);
    else
    {
        for (uint8_t function = 0; function < 8; ++function)
        {
            if (pci_get_vendor(0, 0, function) != 0xFFFF) break;
            check_bus(function);
        }
    }
}

void check_bus(uint8_t bus)
{
    for (uint8_t device = 0; device < 32; ++device)
        check_device(bus, device);
}

void check_device(uint8_t bus, uint8_t device)
{
    uint8_t function = 0;

    uint16_t vendor = pci_get_vendor(bus, device, function);
    if (vendor == 0xFFFF) return;
    check_function(bus, device, function);
    uint8_t header_type = pci_get_header_type(bus, device, function);
    if ((header_type & 0x80) != 0)
        for (function = 1; function < 8; ++function)
            if (pci_get_vendor(bus, device, function) != 0xFFFF)
                check_function(bus, device, function);
}

void check_function(uint8_t bus, uint8_t device, uint8_t function)
{
    uint8_t class = pci_get_class(bus, device, function);
    uint8_t subclass = pci_get_subclass(bus, device, function);

    if (class == 0x06 && subclass == 0x04)
        check_bus(pci_get_secondary_bus(bus, device, function));
    
    ++pci_connected_devices_length;
    void* old_ptr = pci_connected_devices;
    pci_connected_devices = realloc(old_ptr, pci_connected_devices_length * sizeof(pci_device));
    if (!pci_connected_devices) 
    {
        puts("ERROR: Couldn't allocate PCI devices list!");
        abort();
    }

    pci_connected_devices[pci_connected_devices_length-1].bus = bus;
    pci_connected_devices[pci_connected_devices_length-1].device = device;
    pci_connected_devices[pci_connected_devices_length-1].function = function;
}

void pci_initialize(void)
{
    pci_connected_devices = NULL;
    pci_connected_devices_length = 0;

    scan_pci_buses();
}

uint16_t pci_config_read_word(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset)
{
    outl(PCI_CONFIG_ADDRESS_PORT, (uint32_t)(((uint32_t)bus << 16) | ((uint32_t)device << 11) | ((uint32_t)function << 8) | (offset & 0xFC) | ((uint32_t)0x80000000)));
    return (uint16_t)((inl(PCI_CONFIG_DATA_PORT) >> ((offset & 2) * 8)) & 0xFFFF);
}

uint16_t pci_get_device(uint8_t bus, uint8_t device, uint8_t function)
{
    return pci_config_read_word(bus, device, function, 0x3);
}

uint16_t pci_get_vendor(uint8_t bus, uint8_t device, uint8_t function)
{
    return pci_config_read_word(bus, device, function, 0x0);
}

uint8_t pci_get_class(uint8_t bus, uint8_t device, uint8_t function)
{
    return (uint8_t)(pci_config_read_word(bus, device, function, 0xB) >> 8);
}

uint8_t pci_get_subclass(uint8_t bus, uint8_t device, uint8_t function)
{
    return (uint8_t)(pci_config_read_word(bus, device, function, 0xB) & 0xFF);
}

uint8_t pci_get_progif(uint8_t bus, uint8_t device, uint8_t function)
{
    return (uint8_t)(pci_config_read_word(bus, device, function, 0x8) >> 8);
}

uint8_t pci_get_revision(uint8_t bus, uint8_t device, uint8_t function)
{
    return (uint8_t)(pci_config_read_word(bus, device, function, 0x8) & 0xFF);
}

uint8_t pci_get_header_type(uint8_t bus, uint8_t device, uint8_t function)
{
    return (uint8_t)(pci_config_read_word(bus, device, function, 0xF) & 0xFF);
}

uint8_t pci_get_secondary_bus(uint8_t bus, uint8_t device, uint8_t function)
{
    return (uint8_t)(pci_config_read_word(bus, device, function, 0x18) >> 8);
}
