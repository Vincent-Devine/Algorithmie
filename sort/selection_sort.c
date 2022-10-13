#include "sort.h"

void sort_list(int* list, int size)
{
    for(int i = 0; i < size - 1; i++)
    {
        int jMin = i;
        for(int j = i + 1; j < size; j++)
        {
            if(list[j] < list[jMin])
                jMin = j;
        }
        if(jMin != i)
            swap(&list[i], &list[jMin]);
    }
}
