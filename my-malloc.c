 /*
  * my-malloc.c
  * Trey Oehmler
  * CS315 Assignment 3 Fall 2018
  * 
  */

#include "mymalloc.h"

void *malloc(size_t size)
{
	/* initializes head block /
	 * checks for free blocks, calls replace_free()
	 * if neccesary calls append_block() */
	struct block_meta *ret_block;

	if (!size) // size = 0
	{
		return NULL;
	}
	if (!head_block) // first call
	{
		//init head_block to current location of break
		char *start = sbrk(0);
		size_t head_offset = find_offset(start);

		if (!extend_segment(size + META_SIZE + head_offset))
		{
			return NULL; //sbrk error
		}
		
		// init head_block fields
		start += head_offset;
		head_block = (struct block_meta *)(start);
		head_block->size = size;
		head_block->next = NULL;
		head_block->prev = NULL;
		head_block->offset = head_offset;
		head_block->free = 0;
		head_block->garbage = 0;

		tail_block = head_block;

		segment_free -= (head_offset + META_SIZE + size);

		ret_block = head_block;
	}

	// look for free blocks 
	ret_block = find_free(size);
	if (ret_block)
	{
		replace_free(ret_block, size);
	}
	else // otherwise append to list of allocations
	{
		ret_block = append_block(size);
		if (!ret_block) 
		{
			return NULL;
		}
	}
	ret_block += 1;
	return (void *)ret_block;
}

void free(void *ptr)
{
	/* sets meta free flag to 1
	 * adds any garbage in allocation to its size
	 * reduces garbage count for block to 0 */
	if (!ptr)
	{
		return;
	}

	struct block_meta *free_block = ptr_to_block(ptr);
	free_block->free = 1;
	free_block->size += free_block->garbage;
	free_block->garbage = 0;
	
	/*
	if (free_block->next->free)
	{
		merge_free(free_block);
	}
	if (free_block->prev->free)
	{
		merge_free(free_block->prev);
	}
	*/
	
}

void *calloc(size_t nmemb, size_t size) 
{
	/* calls malloc(nmemb * size) 
	 * sets all addresses in allocation 0 */
	if (!nmemb || !size)
	{
		return NULL;
	}
	size_t total_size = (nmemb * size);
	void *block_ptr = malloc(total_size);
	memset(block_ptr, 0, total_size);
	return block_ptr;
}


void *realloc(void *ptr, size_t size)
{
	/* changes size of allocation pointed to by ptr
	 * if large allocation size:
	 * checks for room in garbage to extend
	 * checks for free blocks and copies data if found
	 * otherwise calls malloc for new size and frees old block */

	struct block_meta *block = ptr_to_block(ptr);
	
	if (!size && ptr)
	{
		free(ptr);
		return NULL;	
	}
	if (!ptr)
	{
		return malloc(size);
	}
	if (size < block->size)
	{
		block->garbage += (block->size - size);
		block->size = size;
		return (void *)(block + 1);
	}
	if (size > block->size)
	{
		if ((block->garbage + block->size) >= size)
		{
			block->garbage = ((block->garbage + block->size) - size);
			block->size = size;
			return (void *)(block + 1);
		}
		void *ret_ptr = malloc(size);
		if (!ret_ptr) 
		{
			return NULL;
		}
		memcpy(ret_ptr, ptr, block->size);
		free(ptr);
		return ret_ptr;
	}
	return ptr;
}

/*
static void merge_free(struct block_meta *block)
{
	block->size += (block->next->offset + META_SIZE + block->next->size);
	struct block_meta *next_block = block->next->next;
	next_block->prev = block;
	block->next = next_block;
	test_merge();
}
*/

static struct block_meta *append_block(size_t size)
{
	/* appends a block to the end fo the list
	 * extends segment if neccesary by calling extend_segment */

	char *request_start = (char *)tail_block + META_SIZE;
	request_start += (tail_block->size + tail_block->garbage);
	size_t request_offset = find_offset(request_start);
	
	if (segment_free < (request_offset + META_SIZE + size))
	{
		if (!extend_segment(request_offset + META_SIZE + size))
		{
			return NULL; //sbrk error
		}
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

	return tail_block;
}

static int extend_segment(size_t size)
{
	/* incrase size of data segment to fit new block size
	* returns 1 on success
	* returns NULL on sbrk error */
	void *request_break;
	while (segment_free < size)
	{
		request_break = sbrk(SEGMENT_SIZE);
		if (request_break == (void *) -1)
		{
			return 0;
		}
		segment_free += SEGMENT_SIZE;
	}
	return 1;
	

}


static void replace_free(struct block_meta *free_block, size_t size)
{
	/* replace a free block with an allocation of new size
	 * set garbage of block to difference between old - new sizes */
	free_block->garbage = free_block->size - size;
	free_block->size = size;
	free_block->free = 0;
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

static struct block_meta *ptr_to_block(void *ptr)
{
	/* perform pointer arithmetic to return block_meta ptr */
	return ((struct block_meta*)ptr) - 1;
}

static size_t find_offset(char *start)
{
	/* calculate offset in bytes for start of block to align memory */
	size_t offset = ALIGNMENT - ((intptr_t)(start + META_SIZE) % ALIGNMENT);
	if (offset == 16)
	{
		return 0;
	}
	return offset;
}

