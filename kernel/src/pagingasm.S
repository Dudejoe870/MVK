.section .text

.global load_page_directory
.type load_page_directory, @function
load_page_directory:
    pushl %ebp
    movl %esp, %ebp

    movl 8(%esp), %eax
    movl %eax, %cr3

    movl %ebp, %esp
    popl %ebp
    ret
