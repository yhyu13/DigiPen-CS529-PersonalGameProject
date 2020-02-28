#pragma once
#include "src/SerializedLoader.hpp"

namespace My
{
	class Scene
	{
	public:
		void Load(const char* level_file_path);
		void Unload();
		void LoadWave(const char* level_file_path);
	public:
		GfxConfiguration m_config;
	};

	extern std::shared_ptr<Scene> g_pScene;

	class SceneManager : public IRunTimeModule
	{
	public:
		SceneManager() noexcept;
		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;

		virtual int Initialize() override;
		virtual int Tick(double deltaTime) override;
		virtual int Finalize() override;
	public:
		std::string m_currentLevelConfigPath;
		std::string m_queueLevelConfigPath;

		std::string m_currentWaveConfigPath;
		std::string m_queueWaveConfigPath;
	};

	extern std::shared_ptr<SceneManager> g_pSceneManager;
}