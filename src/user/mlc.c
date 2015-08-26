/*
 * Copyright (c) 2001,2003,2004 David H. Hovemeyer <daveho@cs.umd.edu>
 * Copyright (c) 2003,2013,2014 Jeffrey K. Hollingsworth <hollings@cs.umd.edu>
 *
 * All rights reserved.
 *
 * This code may not be resdistributed without the permission of the copyright holders.
 * Any student solutions using any of this code base constitute derviced work and may
 * not be redistributed in any form.  This includes (but is not limited to) posting on
 * public forums or web sites, providing copies to (past, present, or future) students
 * enrolled in similar operating systems courses the University of Maryland's CMSC412 course.
 */

/* this is a test version of the program, which checks to
   ensure that the user program itself is loaded properly,
   including all pages, including pages that should be
   zeroed before getting assigned to our process.  */

#include <conio.h>
#include <string.h>
#include <process.h>
#include <malloc.h>
#include <libuser.h>            /* for Malloc in the integrity check */

#define PARANOIA

unsigned int Quiet = 0;
extern void *_Entry(), *_end;
int integrity_check_enabled;

int check_static() {
    static int f;
    if(f == 0) {
        return 1;
    } else {
        return 0;
    }
}


void Check_Integrity_Or_Die() {
    static unsigned int stored_code_checksum;
    static void *top;

    if(!integrity_check_enabled) {
        return;
    }

    if(top == 0x0) {
        top = Malloc;
        if(top < (void *)snprintf) {
            top = snprintf;
        }
    }

    int *p;
    unsigned int working_checksum = 0;
    if((void *)_Entry >= top) {
        Print("Code Integrity Check assumption failed.\n");
        if((void *)_Entry >= (void *)0x1000) {
            Print("Detail: _Entry %p.\n", &_Entry);
        }
    }
    if((void *)top <= (void *)0x2000) {
        Print("Detail: top %p.\n", top);
    }
    if((void *)top >= (void *)0x3000) {
        Print("Detail: top %p.\n", top);
    }
    /* the following can trigger if pages don't get bzeroed,
       at least based on student-reported symptoms.  I don't
       understand the mechanism by which Malloc / snprintf
       acquire an apparent value so large because of it.  */
    if(top > (void *)&top) {
        Print
            ("Code Integrity Check assumption failed: may fault accessing pages above the text (code) and globals.\n");
        Print("Detail: top %p exceeds &top %p.\n", top, &top);
    }
    if((char *)top < (char *)&top - 0x2000) {
        static int warned;
        if(!warned) {
            Print("Code Integrity Check is Weak.\n");
            Print("Detail: top %p way lower than &top %p.\n", top, &top);
            warned = true;
        }
    }

    if((void *)_Entry < (void *)0x1000) {
        Print("Code Integrity Check Internal error _Entry too small: %p",
              _Entry);
        integrity_check_enabled = false;
        return;
    }

    for(p = (int *)_Entry; p < (int *)top; p++) {
        working_checksum += *p;
    }
    if(stored_code_checksum == 0) {
        stored_code_checksum = working_checksum;
    } else if(stored_code_checksum != working_checksum) {
        Print("Code Integrity Check Failed.\n");
        Print("Detail: Code Integrity Check Failed %x != %x.\n",
              stored_code_checksum, working_checksum);
        Exit(-1);
    }
}


int main(int argc, char **argv) {
    /* change recurse to 5-10 to see stack faults without page outs */
    unsigned int pages = 512;
    unsigned int iterations = 100;

    if(check_static()) {
        integrity_check_enabled = 1;
    } else {
        Print("Integrity check disabled, since statics are busted.\n");
        integrity_check_enabled = 0;
    }


    Check_Integrity_Or_Die();

    if(argc > 1) {
        pages = atoi(argv[1]);
        Print("Pages is %d\n", pages);
        if(argc > 2) {
            iterations = atoi(argv[2]);
            if(iterations == 0) {
                Quiet = 1;
                iterations = 100;
            } else {
                Print("Iterations is %d\n", iterations);
                iterations++;   /* considering how used below. */
            }
        }
    }


    unsigned int *region = Malloc(pages * 4096);
    if(region == NULL) {
        Print("failed to allocate %d pages\n", pages);
        return 1;
    }

    Print("testing last byte...\n");
    /* confirm that it's all legit. */
    ((char *)region)[pages * 4096 - 1] = '0';
    Print("success.\n");

    unsigned int stride = 1;
    unsigned int i;
    unsigned int pagey;
    for(i = 0; i < pages; i += 1) {
        pagey = ((i * stride) % pages) * 1024;
        region[pagey] = pagey;
    }
    for(stride = 1; stride < iterations; stride++) {
        for(i = 0; i < pages; i += 1) {
            pagey = ((i * stride) % pages) * 1024;
            assert(region[pagey] == pagey);
        }
        assert(i == pages);
        assert(stride < iterations);    /* detect corruption */
        if(stride % 5 == 1 || iterations < 10)
            Print("%d...", stride);
    }

    Print("Mlc %d success\n", pages);

    return 0;
}
