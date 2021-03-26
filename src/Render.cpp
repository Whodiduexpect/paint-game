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

void Atlas::draw(SDL_Renderer* renderer, TextureId textureid, int x, int y)
{
    int texture_index = static_cast<int>(textureid);

    dest.x = x;
    dest.y = y;
    dest.w = TEXTURE_REGIONS[texture_index].w;
    dest.h = TEXTURE_REGIONS[texture_index].h;

    SDL_RenderCopy(renderer, img, &TEXTURE_REGIONS[texture_index], &dest);
}