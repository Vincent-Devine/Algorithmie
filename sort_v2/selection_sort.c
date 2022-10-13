#include <stdio.h>

#include "sort.h"

void selection_sort(list_t list, int size)
{
    for(int i = 0; i < size - 1; i++)
    {
        int jMin = i;
        for(int j = i + 1; j < size; j++)
        {
            if(list_read(list, j) < list_read(list, jMin))
                jMin = j;
        }
        if(jMin != i)
            list_swap(list, i, jMin);
    }
}
