/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: WinSDLOpenGLApplication.cpp
Purpose: implementation file of WinSDLOpenGLApplication
Language: c++ 11
Platform: win32 x86
Project: CS529_final_project
Author: Hang Yu hang.yu 60001119
Creation date: 10/13/2019
- End Header ----------------------------*/

#include "WinSDLOpenGLApplication.hpp"
#include "Common/Event/EventDispatcher.hpp"
#include "Common/Utility/Utility.hpp"
#include "Common/Entity/GameObjectResourceManager.hpp"

#include "src/Event/MyEvent.hpp"

PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLVALIDATEPROGRAMPROC glValidateProgram;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLUSEPROGRAMPROC glUseProgram;

PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap;

PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;

PFNGLUNIFORM1IPROC glUniform1i;

bool initGLExtensions()
{
	glCreateShader = (PFNGLCREATESHADERPROC)SDL_GL_GetProcAddress("glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)SDL_GL_GetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)SDL_GL_GetProcAddress("glCompileShader");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)SDL_GL_GetProcAddress("glGetShaderiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)SDL_GL_GetProcAddress("glGetShaderInfoLog");
	glDeleteShader = (PFNGLDELETESHADERPROC)SDL_GL_GetProcAddress("glDeleteShader");
	glAttachShader = (PFNGLATTACHSHADERPROC)SDL_GL_GetProcAddress("glAttachShader");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)SDL_GL_GetProcAddress("glCreateProgram");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)SDL_GL_GetProcAddress("glLinkProgram");
	glValidateProgram = (PFNGLVALIDATEPROGRAMPROC)SDL_GL_GetProcAddress("glValidateProgram");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)SDL_GL_GetProcAddress("glGetProgramiv");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)SDL_GL_GetProcAddress("glGetProgramInfoLog");
	glUseProgram = (PFNGLUSEPROGRAMPROC)SDL_GL_GetProcAddress("glUseProgram");

	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)SDL_GL_GetProcAddress("glGenVertexArrays");
	glGenBuffers = (PFNGLGENBUFFERSPROC)SDL_GL_GetProcAddress("glGenBuffers");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)SDL_GL_GetProcAddress("glBindVertexArray");
	glBindBuffer = (PFNGLBINDBUFFERPROC)SDL_GL_GetProcAddress("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)SDL_GL_GetProcAddress("glBufferData");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)SDL_GL_GetProcAddress("glVertexAttribPointer");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)SDL_GL_GetProcAddress("glEnableVertexAttribArray");
	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)SDL_GL_GetProcAddress("glGenerateMipmap");

	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)SDL_GL_GetProcAddress("glGetUniformLocation");
	glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)SDL_GL_GetProcAddress("glUniformMatrix3fv");

	glUniform1i = (PFNGLUNIFORM1IPROC)SDL_GL_GetProcAddress("glUniform1i");

	return glCreateShader && glShaderSource && glCompileShader && glGetShaderiv &&
		glGetShaderInfoLog && glDeleteShader && glAttachShader && glCreateProgram &&
		glLinkProgram && glValidateProgram && glGetProgramiv && glGetProgramInfoLog &&
		glUseProgram && glGenVertexArrays && glGenBuffers && glBindVertexArray && glBindBuffer &&
		glBufferData && glVertexAttribPointer && glEnableVertexAttribArray && glGenerateMipmap &&
		glGetUniformLocation && glUniformMatrix3fv && glUniform1i;
}

GLuint compileShader(const char* source, GLuint shaderType) {
	DEBUG_PRINT("Compiling shader:" << std::endl << source << std::endl);
	// Create ID for shader
	GLuint result = glCreateShader(shaderType);
	// Define shader text
	glShaderSource(result, 1, &source, nullptr);
	// Compile shader
	glCompileShader(result);

	//Check vertex shader for errors
	GLint shaderCompiled = GL_FALSE;
	glGetShaderiv(result, GL_COMPILE_STATUS, &shaderCompiled);
	if (shaderCompiled != GL_TRUE) {
		PRINT("Error in compiling: " << result << "!" << std::endl);
		GLint logLength;
		glGetShaderiv(result, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0)
		{
			GLchar* log = (GLchar*)malloc(logLength);
			glGetShaderInfoLog(result, logLength, &logLength, log);
			DEBUG_PRINT("Shader compile log:" << log << std::endl);
			free(log);
		}
		glDeleteShader(result);
		result = 0;
	}
	else {
		DEBUG_PRINT("Shader compilation success. Id = " << result << std::endl);
	}
	return result;
}

