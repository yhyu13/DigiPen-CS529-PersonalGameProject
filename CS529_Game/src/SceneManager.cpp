#include "SceneManager.hpp"
#include "Common/Event/EventDispatcher.hpp"

void My::Scene::Load(const char* level_file_path)
{
	if (g_pRootObject) g_pRootObject.reset();
	g_pRootObject = std::make_shared<RootObject>();

	LoadLevelConfigFromFile(level_file_path, m_config);
	LoadLevelObjectFromFile(level_file_path);
}

void My::Scene::Unload()
{
	if (g_pRootObject) g_pRootObject.reset();
	g_pGameObjectResourceManager->Clear();
	g_pBodyResourceManager->Clear();
	g_pEventDispatcher->Clear();
}

void My::Scene::LoadWave(const char* level_file_path)
{
	LoadLevelObjectFromFile(level_file_path);
}

My::SceneManager::SceneManager() noexcept
	:
	m_queueLevelConfigPath(""),
	m_currentLevelConfigPath(""),
	m_currentWaveConfigPath(""),
	m_queueWaveConfigPath("")
{
}

int My::SceneManager::Initialize()
{
	return 0;
}

int My::SceneManager::Tick(double deltaTime)
{
	if (m_queueLevelConfigPath != "")
	{
		g_pScene->Unload();
		g_pScene->Load(m_queueLevelConfigPath.c_str());
		m_currentLevelConfigPath = m_queueLevelConfigPath;
		m_queueLevelConfigPath = "";
	}
	if (m_queueWaveConfigPath != "")
	{
		g_pScene->LoadWave(m_queueWaveConfigPath.c_str());
		m_currentWaveConfigPath = m_queueWaveConfigPath;
		m_queueWaveConfigPath = "";
	}
	return 0;
}

int My::SceneManager::Finalize()
{
	g_pScene->Unload();
	return 0;
}
