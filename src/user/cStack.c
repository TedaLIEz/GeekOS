#include <fileio.h>
#include <conio.h>

// verify the user stack is now near 2 GB
int main(int argc, char **argv) {
    int local = 0;
    unsigned int ptr = (unsigned int)&local;

    if((ptr > 0x6fffe000) && (ptr < 0x80000000)) {
        Print("PASSED: stack is near 2GB\n");
    } else {
        Print("FAILED: local var on stack at %x\n", (unsigned int)&local);
    }
    return 0;
}
