 /*
  * my-malloc.c
  * Trey Oehmler
  * CS315 Assignment 3 Fall 2018
  * 
  */

 #include "mymalloc.h"

void *malloc(size_t size) 
{

	if (!size) // size = 0 
	{
		return NULL;
	}

	if (!head_block) // first call
	{
		// init head_block to current location or prog break
		head_block = sbrk(0);

		size_t head_offset = find_offset(head_block);

		if (!extend_segment(size + head_offset))
		{
			return NULL; // sbrk error
		}

		// init head_block elements
		head_block->size = size;
		head_block->next = NULL;
		head_block->prev = NULL;
		head_block->data_ptr = ((char *) head_block + META_SIZE + head_offset);
		head_block->free = 0;

		return (char *) head_block->data_ptr;
	} 
	else
	{
		return NULL; // testing only
	}

}
/*

void free(void *ptr) 
{

}

void *calloc(size_t nmemb, size_t size) 
{

}

void *realloc(void *ptr, size_t size)
{

}
*/


/* ---------- HELPERS ---------- */

static size_t find_offset(struct block_meta *block)
{
	/* find size of offset (in bytes) for a block */

	char *start = (char *)block + META_SIZE;
	size_t offset = ALIGNMENT - (start % ALIGNMENT);
	return offset;
}

static int extend_segment(size_t size)
{
	/* incrase size of data segment to fit new block size
	 * returns 1 on success
	 * returns NULL on sbrk error */

	int factor = ((size - segment_free) / SEGMENT_SIZE) + 1;
	void *request_break;

	for (int i = factor; i > 0; i --)
	{
		request_break = sbrk(SEGMENT_SIZE);
		if (request_break == (void *) -1)
		{
			return NULL; //sbrk error
		}
		segment_free += SEGMENT_SIZE:
	}

	return 1;
}