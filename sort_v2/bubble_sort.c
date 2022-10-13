
#include <stdbool.h>
#include <stdio.h>

#include "sort.h"

// Non-optimized bubble sort
void bubble_sort(list_t list, int size)
{
    bool swapped;
    do
    {
        swapped = false;
        for (int i = 1; i < size; ++i)
        {
            // list[i-1] > list[i]
            if (list_read(list, i-1) > list_read(list, i))
            {
                // swap(&list[i-1], &list[i])
                list_swap(list, i-1, i);
                swapped = true;
            }
        }
    } while (swapped);
}

