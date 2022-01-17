#pragma once

#include <stdcommon.h>

#define PCI_CONFIG_ADDRESS_PORT 0xCF8
#define PCI_CONFIG_DATA_PORT 0xCFC

typedef struct
{
    uint8_t bus;
    uint8_t device;
    uint8_t function;
} pci_device;

extern pci_device* pci_connected_devices;
extern size_t pci_connected_devices_length;

void pci_initialize(void);

uint16_t pci_config_read_word(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);

uint16_t pci_get_device(uint8_t bus, uint8_t device, uint8_t function);
uint16_t pci_get_vendor(uint8_t bus, uint8_t device, uint8_t function);
uint8_t pci_get_class(uint8_t bus, uint8_t device, uint8_t function);
uint8_t pci_get_subclass(uint8_t bus, uint8_t device, uint8_t function);
uint8_t pci_get_progif(uint8_t bus, uint8_t device, uint8_t function);
uint8_t pci_get_revision(uint8_t bus, uint8_t device, uint8_t function);
uint8_t pci_get_header_type(uint8_t bus, uint8_t device, uint8_t function);
uint8_t pci_get_secondary_bus(uint8_t bus, uint8_t device, uint8_t function);
