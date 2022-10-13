
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "list.h"
#include "sort.h"
#include "replay.h"
#include "internal.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

int main(int argc, char* argv[])
{
    srand(time(NULL));

    // Change those value for testing different cases
    //int seed      = rand();
    int seed       = time(NULL);
    int max_value  = 1000;
    int array_size = 100;

    printf("Random seed = %d\n\n", seed);

    int* array = malloc(array_size * sizeof(int));
    generate_random_array(seed, array, array_size, max_value);

    // Switch mode between test ONE and test ALL
#if 0
    // Change sort_func to test other function
    sort_func_t sort_func = merge_sort;

    stats_t stats = test_sort(array, array_size, max_value, sort_func, true);
    replay_sort(&stats, array, array_size);
    free(stats.events); // Manually destroy events array. TODO: Find a best way
#else
    struct { sort_func_t sort; char* name; } sort_funcs[] =
    {
        { bubble_sort,    "Bubble sort" },
        { selection_sort, "Selection sort" },
        { insertion_sort, "Insertion sort" },
        { quick_sort,     "Quick sort" },
        { merge_sort,     "Merge sort" },
        
        // Uncomment lines when it's implemented
    };

    for (int i = 0; i < ARRAY_SIZE(sort_funcs); ++i)
    {
        // Run test
        printf("========================\n");
        printf("%s... ", sort_funcs[i].name);
        stats_t stats = test_sort(array, array_size, max_value, sort_funcs[i].sort, false);
        replay_sort(&stats, array, array_size);
        free(stats.events); // Manually destroy events array. TODO: Find a best way
        printf("\n");
    }
#endif

    return 0;
}


// http://www.pcg-random.org/

struct pcg32_random_t
{
    uint64_t state;
    uint64_t inc;
};

struct pcg32_random_t g_rand;

uint32_t pcg32_random(void)
{
    uint64_t oldstate = g_rand.state;
    g_rand.state = oldstate * 6364136223846793005ULL + g_rand.inc;
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

void pcg32_srandom(uint64_t initstate, uint64_t initseq)
{
    g_rand.state = 0U;
    g_rand.inc = (initseq << 1u) | 1u;
    pcg32_random();
    g_rand.state += initstate;
    pcg32_random();
}

void generate_random_array(int seed, int* array, int size, int value_max)
{
    pcg32_srandom(seed, 1234);

    for (int i = 0; i < size; ++i)
    {
        array[i] = pcg32_random() % value_max;
    }
}

void swap_values(int* a, int* b, int count)
{
    for (int i = 0; i < count; ++i)
    {
        int tmp = *a;
        *a = *b;
        *b = tmp;
        a++;
        b++;
    }
}

void apply_sort_event(int* array, sort_event_t* event)
{
    switch (event->type)
    {
        case SE_SWAP:
            swap_values(&array[event->swap.id_a], &array[event->swap.id_b], event->swap.count);
            break;

        case SE_WRITE:
            array[event->write.id] = event->write.value;
            break;

        case SE_MOVE:
            memmove(array + event->move.dst_id, array + event->move.src_id, event->move.count * sizeof(int));
            break;

        case SE_COPY:
            memcpy(array + event->move.dst_id, array + event->move.src_id, event->move.count * sizeof(int));
            break;

            // TODO: Handle other cases
        default:;
    }
}

