#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dataset.h"

// Complexité de l'algorithme: O(n)
PokemonDef searchPokemonByName(const char* name)
{
    // TODO: Implémentez la fonction
    for(int i = 0; i < datasetGetCount(); i++)
    {
        if(strcmp(datasetGetValueAt(i).PokemonName, name) == 0)
            return datasetGetValueAt(i);
    }
}

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

    if (verifySearchFunction(searchPokemonByName, iterations) != 1)
        fprintf(stderr, "\e[31mFailure\e[0m\n");

    // Shutdown
    datasetDestroy();

    return 0;
}