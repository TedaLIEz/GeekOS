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
 * A test program for GeekOS user mode
 */

#include <process.h>
#include <signal.h>
#include <conio.h>

static volatile int g = 0;
static volatile int count = 0;

void my_handler(int signo) {
    int i;
    Print("In test my_handler(%d)\n", signo);
    g = 1;
    for(i = 0; i < 10; i++)
        Get_PID();
    Print("Leaving test my_handler(%d)\n", signo);
    return;
}

void my_handler2(int signo) {
    Print("In test my_handler2(%d)\n", signo);
    Print("Leaving test my_handler2(%d)\n", signo);
    return;
}

int main() {
    Signal(my_handler, SIGUSR1);
    Signal(my_handler2, SIGUSR2);

    Print("entering test g loop\n");
    for(; g == 0;)
        count++;
    Print("done with test g loop!\n");

    return 0;
}
