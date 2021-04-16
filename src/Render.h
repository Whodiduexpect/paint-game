#pragma once
#include <SDL.h>

enum class TileID
{
    NONE,
    TILE_GRASS,
    TILE_CONCRETE
};

class Atlas
{
    private:
        SDL_Texture* img;
        SDL_Rect src, dest;
    public:
        Atlas(SDL_Texture* img);
        void draw(SDL_Renderer* renderer, TileID tileID, int x, int y);
};