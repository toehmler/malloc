/*
  * my-malloc.c
  * Trey Oehmler
  * CS315 Assignment 3 Fall 2018
  * 
  */


#define META_SIZE sizeof(block_meta)
#define SEGMENT_SIZE 1024
#define ALIGNMENT 16
#define PADDING ALIGNMENT - 1



struct block_meta 
{
	size_t size, offset;
	struct block_meta *next, *prev;
	int free;
};


static void *head_block = NULL;
static void *tail_block = NULL;
static size_t segment_free = 0;


void *malloc(size_t size) 
{
	

	// return NULL on size = 0
	if (size == 0) 
	{
		return NULL;
	}
	// initalize first segment
	if (head_block == NULL)
	{
		head_block = sbrk(0); 

		head_block->offset = find_offset(size);

		if (!extend_segment(size + offset))
		{
			return NULL; // sbrk error
		}

		head_block->size = size;
		head_block->next = NULL;
		head_block->prev = NULL;
		head_block->free = 0;
		head_block->offset = find_offset(size);

		segment_free -= head_block->size + head_block->offset + META_SIZE;

		tail_block = head_block;

		return (head_block + 1);
	}

	// check for any exsisting free blocks

	size_t offset = find_offset(size);
	struct block_meta *free_block = find_free(size + offset);
	
	if (free_block) // freed block exists within segment
	{
		insert_block(free_block);
		return free_block;
	}
	else // no freed blocks within segment
	{
		if (segment_free < (size + META_SIZE + offset))
		{
			if (!extend_segment(size + offset))
			{
				return NULL; // sbrk error
			}
		}
		append_block(size);
		return tail_block;

	}

	struct block_meta *current = head_block;
	while (current && !(current->free && current->size >= size))
	{
		current = current->next;
	}

	if (!current) // no free blocks exsist
	{
		if (segment_free >= (size + sizeof(block_meta) + (ALIGNMENT - 1)))
		{
			struct block_meta *new_block;
			new_block = tail_block + sizeof(block_meta) + tail_block->size + tail_block->offset;
			new_block->size = size;
			new_block->next = NULL;
			new_block->prev = tail_block;
			tail_block->next = new_block;
			tail_block = new_block;

			void *data_start = new_block + 1;
			size_t offset = 16 - ((data_start + new_block->size) % 16);
			new_block->offset = offset;

			segment_free -= sizeof(block_meta) + new_block->size + new_block->offset;
		} 
		else 
		{
			if (!extend_size(size))
			{
				return NULL; // sbrk error
			}
			struct block_meta *new_block;
			new_block = tail_block + sizeof(block_meta) + tail_block->size + tail_block->offset;
			new_block->size = size;
			new_block->next = NULL;
			tail_block->next = new_block;
			tail_block = new_block;

		}
	}
	else
	{


	}

	// check for space within segment

	// extend segment if neccesary

	// insert new block & link to list

}

void free(void *ptr)
{
	// 

}

void *calloc(size_t nmemb, size_t size) 
{

}

void *realloc(void *ptr, size_t size)
{

}

int extend_segment(size_t size) 
{
	void *request;
	size_t request_size = (META_SIZE + size + PADDING) - segment_free;
	size_t factor = (request_size / SEGMENT_SIZE) + 1;

	for (int i = factor; i > 0; i --)
	{
		request = sbrk(SEGMENT_SIZE);
		if (request == (void *) -1)
		{
			return NULL;
		}
	}

	segment_free += (SEGMENT_SIZE * factor);
	return 0;
}

void append_block(size_t size)
{
	struct block_meta *block;
	block = tail_block + meta_size + tail_block->size + tail_block->offset;
	block->size = size;
	block->next = NULL;
	block->prev = tail_block;

	tail_block->next = tail_block;
	tail_block = block;
}

size_t find_offset(size_t size)
{
	void *data_start = head_block + 1;
	size_t offset = ALIGNMENT - ((data_start + block->size) % ALIGNMENT);
	return offset;
}

struct block_meta *find_free(size_t size)
{
	struct block_meta *current = head_block;
	while (current && !(current->free && current->size >= size))
	{
		current = current->next;
	}
	return current;
}