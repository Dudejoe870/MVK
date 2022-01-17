mkdir -p build/iso/boot/grub
cp build/MVK.elf build/iso/boot/MVK.elf
cp grub.cfg build/iso/boot/grub/grub.cfg
grub-mkrescue -o build/MVK.iso build/iso
rm -r build/iso