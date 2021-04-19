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
                bool generated = false;
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
        std::pair<int, int> get_chunk(float x, float y);
        void render_chunk_view(SDL_Renderer* renderer, Atlas* atlas, float x, float y);
};