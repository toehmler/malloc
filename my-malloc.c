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

		tail_block = head_block;

		segment_free -= META_SIZE + head_offset + size;
		
		return (void *) head_block->data_ptr;
	} 

	struct block_meta *free_block = find_free(size);

	if (free_block)
	{
		insert_block(free_block, size);
		return (void *) free_block->data_ptr;
	}

	if (!append_block(size))
	{
		void *new_block = (void *)(tail_block->data_ptr + tail_block->size);
		size_t new_offset = find_offset(new_block);

		if (!extend_segment(size + new_offset))
		{
			return NULL; //sbrk error
		}

		append_block(size);
		segment_free -= META_SIZE + new_offset + size;
	}
	
	return (void *) tail_block->data_ptr;

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

static size_t find_offset(void *block_ptr)
{
	/* find size of offset (in bytes) for a block */

	char *start = (char *)block_ptr + META_SIZE;
	size_t offset = ALIGNMENT - (atoi(start) % ALIGNMENT);
	if (offset == 16)
	{
		return 0;
	}
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
			return 0; //sbrk error
		}
		segment_free += SEGMENT_SIZE;
	}

	return 1;
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

static void insert_block(struct block_meta *block, size_t size)
{
	/* replaces freed block with new block size
	 * creates new block if sufficent memory remains */

	ssize_t free_mem = block->size - size;
	void *request_block = block->data_ptr + size;
	size_t request_offset = find_offset(request_block);
	ssize_t request_size = free_mem - (META_SIZE + request_offset);

	block->size = size;
	block->free = 0;

	if (request_size > 0)
	{
		struct block_meta *new_block = request_block;
		new_block->size = (size_t) request_size;
		new_block->next = block->next;
		new_block->prev = block;
		new_block->data_ptr = (char *)request_block + META_SIZE + request_offset;
		new_block->free = 1;

		block->next->prev = new_block;
		block->next = new_block;
	}
}

static int append_block(size_t size)
{
	/* appends a new block to end of list
	 * returns 0 if insufficent segment memory
	 * returns 1 otherwise (block appended successfully) */

	void *request_block = tail_block->data_ptr + tail_block->size;
	size_t request_offset = find_offset(request_block);

	if ((segment_free - (META_SIZE + request_offset + size)) < 0)
	{
		return 0;
	} 
	
	struct block_meta *new_block = request_block;
	new_block->next = NULL;
	new_block->prev = tail_block;
	new_block->data_ptr = (char *) request_block + META_SIZE + request_offset;
	new_block->free = 0;

	tail_block->next = new_block;
	tail_block = new_block;

	segment_free -= META_SIZE + request_offset + size;

	return 1;
}
