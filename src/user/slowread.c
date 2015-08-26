/*
 * Copyright (c) 2001,2003,2004 David H. Hovemeyer <daveho@cs.umd.edu>
 * Copyright (c) 2003,2013,2014 Jeffrey K. Hollingsworth <hollings@cs.umd.edu>
 *
 * All rights reserved.
 *
 * This code may not be resdistributed without the permission of the copyright holders.
 * Any student solutions using any of this code base constitute derviced work and may
 * not be redistributed in any form.  This includes (but is not limited to) posting on
 * public forums or web sites, providing copies to (past, present, or future) students
 * enrolled in similar operating systems courses the University of Maryland's CMSC412 course.
 */
#include <conio.h>
#include <process.h>
#include <sched.h>
#include <sema.h>
#include <string.h>
#include <fileio.h>
#include <signal.h>
#include <geekos/errno.h>

#define VOLUME (2<<20)

int global = 0;
int sigpiped = 0;
char bigbuf[2048];
void sigpipe_handler(int signal) {
    sigpiped = 1;
}

int main(int argc, char **argv) {
    int n = 0;
    int child_pid = 0;
    int i;
    int read_fd, write_fd;
    int read_bytes, written_bytes, pipe_retval;
    char buf[256];


    Print("original pid=%d\n", Get_PID());
    pipe_retval = Pipe(&read_fd, &write_fd);
    assert(pipe_retval == 0);

    child_pid = Fork();

    /* each process should increment these independently */
    n++;
    global ++;

    if(child_pid > 0) {
        /* am the parent, let's read. */
        int total_read_bytes = 0;
        Print("parent n=%d, global=%d, child_pid=%d, my_pid=%d\n",
              n, global, child_pid, Get_PID());
        Close(write_fd);
        while ((read_bytes = Read(read_fd, buf, 16)) > 0 &&
               total_read_bytes < VOLUME) {
            //Print(":");
            total_read_bytes += read_bytes;
        }
        Close(read_fd);

        if(read_bytes != 0) {
            Print
                ("FAIL: should end with EOF, since all writers are closed.");
        }
    } else if(child_pid == 0) {
        unsigned long totally_written = 0;
        Close(read_fd);
        Print("child n=%d, global=%d, child_pid=%d, my_pid=%d\n",
              n, global, child_pid, Get_PID());
        for(i = 0; i < 2048; i++)
            bigbuf[i] = i % 256;
        for(totally_written = 0; totally_written <= VOLUME / 2;
            totally_written += sizeof(bigbuf)) {
            written_bytes = Write(write_fd, bigbuf, sizeof(bigbuf));
            Print(".");
        }
        Print("Good so far, child could write a lot.");
        for(totally_written = 0; totally_written <= VOLUME / 4;
            totally_written += sizeof(bigbuf)) {
            written_bytes = Write(write_fd, bigbuf, sizeof(bigbuf));
            Print(".");
        }
        Close(write_fd);
    } else {
        Print("FAIL: Error in Fork(): %d\n", child_pid);
        return 1;
    }

    return 0;
}
