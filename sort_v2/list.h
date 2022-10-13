#pragma once

#include <stdbool.h>

// List opaque type
// We don't want to allow direct access so we hide it behind an opaque identifier
typedef int list_t;

// Sort func pointer type
// First parameter: The list to sort
// Second parameter: The size of the list
typedef void (*sort_func_t)(list_t, int);

/////////////////////////////////////////////////////////////////////
// Simple interface
/////////////////////////////////////////////////////////////////////

// Read one element of the list
// Same meaning as:
// int a = list[index]
int list_read(list_t list, int index);

// Write one element of the list
// Same meaning as:
// list[index] = value
void list_write(list_t list, int index, int value);

// Swap blocks of elements in list
void list_swap_block(list_t list, int index1, int index2, int count);

// Swap one element with another in a list
// Same meaning as:
// swap(&list[index1], &list[index2]);
static inline void list_swap(list_t list, int index1, int index2)
{
    list_swap_block(list, index1, index2, 1);
}

/////////////////////////////////////////////////////////////////////
// Advanced functions (for merge sort)
/////////////////////////////////////////////////////////////////////

// Alloc a new list
list_t list_alloc(int size);

// Free 
void list_free(list_t list);

// Move a block of a list, src and dst can be the same
// Same meaning as;
// memmove(dst + dst_index, src + src_index, count * sizeof(int));
void list_move(list_t dst, list_t src, int dst_index, int src_index, int count);

// Copy a block of a list inside another list
// Same meaning as;
// memcpy(dst + dst_index, src + src_index, count * sizeof(int));
void list_copy(list_t dst, list_t src, int dst_index, int src_index, int count);

/////////////////////////////////////////////////////////////////////
// Testing API
/////////////////////////////////////////////////////////////////////

enum sort_event_type
{
    SE_SWAP,
    SE_READ,
	SE_WRITE,
	SE_MOVE,
	SE_COPY
};

typedef struct sort_event_t
{
    enum sort_event_type type;
    union
    {
        struct swap_event_t
        {
            int id_a;
            int id_b;
            int count;
        } swap;

        struct read_event_t
        {
            int id;
        } read;
    };

	struct write_event_t
	{
		int id;
		int value;
	} write;

	struct move_event_t
	{
		int dst_id;
		int src_id;
		int count;
	} move;

	struct copy_event_t
	{
		int dst_id;
		int src_id;
		int count;
	} copy;
} sort_event_t;

typedef struct stats_t
{
    int swap_count;
    int read_count;

    int write_count;
    int alloc_count;
    int free_count;
    int move_count;
    int copy_count;

    int events_count;
    int events_capacity;
    sort_event_t* events;
} stats_t;

stats_t test_sort(const int* ref_array, int ref_array_size, int max_value, sort_func_t sort_func, bool show_content);
