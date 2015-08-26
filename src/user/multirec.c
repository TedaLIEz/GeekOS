#include <conio.h>
#include <process.h>
#include <sched.h>
#include <string.h>

int main() {
    int a, b, c;
    int ra, rb, rc;
    a = Spawn_Program("/c/rec.exe", "/c/rec.exe 15000 q", 0);
    b = Spawn_Program("/c/rec.exe", "/c/rec.exe 15000 q", 0);
    c = Spawn_Program("/c/rec.exe", "/c/rec.exe 15000 q", 0);
    ra = Wait(a);
    rb = Wait(b);
    rc = Wait(c);
    Print("done. %d %d %d\n", ra, rb, rc);
    return 0;
}
