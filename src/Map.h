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

        class Player
        {
            public:
                float x = 0;
                float y = 0;
                float speed = 10;
                void render(SDL_Renderer* renderer, Atlas* atlas);
                void do_tick(float deltaT, const Uint8* keyStates);
        };

        std::map<std::pair<int, int>, Chunk> chunks;

        void generate_chunk(std::pair<int, int> chunkID);

        void render_chunk(SDL_Renderer* renderer, Atlas* atlas, std::pair<int, int> chunkID);

};