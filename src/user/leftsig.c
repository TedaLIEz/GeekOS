#include <conio.h>
#include <process.h>
#include <sched.h>
#include <string.h>
#include <fileio.h>
#include <signal.h>
#include <geekos/errno.h>

/*-----------------------Helper functions-----------------*/

typedef int (*test) (void);     /* --Eric */

int test_always_succeeds(void) {
    return 0;
}

void echo(int in) {
    Print("The signal was %d\n", in);
}


int kill_by_default_helper(int signal_num) {
    int pid = Fork(), status, ret = 0;
    if(pid > 0) {
        // parent code
        Kill(pid, signal_num);
        ret = WaitNoPID(&status);
        if(ret == ENOZOMBIES)
            return 1;
        else if(ret == 0 && status == EINVALID)
            return 0;
        else
            return 1;
    } else if(pid == 0) {
        // child code
        while (1) ;
        // Exit(SOME_WEIRD_VALUE); ?
    } else {
        Print("Screw error handling\n");
    }

    return 0;
}

int nothing_by_default_helper(int signal_num) {
    int pid = Fork(), status, ret = 0, i;
    if(pid > 0) {
        // parent code
        Kill(pid, signal_num);
        ret = WaitNoPID(&status);
        if(ret == ENOZOMBIES)
            return 1;
        else if(ret == 0 && status == 0)
            return 0;
        else
            return 1;
    } else if(pid == 0) {
        // child code
        for(i = 0; i < 50000; ++i) ;    // sleep ...
        Exit(0);
    } else {
        Print("Screw error handling\n");
    }

    return 0;
}

/*------------------END-----------------------*/


/* Brendan Rowan + Matt Bender
 * Test default signal functionality
 */
int test_default_behavior() {
    int default_kills[4] =
        { SIGKILL, SIGPIPE, SIGUSR1, SIGUSR2 }, i, failure = 0;
    for(i = 0; i < 4; ++i)
        failure |= kill_by_default_helper(default_kills[i]);
    return failure;
}


/* Ian Sweet
   Tests basic functionality associated with WaitNoPID() system call.
   Expect 0 on success, -1 on failure.
*/
int test_waitnopid(void) {
    int i, res;

#define TEST_WAITNOPID_LENGTH ((2)+(3))
    for(i = 0; i < TEST_WAITNOPID_LENGTH; i++) {
        if(Fork() == 0)
            Exit(i);
    }

    for(i = 0; i < TEST_WAITNOPID_LENGTH; i++) {
        while ((res = WaitNoPID(NULL)) == ENOZOMBIES) {
            Get_PID();
            Get_PID();
        }
        Print("reaped %d/%d\n", i + 1, TEST_WAITNOPID_LENGTH);
    }
    if((res = WaitNoPID(NULL)) != ENOZOMBIES) {
        Print("expected ENOZOMBIES(%d) got %d\n", ENOZOMBIES, res);
        return -1;
    }
    return 0;
}


// Changed to SIG_IGN by Eric
int Sig_Kill_Return_Test() {
    int res;
    res = Signal(SIG_IGN, SIGKILL);
    if(res != EINVALID) {
        Print
            ("Expected ignoring sigkill to yield EINVALID (%d) instead of %d\n",
             EINVALID, res);
        return -1;
    };
    return 0;
}


/**
*Jason Cookman - Tests whether a handler set to a process behaves as expected.
*/
int Register_Handler_test() {
    Signal(echo, SIGPIPE);
    int pid = Fork();
    if(pid == 0) {              //child
        int i = 0;
        for(; i < 10000; i++) {
            Get_PID();          //a really bad wait function
        }
    } else if(pid > 0) {
        if(Kill(pid, SIGPIPE) < 0) {
            return -1;
        }
    } else {
        Print("Fork failed");
        return -1;
    }

    return 0;
}


/* Kevin Stubbs
 * Test that fork correctly copies the signal handlers for SIGPIPE
 * and correctly frees memory after killing a child. It assumes that
 * WaitNoPid() works correctly. Returns 0 on success, non-zero on
 * failure.
 */
void PipeResponse(int x) {
    Print("Pipe has no readers!\n");
}

int fork_pipe_test() {
    int i = 0, pid = 0, ret = 0, failure = 0;
    int readEnd, writeEnd;

    Signal(PipeResponse, SIGPIPE);
    ret = Pipe(&readEnd, &writeEnd);
    assert(ret == 0);
    Close(readEnd);

    // Make the parent spawn a bunch of processes
    for(i = 0; i < 100; i++) {
        // This call can fail by returning ENOMEM
        pid = Fork();

        // If we’re a child, stop spawning processes
        if(!pid) {
            break;
        }
    }

    // Wait for all of the children to finish
    if(pid) {
        while (WaitNoPID(&ret) != ENOZOMBIES) {
            failure |= ret;
        }
        return failure;
    } else {
        Write(writeEnd, "Neil", 4);     // Should send SIGPIPE
        Exit(0);
        return 0;
    }
}


// Sys_Signal/Kill basics -dhc
// - Tests user defined sig handlers being registered as
//   desired
// Should go inside of main;
// Buildmaster or whatever can put it there
// It’s not me
void usr1Handler(int signo) {
    Print("usr1Handler: received signo %d\n", signo);
}

void usr2Handler(int signo) {
    int x;
    for(x = 0;; x++) {
        if(x == 15)
            break;
    }
    Print("usr2Handler: received signo %d, computed %d\n", signo, x);
}

int test_sys_sig() {
    Signal(usr1Handler, 1);
    Signal(usr2Handler, 2);
    int pid = Fork();

    if(pid) {
        // Parent
        // child may not actually process all sigs depending
        // on scheduling/ order you handle signals
        // i.e., receive all 3, process sigkill first, then
        // sigusr1/2 will not be processed
        return (!(Kill(pid, 2)) && !(Kill(pid, 3)) && !(Kill(pid, 1)));
    } else {
        while (1) {
        }
    }
    return 0;
}

// END Sys_Signal/Kill basics

//
int checkPendingSignal(void) {
    Signal(usr1Handler, 1);
    Signal(usr1Handler, 2);
    Signal(usr1Handler, 1);

    //if it does not crash, then it Check_Pending_Signal should
    //have worked

    return 0;
}

/*Carol Rogan*/
//make sure that the signal number being passed in is less than MAXSIG
int test_signum_inbounds() {
    if(Signal(usr1Handler, 100) == 0) {
        return -1;
    } else {
        return 0;
    }
}


/* Basic harness for writing the tests. --Eric */
int main() {
    int failure = 0, failedTest = -1;
    int i, ret;

/* Add your test function names here, before the NULL --Eric */
    test tests[] = { test_always_succeeds,
        test_waitnopid,
        Sig_Kill_Return_Test,
        test_default_behavior,
        fork_pipe_test,
        test_signum_inbounds,
        test_sys_sig,
        NULL
    };
    for(i = 0; tests[i]; i++) {
        ret = tests[i] ();
        if(ret) {
            failedTest = i;
            failure = ret;
            break;
        }
    }

    if(failure) {
        Print("You suck because of test %d, with error %d.\n", failedTest,
              failure);
        return failure;
    } else {
        Print("Yay you don't suck.\n");
        return 0;
    }

}
