#include <stddef.h>
#include <signal.h>
#include <string.h>
#include <conio.h>

int main(int argc, char *argv[]) {
    if(argc != 3)
        return -1;

    int pid, result;

    char *signal;

    pid = atoi(argv[1]);
    signal = argv[2];

    if(strcmp(signal, "SIGKILL") == 0) {
        result = Kill(pid, SIGKILL);
    } else if(strcmp(signal, "SIGUSR1") == 0) {
        result = Kill(pid, SIGUSR1);
    } else if(strcmp(signal, "SIGUSR2") == 0) {
        result = Kill(pid, SIGUSR2);
    } else if(strcmp(signal, "SIGALARM") == 0) {
        result = Kill(pid, SIGALARM);
    } else {
        Print("Invalid signal %s\n", signal);
        return -1;
    }

    if(result != 0) {
        Print("Kill ret %d\n", result);
    }

    return result;

}
