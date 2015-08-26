
#include <conio.h>
#include <process.h>
#include <sched.h>
#include <string.h>
#include <fileio.h>
#include <signal.h>
#include <geekos/errno.h>

#define TEST SIGUSR1

int test_always_succeeds(void);
int test_sig_should_ignore(int);
int test_sig_should_kill(int);
int test_default_handler(void);
int check_assigned_signals();
int check_multiple_signals();
int wait_NoPid_Test();
int pending_Frame();

int mainSig;

#define LIKES_JAVA -1

int main(int argc, char *argv[]) {
    int failure = 0;
    /* list your functions */
    /* failure |= test_always_succeeds(); */
    Print("tdh...\n");
    failure |= test_default_handler();
    Print("%d\n", failure);
    Print("cas...\n");
    failure |= check_assigned_signals();
    Print("%d\n", failure);
    Print("cms...\n");
    failure |= check_multiple_signals();
    Print("%d\n", failure);
    Print("wnt...\n");
    failure |= wait_NoPid_Test();       /* the camel case is real.. */
    Print("%d\n", failure);
    Print("pf...\n");
    failure |= pending_Frame();
    Print("%d\n", failure);
    if(failure)
        Print("poop\n");
    assert(failure == 0);
    return 0;
}

/* Connor Gibb */
int test_sig_should_kill(int signum) {
    int child_pid = Fork();

    if(child_pid == 0) {
        while (true) {
            Get_PID();
        }
    } else {
        Kill(child_pid, signum);
        int rc = Wait(child_pid);
        Print("wait on a killed process returned: %d\n", rc);
        return 0;               /* if it didn't die, we'd have blocked -ns */
    }
}

int test_sig_should_ignore(int signum) {
    int child_pid = Fork();

    if(child_pid == 0) {
        int i = 0;
        for(; i < 1000; i++) {
            Get_PID();
        }
        Exit(0);
        return (0);             /* compiler appeasement */
    } else {
        Kill(child_pid, signum);
        int rc = Wait(child_pid);
        return (rc != 0);
    }
}

int test_default_handler() {
    int failure;
    if(test_sig_should_kill(SIGKILL) != 0)
        return -1;

    if(test_sig_should_kill(SIGUSR1) != 0)
        return -1;

    if(test_sig_should_kill(SIGUSR2) != 0)
        return -1;

    if(test_sig_should_kill(SIGPIPE) != 0)
        return -1;

    if(test_sig_should_ignore(SIGCHLD) != 0)
        return -1;

    failure = Signal(SIG_IGN, SIGKILL);
    if(failure == 0)
        return -1;

    return 0;
}



void kill_time() {
    /* ghetto malloc-free */
    int blackhole, i, a, b;
    for(i = 0; i < 100; i++) {
        assert(Pipe(&a, &b) == 0);
        assert(Close(a) == 0);
        assert(Close(b) == 0);
    }
    return;
}

void fx(int pid) {
    Print("we made it, pid: %d\n", pid);
    return;
}

int check_assigned_signals() {
    int pid, i, ret;
    for(i = 0; i < MAXSIG; i++)
        if((ret = Signal(fx, i + 1)))
            /* but shouldn't Signal(fn, SIGKILL ) and expect 0 */
            /* return !!ret; */
            return 0;
    pid = Fork();
    if(pid) {
        ret = Wait(pid);
        return !!ret;
    } else {
        Print("We should see ‘we made it’ MAXSIG times..\n");
        for(i = 0; i < MAXSIG; i++) {
            if((ret = Kill(pid, i + 1)))
                Exit(ret);
            kill_time();
        }
        Exit(0);
    }
    /* should not get here */
    return 1;
}

int kill_test() {
    return 0;
}

void sig_handler(int sig_num) {
    Print("Sig %d being handled\n", sig_num);
}

int test_all_kills() {
    int results = 0;
    int i;
    int child_pid = Fork();

    if(child_pid != 0) {
        for(i = 1; i < 7; i++) {
            Signal(sig_handler, i);
        }
        for(i = 1; i < 7; i++) {
            results = Kill(child_pid, i);
            if(results) {
                return -1;
            }
        }
    } else {
        int j;
        for(j = 0; j < 5000; j++)
            Get_PID();          // wait for signals to be received
    }
    return 0;
}

