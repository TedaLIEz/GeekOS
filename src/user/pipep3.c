#include <conio.h>
#include <process.h>
#include <sched.h>
#include <sema.h>
#include <string.h>
#include <fileio.h>
#include <geekos/errno.h>       /* ensure not /usr/include/errno.h */

int main(int argc, char **argv) {
    /* ensures that the file descriptors are allocated starting with the
       first ones available, then are reclaimed by close */

    /* we don't have stdin / stdout / stderr, so the first few file
       descriptors are available */
    int read_fd, write_fd;
    int pipe_retval = Pipe(&read_fd, &write_fd);
    assert(pipe_retval == 0);
    assert(read_fd == 0);
    assert(write_fd == 1);

    Close(0);
    Close(1);

    pipe_retval = Pipe(&read_fd, &write_fd);
    assert(pipe_retval == 0);
    assert(read_fd == 0);
    assert(write_fd == 1);

    Close(0);
    Close(1);

    return 0;
}
