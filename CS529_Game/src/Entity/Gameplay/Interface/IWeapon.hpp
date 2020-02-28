#pragma once

#include "WeaponType.hpp"
#include "src/Entity/Gameplay/Interface/IMyGameObject.hpp"

namespace My
{
	/**
	Comment :
	To create a custom weapon
	1. Add a new element to the enum type in the weapon type header
	2. Create a derived class which inherits from IWeapon
	3. Create a derived class which inherits from IWeaponCreator
	4. RegisterSerialization (g_weaponSerializationMap) in your initializer
	5. RegisterCreator (g_weaponCreatorMap) in your initializer
	*/
	class IWeapon : public IMyGameObject
	{
	public:
		IWeapon(const IWeapon&) = delete;
		IWeapon& operator=(const IWeapon&) = delete;
		explicit IWeapon(WEAPON_TYPE type, void* owner, std::string name) noexcept;
		// Virtual destructor is needed since we will use IWeapon* (the base class pointer)
		virtual ~IWeapon() {};
		virtual int Tick(double deltaTime) override;
		virtual int Draw() override;
		virtual void Serialize(std::wostream& wostream) override;

		WEAPON_TYPE GetType() const noexcept;

		void UpgradeWeapons();
		void DowngradeWeapons();
		void SetUpgradeLevel(int lvl);

	protected:
		WEAPON_TYPE m_type;
		int m_level;

	public:
		static int max_level;

	};

	class IWeaponCreator
	{
	public:
		/* owner = g_pRootObject.get(), pool = g_pMemoryManager.get() */
		virtual IWeapon* Create(void* owner, MemoryManager* pool, std::string name) = 0;
	};

	// TODO : Initialize in your application
	extern std::map<std::string, WEAPON_TYPE> g_weaponSerializationMap;
	extern std::map<WEAPON_TYPE, std::shared_ptr<IWeaponCreator>> g_weaponCreatorMap;

	IWeapon* LoadIWeaponFromFile(void* owner, const char* file_path, std::string name);
}