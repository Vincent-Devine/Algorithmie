
typedef enum Type
{
    T_NOT_SET,

    T_NORMAL,
    T_FIRE,
    T_WATER,
    T_ELECTRIC,
    T_GRASS,
    T_ICE,
    T_FIGHTING,
    T_POISON,
    T_GROUND,
    T_FLYING,
    T_PSYCHIC,
    T_BUG,
    T_ROCK,
    T_GHOST,
    T_DRAGON,
    T_DARK,
    T_STEEL,
    T_FAIRY,
} Type;

typedef struct PokemonDef
{
    int PokemonId;
    int PokedexNumber;
    char PokemonName[32];
    char AlternateFormName[32];
    Type PrimaryType;
    Type SecondaryType;
} PokemonDef;

// Init/Destroy
void datasetCreate(int maxEntries);
void datasetDestroy(void);

// Data accessors
int datasetGetCount(void);
PokemonDef datasetGetValueAt(int index);

// Apply custom sorting to dataset
void datasetSort(void (*sortFunc)(PokemonDef*, int));

// Verify if search function is correct
// @param searchFunc Function pointer that return a Pokemon entry from its name
// @param iterations Number of queries to execute
int verifySearchFunction(PokemonDef (*searchFunc)(const char*), int iterations);