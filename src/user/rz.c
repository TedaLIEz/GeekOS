#include <conio.h>
#include <string.h>
#include <process.h>
#include <signal.h>
#include <geekos/errno.h>

int main(void) {
    int pid, ret;

    ret = WaitNoPID(&pid);
    if(ret == ENOZOMBIES) {
        Print("No process available for reaping\n");
    } else {
        Print("Process %d reaped\n", pid);
    }

    return 0;

}
