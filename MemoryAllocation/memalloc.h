//
//  memalloc.h
//  MemoryAllocation
//
//  Created by Justin Miller on 12/2/14.
//  Copyright (c) 2014 Justin Miller. All rights reserved.
//

#ifndef MemoryAllocation_segment_h
#define MemoryAllocation_segment_h

#define FREE 0
#define ALLOCATED 1
#define MAXMEMORY 1024

typedef struct segment {
    int start;
    int size;
    int status;
    char data[1];
    struct segment* next;
    struct segment* prev;
    void* ptr;
} segment_t ;

segment_t* mem_alloc(int);
void mem_free(segment_t*);

int getTotalSize();
segment_t* getBase();

#endif
