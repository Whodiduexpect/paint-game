#include "Render.h"

Atlas::Atlas(SDL_Texture* img)
{
    this->img = img;
}

const SDL_Rect TEXTURE_REGIONS[3] =
{
    SDL_Rect {0, 0, 0, 0},    // NONE
    SDL_Rect {0, 0, 32, 32},  // TILE_GRASS
    SDL_Rect {32, 0, 32, 32}  // TILE_CONCRETE
};

void Atlas::draw(SDL_Renderer* renderer, TileID tileID, int x, int y)
{
    int textureIndex = static_cast<int>(tileID);

    dest.x = x;
    dest.y = y;
    dest.w = TEXTURE_REGIONS[textureIndex].w;
    dest.h = TEXTURE_REGIONS[textureIndex].h;

    SDL_RenderCopy(renderer, img, &TEXTURE_REGIONS[textureIndex], &dest);
}