#include <raylib.h>

class Resources
{
public:
    Resources();
    ~Resources();

    Texture tiles;
    static constexpr int TILE_SIZE = 64;
    static constexpr int TILES_PER_LINE = 23;
};

extern Resources* gRes;