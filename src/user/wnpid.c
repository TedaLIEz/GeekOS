/*
 * A really, really simple shell program
 * Copyright (c) 2003, Jeffrey K. Hollingsworth <hollings@cs.umd.edu>
 * Copyright (c) 2003,2004 David H. Hovemeyer <daveho@cs.umd.edu>
 * $Revision: 1.21 $
 * 
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "COPYING".
 */

#include <geekos/errno.h>
#include <conio.h>
#include <process.h>
#include <string.h>
#include <signal.h>

#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif


#define BUFSIZE 79
#define DEFAULT_PATH "/c:/a"

static volatile double d = 0.0;

static void PrintProcessTable(const int processCount,
                              const struct Process_Info *processTable) {
    int i;

    Print("RETURN %d\n", processCount);
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
}

#define PROCESS_TABLE_CAPACITY 20

int main() {
    int pid1;
    int pid;
    int i;
    struct Process_Info processTable[PROCESS_TABLE_CAPACITY];
    struct Process_Info processTable2[PROCESS_TABLE_CAPACITY];
    int processCount, processCount2;
    const int sysNum = 0;       // modified SYS_NULL
    const int typeNum = 0;      // modified SYS_PS (correct implementation)
    const int numPS = PROCESS_TABLE_CAPACITY;
    int ret;
    int zombified = FALSE;
    int gone = FALSE;
    bool found_this_iteration;

    // Spawn the b program
    Print("wnpid begins.\n");
    pid1 = Spawn_With_Path("b.exe", "b.exe 0 1 2 3", DEFAULT_PATH, 0);

    Print("spawned pid1=%d.\n", pid1);
    while (!zombified) {
        /* give it some time */
        for(i = 0; i < 0xFF; ++i) {
            d = d / 15 * d * d / (i - 10);
        }
        /* get PS data. */
        __asm__ __volatile__("int $0x90":"=a"(processCount):"a"(sysNum),
                             "b"(processTable), "c"(numPS), "D"(typeNum));
        if(processCount == 0) {
            Print
                ("Internal error, process table too small at %d entries\n",
                 numPS);
            return (-1);
        }
        // Print("looking for zombies in %d procs.\n", processCount);
        found_this_iteration = false;
        for(i = 0; i < processCount && !zombified; i++) {
            if(processTable[i].pid == pid1) {
                found_this_iteration = true;
                if(processTable[i].status == STATUS_ZOMBIE) {
                    Print("Good so far: Found B zombified.\n");
                    zombified = TRUE;
                } else if(processTable[i].status == STATUS_BLOCKED) {
                    Print("Inconsistency: B is blocked.\n");
                } else {
                    Print("Warning: B is running.\n");
                }
            }
        }
        if(!found_this_iteration) {
            Print("FAIL: B, pid %d, is gone, no zombie to wait for.\n",
                  pid1);
            PrintProcessTable(processCount, processTable);
            Exit(-1);
        }
    }
    Print("believe we're zombied.\n");

    // Spin for a while to let the b program finish
    //for(i = 0; i < 0xFFFFFF; ++i) {
    //  d = d / 15 * d * d / (i - 10);
    //}

    Print("about to fake ps\n");
    __asm__ __volatile__("int $0x90":"=a"(processCount):"a"(sysNum),
                         "b"(processTable), "c"(numPS), "D"(typeNum));
    Print("faked ps\n");

    PrintProcessTable(processCount, processTable);

    ret = WaitNoPID(&pid);
    if(ret == ENOZOMBIES) {
        Print("No process available for reaping\n");
    } else {
        Print("Process reaped\n");
    }

    // Wait for the process to be cleared out of the system
    //for(i = 0; i < 0xFFFFFF; ++i) {
    //  d = d / 15 * d * d / (i - 10);
    //}
    while (!gone) {
        /* give it some time */
        for(i = 0; i < 0xFF; ++i) {
            d = d / 15 * d * d / (i - 10);
        }
        /* get PS data. */
        __asm__ __volatile__("int $0x90":"=a"(processCount):"a"(sysNum),
                             "b"(processTable), "c"(numPS), "D"(typeNum));
        for(i = 0; i < processCount && processTable[i].pid != pid1; i++) ;
        if(i == processCount) {
            gone = TRUE;
            Print("Good so far: process gone\n");
        };
    }

    __asm__ __volatile__("int $0x90":"=a"(processCount2):"a"(sysNum),
                         "b"(processTable2), "c"(numPS), "D"(typeNum));

    PrintProcessTable(processCount2, processTable2);

    return 0;
}
