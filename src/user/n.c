#include <process.h>
#include <conio.h>

int my_atoi(char *val) {
    char *tmp = val;
    int value = 0;
    while (*tmp) {
        if(*tmp < '0' || *tmp > '9')
            break;
        value *= 10;
        value += (*tmp - '0');
        tmp++;
    }
    return value;
}

int main(int argc, char **argv) {
    struct Process_Info processTable[50];
    int processCount;
    int i;
    int sysNum, typeNum;
    sysNum = 0;                 // modified SYS_NULL
    typeNum = 0;                // modified SYS_PS (correct implementation)
    int numPS = my_atoi(argv[2]);

    if(*(argv[1]) == '0')
        processCount = PS(processTable, numPS);
    else
      __asm__ __volatile__("int $0x90": "=a"(processCount):"a"(sysNum), "b"(processTable), "c"(numPS),
                             "D"
                             (typeNum));

    Print("RETURN %d\n", processCount);

    if(processCount == -1 || processCount == 0)
        return -1;

    Print("PID PPID PRIO STAT COMMAND\n");

    for(i = 0; i < processCount; ++i) {
        char status;

        if(processTable[i].status == STATUS_ZOMBIE)
            status = 'Z';
        else if(processTable[i].status == STATUS_BLOCKED)
            status = 'B';
        else
            status = 'R';

        Print("%3d %4d %4d %4c %s\n", processTable[i].pid,
              processTable[i].parent_pid,
              processTable[i].priority, status, processTable[i].name);

    }

    return 0;
}
