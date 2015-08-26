/* #include <malloc.h> */
#include <conio.h>

/* reasonable chance malloc.h is busted */
void *Malloc(unsigned long sz);
void Free(void *);

int main() {
    int i;

    for(i = 0; i < 1000000; i++) {
        char *p = Malloc(4096);
        char *q = Malloc(4096);
        assert(p != NULL);
        assert(q != NULL);
        assert(p != q);
        assert(p < (char *)0x40000000);
        assert(q < (char *)0x40000000);
        *p = 1;
        *q = 2;
        Free(p);
        Free(q);
    }

    Print("Free seems okay.\n");
    return 0;
}
