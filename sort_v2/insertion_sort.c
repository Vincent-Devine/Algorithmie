#include <stdio.h>

#include "sort.h"

void insertion_sort(list_t list, int size)
{
    for(int i = 1; i < size; i++)
    {
        for(int j = i; j > 0; j--)
        {
            if(list_read(list,j-1) > list_read(list,j))
                list_swap(list, j, j-1);
        }
    }
}