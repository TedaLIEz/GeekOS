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

int Pipe_Create(struct File **read_file, struct File **write_file) {
    // Create Pipe struct
    struct Pipe *pipe = Malloc(sizeof(struct Pipe));
    if (pipe == 0) {
        return ENOMEM;
    }
    pipe->readers = 1;
    pipe->writers = 1;
    pipe->p_read = 0;
    pipe->p_write = 0;
    pipe->bytes_used = 0;
    pipe->buffer = Malloc(PIPE_BUFFER_SIZE);
    if (pipe->buffer == 0) {
        return ENOMEM;
    }

    // Create Pipe, some params are useless
    (*read_file) = Allocate_File(&Pipe_Read_Ops, 0, 0, pipe, 0, 0);
    (*write_file) = Allocate_File(&Pipe_Write_Ops, 0, 0, pipe, 0, 0);

    return 0;
}

int Pipe_Read(struct File *f, void *buf, ulong_t numBytes) {
    struct Pipe *pipe = (struct Pipe*)f->fsData;
    char* dst = (char *)buf;
    char* src = (char *)pipe->buffer;

    if (pipe->p_write == pipe->p_read) {
        if (pipe->writers != 0) {
            return EWOULDBLOCK;
        }
        else {
            return 0;
        }
    }

    ulong_t i;
    for (i = 0; i < pipe->bytes_used && i < numBytes; i++) {
        dst[i] = src[(pipe->p_read + i) % PIPE_BUFFER_SIZE];
    }
    pipe->bytes_used -= i;
    pipe->p_read += i;

    return i;
}

int Pipe_Write(struct File *f, void *buf, ulong_t numBytes) {
    ulong_t p;
    struct Pipe *pipe = (struct Pipe*)f->fsData;
    char* dst = (char *)pipe->buffer;
    char* src = (char *)buf;

    if (pipe->readers == 0) {
        return EPIPE;
    }
    if (numBytes + pipe->bytes_used > PIPE_BUFFER_SIZE) {
        return ENOMEM;
    }

    // Write to Pipe buffer
    for (p = 0; p < numBytes; p++) {
        dst[(pipe->p_write + p) % PIPE_BUFFER_SIZE] = src[p];
    }

    pipe->bytes_used += numBytes;
    pipe->p_write = (pipe->p_write + numBytes) % PIPE_BUFFER_SIZE;
    return numBytes;
}

int Pipe_Close(struct File *f) {
    struct Pipe *pipe = f->fsData;
    if (pipe->readers == 0 || pipe->writers == 0) {
        return 0;
    }
    if (f->ops->Read != NULL) {
        pipe->readers -= 1;
    }
    else if (f->ops->Write != NULL) {
        pipe->writers -= 1;
    }
    return 0;
}
