#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "Map.h"
#include <SDL_image.h>
#include <GL/glew.h>
#include <spdlog/spdlog.h>

void init();
void load_media();
void close();

Uint32 start;
//SDL_Texture* load_texture(std::string path);
SDL_GLContext glContext;
SDL_Window* window;

void init()
{
	start = SDL_GetTicks();

	spdlog::set_pattern("%^[%T %l]: %v%$");
    spdlog::info("Initializing PaintGame v0.0.1");

	// Initialize GL 3.2 Core + GLSL 150
	const char* glslVersion = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Apple go brrrrrr
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN_DESKTOP);
    window = SDL_CreateWindow("Paint Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, windowFlags);
	if (!window)
	{
		spdlog::critical("Failed to create window: {}, aborting", SDL_GetError());
		exit(1);
	}
	glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SetSwapInterval(1); // Enable vsync

	glewInit();

	// Setup ImGui context
	IMGUI_CHECKVERSION();
    ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init(glslVersion);
}

/*
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
*/

void close()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

/*
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
*/

Uint32 lastTick = SDL_GetTicks();

int main(int argc, char* args[]) 
{

	init();

	/*
	Atlas atlas = Atlas(atlasTexture);
	Map map;
	Map::Player player;
	*/

	bool quit = false;

	SDL_Event e;

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	spdlog::info("Loading done (in {}s)", (SDL_GetTicks() - start) / 1000.0f);

	// Game loop
	while(!quit)
	{
		while(SDL_PollEvent(&e) != 0)
		{
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch(e.type)
			{
				case SDL_QUIT:
					quit = true;
					spdlog::info("Application quit by user");
					break;
				/*
				case SDL_MOUSEWHEEL:
					// Tried to implement exponetial interpolation but failed
					atlas.zoom += e.wheel.y * 0.5f;

					if (atlas.zoom < 1.0f)
					{
						atlas.zoom = 1;
					}
					else if (atlas.zoom > 12.0f)
					{
						atlas.zoom = 12;
					}
				*/
			}
		}

		// Calculate time since last tick/frame (delta tick)
		float current = SDL_GetTicks();
		float deltaT = (current - lastTick) / 1000.f;
		
		// Get keystates (keyboard input)
		const Uint8* keyStates = SDL_GetKeyboardState(NULL);

		/* Process input
		player.do_tick(deltaT, keyStates);
		atlas.cOffsetX = player.x + 0.5;
		atlas.cOffsetY = player.y + 0.5;
		*/

		// Render
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		ImGui::Render();
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);

		/*
		map.render_chunk_view(gRenderer, &atlas, player.x, player.y);
		player.render(gRenderer, &atlas);
		*/

		// Update last tick
		lastTick = current;
	}
	
	close();
}