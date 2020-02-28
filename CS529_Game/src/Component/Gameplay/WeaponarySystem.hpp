#pragma once
#include "Common/Component/Interface/IComponent.hpp"
#include "src/Entity/Gameplay/Interface/IWeapon.hpp"

namespace My
{
	namespace Component
	{
		class WeaponarySystem : public IComponent
		{
		public:
			WeaponarySystem() = delete;
			explicit WeaponarySystem(void* owner, std::string name);
			virtual void Update(double deltaTime) override;
			virtual ~WeaponarySystem();

			virtual void Serialize(std::ifstream& fstream) override;
			virtual void Serialize(std::wostream& wostream) override;

			void UpgradeWeapons();
			void DowngradeWeapons();
			void ChangeWeapon(int id, std::string name);
		private:
			int m_maxWeapons;
			std::vector<IWeapon*> m_currentWeapons;
			std::vector<std::map<std::string, IWeapon*>> m_weaponMaps;
		};
	}

	class WeaponarySystemCreator : public IComponentCreator
	{
	public:
		/* (Optional) Using a custom memory pool to create a allocated space */
		virtual IComponent* Create(void* owner = nullptr, MemoryPool* pool = nullptr, std::string name = "WeaponarySystem0") override
		{
			if (pool)
			{
				return pool->New<Component::WeaponarySystem>(owner, name);
			}
			else
			{
				return new Component::WeaponarySystem(owner, name);
			}
		}
	};
}