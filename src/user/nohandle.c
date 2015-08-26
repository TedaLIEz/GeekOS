/*************************************************************************/
/*
 * GeekOS master source distribution and/or project solution
 * Copyright (c) 2001,2003,2004 David H. Hovemeyer <daveho@cs.umd.edu>
 * Copyright (c) 2003 Jeffrey K. Hollingsworth <hollings@cs.umd.edu>
 * Copyright (c) 2004 Iulian Neamtiu <neamtiu@cs.umd.edu>
 *
 * This file is not distributed under the standard GeekOS license.
 * Publication or redistribution of this file without permission of
 * the author(s) is prohibited.
 */
/*************************************************************************/
/*
 * A test program for GeekOS user mode - test_no_signal_handler
 */

#include <process.h>
#include <signal.h>
#include <conio.h>

int isAlive(int pid) {
    int i;
    struct Process_Info processTable[50];
    int processCount;
    int numPS = 50;
    int sysNum = 0;             // modified SYS_NULL
    int typeNum = 0;            // ps system call

    __asm__ __volatile__("int $0x90":"=a"(processCount):"a"(sysNum),
                         "b"(processTable), "c"(numPS), "D"(typeNum));

    for(i = 0; i < processCount; i++) {
        if(processTable[i].pid == pid &&
           processTable[i].status != STATUS_ZOMBIE) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int count;
    if(argc > 1) {
        /* forked child, hang around see if we get killed */
        while (1) ;
    } else {
        int pid =
            Spawn_With_Path("nohandle.exe", "nohandle.exe child", "/c:/a",
                            0);
        // wait some time
        if(pid < 0) {
            Print("FAIL: unable for spawn second copy of program\n");
            Exit(-1);
        } else {
            Kill(pid, SIGUSR1);
            for(count = 0; count < 10000000; count++) ;
            // verify it left the system
            if(isAlive(pid)) {
                Print("FAIL: should still be dead after SIGUSR1\n");
                Exit(-1);
            }

        }
        Print("PASS: no SIGUSR1 or SIGUSR2 handlers\n");
        Exit(0);
    }

    return 0;
}
