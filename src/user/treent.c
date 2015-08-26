#include <conio.h>
#include <string.h>
#include <process.h>
#include <signal.h>
#include <geekos/errno.h>

volatile double d = 0.0;
void f_signal_handler3(int n) {
    int i;
    n = n + n;
    //Kill(Get_PID(), SIGKILL);
    // Spin for a while
    Print("In signal handler3\n");
    Exit(0);
}

void f_signal_handler2(int n) {
    Kill(Get_PID(), SIGUSR1);
    n = n + n;
    Print("In signal handler2\n");
    Signal(f_signal_handler3, SIGUSR1);
    Kill(Get_PID(), SIGUSR1);
}



void f_signal_handler1(int n) {
    n = n + 1;

    Print("In signal handler1\n");
    Kill(Get_PID(), SIGUSR1);
    Kill(Get_PID(), SIGUSR1);
    Signal(f_signal_handler2, SIGUSR1);
    Kill(Get_PID(), SIGUSR1);
}


int main(void) {
    int r, p;
    r = Signal(f_signal_handler1, SIGUSR1);
    if(r < 0) {
        Print("Signal() failed %d", r);
        Exit(-1);
    }
    p = Get_PID();
    if(p < 0) {
        Print("GetPID() failed %d", p);
        Exit(-1);
    }

    r = Kill(p, SIGUSR1);
    if(r < 0) {
        Print("Kill() failed %d", r);
        Exit(-1);
    }
    // Wait for signal
    while (1) ;

    return 0;
}
