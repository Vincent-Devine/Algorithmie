#include <stdio.h>

#include "sort.h"

int Partition(list_t list, int lo, int hi)
{
    int pivot = list_read(list, hi);
    int i = lo - 1;

    for(int j = lo; j < hi; j++)
    {
        if(list_read(list, j) <= pivot)
        {
            i++;
            list_swap(list, i, j);
        }
    }
    i++;
    list_swap(list, i, hi);
    return i;
}

void QuickSort(list_t list, int lo, int hi)
{
    if(lo >= 0 && hi >= 0 && lo < hi)
    {
        int p = Partition(list, lo, hi);

        QuickSort(list, lo, p - 1);
        QuickSort(list, p + 1, hi);
    }
}

void quick_sort(list_t list, int size)
{
    QuickSort(list, 0, size - 1);
}
