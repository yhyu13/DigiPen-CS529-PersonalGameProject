#include "Background.hpp"

int My::Entity::Background::Draw()
{
	// The scrolling background is also a player object 
	if (auto sceneCom = m_componentManager.GetComponent(COMPONENT_TYPE::SCENECOMPONENT))
	{
		static_cast<Component::SceneComponent*>(sceneCom)->Draw();
	}
	return 0;
}