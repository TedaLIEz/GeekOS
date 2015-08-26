#include <conio.h>
int main() {
    const char *foo = "abcdefg";
    ((char *)foo)[0] = 'q';
    Print("FAIL: should have aborted before getting here.\n");
    return 1;
}
