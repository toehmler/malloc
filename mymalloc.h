/*
  * my-malloc.h
  * Trey Oehmler
  * CS315 Assignment 3 Fall 2018
  * 
  */



#ifndef __MYMALLOC_H
#define __MYMALLOC_H

#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

struct block_meta
{
    size_t size;
    struct block_meta *next, *prev;
    char *data_ptr;                         // char * size useful in arithmetic
    int free;                               // (free = 1) : block is free
};

#define SEGMENT_SIZE 1024
#define META_SIZE sizeof(struct block_meta)
#define ALIGNMENT 16

static struct block_meta *head_block = NULL;
static struct block_meta *tail_block = NULL;
static size_t segment_free = 0;

void *malloc(size_t size);
/*
void free(void *ptr);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
*/

/* ---------- HELPERS ---------- */

static size_t find_offset(void *block_ptr);
static int extend_segment(size_t size);
static struct block_meta *find_free(size_t size);
static void insert_block(struct block_meta *block, size_t size);
static int append_block(size_t size);

#endif
