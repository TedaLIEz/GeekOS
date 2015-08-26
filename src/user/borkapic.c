#include <conio.h>

int main() {
    /* checks whether apic pages are writable at user space. */
    char *x = (char *)0x7EC00000;
    Print("read %x.\n", *x);    /* probably shouldn't be able to read. */
    *x = 'b';
    Print("FAIL: should have been terminated before here.\n");
    return 1;
}