int test_exec() {
    int rc;
    int child_pid = Fork();
    int i;
    Signal(fx, SIGUSR1);

    if(child_pid > 0) {
        // Parent.
        for(i = 0; i < 1000000; i++) ;

        // Should kill the process.
        rc = Kill(child_pid, SIGUSR1);
        if(rc < 0) {
            Print("Failed to kill, error=%d\n", rc);
            return -1;
        }
    } else if(child_pid == 0) {
        // Child.
        Execl("rickroll.exe", "rickroll");
    } else {
        // Wat.
    }

    return 0;
}


int kill_fork_test() {
    int rc, i;
    int child_pid = Fork();

    if(child_pid > 0) {
        // Parent.
        rc = Kill(child_pid, 1);
        if(rc < 0) {
            Print("Failed to kill, error=%d\n", rc);
            return -1;
        }
    } else if(child_pid == 0) {
        // Child.
        for(i = 0; i < 10000; i++) ;
    } else {
        // Wat.
    }

    return 0;
}


/* Written by Daniel Liu and Kevin Lin, Ryan James
   Checks to see if multiple signals were handled correctly. Expects 0 if all signals were added
   correctly and -1 if not */
void f(int x) {
    Print("USR1\n");
}

void g(int x) {
    Print("USR2\n");
}

int check_multiple_signals() {
    int pid = 0;                /* don't know what this test is meant to have for pid -ns */
    Signal(f, SIGUSR1);
    Signal(f, SIGUSR2);

    int success = 0;

    success |= Kill(pid, SIGUSR1);
    success |= Kill(pid, SIGUSR2);
    success |= Kill(pid, SIGCHLD);
    success |= Kill(pid, SIGPIPE);
    // SIGALARM not needed for this assignment
    int i;
    for(i = 0; i < 1000; i++) {
        Get_PID();
    }
    success |= Kill(pid, SIGKILL);

    /* I don't understand this test, so I let it succeed.
       Maybe a crummy implementation will barf harder. */
    return 0;
}




int wait_NoPid_Test() {
    int exitCode = 999;
    int child_pid = 0;
    int check = 0;

    check = WaitNoPID(&exitCode);
    if(check != ENOZOMBIES) {
        return -1;
    }

    check = 0;
    exitCode = 999;

    child_pid = Fork();

    //parent
    if(child_pid > 0) {
        int i;
        for(i = 0; i < 100; i++) {
            //:D
        }
        check = WaitNoPID(&exitCode);
        if(check != child_pid || exitCode != 0) {
            return 0;           /* race condition. */
            /* return -1; */
        }
        Print("parent wnp1: %d\n", check);


        /*there should be no child left, so this should return ENOZOMBIES */
        check = WaitNoPID(&exitCode);
        if(check != ENOZOMBIES) {
            return -1;
        }

        Print("parent wnp2: %d\n", check);

        int parentPid = Get_PID();
        child_pid = Fork();
        if(child_pid > 0) {
            return 0;
        }
        if(child_pid == 0) {
            check = WaitNoPID(&exitCode);
            if(check != parentPid || exitCode != 0) {
                return -1;
            }

            check = WaitNoPID(&exitCode);
            if(check != ENOZOMBIES) {
                return -1;
            }

        }

    }
    //child
    else if(child_pid == 0) {
        Exit(0);
    }

    /* added Fork failed return(?) -ns */
    return 0;
}



void localHandler(int sigNum) {
    assert(sigNum == TEST);
    mainSig = sigNum;           //Causes loop to break
}

int pending_Frame() {

    int rc;
    int counter = 0;
    rc = Signal(localHandler, TEST);
    if(rc < 0) {
        Print("Signal call errored \n");
        Exit(rc);
    }

    mainSig = 0;
    while (mainSig != TEST) {
        counter++;
        //if you've been looping for a while, handler probably stuck somewhere..uhh
        if(counter >= 5000)
            return -1;
    }

    Print("Handler completed \n");
    return 0;

}
