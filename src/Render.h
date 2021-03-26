#pragma once
#include <SDL.h>

enum class TextureId
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
        void draw(SDL_Renderer* renderer, TextureId textureid, int x, int y);
};