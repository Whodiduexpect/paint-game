#include "Map.h"
#include <spdlog/spdlog.h>

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
        }
    }

    chunks.insert(std::make_pair(chunkID, chunk));
}

void Map::render_chunk(SDL_Renderer* renderer, Atlas* atlas, std::pair<int, int> chunkID)
{
    Chunk chunk = chunks.find(chunkID)->second;
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++)
        {
            if (chunk.tiles[i][j] > TileID::TILE_CONCRETE || chunk.tiles[i][j] < TileID::NONE)
            {
                spdlog::error("Tried to render invalid tile id {0} at chunk {1} {2} at X: {3} Y: {4}", chunk.tiles[i][j], chunkID.first, chunkID.second, i, j);
            }
            else
            {
              atlas->draw(renderer, chunk.tiles[i][j], (i + chunkID.first * 32) * 32, (j + chunkID.second * 32) * 32);  
            }
        }
    }
}