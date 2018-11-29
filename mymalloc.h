
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
	int free;
};

#define SEGMENT_SIZE 4096
#define META_SIZE sizeof(struct block_meta)
#define ALIGNMENT 16

static struct block_meta *head_block = NULL;
static struct block_meta *tail_block = NULL;
static size_t segment_free = 0;

void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);


static struct block_meta *append_block(size_t size);
static int extend_segment(size_t size);
static void replace_free(struct block_meta *free_block, size_t size);
static struct block_meta *find_free(size_t size);
static struct block_meta *ptr_to_block(void *ptr);
static size_t find_offset(char *start);


#endif
