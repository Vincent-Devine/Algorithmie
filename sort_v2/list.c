
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>

#include "list.h"

#define MAX_LISTS 1024

// Global variables used for list_sort_test()
static int* g_lists[MAX_LISTS];
static int g_lists_size[MAX_LISTS];
struct stats_t g_stats;

static bool list_is_valid(list_t list, bool check_for_null)
{
    return (list >= 0 && list < MAX_LISTS) && (!check_for_null || g_lists[list] != NULL);
}

static bool list_check_bounds(list_t list, int index, int count)
{
    bool res = (index >= 0) && (index+count <= g_lists_size[list]);
    if (res == false)
    {
        printf("index = %d, count = %d, list_size = %d\n", index, count, g_lists_size[list]);
        fflush(stdout);
    }

    return res;
}

static sort_event_t* add_event()
{
    if (g_stats.events_count == g_stats.events_capacity)
    {
        if (g_stats.events_capacity == 0)
            g_stats.events_capacity = 1;
        else
            g_stats.events_capacity *= 2;
        g_stats.events = realloc(g_stats.events, g_stats.events_capacity * sizeof(sort_event_t));
    }
    return &g_stats.events[g_stats.events_count++];
}

list_t list_alloc(int size)
{
    int free_index = -1;
    for (int i = 0; i < MAX_LISTS; ++i)
    {
        if (g_lists[i] == NULL)
        {
            free_index = i;
            break;
        }
    }

    if (free_index == -1)
    {
        fprintf(stderr, "No more space to alloc array");
        exit(1);
    }

    g_lists[free_index] = calloc(size, sizeof(int));
    g_lists_size[free_index] = size;

    g_stats.alloc_count++;

    return free_index;
}

void list_free(list_t list)
{
    assert(list_is_valid(list, false));
    free(g_lists[list]);
    g_lists[list] = NULL;

    g_stats.alloc_count++;
}

void list_move(list_t dst, list_t src, int dst_index, int src_index, int count)
{
    assert(list_is_valid(dst, true));
    assert(list_is_valid(src, true));
    assert(list_check_bounds(dst, dst_index, count));
    assert(list_check_bounds(dst, src_index, count));

    int* array_src = g_lists[src];
    int* array_dst = g_lists[dst];
    memmove(array_dst + dst_index, array_src + src_index, count * sizeof(int));

    g_stats.move_count++;
    sort_event_t* event = add_event();
    event->type = SE_MOVE;
    event->move.dst_id = dst_index;
    event->move.src_id = src_index;
    event->move.count  = count;
}

void list_copy(list_t dst, list_t src, int dst_index, int src_index, int count)
{
    assert(list_is_valid(dst, true));
    assert(list_is_valid(src, true));
    assert(list_check_bounds(dst, dst_index, count));
    assert(list_check_bounds(dst, src_index, count));

    int* array_src = g_lists[src];
    int* array_dst = g_lists[dst];
    memcpy(array_dst + dst_index, array_src + src_index, count * sizeof(int));
    g_stats.copy_count++;

    sort_event_t* event = add_event();
    event->type = SE_COPY;
    event->copy.dst_id = dst_index;
    event->copy.src_id = src_index;
    event->copy.count  = count;
}

void list_write(list_t list, int index, int value)
{
    assert(list_is_valid(list, true));
    assert(list_check_bounds(list, index, 1));

    int* array = g_lists[list];
    array[index] = value;

    g_stats.write_count++;
    sort_event_t* event = add_event();
    event->type        = SE_WRITE;
    event->write.id    = index;
    event->write.value = value;
}

int list_read(list_t list, int index)
{
    assert(list_is_valid(list, true));
    assert(list_check_bounds(list, index, 1));

    g_stats.read_count++;

    sort_event_t* event = add_event();
    event->type = SE_READ;
    event->read.id = index;

    int* array = g_lists[list];
    return array[index];
}

