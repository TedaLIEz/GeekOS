#include <conio.h>
#include <process.h>
#include <sched.h>
#include <sema.h>
#include <string.h>
#include <fileio.h>
#include <geekos/errno.h>       /* ensure not /usr/include/errno.h */

int main(int argc, char **argv) {
    int i;
    int read_fd, write_fd;
    int pipe_retval;

    for(i = 0; i < 100000; i++) {
        if(i % 10000 == 0)
            Print(".");
        pipe_retval = Pipe(&read_fd, &write_fd);
        if(pipe_retval != 0 || read_fd < 0 || write_fd < 0 ||
           read_fd > 100 || write_fd > 100) {
            Print("Iteration %d failed\n", i);
        }
        assert(pipe_retval == 0);
        assert(read_fd >= 0);
        assert(write_fd >= 0);
        assert(read_fd <= 100);
        assert(write_fd <= 100);
        Close(read_fd);
        Close(write_fd);
    }

    Print("bababooey\n");
    return 0;
}
