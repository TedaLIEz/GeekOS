#include <process.h>
#include <conio.h>
#include <signal.h>
#include <sched.h>
#include <string.h>

static int done1 = 0;

void alarm_handler(int n) {
    Print("Signal alarm hanlder\n");
    done1++;
}

void alarm_handler2(int n) {
    done1++;
    Alarm(1);
}

void alarmTest1() {
    int i;
    Alarm(1);
    for(i = 0; i < 1000000; i++) ;
    if(done1 == 1) {
        Print("PASS: alarm 1 - deliver one alarm in 1 mili-second\n");
    } else {
        Print("FAIL: alarm 1 - deliver one alarm in 1 mili-second\n");
    }
}

void alarmTest2() {
    int i;
    Alarm(10000);
    for(i = 0; i < 1000000; i++) ;
    if(done1 == 0) {
        Print
            ("PASS: alarm 2 - did not deliver an alarm before its time\n");
    } else {
        Print("FAIL: alarm 2 - delivered an alarm before its time\n");
    }
}

void alarmTest3() {
    int i;

    // set alarm
    Alarm(10);

    // clear alarm
    Alarm(0);
    for(i = 0; i < 1000000; i++) ;

    if(done1 == 0) {
        Print("PASS: alarm 3 - canceled alarm\n");
    } else {
        Print("FAIL: alarm 3 - canceled alarm\n");
    }
}

void alarmTest4() {
    int i;

    // set alarm
    Alarm(10);

    // clear alarm
    Alarm(0);

    // double cancel an alarm
    Alarm(0);
    for(i = 0; i < 1000000; i++) ;

    if(done1 == 0) {
        Print("PASS: alarm 4 - double canceled alarm\n");
    } else {
        Print("FAIL: alarm 4 - double canceled alarm\n");
    }
}

void alarmTest5() {
    int i;

    // set alarm
    Alarm(1);

    // reset alarm
    Alarm(2);
    for(i = 0; i < 1000000; i++) ;

    if(done1 == 1) {
        Print("PASS: alarm 5 - double set alarm\n");
    } else {
        Print("FAIL: alarm 5 - double set alarm\n");
    }
}

void alarmTest6() {
    int i;

    Signal(alarm_handler2, SIGALARM);

    // set alarm
    Alarm(1);

    for(i = 0; i < 10000000; i++) ;

    if(done1 > 10) {
        Print("PASS: alarm 6 - reenable alarm worked\n");
    } else {
        Print("FAIL: alarm 6 - reenable alarm only called %d\n", done1);
    }
}

int main(int argc, char **argv) {
    Signal(alarm_handler, SIGALARM);

    if(argc < 2) {
        Print("Usage: alarm <test>\n");
        Exit(-1);
    }

    switch (atoi(argv[1])) {
        case 1:
            alarmTest1();
            break;

        case 2:
            alarmTest2();
            break;

        case 3:
            alarmTest3();
            break;

        case 4:
            alarmTest4();
            break;

        case 5:
            alarmTest5();
            break;

        case 6:
            alarmTest6();
            break;

        default:
            Print("Invalid test %d requested\n", atoi(argv[1]));
    }

    return 0;
}