void list_swap_block(list_t list, int index1, int index2, int count)
{
    assert(list_is_valid(list, true));
    assert(list_check_bounds(list, index1, count));
    assert(list_check_bounds(list, index2, count));

    int* array = g_lists[list];

    // We swap a buffer with b buffer
    int* a = &array[index1];
    int* b = &array[index2];

    for (int i = 0; i < count; ++i)
    {
        int tmp = *a;
        *a = *b;
        *b = tmp;
        a += 1;
        b += 1;
    }

    g_stats.swap_count++;
    sort_event_t* event = add_event();
    event->type = SE_SWAP;
    event->swap.id_a  = index1;
    event->swap.id_b  = index2;
    event->swap.count = count;
}

int list_comp(const void* a, const void* b)
{
    return (*(int*)a - *(int*)b);
}

static bool list_is_sorted(const int* orig, int* array_sorted, int size)
{
    // Check sort
    for (int i = 1; i < size; ++i)
        if (array_sorted[i-1] > array_sorted[i])
            return false;

    // Check that the sorting is right (using qsort() from stdlib)
    int* copy = malloc(size * sizeof(int));
    memcpy(copy, orig, size * sizeof(int));
    qsort(copy, size, sizeof(int), list_comp);
    if (memcmp(copy, array_sorted, size * sizeof(int)) != 0)
    {
        printf("Array is sorted but some value are wrong\n");
        return false;
    }
    free(copy);

    return true;
}

static void display_array(int* array, int size, bool full_display)
{
    const int nb_values_displayed = 11;
    for (int i = 0; i < size; ++i)
    {
        if (!full_display && size > nb_values_displayed && i == nb_values_displayed / 2)
        {
            i = size - nb_values_displayed / 2;
            printf("... (jump to index %d)\n", i);
        }
        printf("%d\n", array[i]);
    }
}

static int free_lists()
{
    int free_count = 0;
    for (int i = 0; i < MAX_LISTS; ++i)
    {
        if (g_lists[i] != NULL)
        {
            free(g_lists[i]);
            free_count++;
        }
    }

    return free_count;
}

double now()
{
    struct timespec Time ={0,0};
    clock_gettime(CLOCK_MONOTONIC, &Time);
    return (double)Time.tv_sec + 1.0e-9*Time.tv_nsec;
}

stats_t test_sort(const int* array, int array_size, int max_value, sort_func_t sort, bool show_content)
{
    // Reinit counters and lists
    memset(&g_stats, 0, sizeof(struct stats_t));
    memset(g_lists, 0, MAX_LISTS * sizeof(int*));

    // Set this to true to display all elements of the list
    bool full_display = false;

    // Manually set list[0] (list to sort)
    int* array_copy = malloc(array_size * sizeof(int));
    memcpy(array_copy, array, array_size * sizeof(int));

    g_lists[0]      = array_copy;
    g_lists_size[0] = array_size;
    list_t list = 0;

    if (show_content)
    {
        printf("\nBefore:\n");
        display_array(array_copy, array_size, full_display);
    }

    double start_time = now();

    sort(list, array_size);

    double duration = now() - start_time;

    if (show_content)
    {
        printf("\nAfter:\n");
        display_array(array_copy, array_size, full_display);
        printf("\n");
    }

    bool sorted = list_is_sorted(array, array_copy, array_size);
    printf("%s\n", sorted ? "Sorted" : "\033[31mERROR: NOT SORTED!!!!!!!\033[0m");
    printf("Duration: %.6fms\n", duration * 1000.0);
    printf("List size: %d\n", array_size);
    printf("Stats:\n");
    printf(" -  swaps: %7d\n", g_stats.swap_count);
    printf(" -  reads: %7d\n", g_stats.read_count);
    if (g_stats.move_count)
        printf(" -  moves: %7d\n", g_stats.move_count);
    if (g_stats.copy_count)
        printf(" - copies: %7d\n", g_stats.copy_count);
    if (g_stats.write_count)
        printf(" - writes: %7d\n", g_stats.write_count);


    // Manually free list[0]
    free(array_copy);
    g_lists[0] = NULL;

    // Check leaks
    int leaks = free_lists();
    if (leaks > 0)
        printf("ERROR: %d list were not freed", leaks);

    return g_stats;
}

