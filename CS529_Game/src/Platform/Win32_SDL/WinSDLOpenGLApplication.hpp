/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: WinSDLOpenGLApplication.hpp
Purpose: header file of WinSDLOpenGLApplication
Language: c++ 11
Platform: win32 x86
Project: CS529_final_project
Author: Hang Yu hang.yu 60001119
Creation date: 10/13/2019
- End Header ----------------------------*/

#pragma once
#include "Common/BaseApplication.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>
#include <windows.h>


extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLVALIDATEPROGRAMPROC glValidateProgram;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLUSEPROGRAMPROC glUseProgram;

extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;

extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;

extern PFNGLUNIFORM1IPROC glUniform1i;

bool initGLExtensions();


namespace My
{
	class WinSDLOpenGLApplication : public BaseApplication
	{
	public:

		WinSDLOpenGLApplication() = delete;
		WinSDLOpenGLApplication(const WinSDLOpenGLApplication&) = delete;
		WinSDLOpenGLApplication& operator=(const WinSDLOpenGLApplication&) = delete;
		explicit WinSDLOpenGLApplication(const GfxConfiguration& config)
			:
			BaseApplication(config),
			m_gContext(nullptr),
			m_pWindow(nullptr),
			m_renderer(nullptr),
			m_texTarget(nullptr),
			m_programId(0)
		{}

		virtual int Initialize() override;
		virtual int Tick(double deltaTime) override;
		virtual int Finalize() override;

		virtual int BeginDraw() override;
		virtual int OnDraw() override;
		virtual int EndDraw() override;

		virtual void ShowMessage(const std::wstring& title, const std::wstring& message) override;

		SDL_Window* GetWindow() noexcept;
		SDL_Renderer* GetRenderer() noexcept;
		virtual void SetConfig(const GfxConfiguration& config) noexcept override;

		void AddCallBackToOnDraw(void(*func)()) { m_OnDrawCallbacks.push_back(func); };
		void ClearCallBackOnDraw() { m_OnDrawCallbacks.clear(); };

	private:
		GLuint m_programId;
		SDL_Window* m_pWindow;
		SDL_GLContext m_gContext;
		SDL_Renderer* m_renderer;
		SDL_Texture* m_texTarget;
		std::vector<std::function<void()>> m_OnDrawCallbacks;
	};
}