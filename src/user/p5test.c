/*
 * Automated test program for Project 5
 * Copyright (c) 2004 David H. Hovemeyer <daveho@cs.umd.edu>
 * Copyright (c) 2004 Iulian Neamtiu <neamtiu@cs.umd.edu>
 *
 * All rights reserved.
 *
 * This code may not be resdistributed without the permission of the copyright holders.
 * Any student solutions using any of this code base constitute derviced work and may
 * not be redistributed in any form.  This includes (but is not limited to) posting on
 * public forums or web sites, providing copies to (past, present, or future) students
 * enrolled in similar operating systems courses the University of Maryland's CMSC412 course.
 *
 * $Revision: 1.3 $
 * 
 */

#include <conio.h>
#include <process.h>
#include <fileio.h>
#include <string.h>
#include <geekos/errno.h>
#include <geekos/projects.h>

int doTest(const char *testName, int (*testFunction) (), int points,
           int *score, int *totalTests, int *successfulTests) {
    int ret;

    (*totalTests)++;

    Print("Testing: %s...", testName);

    ret = testFunction();

    if(ret < 0)
        Print("FAILED (%d)", ret);
    else {
        Print("PASSED (%d)", ret);
        (*score) += points;
        (*successfulTests)++;
    }

    Print(" crt score: %d \n", (*score));

    return ret;

}

int ttestFormat() {
    // if (PROJECT_CFS) {
    // return Format("ide1", "cfs");
    // } else 
    if(PROJECT_GOSFS) {
        int ret = Format("ide1", "gosfs");
        Print("format returned %d\n", ret);
        return ret;
    } else {
        Print("skipped format test for gfs2 project");
        return 0;
    }
}

int ttestMount() {
    int rc;
    rc = Mount("ide1", "/d", "gfs3");
    /* ns15 partially fixed to chain. dislike the preprocessor switches. */
    if(rc == ENOFILESYS) {
        if(PROJECT_CFS) {
            return Mount("ide1", "/d", "cfs");
        } else if(PROJECT_GFS2) {
            return Mount("ide1", "/d", "gfs2");
        } else {
            return Mount("ide1", "/d", "gosfs");
        }
    }
    return rc;
}

int tOpenInexistentFile() {
    return (Open("/d/InexistentFile", O_READ) < 0) ? 1 : -1;
}

int tCreat() {
    int retC;

    int fd = Open("/d/somefile", O_CREATE | O_READ);
    if(fd < 0) {
        Print("failed to creat: %d", fd);
        return -1;
    }

    retC = Close(fd);

    return (retC >= 0) ? 1 : -1;
}

int tCreatLongFilename() {
    int retC;

    int fd =
        Open("/d/somePrettyLongFileNameToBeCreated", O_CREATE | O_READ);
    if(fd < 0)
        return -1;

    retC = Close(fd);

    return (retC >= 0) ? 1 : -1;
}

int tCreatInexistentPath() {
    int fd;

    fd = Open("/d/InexistentPath/file", O_CREATE | O_READ);

    return (fd < 0) ? 1 : -1;
}

int tBasicReadWrite() {
    int fd, retW, retR;
    char buffer[100];

    fd = Open("/d/basic", O_CREATE | O_WRITE);
    if(fd < 0)
        return -1;

    retW = Write(fd, buffer, 10);
    if(retW < 0)
        return -1;

    Close(fd);

    fd = Open("/d/basic", O_READ);
    if(fd < 0)
        return -1;

    retR = Read(fd, buffer, 10);
    if(retR < 0)
        return -1;

    Close(fd);

    return ((retW == 10) || (retR == 10)) ? 1 : -1;
}


int tReadFromWriteOnly() {
    int fd, retR, retW;
    char buffer[100];

    fd = Open("/d/basic2f", O_CREATE | O_WRITE);
    if(fd < 0)
        return -1;

    retW = Write(fd, buffer, 10);
    if(retW < 0)
        return -1;

    Close(fd);

    fd = Open("/d/basic2f", O_WRITE);
    if(fd < 0)
        return -1;

    retR = Read(fd, buffer, 10);

    Close(fd);

    return (retR < 0) ? 1 : -1;
}

