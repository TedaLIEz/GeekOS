#include <process.h>
#include <conio.h>
#include <signal.h>

static int done1 = 0, done2 = 0, done3 = 0;

void sig_handler1(int n) {
    n = n + 1;
    Print("Signal handler 1\n");
    done1 = 1;
}

void sig_handler2(int n) {
    n = n + 1;
    Print("Signal handler 2\n");
    done2 = 1;
}

void sig_handler3(int n) {
    n = n + 1;
    Print("Signal handler 3\n");
    done3 = 1;
}

int main(int argc, char **argv) {

    int ret;
    int sysNum, typeNum;
    sysNum = 0;                 // modified SYS_NULL
    typeNum = 2;                // Sleep system call

    Signal(sig_handler1, SIGUSR1);
    Signal(sig_handler2, SIGUSR2);
    Signal(sig_handler3, SIGCHLD);

    __asm__ __volatile__("int $0x90":"=a"(ret):"a"(sysNum), "D"(typeNum));

    // Spin until done
    while (!done1 || !done2 || !done3) ;

    Print("DONE!\n");

    return 0;
}
