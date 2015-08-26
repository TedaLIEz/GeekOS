#include <conio.h>
#include <string.h>
#include <process.h>
#include <signal.h>
#include <geekos/errno.h>

static int count = 0;
static int done = 0;

volatile double d = 0.0;

void f_signal_handler(int n) {
    ++count;
    done = 1;

    n = n + 1;

    Print("In signal handler\n");
}

int main(void) {
    int i;
    Signal(f_signal_handler, SIGUSR1);

    Kill(Get_PID(), SIGUSR1);

    // Wait for signal
    while (!done) ;

    Signal(SIG_DFL, SIGUSR1);

    Kill(Get_PID(), SIGUSR1);

    // Spin for a while
    for(i = 0; i < 0xFFFFFF; ++i) {
        d = d * d / d + 15;
    }

    Print("Count: %d\n", count);

    return 0;

}
