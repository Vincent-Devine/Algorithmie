#include "resources.hpp"

Resources* gRes = nullptr;

Resources::Resources()
{
    tiles = LoadTexture("assets/towerDefense_tilesheet.png");
}

Resources::~Resources()
{
    UnloadTexture(tiles);
}
