#pragma once

#include <mutex>
#include "Common/Component/Interface/IComponent.hpp"

namespace My
{
	namespace Component
	{
		class HealthPoint : public IComponent
		{
		public:
			HealthPoint() = delete;
			explicit HealthPoint(void* owner, std::string name);;
			virtual void Update(double deltaTime) override;

			virtual void Serialize(std::ifstream& fstream) override;
			virtual void Serialize(std::wostream& wostream) override;

			/* Lock HP for deltaTime seconds */
			void LockHP();
			void UnLockHP();
			void Restore(float value);
			void Reduce(float value);
			bool IsDeplited();
			bool IsLockHP();

		public:
			std::mutex m_mutex;
		private:
			float m_value;
			float m_maxValue;
			bool b_lockHP;
		};
	}

	class HealthPointCreator : public IComponentCreator
	{
	public:
		/* (Optional) Using a custom memory pool to create a allocated space */
		virtual IComponent* Create(void* owner = nullptr, MemoryPool* pool = nullptr, std::string name = "HealthPoint0") override
		{
			if (pool)
			{
				return pool->New<Component::HealthPoint>(owner, name);
			}
			else
			{
				return new Component::HealthPoint(owner, name);
			}
		}
	};
}