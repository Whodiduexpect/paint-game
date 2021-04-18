#include <spdlog/spdlog.h>
#include "Map.h"
#include <SDL_image.h>

void init();
void load_media();
void close();

Uint32 start;
SDL_Texture* load_texture(std::string path);
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* atlasTexture = NULL;

void init()
{
	Uint32 start = SDL_GetTicks();

	spdlog::set_pattern("%^[%T %l]: %v%$");
    spdlog::info("Initializing PaintGame v0.0.1");

	SDL_Init(SDL_INIT_VIDEO);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	gWindow = SDL_CreateWindow("Paint Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (!gWindow)
	{
		spdlog::critical("Failed to create window: {}, aborting", SDL_GetError());
		exit(1);
	}

	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	int imgFlags = IMG_INIT_PNG;

	load_media();
}

void load_media()
{

	atlasTexture = load_texture("data/graphics/atlas.png");
	if (!atlasTexture)
	{
		spdlog::critical("Failed to load atlas (is the data folder missing?), aborting\nFull Error Message: {}", SDL_GetError());
		exit(1);
	}
	else
	{
		spdlog::info("Atlas loaded succesfully");
	}
	
}

void close()
{
	SDL_DestroyTexture(atlasTexture);
	atlasTexture = NULL;

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* load_texture(std::string path)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (!loadedSurface)
	{
		spdlog::error("Failed to load texture {0}: {1}", path, SDL_GetError());
	}

	newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	SDL_FreeSurface(loadedSurface);

	return newTexture;
}

Uint32 lastTick = SDL_GetTicks();

int main(int argc, char* args[]) 
{

	init();

	Atlas atlas = Atlas(atlasTexture);
	Map map;
	Map::Player player;

	// Generate two chunks for testing purpses
	// (there is no system yet that automatically picks nearby chunks)
	map.generate_chunk(std::make_pair(0, 0));
	map.generate_chunk(std::make_pair(1, 0));

	bool quit = false;

	SDL_Event e;

	spdlog::info("Loading done (in {}s)", (SDL_GetTicks() - start) / 1000.0f);

	// Game loop
	while(!quit)
	{
		while(SDL_PollEvent(&e) != 0)
		{
			switch(e.type)
			{
				case SDL_QUIT:
					quit = true;
					spdlog::info("Application quit by user");
					break;
			}
		}

		// Calculate time since last tick/frame (delta tick)
		float current = SDL_GetTicks();
		float deltaT = (current - lastTick) / 1000.f;
		
		// Get keystates (keyboard input)
		const Uint8* keyStates = SDL_GetKeyboardState(NULL);

		// Process input
		player.do_tick(deltaT, keyStates);
		atlas.cOffsetX = player.x;
		atlas.cOffsetY = player.y;

		// Render
		SDL_RenderClear(gRenderer);

		map.render_chunk(gRenderer, &atlas, std::make_pair(0, 0));
		map.render_chunk(gRenderer, &atlas, std::make_pair(1, 0));
		player.render(gRenderer, &atlas);

		SDL_RenderPresent(gRenderer);

		// Update last tick
		lastTick = current;
	}
	
	close();
}