#include <conio.h>
#include <process.h>
#include <sched.h>
#include <sema.h>
#include <string.h>
#include <fileio.h>

int main(int argc, char **argv) {
    int rc, i, j;
    if(argc > 1) {
        j = atoi(argv[1]);
        if(j < 1) {
            j = 4;
        }
    } else {
        j = 4;
    }
    do {
        rc = Fork();
        if(rc > 0) {
            if(rc <= Get_PID()) {
                Print(" %d borked %d\n", Get_PID(), rc);
            } else {
                Print(" %d forked %d at %d\n", Get_PID(), rc, j);
            }
        }
    } while (rc >= 0 && --j > 0);
    Print(" so long from %d\n", Get_PID());

    /* hold resources a bit to make exhaustion happen earlier */
    /*     for(i=1000; i>0; i--) {
       Null();
       } */
    return 0;
}
