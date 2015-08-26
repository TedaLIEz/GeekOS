#include <conio.h>
#include <process.h>
#include <sched.h>
#include <sema.h>
#include <string.h>
#include <fileio.h>
#include <geekos/errno.h>       /* ensure not /usr/include/errno.h */

void dostuff() {
    int i, j;
    int read_fds[10], write_fds[10];
    int read_bytes, written_bytes, pipe_retval;
    char buf[1034];

    for(i = 0; i < 1034; i++) {
        buf[i] = i;
    }

    for(i = 0; i < 5; i++) {
        pipe_retval = Pipe(&read_fds[i], &write_fds[i]);
        /* Print("pipe pair %d allocated to %d and %d, ret %d\n", i, read_fds[i], write_fds[i], pipe_retval); */
        assert(pipe_retval == 0);
        assert(read_fds[i] >= 0);
        assert(write_fds[i] >= 0);
    }

    pipe_retval = Pipe(&read_fds[i], &write_fds[i]);
    /* Print("excess pipe pair %d allocated to %d and %d, ret %d\n", i, read_fds[i], write_fds[i], pipe_retval); */
    assert(pipe_retval < 0);

    for(i = 0; i < 5; i++) {
        written_bytes = Write(write_fds[i], buf + i, 1024);
        if(written_bytes != 1024) {
            Print("wrote %d bytes instead of 1024 to pipe %d\n",
                  written_bytes, i);
        }
        assert(written_bytes == 1024);
    }

    for(i = 0; i < 5; i++) {
        read_bytes = Read(read_fds[i], buf, 1024);
        assert(read_bytes == 1024);
        for(j = 0; j < 1024; j++) {
            if(buf[j] != (char)(j + i)) {
                Print("mismatch at iter %d index %d, buf[j] = %d\n", i, j,
                      buf[j]);
            }
            assert(buf[j] == (char)(j + i));
        }
    }

    for(i = 0; i < 5; i++) {
        Close(write_fds[i]);
        Close(read_fds[i]);
    }
}


int main(int argc, char **argv) {
    int i;
    for(i = 0; i < 1000; i++) {
        dostuff();
    }
    Print("bababooey\n");
    return 0;
}
