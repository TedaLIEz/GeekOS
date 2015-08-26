#include <conio.h>
#include <string.h>

typedef int (*print_type) ();
int (*print) () = (print_type) Print;
int returns(void) {
    (print) ("ee\n");
    return 0;
}

typedef int returny(void);

int main() {
    char runme[100];            /* 100 bytes should do it. */
    memcpy(runme, returns, 100);
    ((returny *) runme) ();
    Print("I should not get here if the stack cannot be executed.\n");
    return 1;
}
