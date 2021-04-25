#include "Map.h"
#include <spdlog/spdlog.h>

#include <SDL_opengl.h>

void Map::generate_chunk(std::pair<int, int> chunkID)
{
    Chunk chunk;

    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++)
        {
            // Placeholder generation
            if (i % 2 == 0 && j % 2 == 0)
            {
                chunk.tiles[i][j] = TileID::TILE_CONCRETE;
            }
            else
            {
                chunk.tiles[i][j] = TileID::TILE_GRASS;
            }

            if (i == 0 || j == 0)
            {
                chunk.tiles[i][j] = TileID::TILE_CONCRETE;
            }
        }
    }

    chunk.generated = true;
    chunks.insert(std::make_pair(chunkID, chunk));
}

void Map::render_chunk(SDL_Renderer* renderer, Atlas* atlas, std::pair<int, int> chunkID)
{
    Chunk chunk = chunks.find(chunkID)->second;

    if (!chunk.generated)
    {
        generate_chunk(chunkID);
        chunk = chunks.find(chunkID)->second;
    }

    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++)
        {
            if (chunk.tiles[i][j] > TileID::TILE_CONCRETE || chunk.tiles[i][j] < TileID::NONE)
            {
                spdlog::error("Tried to render invalid tile id {0} at chunk {1} {2} at X: {3} Y: {4}", chunk.tiles[i][j], chunkID.first, chunkID.second, i, j);
            }
            else
            {
              atlas->draw(renderer, chunk.tiles[i][j], (i + chunkID.first * 32), (j + chunkID.second * 32));  
            }
        }
    }
}

void Map::Player::render(SDL_Renderer* renderer, Atlas* atlas)
{
    atlas->draw(renderer, TileID::PLAYER, x, y);
}

std::pair<int, int> Map::get_chunk(float x, float y)
{
    return std::make_pair((int)floor((float)x / 32), (int)floor((float)y / 32));
}

void Map::render_chunk_view(SDL_Renderer* renderer, Atlas* atlas, float x, float y)
{
    std::pair<int, int> mChunk = get_chunk(x, y);

    render_chunk(renderer, atlas, mChunk);

    // Render all neighboring chunks
    render_chunk(renderer, atlas, std::make_pair(mChunk.first+1, mChunk.second));
    render_chunk(renderer, atlas, std::make_pair(mChunk.first, mChunk.second + 1));
    render_chunk(renderer, atlas, std::make_pair(mChunk.first + 1, mChunk.second + 1));
    render_chunk(renderer, atlas, std::make_pair(mChunk.first - 1, mChunk.second));
    render_chunk(renderer, atlas, std::make_pair(mChunk.first, mChunk.second - 1));
    render_chunk(renderer, atlas, std::make_pair(mChunk.first - 1, mChunk.second - 1));
    render_chunk(renderer, atlas, std::make_pair(mChunk.first + 1, mChunk.second - 1));
    render_chunk(renderer, atlas, std::make_pair(mChunk.first - 1, mChunk.second + 1));
}

void Map::Player::do_tick(float deltaT, const Uint8* keyStates)
{

    if (keyStates[SDL_SCANCODE_W] || keyStates[SDL_SCANCODE_UP])
    {
        y -= speed * deltaT;
    }


    if (keyStates[SDL_SCANCODE_A] || keyStates[SDL_SCANCODE_LEFT])
    {
        x -= speed * deltaT;
    }

    if (keyStates[SDL_SCANCODE_S] || keyStates[SDL_SCANCODE_DOWN])
    {
        y += speed * deltaT;
    }

    if (keyStates[SDL_SCANCODE_D] || keyStates[SDL_SCANCODE_RIGHT])
    {
        x += speed * deltaT;
    }
}