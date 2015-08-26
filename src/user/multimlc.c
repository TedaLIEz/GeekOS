#include <conio.h>
#include <process.h>
#include <sched.h>
#include <string.h>

int main(int argc, char **argv) {
    int a, b, c;
    int ra, rb, rc;
    const char *prog = "/c/mlc.exe";
    char commandline[128];
    strcpy(commandline, prog);
    strcat(commandline, " ");
    if(argc > 1) {
        strcat(commandline, argv[1]);
        if(argc > 2) {
            strcat(commandline, " ");
            strcat(commandline, argv[2]);
        }
    } else {
        strcat(commandline, "5000");
    }
    strcat(commandline, " q");


    a = Spawn_Program(prog, commandline, 0);
    b = Spawn_Program(prog, commandline, 0);
    c = Spawn_Program(prog, commandline, 0);
    Print("spawned three %s : %d %d %d\n", commandline, a, b, c);
    ra = Wait(a);
    rb = Wait(b);
    rc = Wait(c);
    Print("done. %d %d %d\n", ra, rb, rc);
    return 0;
}
