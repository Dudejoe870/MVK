#include "pic.h"

void pic_remap(int offset1, int offset2)
{
    unsigned char a1, a2;

    a1 = inb(PIC1_DATA); // Save Masks.
    a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); // Starts the initialization sequence (in cascade mode).
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC1_DATA, offset1); // ICW2: Master PIC vector offset
    io_wait();
    outb(PIC2_DATA, offset2); // ICW2: Slave PIC vector offset
    io_wait();
    outb(PIC1_DATA, 0b100); // ICW3: Tell Master PIC that there is a slave PIC at IRQ2.
    io_wait();
    outb(PIC2_DATA, 0b010); // ICW3: Tell Slave PIC its cascade identity.
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1); // Restore saved masks.
    outb(PIC2_DATA, a2);
}
