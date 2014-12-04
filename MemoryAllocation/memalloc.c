//
//  memalloc.c
//  MemoryAllocation
//
//  Created by Justin Miller on 12/2/14.
//  Copyright (c) 2014 Justin Miller. All rights reserved.
//

#include "memalloc.h"
#include <unistd.h>
#include <sys/types.h>

#define align4(x) (((((x) -1)>>2)<<2)+4)
#define SEGMENT_SIZE sizeof(struct segment)

int totalSize = 0;

segment_t* start = NULL;

segment_t* fusion(segment_t* segment) {
    if (segment->next && segment->next->status == FREE) {
        segment->size += segment->next->size;
        segment->next = segment->next->next;
        if (segment->next) {
            segment->next->prev = segment;
        }
    }
    return segment;
}

segment_t* find_segment(segment_t** last , int size){
    segment_t* segment = start;
    while (segment && !(segment->status == FREE && segment->size >= size )) {
        *last = segment;
        segment = segment->next;
        }
    return segment;
}

segment_t* extend_heap(segment_t* last, int size) {
    int sb;
    segment_t* segment;
    segment = sbrk(0);
    sb = (int)sbrk(SEGMENT_SIZE + size);
    if (sb < 0) {
        return (NULL);
    }
    segment->size = size;
    segment->next = NULL;
    segment->prev = last;
    if (last) {
        last->next = segment;
    }
    segment->status = ALLOCATED;
    return segment;
}

void split_segment(segment_t* segment, int size) {
    segment_t* new;
    new = (segment_t*)segment->data + size;
    new->size = segment->size - size - SEGMENT_SIZE;
    new->next = segment->next;
    new->prev = segment;
    new->status = FREE;
    segment->size = size;
    segment->next = new;
    if (new->next) {
        new->next->prev = new;
    }
}

int getTotalSize() {
    return totalSize;
}

segment_t* getBase() {
    return start;
}

segment_t* mem_alloc(int size) {
    segment_t* segment;
    segment_t* lastAllocated;
    int allignedSize;
    allignedSize = align4(size);
    if (start) {
        lastAllocated = start;
        segment = find_segment(&lastAllocated, allignedSize);
        if (segment) {
            if ((segment->size - allignedSize) >= (SEGMENT_SIZE + 4)) {
                split_segment(segment, allignedSize);
            }
            segment->status = ALLOCATED;
        } else {
            segment = extend_heap(lastAllocated, allignedSize);
            if (!segment) {
                return NULL;
            }
        }
    } else {
        segment = extend_heap(NULL, allignedSize);
        if (!segment) {
            return NULL;
        }
        start = segment;
    }
    totalSize += segment->size;
    return segment;
}

void mem_free(segment_t* segment) {
    segment->status = FREE;
    totalSize -= segment->size;
    if (segment->prev && segment->prev->status == FREE) {
        segment = fusion(segment->prev);
    }
    if (segment->next) {
        fusion(segment);
    } else {
        if (segment->prev) {
            segment->prev->next = NULL;
        } else {
            start = NULL;
        }
        brk(segment);
    }
}