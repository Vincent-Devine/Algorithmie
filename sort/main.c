#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "sort.h"

int g_swap_count;

void swap(int* a, int* b)
{
    assert(a != NULL);
    assert(b != NULL);

    int temp = *b;
    *b       = *a;
    *a       = temp;

    g_swap_count++;
}

void generate_random_list(int seed, int* numbers, int size, int value_max)
{
    srand(seed);

    for (int i = 0; i < size; ++i)
    {
        numbers[i] = rand() % value_max;
    }
}

bool is_sorted(int* list, int size)
{
    if (size == 0)
            return true;

    int previous = list[0];
    for (int i = 1; i < size; ++i)
    {
        if (previous > list[i])
            return false;
        previous = list[i];
    }

    return true;
}

void display(int* list, int size)
{
    for (int i = 0; i < size; ++i)
    {
        printf("%d\n", list[i]);
    }
}

int main(int argc, char* argv[])
{
    g_swap_count = 0;

    srand(time(NULL));

    // Change this
    int seed      = rand();
    //int seed      = 1234;
    int max_value = 100;
    int list_size = 10;

    int* list = malloc(list_size * sizeof(int));
    generate_random_list(seed, list, list_size, max_value);

    printf("Before:\n");
    display(list, list_size);

    sort_list(list, list_size);

    printf("After:\n");
    display(list, list_size);

    printf("Swap count: %d\n", g_swap_count);

    if (is_sorted(list, list_size))
    {
        printf("SUCCESS!\n");
    }
    else
    {
        printf("Fail: not sorted!\n");
    }

    free(list);

    return 0;
}

