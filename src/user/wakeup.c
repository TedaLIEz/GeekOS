#include <process.h>
#include <conio.h>
#include <signal.h>

int main(int argc, char **argv) {

    int ret;
    int sysNum, typeNum;
    sysNum = 0;                 // modified SYS_NULL
    typeNum = 3;                // Wakeup system call

    __asm__ __volatile__("int $0x90":"=a"(ret):"a"(sysNum), "D"(typeNum));

    return 0;
}
