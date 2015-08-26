#include <conio.h>
#include <process.h>
#include <string.h>
int main(int argc, char *argv[]) {
    int i;
    const char *prog = "/c/nothing.exe";
    const char *commandline = prog;
    int iterations = 500;
    if(argc > 1)
        iterations = atoi(argv[1]);
    for(i = 0; i < iterations; i++) {
        int g = Spawn_Program(prog, commandline, 0);
        int h = Spawn_Program(prog, commandline, 0);
        int rc = Wait(g);
        if(rc) {
            Print("Wait returned unexpected %d\n", rc);
        } else {
            if(i % 20 == 0)
                Print(".");
        }
        rc = Wait(h);
        if(rc) {
            Print("Wait returned unexpected %d\n", rc);
        } else {
            if(i % 20 == 0)
                Print(".");
        }
    }
    return 0;
}