int tWriteToReadOnly() {
    int fd, retW;
    char buffer[100];

    fd = Open("/d/basic3f", O_CREATE | O_WRITE);
    if(fd < 0)
        return -1;

    retW = Write(fd, buffer, 10);
    if(retW < 0)
        return -1;

    Close(fd);

    fd = Open("/d/basic3f", O_READ);
    if(fd < 0)
        return -1;

    retW = Write(fd, buffer, 10);

    Close(fd);

    return (retW < 0) ? 1 : -1;
}

int tCloseTwice() {
    int fd, retC;

    fd = Open("/d/basic4f", O_CREATE | O_WRITE);
    if(fd < 0)
        return -1;

    Close(fd);

    retC = Close(fd);

    return (retC < 0) ? 1 : -1;
}


int tCloseAberrantFd() {
    int retC;

    retC = Close(100000);

    return (retC < 0) ? 1 : -1;
}

int tBasicDelete() {
    int fd, retD;

    fd = Open("/d/basic5f", O_CREATE | O_WRITE);
    if(fd < 0)
        return -1;

    Close(fd);

    retD = Delete("/d/basic5f", false);

    return (retD >= 0) ? 1 : -1;
}

int tDeleteInexistentFile() {
    int retD;

    retD = Delete("/d/InexistentFile2", false);

    return (retD < 0) ? 1 : -1;
}

int tBasicCreateDirectory() {
    int retC;

    retC = Create_Directory("/d/dir1d");

    return (retC >= 0) ? 1 : -1;
}

int tRecursivelyCreateDirectory() {
    int retC;

    retC = Create_Directory("/d/dir2d");

    if(retC < 0) {
        Print("failed to create directory /d/dir2d: %d\n", retC);
        return -1;
    }

    retC = Create_Directory("/d/dir2d/dir3d");
    if(retC < 0) {
        Print("failed to create directory /d/dir2d/dir3d: %d\n", retC);
        return -1;
    }

    return (retC >= 0) ? 1 : -1;
}

int tFileInRecursivelyCreatedDirectory() {
    int fd = Open("/d/dir2d/dir3d/file4f", O_CREATE | O_READ);
    Close(fd);

    return (fd >= 0) ? 1 : -1;
}


int tDeleteEmptyDirectory() {
    int retC, retD;

    retC = Create_Directory("/d/dir3d");
    if(retC < 0)
        return -1;

    retD = Delete("/d/dir3d", false);

    return (retD >= 0) ? 1 : -1;
}


int tDeleteNonEmptyDirectory() {
    int retC, retD, fd;

    retC = Create_Directory("/d/dir4d");
    if(retC < 0)
        return -1;

    fd = Open("/d/dir4d/file", O_CREATE | O_READ);
    if(fd < 0)
        return -1;

    Close(fd);

    retD = Delete("/d/dir4d", false);

    return (retD >= 0) ? -1 : 1;
}

int tBasicSeek() {
    int fd, retW, retS1, retS2;
    char buffer[11];

    fd = Open("/d/basic6f", O_CREATE | O_WRITE);
    if(fd < 0)
        return -1;

    retW = Write(fd, buffer, 10);
    if(retW < 0)
        return -1;

    retS1 = Seek(fd, 0);
    retS2 = Seek(fd, 9);

    Close(fd);
    Delete("/d/basic6f", false);

    return ((retS1 >= 0) && (retS2 >= 0)) ? 1 : -1;
}

