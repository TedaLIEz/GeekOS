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
#include <geekos/pipe.h>
#include <geekos/malloc.h>
#include <geekos/string.h>
#include <geekos/errno.h>
#include <geekos/projects.h>
#include <geekos/int.h>
struct File_Ops Pipe_Read_Ops =
    { NULL, Pipe_Read, NULL, NULL, Pipe_Close, NULL };
struct File_Ops Pipe_Write_Ops =
    { NULL, NULL, Pipe_Write, NULL, Pipe_Close, NULL };

struct pipe {
    ulong_t reader_count;
    ulong_t writer_count;
    char buffer[32 * 1024];
};
typedef struct pipe PIPE;
PIPE *pipe;
int Pipe_Create(struct File **read_file, struct File **write_file) {
    pipe = (PIPE *)Malloc(sizeof(PIPE));
    (*pipe).reader_count++;
    (*pipe).writer_count++;
    strncpy((*pipe).buffer, "demo buffer", 32 * 1024);
    Print("reader_value:%lu", (*pipe).reader_count);
    Print("buffer: %s", (*pipe).buffer);
    TODO_P(PROJECT_PIPE, "Create a pipe");
    TODO_P(PROJECT_PIPE, "test");
    return EUNSUPPORTED;
}

int Pipe_Read(struct File *f, void *buf, ulong_t numBytes) {
    TODO_P(PROJECT_PIPE, "Pipe read");
    return EUNSUPPORTED;
}

int Pipe_Write(struct File *f, void *buf, ulong_t numBytes) {
    TODO_P(PROJECT_PIPE, "Pipe write");
    return EUNSUPPORTED;
}

int Pipe_Close(struct File *f) {
    TODO_P(PROJECT_PIPE, "Pipe close");
    return 0;
}
