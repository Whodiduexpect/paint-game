#include "Render.h"

Atlas::Atlas(SDL_Texture* img)
{
    this->img = img;
}

const SDL_Rect TEXTURE_REGIONS[4] =
{
    SDL_Rect {0, 0, 0, 0},    // NONE
    SDL_Rect {0, 0, 32, 32},  // TILE_GRASS
    SDL_Rect {32, 0, 32, 32}, // TILE_CONCRETE
    SDL_Rect {64, 0, 32, 32}  // PLAYER
};

// Draws to the screen WITHOUT accounting for 0 0 being the top left
// and camera position
void Atlas::draw_raw(SDL_Renderer* renderer, TileID tileID, float x, float y)
{
    int textureIndex = static_cast<int>(tileID);

    dest.x = x;
    dest.y = y;
    dest.w = TEXTURE_REGIONS[textureIndex].w;
    dest.h = TEXTURE_REGIONS[textureIndex].h;

    SDL_RenderCopy(renderer, img, &TEXTURE_REGIONS[textureIndex], &dest);
}

// Makes 0 0 the center of the screen and accounts for camera position
void Atlas::draw(SDL_Renderer* renderer, TileID tileid, float x, float y)
{
    SDL_GetCurrentDisplayMode(0, &displayM);
    
    draw_raw(renderer, tileid, (x + displayM.w / 2) + cOffsetX * -1, (y + displayM.h / 2) + cOffsetY * -1);
}