int tSeekReread() {
    int fd, retW, retR, retS;
    char buffer[11] = "0123456789\0", buffer2[2], buffer3[2];

    fd = Open("/d/basic7f", O_CREATE | O_WRITE);
    if(fd < 0)
        return -1;

    retW = Write(fd, buffer, 10);
    if(retW < 0)
        return -1;

    Close(fd);

    fd = Open("/d/basic7f", O_READ);
    if(fd < 0)
        return -1;

    retS = Seek(fd, 0);
    if(retS < 0)
        return -1;

    retR = Read(fd, buffer2, 1);
    if(retR < 0)
        return -1;

    retS = Seek(fd, 9);
    if(retS < 0)
        return -1;

    retR = Read(fd, buffer3, 1);
    if(retR < 0)
        return -1;

    Close(fd);
    Delete("/d/basic7f", false);

    return ((buffer2[0] == '0') && (buffer3[0] == '9')) ? 1 : -1;
}

int tBasicStat() {
    int fd, retS;
    struct VFS_File_Stat s;

    fd = Open("/d/basic8f", O_CREATE | O_WRITE);
    if(fd < 0)
        return -1;

    Close(fd);

    fd = Open("/d/basic8f", O_READ);
    if(fd < 0)
        return -1;

    retS = FStat(fd, &s);

    Close(fd);
    Delete("/d/basic8f", false);

    return (retS >= 0) ? 1 : -1;
}

int tStatFile() {
    int fd, retW, retS;
    char buffer[11];
    struct VFS_File_Stat s;

    fd = Open("/d/basic9f", O_CREATE | O_WRITE);
    if(fd < 0)
        return -1;

    retW = Write(fd, buffer, 10);
    if(retW < 0)
        return -1;

    Close(fd);

    retS = Stat("/d/basic9f", &s);

    Delete("/d/basic9f", false);

    return ((retS >= 0) && (s.size == 10)) ? 1 : -1;
}

int tStatDirectory() {
    int fd, retS, retC;
    struct VFS_File_Stat s;

    retC = Create_Directory("/d/basic10d");
    if(retC < 0)
        return -1;

    fd = Open_Directory("/d/basic10d");
    if(fd < 0)
        return -1;

    retS = FStat(fd, &s);

    Close(fd);
    Delete("/d/basic10d", false);

    return ((retS >= 0) && (s.isDirectory)) ? 1 : -1;
}

int tReadEntry() {
    int fd, retR, retC;
    struct VFS_Dir_Entry dirEntry;

    retC = Create_Directory("/d/basic11d");
    if(retC < 0) {
        Print("tReadEntry: couldn't create basic11d: %d\n", retC);
        return -1;
    }

    retC = Create_Directory("/d/basic11d/d1");
    if(retC < 0) {
        Print("tReadEntry: couldn't create basic11d/d1: %d\n", retC);
        return -1;
    }

    retC = Create_Directory("/d/basic11d/d2");
    if(retC < 0) {
        Print("tReadEntry: couldn't create basic11d/d2: %d\n", retC);
        return -1;
    }

    fd = Open("/d/basic11d/f1", O_CREATE);
    if(fd < 0) {
        Print("tReadEntry: couldn't create basic11d/f1: %d\n", fd);
        return -1;
    }

    Close(fd);

    fd = Open_Directory("/d/basic11d");
    if(fd < 0) {
        Print("tReadEntry: couldn't opendir basic11d: %d\n", fd);
        return -1;
    }

    do {
        retR = Read_Entry(fd, &dirEntry);
    } while (retR == 0 && dirEntry.name[0] == '.');

    if((retR < 0) ||
       (strncmp(dirEntry.name, "d1", 2) != 0) ||
       (!dirEntry.stats.isDirectory)) {
        Print("tReadEntry: expected directory d1, found %s %s\n",
              dirEntry.stats.isDirectory ? "directory" : "file",
              dirEntry.name);
        return -1;
    }

    retR = Read_Entry(fd, &dirEntry);

    if((retR < 0) ||
       (strncmp(dirEntry.name, "d2", 2) != 0) ||
       (!dirEntry.stats.isDirectory)) {
        Print("tReadEntry: expected directory d2, found %s %s\n",
              dirEntry.stats.isDirectory ? "directory" : "file",
              dirEntry.name);
        return -1;
    }

    retR = Read_Entry(fd, &dirEntry);

    if((retR < 0) ||
       (strncmp(dirEntry.name, "f1", 2) != 0) ||
       (dirEntry.stats.isDirectory)) {
        Print("tReadEntry: expected file f1, found %s %s\n",
              dirEntry.stats.isDirectory ? "directory" : "file",
              dirEntry.name);
        return -1;
    }

    Close(fd);

    fd = Open_Directory("/d/basic11d");
    if(fd < 0) {
        Print("tReadEntry: failed to open directory again\n");
        return -1;
    }
#ifdef TESTED_FILESYSTEM_SUPPORTS_SEEK_DIR
    retR = Seek(fd, 2);
    if(retR < 0) {
        Print("tReadEntry: failed to seek in directory: %d\n", retR);
        return -1;
    }

    retR = Read_Entry(fd, &dirEntry);

    if((retR < 0) ||
       (strncmp(dirEntry.name, "f1", 2) != 0) ||
       (dirEntry.stats.isDirectory)) {
        Print("tReadEntry: expected after seek file f1, found %s %s\n",
              dirEntry.stats.isDirectory ? "directory" : "file",
              dirEntry.name);
        return -1;
    }
#endif

    Close(fd);
    Delete("/d/basic11d/d1", false);
    Delete("/d/basic11d/d2", false);
    Delete("/d/basic11d/f1", false);

    Delete("/d/basic11d", false);
    return 1;
}


