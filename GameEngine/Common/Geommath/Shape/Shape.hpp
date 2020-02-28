#pragma once
#include <string>
#include <fstream>
#include "ShapeType.h"
#include "Common/MemoryManager.hpp"
#include "Common/ResourceManager.hpp"
#include "Common/Geommath/2D/Math2D.hpp"

/*
Collision bounding box
*/
namespace My
{
	/*
		Base Collision
	*/
	namespace Collision
	{
		class Shape
		{
		public:
			Shape() = delete;
			explicit Shape(SHAPE_TYPE type, void* owner);
			virtual ~Shape();;
			virtual void Serialize(std::ifstream& fstream) = 0;
			SHAPE_TYPE GetType() const noexcept;
			void SetOwner(void* owner) noexcept;
			void* GetOwner() const noexcept;
			bool IsActive() const noexcept;
			void SetActive(bool v) noexcept;
		private:
			SHAPE_TYPE m_type;
			void* m_owner;
			bool m_active;
		};
	}

	class ShapeCreator
	{
	public:
		/* owner = g_pRootObject.get(), pool = g_pMemoryManager.get() */
		virtual Collision::Shape* Create(void* owner = nullptr, MemoryManager* pool = nullptr) = 0;
	};

	// TODO : Initialize in your application
	extern std::map<std::string, SHAPE_TYPE> g_shapeSerializationMap;
	extern std::map<SHAPE_TYPE, std::shared_ptr<ShapeCreator>> g_shapeCreatorMap;

	/*
		Circle
	*/
	namespace Collision
	{
		class Circle : public Shape
		{
		public:
			Circle() = delete;
			explicit Circle(void* owner);;
			virtual void Serialize(std::ifstream& fstream) override;
		public:
			float m_radius;
		};
	}
	class CircleCreator : public ShapeCreator
	{
	public:
		/* owner = g_pRootObject.get(), pool = g_pMemoryManager.get() */
		virtual Collision::Shape* Create(void* owner = nullptr, MemoryManager* pool = nullptr)
		{
			if (pool)
			{
				return pool->New<Collision::Circle>(owner);
			}
			else
			{
				return new Collision::Circle(owner);
			}
		}
	};

	/*
		Rectangle
	*/
	namespace Collision
	{
		class Rectangle : public Shape
		{
		public:
			Rectangle() = delete;
			explicit Rectangle(void* owner);;
			virtual void Serialize(std::ifstream& fstream) override;
		public:
			float m_width;
			float m_height;
		};
	}
	class RectangleCreator : public ShapeCreator
	{
	public:
		/* owner = g_pRootObject.get(), pool = g_pMemoryManager.get() */
		virtual Collision::Shape* Create(void* owner = nullptr, MemoryManager* pool = nullptr)
		{
			if (pool)
			{
				return pool->New<Collision::Rectangle>(owner);
			}
			else
			{
				return new Collision::Rectangle(owner);
			}
		}
	};

}