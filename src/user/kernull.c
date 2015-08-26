#include <string.h>

/* when run with the correct number of pages, should check
   that all kernel pages in that range can be read.  It does
   seem to work, though accessing all these addresess
   blindly may yield trouble with devices.  This might not
   be a terribly useful test, but it did help figure out a
   cr4/pse bit problem for large pages in the solution. */

int main(int argc, char **argv) {
    int retVal;
    int sysNum, typeNum;
    int numPages = 100;
    sysNum = 0;                 // modified SYS_NULL
    typeNum = 15;               // dereference null.
    if(argc > 1) {
        numPages = atoi(argv[1]);
    }

    __asm__ __volatile__("int $0x90":"=a"(retVal):"a"(sysNum),
                         "D"(typeNum), "b"(numPages));

    return 0;
}