int tFiveMegs() {
    int fd, retW, retR, retS;
    int i, j;
    char buffer[1000], buffer2[1000];
    int ret = 1;

    for(i = 0; i < 1000; i++)
        buffer[i] = buffer2[i] = i & 0xff;

    fd = Open("/d/bigfile", O_CREATE | O_WRITE);
    if(fd < 0)
        return -1;

    Print("Writing in first 5MB at random ... \n");

    for(i = 0; i < 50; i++) {
        //retS = Seek(fd, 100000 * i);

        /* nspring didn't comment out the above line, but did comment out the stuff that tests 
           whether the return value was okay 
           if (retS < 0 )
           {
           ret = -1;
           break;
           }
         */

        retW = Write(fd, buffer, 1000);
        if(retW != 1000) {
            ret = -1;
            break;
        }
        Print(" %d", i);
    }

    Close(fd);

    if(ret < 0) {
        Delete("/d/bigfile", false);
        return -1;
    }

    Print("Reading back from first 5MB ... \n");

    fd = Open("/d/bigfile", O_READ);

    if(fd < 0)
        return -1;

    for(i = 0; i < 50; i++) {
        //retS = Seek(fd, 100000 * i);
        retS = 0;               /* no, I don't know what this is about, or why the call to seek has been commented, but 
                                   it does seem like retS should be initialized. -ns */

        if(retS < 0) {
            ret = -1;
            break;
        }

        retR = Read(fd, buffer, 1000);
        if(retR != 1000) {
            ret = -1;
            break;
        }

        for(j = 0; j < 1000; j++) {
            if(buffer[j] != buffer2[j]) {
                ret = -1;
                break;
            }
        }

        Print(" %d", i);
    }

    Close(fd);

    Delete("/d/bigfile", false);

    return ret;
}


unsigned int blocksize = 4096;
unsigned int disksize_mb = 10;

