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
		char *start = sbrk(0);
		size_t head_offset = find_offset(start);
		void *request = extend_segment(size+head_offset+META_S)
		if (!extend_segment(size + head_offset + META_SIZE))
		{
			return NULL; // sbrk error
		}

		// init head_block
		head_block = (struct block_meta *)(start + head_offset);
		head_block->size = size;
		head_block->next = NULL;
		head_block->prev = NULL;
		head_block->offset = head_offset;
		head_block->free = 0;
		head_block->garbage = 0;

		tail_block = head_block;

		segment_free -= head_offset + META_SIZE + size;

		return head_block + 1;
	}

	struct block_meta *free_block = find_free(size);

	if (free_block)
	{
		replace_free(free_block,size);
		return free_block + 1;
	}

	if (!append_block(size))
	{
		char *request_start = (char *)tail_block + META_SIZE + tail_block->size;
		request_start += tail_block->garbage;
		size_t new_offset = find_offset(request_start);

		if (!extend_segment(size + new_offset + META_SIZE))
		{
			return NULL; //sbrk error
		}

		append_block(size);
		segment_free -= new_offset + META_SIZE + size;
	}
	return tail_block + 1;
}


void free(void *ptr) 
{
	if (!ptr)
	{
		return;
	}
	
	struct block_meta *free_block = find_block(ptr);
	free_block->free = 1;
	free_block->size += free_block->garbage;
	free_block->garbage = 0;

}


void *calloc(size_t nmemb, size_t size) 
{
	if (!nmemb || !size)
	{
		return NULL;
	}
	size_t total_size = nmemb * size;
	void *block_ptr = malloc(total_size);
	memset(block_ptr, 0, total_size); 
	return block_ptr;
}

void *realloc(void *ptr, size_t size)
{

	test_realloc();

	if (!ptr) 
	{
		return malloc(size);
	}

	if (!size && ptr)
	{
		free(ptr);
		return ptr;
	}
	
	struct block_meta *block = find_block(ptr);
	ssize_t diff = block->size - size;
	
	if (diff > 0) // smaller allocation size
	{
		block->garbage = diff;
		block->size = size;
		return block + 1;
	}
	else if (diff < 0) // larger allocation size
	{
	/*
		// check for space in garbage 
		if ((block->size + block->garbage) >= size)
		{
			block->garbage -= size - block->size;
			block->size = size;
			return block + 1;
		}
	*/
		// check for tail_block 
	/*
		if (block == tail_block)
		{
			if (segment_free >= (size - block->size))
			{
				segment_free -= (size - block->size);
				block->size = size;
				return block + 1;
			}
		}
*/
/*
		// check for exisitng free blocks
		struct block_meta *free_block = find_free(size);
		if (free_block) 
		{
			replace_free(free_block, size);
			memcpy(ptr, free_block + 1, block->size);
			free(ptr);
			return free_block + 1;
		}
*/

		void *ret_ptr = malloc(size);
		memcpy(ptr, ret_ptr, block->size);
		free(ptr);
		return ret_ptr;
	}
	return ptr;  // size = block->size

}

/* ---------- HELPERS ---------- */

static size_t find_offset(char *start)
{
	/* find size of offset (in bytes) for a block */
	size_t offset = ALIGNMENT - ((intptr_t)(start + META_SIZE) % ALIGNMENT);
	if (offset == 16)
	{
		return 0;
	}
	return offset;
}

static void *extend_segment(size_t size)
{
	/* incrase size of data segment to fit new block size
	* returns 1 on success
	* returns NULL on sbrk error */

	int factor = ((size - segment_free) / SEGMENT_SIZE) + 1;
	void *request_break;

	for (int i = factor; i > 0; i--)
	{
		request_break = sbrk(SEGMENT_SIZE);
		if (request_break == (void *) -1)
		{
			return 0; //sbrk error
		}
		segment_free += SEGMENT_SIZE;
	}

	return request_break;
}

static struct block_meta *find_free(size_t size)
{
	/* searches for freed block of large enough size
	* returns ptr to block if found
	* returns NULL otherwise */

	struct block_meta *current = head_block;
	while (current && !(current->free && current->size >= size))
	{
		current = current->next;
	}
	return current;
}

static void replace_free(struct block_meta *block, size_t size)
{
	/* replaces a free block w block of new size
	 * adds extra bytes in allocation to garbage */

	size_t rem_mem = block->size - size;
	block->garbage = rem_mem;
	block->size = size;
	block->free = 0;

}

static int append_block(size_t size)
{
	/* appends a new block to end of list
	* returns 0 if insufficent segment memory
	* returns 1 otherwise (block appended successfully) */

	char *request_start = (char *) tail_block;
	request_start += META_SIZE + tail_block->size + tail_block->garbage;
	size_t request_offset = find_offset(request_start);

	if (segment_free < (request_offset + META_SIZE + size))
	{
		return 0;
	}

	request_start += request_offset;
	struct block_meta *new_block = (struct block_meta *) request_start;
	new_block->size = size;
	new_block->next = NULL;
	new_block->prev = tail_block;
	new_block->offset = request_offset; 
	new_block->free = 0;
	new_block->garbage = 0;

	tail_block->next = new_block;
	tail_block = new_block;

	segment_free -= (request_offset + META_SIZE + size);

	return 1;
}

static struct block_meta *find_block(void *ptr)
{
	/* returns block associated with particular offset 
	 * returns NULL if no block found ? */

	char *found_start = (char *) ptr - META_SIZE;
	struct block_meta *found_block = (struct block_meta *) found_start;
	return found_block;
}

static void test_realloc()
{
	int p = 12;
}
