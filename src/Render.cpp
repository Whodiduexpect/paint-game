#include "Render.h"

Atlas::Atlas(SDL_Texture* img)
{
    this->img = img;
}

const SDL_Rect TEXTURE_REGIONS[4] =
{
    SDL_Rect {0, 0, 0, 0},    // NONE
    SDL_Rect {0, 0, 32, 32},  // PLAYER
    SDL_Rect {32, 0, 32, 32}, // TILE_GRASS
    SDL_Rect {64, 0, 32, 32}  // TILE_CONCRETE
};

// Draws to the screen WITHOUT accounting for 0 0 being the top left
// and camera position
void Atlas::draw_raw(SDL_Renderer* renderer, TileID tileID, float x, float y)
{
    int textureIndex = static_cast<int>(tileID);

    dest.x = x;
    dest.y = y;
    dest.w = TEXTURE_REGIONS[textureIndex].w * zoom;
    dest.h = TEXTURE_REGIONS[textureIndex].h * zoom;

    SDL_RenderCopy(renderer, img, &TEXTURE_REGIONS[textureIndex], &dest);
}

// Makes 0 0 the center of the screen and accounts for camera position
void Atlas::draw(SDL_Renderer* renderer, TileID tileid, float x, float y)
{

    SDL_GetCurrentDisplayMode(0, &displayM);

    x = (x + cOffsetX * -1) * (32 * zoom) + displayM.w / 2;
    y = (y + cOffsetY * -1) * (32 * zoom) + displayM.h / 2;
    
    draw_raw(renderer, tileid, x, y);
}