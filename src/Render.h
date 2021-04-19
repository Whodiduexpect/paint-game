#pragma once
#include <SDL.h>

enum class TileID
{
    NONE,
    PLAYER,
    TILE_GRASS,
    TILE_CONCRETE
};

class Atlas
{
    private:
        SDL_Texture* img;
        SDL_Rect src, dest;
        SDL_DisplayMode displayM;
    public:
        Atlas(SDL_Texture* img);
        void draw_raw(SDL_Renderer* renderer, TileID tileID, float x, float y);
        void draw(SDL_Renderer* renderer, TileID tileID, float x, float y);
        float zoom = 2;
        float cOffsetX = 0;
        float cOffsetY = 0;
};