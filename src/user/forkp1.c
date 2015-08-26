#include <conio.h>
#include <process.h>
#include <sched.h>
#include <sema.h>
#include <string.h>
#include <fileio.h>

int global = 0;

int main(int argc, char **argv) {
    int n = 0;
    int child_pid = 0;
    Print("original\n");
    child_pid = Fork();
    n++;
    global ++;
    if(child_pid > 0) {
        Print("parent n=%d, global=%d, child_pid=%d, my_pid=%d\n", n,
              global, child_pid, Get_PID());
    } else if(child_pid == 0) {
        Print("child n=%d, global=%d, child_pid=%d, my_pid=%d\n", n,
              global, child_pid, Get_PID());
    } else {
        Print("fork failed: %s (%d)\n", Get_Error_String(child_pid),
              child_pid);
    }

    return 0;
}
