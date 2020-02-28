#include "WinSDLAudioManager.hpp"
#include "src/SerializedLoader.hpp"

My::WinSDLAudioManager::WinSDLAudioManager() noexcept
	:
	m_queueMusicPath(""),
	m_queuechunckPath(""),
	m_currentMusicPath(""),
	m_currentchunckPath("")
{
}

int My::WinSDLAudioManager::Initialize()
{
	if (g_gameObjectConfigurationIfstreamMap.find("AudioList") != g_gameObjectConfigurationIfstreamMap.end())
	{
		std::ifstream& input(g_gameObjectConfigurationIfstreamMap["AudioList"]);
		if (!input.is_open())
		{
			std::string msg("Can't open object file: AudioList");
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
		}
		else
		{
			for (std::string line; getline(input, line);)
			{
				char audioType[256] = { 0 };
				char audioName[256] = { 0 };
				char audioPath[256] = { 0 };

				if (3 == sscanf_s(line.c_str(), "%s %s %s", audioType, 256, audioName, 256, audioPath, 256))
				{
					if (std::string(audioType).compare("Music") == 0)
					{
						auto mus = Mix_LoadMUS(audioPath);
						if (mus)
						{
							AddMusic(audioName, mus);
						}
						else
						{
							printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
							print_fstream(input);
							std::string msg(Str(audioPath) + " fails to load : " + line);
							throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
						}
					}
					else if (std::string(audioType).compare("Chunck") == 0)
					{
						auto wav = Mix_LoadWAV(audioPath);
						if (wav)
						{
							AddChunck(audioName, wav);
						}
						else
						{
							printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
							print_fstream(input);
							std::string msg(Str(audioPath) + " fails to load : " + line);
							throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
						}
					}
					else
					{
						print_fstream(input);
						std::string msg("AudioList does not contain a valid music type : " + line);
						throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
					}
				}
				else
				{
					print_fstream(input);
					std::string msg("AudioList does not contain a valid music in the form ~ ~ ~ : " + line);
					throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
				}
			}
		}
	}
	else
	{
		std::string msg("Please register 'AudioList' into g_gameObjectConfigurationIfstreamMap.");
		throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
	}
	return 0;
}

int My::WinSDLAudioManager::Tick(double deltaTime)
{
	if (m_queueMusicPath != "")
	{
		PlayMusic(m_queueMusicPath);
		m_currentMusicPath = m_queueMusicPath;
		m_queueMusicPath = "";
	}
	if (m_queuechunckPath != "")
	{
		PlayChunck(m_queuechunckPath);
		m_currentchunckPath = m_queuechunckPath;
		m_queuechunckPath = "";
	}
	return 0;
}

int My::WinSDLAudioManager::Finalize()
{
	StopMusic();
	Clear();
	return 0;
}

void My::WinSDLAudioManager::StopMusic()
{
	//Stop the music
	Mix_HaltMusic();
}

void My::WinSDLAudioManager::PlayMusic(std::string name, int loops)
{
	//Stop the music
	Mix_HaltMusic();
	//If there is no music playing
	if (Mix_PlayingMusic() == 0)
	{
		//Play the music
		if (g_musicMap.find(name) != g_musicMap.end())
		{
			if (Mix_PlayMusic(g_musicMap[name], loops) == -1) {
				printf("Mix_PlayMusic: %s\n", Mix_GetError());
				// well, there's no music, but most games don't break without music...
			}
		}
		else
		{
			std::string msg("Audio manager does not contain music with name: " + name);
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
		}
	}
}

void My::WinSDLAudioManager::ToggleMusic()
{
	//If music is being played
	if (Mix_PlayingMusic() != 0)
	{
		//If the music is paused
		if (Mix_PausedMusic() == 1)
		{
			//Resume the music
			Mix_ResumeMusic();
		}
		//If the music is playing
		else
		{
			//Pause the music
			Mix_PauseMusic();
		}
	}
}

void My::WinSDLAudioManager::PlayChunck(std::string name, int channel, int loops)
{
	//Play the music
	if (g_chunckMap.find(name) != g_chunckMap.end())
	{
		if (Mix_PlayChannel(channel, g_chunckMap[name], loops) == -1) {
			printf("Mix_PlayChannel: %s\n", Mix_GetError());
			// well, there's no music, but most games don't break without music...
		}
	}
	else
	{
		std::string msg("Audio manager does not contain music with name: " + name);
		throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
	}
}

void My::WinSDLAudioManager::Clear()
{
	for (auto it = g_chunckMap.begin(); it != g_chunckMap.end(); ++it)
	{
		Mix_FreeChunk(it->second);
	}
	for (auto it = g_musicMap.begin(); it != g_musicMap.end(); ++it)
	{
		Mix_FreeMusic(it->second);
	}
	g_chunckMap.clear();
	g_musicMap.clear();
}

int My::WinSDLAudioManager::AddMusic(std::string name, Mix_Music* mus)
{
	if (g_musicMap[name])
	{
		Mix_FreeMusic(g_musicMap[name]);
	}
	else
	{
		g_musicMap[name] = mus;
	}
	return 0;
}

int My::WinSDLAudioManager::FreeMusicByName(std::string name)
{
	if (g_musicMap.find(name) != g_musicMap.end())
	{
		Mix_FreeMusic(g_musicMap[name]);
		g_musicMap.erase(name);
		return 0;
	}
	else
	{
		return 1;
	}
}

Mix_Music* My::WinSDLAudioManager::GetMusicByName(std::string name)
{
	if (g_musicMap.find(name) != g_musicMap.end())
	{
		return g_musicMap[name];
	}
	else
	{
		return nullptr;
	}
}

int My::WinSDLAudioManager::AddChunck(std::string name, Mix_Chunk* mus)
{
	if (g_chunckMap[name])
	{
		Mix_FreeChunk(g_chunckMap[name]);
	}
	else
	{
		g_chunckMap[name] = mus;
	}
	return 0;
}

int My::WinSDLAudioManager::FreeChunckByName(std::string name)
{
	if (g_chunckMap.find(name) != g_chunckMap.end())
	{
		Mix_FreeChunk(g_chunckMap[name]);
		g_chunckMap.erase(name);
		return 0;
	}
	else
	{
		return 1;
	}
}

Mix_Chunk* My::WinSDLAudioManager::GetChunckByName(std::string name)
{
	if (g_chunckMap.find(name) != g_chunckMap.end())
	{
		return g_chunckMap[name];
	}
	else
	{
		return nullptr;
	}
}
