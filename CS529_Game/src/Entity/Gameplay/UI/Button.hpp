#pragma once
#include "src/Entity/Gameplay/Interface/IMyGameObject.hpp"

namespace My
{
	namespace Entity
	{
		class UI_Button : public IMyGameObject
		{
		public:
			UI_Button() = delete;
			explicit UI_Button(void* owner, std::string name) noexcept 
				: 
				IMyGameObject(MYGAMEOBJECT_TYPE::UI_Button, owner, name),
				b_isShown(true)
			{
				m_eventManager.AddEventRegistry(EventType::ON_MOUSECLICK);
				m_eventManager.AddEventRegistry(EventType::ON_MOUSEMOTION);
			}
			virtual int Draw() override;
			virtual void HandleEvent(std::weak_ptr<Event> e) override
			{
				if (GetIsShown() && IsActive())
				{
					std::shared_ptr<Event> _e = e.lock();
					switch (_e->GetType())
					{
					case EventType::ON_MOUSECLICK:
						OnClick(_e);
						break;
					case EventType::ON_MOUSEMOTION:
						OnHover(_e);
						break;

					default:
						break;
					}
				}
			}
			// Custom events
			void OnClick(std::weak_ptr<Event> e);
			void OnHover(std::weak_ptr<Event> e);

			void SetIsShown(bool v);
			bool GetIsShown();

		private:
			bool b_isShown;
		};
	}

	class UI_ButtonObjectCreator : public IMyGameObjectCreator
	{
	public:
		/* (Optional) Using a custom memory pool to create a allocated space */
		virtual IMyGameObject* Create(void* owner = nullptr, MemoryManager* pool = nullptr, std::string name = "UI_Button0") override
		{
			if (pool)
			{
				return pool->New<Entity::UI_Button>(owner, name);
			}
			else
			{
				return new Entity::UI_Button(owner, name);
			}
		}
	};


	namespace Entity
	{
		class UI_Volume_Button : public IMyGameObject
		{
		public:
			UI_Volume_Button() = delete;
			explicit UI_Volume_Button(void* owner, std::string name) noexcept
				:
				IMyGameObject(MYGAMEOBJECT_TYPE::UI_Volume_Button, owner, name)
			{
				m_eventManager.AddEventRegistry(EventType::ON_MOUSECLICK);
			}
			virtual int Tick(double deltaTime) override;
			virtual int Draw() override;
			virtual void HandleEvent(std::weak_ptr<Event> e) override
			{
				if (IsActive())
				{
					std::shared_ptr<Event> _e = e.lock();
					switch (_e->GetType())
					{
					case EventType::ON_MOUSECLICK:
						OnClick(_e);
						break;
					default:
						break;
					}
				}
			}
			// Custom events
			void OnClick(std::weak_ptr<Event> e);
		private:
			static int mode;
		};
	}

	class UI_Volume_ButtonObjectCreator : public IMyGameObjectCreator
	{
	public:
		/* (Optional) Using a custom memory pool to create a allocated space */
		virtual IMyGameObject* Create(void* owner = nullptr, MemoryManager* pool = nullptr, std::string name = "UI_Volume_Button0") override
		{
			if (pool)
			{
				return pool->New<Entity::UI_Volume_Button>(owner, name);
			}
			else
			{
				return new Entity::UI_Volume_Button(owner, name);
			}
		}
	};
}