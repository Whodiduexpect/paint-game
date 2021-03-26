#include <spdlog/spdlog.h>
#include "Render.h"
#include <SDL_image.h>

void init();

bool loadMedia();

void close();

SDL_Texture* loadTexture(std::string path);

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* atlas_texture = NULL;

void init()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	gWindow = SDL_CreateWindow("Paint Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_FULLSCREEN_DESKTOP);
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	int imgFlags = IMG_INIT_PNG;
}

bool loadMedia()
{
	bool success = true;

	atlas_texture = loadTexture("data/graphics/atlas.png");
	if(atlas_texture == NULL)
	{
		spdlog::critical("Failed to load atlas! Is data folder missing?");
		success = false;
	}

	return success;
}

void close()
{
	SDL_DestroyTexture(atlas_texture);
	atlas_texture = NULL;

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(std::string path)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	loadedSurface == NULL;

	newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	SDL_FreeSurface(loadedSurface);

	return newTexture;
}

int main(int argc, char* args[]) 
{
    spdlog::set_pattern("%^[%T %l]: %v%$");
    spdlog::info("Initializing PaintGame v0.0.1");
    
	init();
	loadMedia();

	Atlas atlas = Atlas(atlas_texture);

	bool quit = false;

	SDL_Event e;

	spdlog::info("Loading done");
	while(!quit)
	{
		while(SDL_PollEvent(&e) != 0)
		{
			switch(e.type)
			{
				case SDL_QUIT:
					quit = true;
			}
		}

		SDL_RenderClear(gRenderer);
		atlas.draw(gRenderer, TextureId::TILE_GRASS, 0, 0);
		atlas.draw(gRenderer, TextureId::TILE_CONCRETE, 32, 32);
		SDL_RenderPresent(gRenderer);
	}
}