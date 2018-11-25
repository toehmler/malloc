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
#include <string.h>

struct block_meta
{
    size_t size, offset, garbage;
    struct block_meta *next, *prev;
    int free;     // (free = 1) : block is free
};

#define SEGMENT_SIZE 1024
#define META_SIZE sizeof(struct block_meta)
#define ALIGNMENT 16
#define MIN_SEGMENT_SIZE 64

static struct block_meta *head_block = NULL;
static struct block_meta *tail_block = NULL;
static size_t segment_free = 0;

void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);

/* ---------- HELPERS ---------- */

static size_t find_offset(char *start);
static int extend_segment(size_t size);
static struct block_meta *find_free(size_t size);
static void insert_block(struct block_meta *block, size_t size);
static int append_block(size_t size);
static struct block_meta *find_block(void *ptr);
static void merge_free_blocks(struct block_meta *first, struct block_meta *second);
static void insert_free_block(size_t size, struct block_meta *block);

#endif