int tExhaustDisk() {
    /* (indirect + direct) * blocksize div by B/KB */
    unsigned long max_file_size_k =
        ((blocksize / 4) + 4) * blocksize / 1024;
    unsigned int files_needed_to_fill_disk =
        disksize_mb * 1024 * 63 / 64 / max_file_size_k;
    unsigned int files_needed_to_use_inodes = 0;
    size_t i;
    unsigned int fi;
    int retC, retW, retD;
    char writeme[512];
    char fname[50];
    char dirname[25] = "";
    int repetition;


    Print("need %d files to fill disk, %d to use all inodes\n",
          files_needed_to_fill_disk, files_needed_to_use_inodes);

    if(files_needed_to_fill_disk > files_needed_to_use_inodes) {
        files_needed_to_fill_disk = files_needed_to_use_inodes;
    }
#define MARK  Print("%d:", __LINE__);
    retC = Create_Directory("/d/exhaust");
    if(retC != 0) {
        Print("couldn't create /d/exhaust\n");
        return -1;
    }

    for(i = 0; i < sizeof(writeme); i++) {
        writeme[i] = i % 256;
    }

    for(repetition = 0; repetition < 3; repetition++) {
        unsigned int files_worked_on;
        retW = 0;
        for(fi = 0; retW >= 0 && fi < files_needed_to_fill_disk; fi++) {
            int fd;
            unsigned long b;

            if(fi % 100 == 0) {
                snprintf(dirname, 25, "/d/exhaust/%d", fi / 100);
                retC = Create_Directory(dirname);
                Print("%d/%d", fi, files_needed_to_fill_disk);
            }

            snprintf(fname, 50, "%s/%d", dirname, fi);
            Print((fi % 25 == 0) ? ":" : ".");

            fd = Open(fname, O_WRITE | O_CREATE);
            if(fd < 0) {
                Print("failed to open %s\n", fname);
                return -1;
            }

            for(b = 0; b < max_file_size_k * 1024 - 100; b += retW) {
                retW = Write(fd, writeme, 1024);
                if(retW < 0) {
                    Print("write %s %lu failed: %d", fname, b, retW);
                    break;
                }
            }

            if(Close(fd) < 0) {
                Print("failed to close");
                return -1;
            }
        }
        files_worked_on = fi;
        for(fi = 0; fi < files_worked_on; fi++) {
            snprintf(dirname, 25, "/d/exhaust/%d", fi / 100);
            snprintf(fname, 50, "%s/%d", dirname, fi);
            (void)Delete(fname, false);
        }
        for(fi = 0; fi < files_worked_on; fi += 100) {
            snprintf(dirname, 25, "/d/exhaust/%d", fi / 100);
            if(Delete(dirname, false) < 0) {
                Print("couldnt remove %s", dirname);
                return -1;
            }
        }

    }

    retD = Delete("/d/exhaust", false);
    if(retD != 0) {
        Print("failed to remove /d/exhaust: %d", retD);
        return retD;
    }

    Print("PASSL Exhaust disk\n");
    return 0;
}

int tWriteReread(int howManyKBs, char const *fileName) {
    int fd, retW, retR;
    int i, j;
    char buffer[100], buffer2[100];
    int ret = 1;

    for(j = 0; j < 100; j++)
        buffer[j] = j;

    fd = Open(fileName, O_CREATE | O_WRITE);
    if(fd < 0)
        return -1;

    for(i = 0; i < (howManyKBs * 10); i++) {
        retW = Write(fd, buffer, 100);
        if(retW != 100) {
            ret = -1;
            break;
        }
        if(i % 50 == 0)
            Print(" %d", i);
    }

    Close(fd);

    if(ret != -1) {
        fd = Open(fileName, O_READ);
        if(fd < 0)
            return -1;

        for(i = 0; i < (howManyKBs * 10); i++) {
            retR = Read(fd, buffer2, 100);

            if(retR != 100) {
                ret = -1;
                break;
            }

            for(j = 0; j < 100; j++) {
                if(buffer2[j] != j) {
                    ret = -1;
                    break;
                }
            }

            if(ret < 0)
                break;

            if(i % 50 == 0)
                Print(" %d", i);
        }

        Close(fd);
        Delete(fileName, false);
    }

    return ret;
}

int t10KWriteReread() {
    return tWriteReread(10, "/d/file_10k");
}

int t100KWriteReread() {
    return tWriteReread(100, "/d/file_100k");
}


