#include <stdio.h>

#include "sort.h"

void CopyArray(list_t A, int iBegin, int iEnd, list_t B)
{
    for(int i = iBegin; i < iEnd; i++)
        list_write(B, i, list_read(A, i));
}

void TopDownMerge(list_t A, int iBegin, int iMiddle, int iEnd, list_t B)
{
    int i = iBegin;
    int j = iMiddle;

    for(int k = iBegin; k < iEnd; k++)
    {
        if(i < iMiddle && (j >= iEnd || list_read(A, i) <= list_read(A, j)))
        {
            list_write(B, k, list_read(A, i));
            i++;
        }
        else
        {
            list_write(B, k, list_read(A, j));
            j++;
        }
    }
}

void TopDownSplitMerge(list_t B, int iBegin, int iEnd, list_t A)
{
    if(iEnd - iBegin <= 1)
        return;

    int iMiddle = (iEnd + iBegin) / 2;

    TopDownSplitMerge(A, iBegin, iMiddle, B);
    TopDownSplitMerge(A, iMiddle, iEnd, B);

    TopDownMerge(B, iBegin, iMiddle, iEnd, A);
}

void TopDownMergeSort(list_t A, list_t B, int size)
{
    CopyArray(A, 0, size, B);
    TopDownSplitMerge(B, 0, size, A);
}

void merge_sort(list_t list, int size)
{
    list_t workList = list_alloc(size * sizeof(int));

    TopDownMergeSort(list, workList, size);

    list_free(workList);
}
