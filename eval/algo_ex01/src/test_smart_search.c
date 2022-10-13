#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dataset.h"

PokemonDef dichotomous_research(const char* name, int first, int end)
{
    int middle = (first + end) / 2;
    if(strcmp(datasetGetValueAt(middle).PokemonName, name) == 0)
        return datasetGetValueAt(middle);
    else if(strcmp(name, datasetGetValueAt(middle).PokemonName) < 0)
        return dichotomous_research(name, first, middle);
    else
        return dichotomous_research(name, middle, end);
}

// Complexité de l'algorithme: O(log(n))
PokemonDef searchPokemonByName(const char* name)
{
    // TODO: (Part 2) Implémentez la fonction (sans bubble sort!)
    // Prérequis : La liste doit avoir été préalablement trié (grâce à sortFunc())
    return dichotomous_research(name, 0, datasetGetCount());
}

#if 0
int comparePkDef(const void* a, const void* b)
{
    return strcmp(((const PokemonDef*)a)->PokemonName, ((const PokemonDef*)b)->PokemonName);
}

void sortFunc(PokemonDef* array, int size)
{
    qsort(array, size, sizeof(PokemonDef), comparePkDef);
}
#else

void swap(PokemonDef* array, int firstIndex, int secondIndex)
{
    PokemonDef firstPokemonDef = array[firstIndex];
    array[firstIndex] = array[secondIndex];
    array[secondIndex] = firstPokemonDef;
}

void sortFunc(PokemonDef* array, int size)
{
    // TODO: (Part 1) Implémentez la fonction (sans bubble sort!)
    for(int i = 1; i < size; i++)
    {
        for(int j = i; j > 0; j--)
        {
            if(strcmp(datasetGetValueAt(j).PokemonName, datasetGetValueAt(j-1).PokemonName) < 0)
                swap(array, j-1, j);
        }
    }
}
#endif

int main(int argc, char* argv[])
{
    // Parse command line
    int maxEntries = -1;
    int iterations = 10000;
    {
        if (argc >= 2)
            maxEntries = atoi(argv[1]);
        if (argc >= 3)
            iterations = atoi(argv[2]);
    }

    // Init dataset
    datasetCreate(maxEntries);
    
    // Sort dataset
    datasetSort(sortFunc);

    if (verifySearchFunction(searchPokemonByName, iterations) != 1)
        fprintf(stderr, "\e[31mFailure\e[0m\n");

    // Shutdown
    datasetDestroy();

    return 0;
}
