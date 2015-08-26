#include <conio.h>
#include <string.h>
#include <process.h>
#include <signal.h>
#include <geekos/errno.h>

int main(void) {
    int pid, ret;

    ret = WaitNoPID(&pid);
    if(ret == ENOZOMBIES) {
        Print("Good, ENOZOMBIES returned\n");
    } else {
        Print("Bad, ENOZOMBIES not returned\n");
    }

    return 0;

}
