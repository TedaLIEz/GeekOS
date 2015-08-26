#include <process.h>
#include <signal.h>
#include <conio.h>

static volatile int sighandler_count = 0;
static volatile int count = 0;
static volatile int pid = 0;
static volatile unsigned int *int_array_on_stack = 0;

#define INT_ARRAY_LEN 120

void my_handler(int signo) {
    int i;
    static void *base_addr;
    /* who cares */
    if(signo > 4) {
        Print("bad arg %d.\n", signo);
        Exit(-1);
    }

    if(base_addr == 0) {
        base_addr = &i;
    } else if(base_addr != &i) {
        Print("stack moved: %p != %p\n", base_addr, &i);
        Exit(-1);
    }

    for(i = 0; i < INT_ARRAY_LEN; i++) {
        if(int_array_on_stack[i] != 0xdeadbeef) {
            Print("stack corrupted at %d.\n", i);
            Exit(-1);
        }
    }

    sighandler_count += 1;
    if(sighandler_count > 40) {
        Print
            ("Successful repeated signaling without obvious stack damage\n");
        Exit(0);
    }

    Kill(pid, signo);
    return;
}

int main() {
    volatile unsigned int dont_mess_with_me[INT_ARRAY_LEN];
    int i;
    for(i = 0; i < INT_ARRAY_LEN; i++) {
        dont_mess_with_me[i] = 0xdeadbeef;
    }
    int_array_on_stack = dont_mess_with_me;
    pid = Get_PID();

    Signal(my_handler, SIGUSR1);

    Kill(pid, SIGUSR1);

    Print("going to lunch\n");
    for(; dont_mess_with_me[INT_ARRAY_LEN - 1] == 0xdeadbeef;)
        count++;
    Print("Bollocks!\n");

    return 0;
}
