#include <signal.h>
#include <conio.h>
#include <string.h>

int coolatoi(char *val) {
    int value = 0;
    while (*val >= '0' && *val <= '9') {
        value *= 10;
        value += (*val - '0');
        val++;
    }
    return value;
}

void default_handler() {
    Print("Default Handler\n");
}

int main(int argc, char **argv) {
    char buff[10], *spos;
    int i, rv, p1, p2;
    int handlers[] = { (int)default_handler, 0 };
    for(i = 1; i < argc; i++) {
        if(argv[i][0] == 'r') {
            strcpy(buff, argv[i] + 1);
            *(spos = strchr(buff, 'Q')) = 0;
            rv = Signal((signal_handler) handlers[coolatoi(spos + 1)],
                        coolatoi(buff));
            Print("[%s : %d]\n", argv[i], rv);
        } else if(argv[i][0] == 'k') {
            strcpy(buff, argv[i] + 1);
            *(spos = strchr(buff, 'Q')) = 0;
            rv = Kill(coolatoi(spos + 1), coolatoi(buff));
            Print("[%s : %d]\n", argv[i], rv);
        }

    }
    //  Kill(coolatoi(argv[1]), coolatoi(argv[2]));
    return 0;
}
