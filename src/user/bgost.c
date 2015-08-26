
#include <process.h>
#include <sched.h>
#include <signal.h>

int main(int argc, char **argv) {
    int pid;
    int start, end;
    pid = Spawn_Program("/c/ost.exe", "/c/ost.exe", 0);

    // allow spawn'd child to get running
    start = Get_Time_Of_Day();
    while (1) {
        end = Get_Time_Of_Day();
        if(end - start > 100)
            break;
    }

    Kill(pid, 3);

    start = Get_Time_Of_Day();
    while (1) {
        end = Get_Time_Of_Day();
        if(end - start > 100)
            break;
    }

    Kill(pid, 2);

    Wait(pid);

    return 0;
}
