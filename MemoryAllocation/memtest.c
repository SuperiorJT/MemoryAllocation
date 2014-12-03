//
//  memtest.c
//  MemoryAllocation
//
//  Created by Justin Miller on 12/2/14.
//  Copyright (c) 2014 Justin Miller. All rights reserved.
//

#include <stdio.h>
#include "memalloc.h"

int main(int argc, const char * argv[]) {
    int exit = 0;
    char input;
    while (!exit) {
        
        printf("Welcome to MemTest!\n\n");
        printf("1. Allocate Memory\n");
        printf("2. Free Memory\n");
        printf("3. Exit\n\n");
        
        input = getchar();
        
        if (input == '1') {
            int size = 0;
            printf("Enter the size you would like to allocate: ");
            scanf("%d", &size);
            getchar();
            if ((size + getTotalSize()) > MAXMEMORY) {
                printf("Not enough space for %d bytes!\n", size);
            } else {
                segment_t* segment;
                segment = mem_alloc(size);
                if (!segment) {
                    printf("Memory could not be allocated!\n");
                } else {
                    printf("A segment of %d bytes has been allocated at %d\n", segment->size, segment);
                }
            }
        }
        
        if (input == '2') {
            segment_t* next;
            int count = 0;
            int indexInput;
            if (!getBase()) {
                printf("You must allocate before you can free!\n");
                getchar();
            } else {
                printf("Here is a list of segment pointers:\n\n");
                next = getBase();
                while (next) {
                    count++;
                    char* allocated;
                    if (next->status == ALLOCATED) {
                        allocated = "Allocated";
                    } else {
                        allocated = "Free";
                    }
                    printf("%d. %d | %d Bytes | %s\n", count, next, next->size, allocated);
                    next = next->next;
                }
                printf("\nType the index for the pointer you wish to free: ");
                scanf("%d", &indexInput);
                getchar();
                count = 1;
                next = getBase();
                while (next) {
                    if (count == indexInput) {
                        mem_free(next);
                        break;
                    } else {
                        count++;
                        next = next->next;
                    }
                }
                if (count == indexInput && next) {
                    printf("Segment \"%d\" has been freed! %d bytes are freed!\n", next, next->size);
                } else {
                    printf("That segment was not found!\n");
                }
            }
        }
        
        if (input == '3') {
            if (getBase()) {
                segment_t* next = getBase();
                while (next) {
                    mem_free(next);
                    next = next->next;
                }
            }
            exit = 1;
        }
        
    }
    return 0;
}
