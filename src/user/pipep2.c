#include <conio.h>
#include <process.h>
#include <sched.h>
#include <sema.h>
#include <string.h>
#include <fileio.h>
#include <geekos/errno.h>       /* ensure not /usr/include/errno.h */

#define K_TO_WRITE 5

int main(int argc, char **argv) {
    int i, j;
    int read_fd, write_fd;
    int read_bytes, written_bytes, pipe_retval;
    char buf[1024];

    for(i = 0; i < 1024; i++) {
        buf[i] = i;
    }

    /* Print("calling pipe"); */
    pipe_retval = Pipe(&read_fd, &write_fd);
    assert(pipe_retval == 0);

    for(i = 0; i < K_TO_WRITE; i++) {
        written_bytes = Write(write_fd, buf, 1024);
        assert(written_bytes == 1024);
    }

    for(i = 0; i < K_TO_WRITE; i++) {
        read_bytes = Read(read_fd, buf, 1024);
        assert(read_bytes == 1024);
        for(j = 0; j < 1024; j++) {
            if(buf[j] != (char)j) {
                Print("mismatch at iter %d index %d, buf[j] = %d\n", i, j,
                      buf[j]);
            }
            assert(buf[j] == (char)j);
        }
    }

    read_bytes = Read(read_fd, buf, 256);
    if(read_bytes != EWOULDBLOCK) {
        Print
            ("FAIL: expected EWOULDBLOCK (%d), got %d reading at the end\n",
             EWOULDBLOCK, read_bytes);
        assert(read_bytes == EWOULDBLOCK);
    }

    Close(write_fd);

    read_bytes = Read(read_fd, buf, 256);
    if(read_bytes != 0) {
        Print
            ("FAIL: read should return 0 (not %d) when all writers are closed.\n",
             read_bytes);
        assert(read_bytes == 0);
    }

    Close(read_fd);

    written_bytes = Write(write_fd, "beep", 4);
    assert(written_bytes <= -1);

    read_bytes = Read(read_fd, buf, 256);
    assert(read_bytes <= -1);

    return 0;
}