GLuint compileProgram(const char* vtxFile, const char* fragFile) {
	GLuint programId = 0;
	GLuint vtxShaderId, fragShaderId;

	programId = glCreateProgram();

	std::ifstream f(vtxFile);
	std::string source((std::istreambuf_iterator<char>(f)),
		std::istreambuf_iterator<char>());
	vtxShaderId = compileShader(source.c_str(), GL_VERTEX_SHADER);

	f = std::ifstream(fragFile);
	source = std::string((std::istreambuf_iterator<char>(f)),
		std::istreambuf_iterator<char>());
	fragShaderId = compileShader(source.c_str(), GL_FRAGMENT_SHADER);

	if (vtxShaderId && fragShaderId) {
		// Associate shader with program
		glAttachShader(programId, vtxShaderId);
		glAttachShader(programId, fragShaderId);
		glLinkProgram(programId);
		glValidateProgram(programId);

		// Check the status of the compile/link
		GLint logLen;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0) {
			char* log = (char*)malloc(logLen * sizeof(char));
			// Show any errors as appropriate
			glGetProgramInfoLog(programId, logLen, &logLen, log);
			DEBUG_PRINT("Prog Info Log: " << std::endl << log << std::endl);
			free(log);
		}
	}
	if (vtxShaderId) {
		glDeleteShader(vtxShaderId);
	}
	if (fragShaderId) {
		glDeleteShader(fragShaderId);
	}
	return programId;
}


void presentBackBuffer(SDL_Renderer* renderer, SDL_Window* win, SDL_Texture* backBuffer, GLuint programId, int W, int H) {
	GLint oldProgramId;

	SDL_SetRenderTarget(renderer, nullptr);
	SDL_RenderClear(renderer);

	SDL_GL_BindTexture(backBuffer, nullptr, nullptr);
	if (programId != 0) {
		glGetIntegerv(GL_CURRENT_PROGRAM, &oldProgramId);
		glUseProgram(programId);
	}

	GLfloat minx, miny, maxx, maxy;
	GLfloat minu, maxu, minv, maxv;

	minx = 0.0f;
	miny = 0.0f;
	maxx = static_cast<GLfloat>(W);
	maxy = static_cast<GLfloat>(H);

	minu = 0.0f;
	maxu = 1.0f;
	minv = 0.0f;
	maxv = 1.0f;

	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(minu, minv);
	glVertex2f(minx, miny);
	glTexCoord2f(maxu, minv);
	glVertex2f(maxx, miny);
	glTexCoord2f(minu, maxv);
	glVertex2f(minx, maxy);
	glTexCoord2f(maxu, maxv);
	glVertex2f(maxx, maxy);
	glEnd();
	SDL_GL_SwapWindow(win);

	if (programId != 0) {
		glUseProgram(oldProgramId);
	}
}


/*Init Tick Final***************************************************************************************************************/

int My::WinSDLOpenGLApplication::Initialize()
{
	// 1. SDL init
	if ((SDL_Init(SDL_INIT_EVERYTHING | SDL_VIDEO_OPENGL)) < 0)
	{
		printf("Couldn't initialize SDL, error %s\n", SDL_GetError());
		exit(-1);
	}
	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		exit(-1);
	}

	// 2. SDL window init
	std::wstring w_title(m_config.appName);
	std::string t(w_title.begin(), w_title.end());
	m_pWindow = SDL_CreateWindow(t.c_str(),		// window title
		SDL_WINDOWPOS_UNDEFINED,					// initial x position
		SDL_WINDOWPOS_UNDEFINED,					// initial y position
		m_config.screenWidth,										// width, in pixels
		m_config.screenHeight,										// height, in pixels
		SDL_WINDOW_OPENGL);
	// Check that the window was successfully made
	if (nullptr == m_pWindow)
	{
		// In the event that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		exit(-1);
	}

	// 3. SDL opengl init
	//Create context
	m_gContext = SDL_GL_CreateContext(m_pWindow);
	if (m_gContext == nullptr)
	{
		printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
		exit(-1);
	}
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetSwapInterval(-1);
	// 4. Create renderer
	m_renderer = SDL_CreateRenderer(m_pWindow, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	// 5. Init OpenGL
	SDL_RendererInfo rendererInfo;
	SDL_GetRendererInfo(m_renderer, &rendererInfo);
	if (!strncmp(rendererInfo.name, "opengl", 6)) {
		PRINT("Init OpenGL! " + Str(rendererInfo.name) + "\n");
		// If you want to use GLEW or some other GL extension handler, do it here!
		if (!initGLExtensions()) {
			PRINT("Couldn't init GL extensions!\n" << std::endl);
			exit(-1);
		}
		// Compile shader
		m_programId = compileProgram("./Resource/Shader/std.vertex", "./Resource/Shader/std.fragment");
		PRINT("programId = " << m_programId << "\n");
	}
	else
	{
		PRINT("Can't init OpenGL! Exit!\n");
		exit(-1);
	}
	// https://gamedev.stackexchange.com/questions/119414/resolution-scaling
	// 6. Enable SDL Logical Scaling for everything managed by m_renderer with the resolution that UI and sprites are designed for.
	if (0 != SDL_RenderSetLogicalSize(m_renderer, m_config.screenWidth, m_config.screenHeight))
	{
		printf("Could not set logical size for renderer: %s\n", SDL_GetError());
		exit(-1);
	}
	// 7. Make a target texture to render too
	m_texTarget = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET, m_config.screenWidth, m_config.screenHeight);
	// 8. Reset render color
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);

	return 0;
}

