/*
  * my-malloc.h
  * Trey Oehmler
  * CS315 Assignment 3 Fall 2018
  * 
  */

#include <stddef.h>
#include <unistd.h>

#ifndef __MYMALLOC_H
#define __MYMALLOC_H

static struct block_meta
{
    size_t size;
    struct block_meta *next, *prev;
    char *data_ptr;                         // char * size useful in arithmetic
    int free;                               // (free = 1) : block is free
}

#define SEGMENT_SIZE 1024
#define META_SIZE size_of(struct block_meta)
#define ALIGNMENT 16

static struct block_meta *head_block = NULL;
static truct block_meta *tail_block = NULL;
static size_t segment_free = 0;

void *malloc(size_t size);
/*
void free(void *ptr);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
*/

/* ---------- HELPERS ---------- */

static size_t find_offset(struct block_meta *block);
static int extend_segment(size_t size);

#endif
