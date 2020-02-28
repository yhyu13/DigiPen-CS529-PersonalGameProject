#pragma once
#include <map>
#include <SDL_mixer.h>
#include "Common/AudioManager.hpp"

namespace My
{
	class WinSDLAudioManager : public AudioManager
	{
	public:

		WinSDLAudioManager() noexcept;
		WinSDLAudioManager(const WinSDLAudioManager&) = delete;
		WinSDLAudioManager& operator=(const WinSDLAudioManager&) = delete;

		virtual int Initialize() override;
		virtual int Tick(double deltaTime) override;
		virtual int Finalize() override;

		void StopMusic();
		void PlayMusic(std::string name, int loops = -1);
		void ToggleMusic();
		void PlayChunck(std::string name, int channel = -1, int loops = 0);
		void Clear();

		int AddMusic(std::string name, Mix_Music* mus);
		int FreeMusicByName(std::string name);
		Mix_Music* GetMusicByName(std::string name);
		int AddChunck(std::string name, Mix_Chunk* mus);
		int FreeChunckByName(std::string name);
		Mix_Chunk* GetChunckByName(std::string name);

	public:
		std::string m_queueMusicPath;
		std::string m_queuechunckPath;
		std::string m_currentMusicPath;
		std::string m_currentchunckPath;
	private:
		std::map<std::string, Mix_Music*> g_musicMap;
		std::map<std::string, Mix_Chunk*> g_chunckMap;
	};

	extern std::shared_ptr<WinSDLAudioManager> g_pAudioManager;
}