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

SDL_GLContext glContext;
SDL_Window* window;
unsigned int shaderProgram;

// Shader Code
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

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
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

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

	// Build and compile shaders
	// Vertex Shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Check for compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		spdlog::critical("Failed to compile vertex shader: {}", infoLog);
	}
	// Fragment Shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Check for compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		spdlog::critical("Failed to compile fragment shader: {}", infoLog);
	}
	// Link Shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		spdlog::critical("Failed to link shaders: {}", infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
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

	// Set up vertex data for test square
	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
			0.5f,  0.5f, 0.0f,  // top right
			0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	glBindVertexArray(0); 

	bool renderWireframe = false;
	bool overlayOpen = true;
	bool enableVsync = true;

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


		// ImGui Logic
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		// Debug Panel
		ImGui::Begin("Debug Menu");
		ImGui::Text("Paint Game v0.0.1");
		if (ImGui::CollapsingHeader("Rendering"))
		{
			if (overlayOpen) {
				if (ImGui::Button("Close Performance Overlay"))
				{
					overlayOpen = false;
				}
			}
			else
			{
				if (ImGui::Button("Open Performance Overlay"))
				{
					overlayOpen = true;
				}
			}
			ImGui::Checkbox("Render Wireframe", &renderWireframe);
			ImGui::Checkbox("Enable Vsync", &enableVsync);
		}
		ImGui::End();

		// Overlay
		if (overlayOpen)
		{
			const float PAD = 10.0f;
			static int corner = 0;
			ImGuiIO& io = ImGui::GetIO();
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
			if (corner != -1)
			{
				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any
				ImVec2 work_size = viewport->WorkSize;
				ImVec2 window_pos, window_pos_pivot;
				window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
				window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
				window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
				window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
				ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
				window_flags |= ImGuiWindowFlags_NoMove;
			}
			ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
			if (ImGui::Begin("Performance Overlay", &overlayOpen, window_flags))
			{
				ImGui::Text("Performance Overlay");
				ImGui::Separator();
				ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				if (ImGui::BeginPopupContextWindow())
				{
					if (ImGui::MenuItem("Custom",       NULL, corner == -1)) corner = -1;
					if (ImGui::MenuItem("Top-left",     NULL, corner == 0)) corner = 0;
					if (ImGui::MenuItem("Top-right",    NULL, corner == 1)) corner = 1;
					if (ImGui::MenuItem("Bottom-left",  NULL, corner == 2)) corner = 2;
					if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
					if (overlayOpen && ImGui::MenuItem("Close")) overlayOpen = false;
					ImGui::EndPopup();
				}
			}

			ImGui::End();
		}

		ImGui::Render();

		// Render Settings
		SDL_GL_SetSwapInterval(enableVsync);
		// Render
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT);

		// Triangle
		if (renderWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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