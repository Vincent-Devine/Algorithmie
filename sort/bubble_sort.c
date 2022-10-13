#include <stdbool.h>

#include "sort.h"

void sort_list(int* list, int size)
{
    bool swapped = false;
    do
    {
        swapped = false;
        for(int i = 1; i < size; i++)
        {
            if(list[i-1] > list[i])
            {
                swap(&list[i-1], &list[i]);
                swapped = true;
            }
        }
        size--;
    } while (swapped);
}