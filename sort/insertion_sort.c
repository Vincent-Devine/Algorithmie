#include "sort.h"

void sort_list(int* list, int size)
{
    for(int i = 1; i < size; i++)
    {
        for(int j = i; j > 0; j--)
        {
            if(list[j-1] > list[j])
                swap(&list[j], &list[j-1]);
        }
    }
}