int My::WinSDLOpenGLApplication::Tick(double deltaTime)
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		switch (e.type)
		{
		// Event quit
		case SDL_QUIT:
			BaseApplication::SetQuit(true);
			return 1;
		// Event mouse click
		case SDL_MOUSEBUTTONDOWN:
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					DEBUG_PRINT(Str(e.button.x) + ", " + Str(e.button.y) + " Mouse left button down. (OnMouseClick event broadcasted)\n");
					auto event = std::make_shared<Events::MouseClickEvent>(e.button.x, e.button.y);
					for (auto& ptr : g_pGameObjectResourceManager->FindAllResourcesByName("UI"))
					{
						event->AddSubscriber(ptr);
					}
					g_pEventDispatcher->DispatchToSubscribers(-1.0f, event);
				}
			}
			break;
		case SDL_MOUSEMOTION:
			{
				DEBUG_PRINT(Str(e.motion.x) + ", " + Str(e.motion.y) + "\n");
				auto event = std::make_shared<Events::MouseMotionEvent>(e.motion.x, e.motion.y);
				for (auto& ptr : g_pGameObjectResourceManager->FindAllResourcesByName("UI"))
				{
					event->AddSubscriber(ptr);
				}
				g_pEventDispatcher->DispatchToSubscribers(-1.0f, event);
			}
			break;
		default:
			break;
		}
	}
	IRunTimeModule::Tick(deltaTime);									// Execute additional callback functions added to the application.

	return 0;
}

int My::WinSDLOpenGLApplication::Finalize()
{
	glDeleteShader(m_programId);
	SDL_DestroyTexture(m_texTarget);
	SDL_DestroyRenderer(m_renderer);
	SDL_GL_DeleteContext(m_gContext);
	SDL_DestroyWindow(m_pWindow);
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}


/*Draw***************************************************************************************************************/

int My::WinSDLOpenGLApplication::BeginDraw()
{
	//Render to the texture
	SDL_SetRenderTarget(m_renderer, m_texTarget);
	SDL_RenderClear(m_renderer);
	return 0;
}

int My::WinSDLOpenGLApplication::OnDraw()
{
	// Process OnDrawCallback
	for (auto& func : m_OnDrawCallbacks) { func(); };
	return 0;
}

int My::WinSDLOpenGLApplication::EndDraw()
{
	int SCREEN_WIDTH, SCREEN_HEIGHT;
	SDL_GetWindowSize(m_pWindow, &SCREEN_WIDTH, &SCREEN_HEIGHT);
	presentBackBuffer(m_renderer, m_pWindow, m_texTarget, m_programId, SCREEN_WIDTH, SCREEN_HEIGHT);
	return 0;
}

/*Setter Getter***************************************************************************************************************/

void My::WinSDLOpenGLApplication::ShowMessage(const std::wstring& title, const std::wstring& message)
{
	std::string t(title.begin(), title.end());
	std::string m(message.begin(), message.end());
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, t.c_str(), m.c_str(), m_pWindow);
}

SDL_Window* My::WinSDLOpenGLApplication::GetWindow() noexcept
{
	return (m_pWindow) ? m_pWindow : nullptr;
}

SDL_Renderer* My::WinSDLOpenGLApplication::GetRenderer() noexcept
{
	return m_renderer;
}

void My::WinSDLOpenGLApplication::SetConfig(const GfxConfiguration& config) noexcept
{
	GfxConfiguration old_config(m_config);
	m_config = config;
	// Reset window size and render target size if configuration has changed.
	if (old_config.screenWidth != m_config.screenWidth || old_config.screenHeight != m_config.screenHeight)
	{
		SDL_SetWindowSize(m_pWindow, m_config.screenWidth, m_config.screenHeight);
		SDL_DestroyTexture(m_texTarget);
		//Make a target texture to render too
		m_texTarget = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, m_config.screenWidth, m_config.screenHeight);
	}
}
