int main(int argc, char **argv) {
    int retVal;
    int sysNum, typeNum;
    sysNum = 0;                 // modified SYS_NULL
    typeNum = 4;                // our Check_Paging

    __asm__ __volatile__("int $0x90":"=a"(retVal):"a"(sysNum),
                         "D"(typeNum));

    return 0;
}
