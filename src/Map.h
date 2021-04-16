#pragma once
#include <utility>
#include <map>
#include "Render.h"

class Map
{
    public:

        class Chunk
        {
            public:
                TileID tiles[32][32];
        };

        std::map<std::pair<int, int>, Chunk> chunks;

        void generate_chunk(std::pair<int, int> chunkID);

        void render_chunk(SDL_Renderer* renderer, Atlas* atlas, std::pair<int, int> chunkID);

};