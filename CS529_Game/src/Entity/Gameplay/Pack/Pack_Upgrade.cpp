#include "Pack_Upgrade.hpp"
#include "src/Component/Gameplay/WeaponarySystem.hpp"

int My::Entity::Pack_Upgrade::Tick(double deltaTime)
{
	if ((life_time -= deltaTime) <= 0.0)
	{
		SetGCToTrue();
	}
	IMyGameObject::Tick(deltaTime);
	return 0;
}

int My::Entity::Pack_Upgrade::Draw()
{
	auto temp = GetComponent(COMPONENT_TYPE::SCENECOMPONENT);
	if (temp)
	{
		static_cast<Component::SceneComponent*>(temp)->Draw();
		return 0;
	}
	return 1;
}

void My::Entity::Pack_Upgrade::OnComponentHit(std::weak_ptr<Event> e)
{
	std::shared_ptr<Event> _e = e.lock();
	auto event = static_cast<Events::ComponentHitEvent*>(_e.get());
	auto other = static_cast<IMyGameObject*>(event->other);

	if (other->GetType() == MYGAMEOBJECT_TYPE::PLAYER)
	{
		SetGCToTrue();
	}
}
