#include "IMyGameObject.hpp"

My::IMyGameObject::IMyGameObject(MYGAMEOBJECT_TYPE type, void* owner, std::string name) noexcept
	:
	IGameObject(GAMEOBJECT_TYPE::CUSTOM_GAME, owner, name),
	m_type(type)
{}

My::MYGAMEOBJECT_TYPE My::IMyGameObject::GetType() const noexcept
{
	return m_type;
}
