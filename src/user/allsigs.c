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

int sleep(int val) {
    int ret;
    int sysNum, typeNum;
    sysNum = 0;                 // modified SYS_NULL
    typeNum = 2;                // Sleep system call

    __asm__ __volatile__("int $0x90":"=a"(ret):"a"(sysNum), "D"(typeNum));
    return ret;
}

void child(int argc, char **argv) {
    int ret;

    Signal(sig_handler1, SIGUSR1);
    Signal(sig_handler2, SIGUSR2);
    Signal(sig_handler3, SIGALARM);

    // sleep(1);

    // Spin until done
    // while(!done1 || !done2 || !done3) {
    while (!done1 || !done2) {
        Print("%d %d %d\n", done1, done2, done3);
    }

    Print("Child Exit Normal!\n");

    Exit(0);
}

void parent() {
    volatile int i;

    int pid =
        Spawn_With_Path("allsigs.exe", "allsigs.exe child", "/c:/a", 0);
    int ret;
    if(pid < 0) {
        Print("FAIL: unable for spawn second copy of program\n");
        Exit(-1);
    } else {

        Print("about to wait for child\n");
        for(i = 0; i < 1024 * 1024; i++) ;

        ret = Kill(pid, SIGUSR1);
        if(ret) {
            Exit(-1);
            Print("FAIL: kill(pid, SIGUSR1)\n");
        }

        ret = Kill(pid, SIGUSR2);
        if(ret) {
            Exit(-1);
            Print("FAIL: kill(pid, SIGUSR2)\n");
        }

        ret = Kill(pid, SIGALARM);
        if(ret) {
            Exit(-1);
            Print("FAIL: kill(pid, SIGALARM)\n");
        }

        Wait(pid);
        Print("Parent Exit Normal!\n");
        Exit(0);
    }
}

int main(int argc, char **argv) {
    if(argc > 1) {
        child(argc, argv);
    } else {
        parent();
    }

    return 0;
}