int main(int argc __attribute__ ((unused)), char **argv
         __attribute__ ((unused))) {


    int score = 0;
    int totalTests = 0;
    int successfulTests = 0;

    // 0
    if(PROJECT_GOSFS || PROJECT_CFS)
        doTest("Format", ttestFormat, 3, &score, &totalTests,
               &successfulTests);

    // 1
    doTest("Mount", ttestMount, 1, &score, &totalTests, &successfulTests);
    // 2
    doTest("Open-Inexistent File", tOpenInexistentFile, 1, &score,
           &totalTests, &successfulTests);
    // 3
    doTest("Creat", tCreat, 2, &score, &totalTests, &successfulTests);
    // 4
    doTest("Creat-Long Filename", tCreatLongFilename, 3, &score,
           &totalTests, &successfulTests);
    // 5
    doTest("Creat-Inexistent Path", tCreatInexistentPath, 3, &score,
           &totalTests, &successfulTests);
    // 6
    doTest("Basic Read/Write", tBasicReadWrite, 5, &score, &totalTests,
           &successfulTests);

    // 7
    doTest("Read from WriteOnly", tReadFromWriteOnly, 3, &score,
           &totalTests, &successfulTests);
    // 8
    doTest("Write to ReadOnly", tWriteToReadOnly, 3, &score, &totalTests,
           &successfulTests);

    // 9
    doTest("Close Twice", tCloseTwice, 3, &score, &totalTests,
           &successfulTests);
    // 10
    doTest("Close Illegal FD", tCloseAberrantFd, 1, &score, &totalTests,
           &successfulTests);

    // 11
    doTest("Basic Delete", tBasicDelete, 3, &score, &totalTests,
           &successfulTests);
    // 12
    doTest("Delete-Inexistent File", tDeleteInexistentFile, 1, &score,
           &totalTests, &successfulTests);

    // 13
    doTest("Basic Create Directory", tBasicCreateDirectory, 3, &score,
           &totalTests, &successfulTests);
    // 14
    doTest("Recursively Create Directory", tRecursivelyCreateDirectory, 5,
           &score, &totalTests, &successfulTests);
    // 15
    doTest("Create File in Recursively Created Directory",
           tFileInRecursivelyCreatedDirectory, 3, &score, &totalTests,
           &successfulTests);

    // 16
    doTest("Basic Seek", tBasicSeek, 2, &score, &totalTests,
           &successfulTests);
    // 17
    doTest("Seek w/ Reread", tSeekReread, 5, &score, &totalTests,
           &successfulTests);

    // 18
    doTest("Basic Stat", tBasicStat, 2, &score, &totalTests,
           &successfulTests);
    // 19
    //doTest( "Stat-File", tStatFile, 2,  &score, &totalTests, &successfulTests);
    // 20
    doTest("Stat-Directory", tStatDirectory, 2, &score, &totalTests,
           &successfulTests);

    // 21
    doTest("Delete Empty Directory", tDeleteEmptyDirectory, 3, &score,
           &totalTests, &successfulTests);
    // 22
    doTest("Delete Non-Empty Directory", tDeleteNonEmptyDirectory, 2,
           &score, &totalTests, &successfulTests);

    // 23
    doTest("10k Write/Reread", t10KWriteReread, 5, &score, &totalTests,
           &successfulTests);
    // 24
    doTest("100k Write/Reread", t100KWriteReread, 7, &score, &totalTests,
           &successfulTests);
    // 25
    doTest("Read Entry", tReadEntry, 4, &score, &totalTests,
           &successfulTests);
    // 26
    doTest("5 MB Write", tFiveMegs, 8, &score, &totalTests,
           &successfulTests);

    // 31
    doTest("Fill Disk", tExhaustDisk, 8, &score, &totalTests,
           &successfulTests);

    Print("********************************************\n");
    Print("Tests attempted: %d. Passed: %d. Failed: %d\n", totalTests,
          successfulTests, (totalTests - successfulTests));
    Print("SCORE: %d\n", score);

    return 0;
}
