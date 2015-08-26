#include <conio.h>

int main() {
    int *i = (int *)0x70000000;
    Print("I should not print %d\n", *i);
    return 0;
}
