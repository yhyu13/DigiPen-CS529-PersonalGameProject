#include "pch.h"
#include "CppUnitTest.h"

#include "Common/Geommath/2D/Math2D.hpp"
#include "Common/MemoryPool.hpp"
#include "Common/MemoryManager.hpp"
#include "Common/Utility/Utility.hpp"
#include "Common/Utility/Timer.hpp"

using namespace std;
using namespace My;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestGameEngine
{
	TEST_CLASS(TestGameEngine)
	{
	public:
		TEST_METHOD(TestMethod1)
		{
			Logger::WriteMessage("TestMethod1 : Memory pool Test.");
			MemoryPool pool;
			pool.New<int>(4);
			pool.New<int>(4);
			pool.New<int>(4);

			for (auto& ptr : pool.GetBookKeeper())
			{
				int temp = *(static_cast<int*>(ptr));
				Assert::AreEqual(temp, 4);
				Logger::WriteMessage((Str(temp) + ", 4").c_str());
			}

			Logger::WriteMessage("Resize pool.");
			pool.Allocate(1024);
			pool.Allocate(1025);

			int i = 0;
			for (auto& ptr : pool.GetBookKeeper())
			{
				int temp = *(static_cast<int*>(ptr));
				Assert::AreEqual(temp, 4);
				Logger::WriteMessage((Str(temp) + ", 4").c_str());
				if (++i == 3) break;
			}
			Logger::WriteMessage("TestMethod1 Success.");
		}

		TEST_METHOD(TestMethod2)
		{
			Logger::WriteMessage("TestMethod2 : Memory manager Test.");
			MemoryManager* g_pMemoryManager = new MemoryManager();
			g_pMemoryManager->Initialize();

			auto timer = Timer();
			for (int i = 0; i < 10000; i++)
			{
				int* a = g_pMemoryManager->New<int>(30);
				g_pMemoryManager->Delete(a);
			}
			Logger::WriteMessage(("MyMemoryManager Score (int): " + Str(timer.Mark()) + "s").c_str());

			timer.Reset();
			for (int i = 0; i < 10000; i++)
			{
				int* a = new int(30);
				delete a;
			}
			Logger::WriteMessage(("C++NativeMalloc Score (int): " + Str(timer.Mark()) + "s").c_str());

			timer.Reset();
			for (int i = 0; i < 10000; i++)
			{
				char* space = (char*)g_pMemoryManager->Allocate(1024);
				g_pMemoryManager->Free(space, 1024);
			}
			Logger::WriteMessage(("MyMemoryManager Score (1kb): " + Str(timer.Mark()) + "s").c_str());

			timer.Reset();
			for (int i = 0; i < 10000; i++)
			{
				char* space = (char*)malloc(1024);
				free(space);
			}
			Logger::WriteMessage(("C++NativeMalloc Score (1kb): " + Str(timer.Mark()) + "s").c_str());

			timer.Reset();
			for (int i = 0; i < 10000; i++)
			{
				char* space = (char*)g_pMemoryManager->Allocate(4096);
				g_pMemoryManager->Free(space, 4096);
			}
			Logger::WriteMessage(("MyMemoryManager Score (4kb): " + Str(timer.Mark()) + "s").c_str());

			timer.Reset();
			for (int i = 0; i < 10000; i++)
			{
				char* space = (char*)malloc(4096);
				free(space);
			}
			Logger::WriteMessage(("C++NativeMalloc Score (4kb): " + Str(timer.Mark()) + "s").c_str());

			timer.Reset();
			for (int i = 0; i < 10000; i++)
			{
				Vector2D* vec = g_pMemoryManager->New<Vector2D>();
				g_pMemoryManager->Delete(vec);
			}
			Logger::WriteMessage(("MyMemoryManager Score (Vector2D): " + Str(timer.Mark()) + "s").c_str());

			timer.Reset();
			for (int i = 0; i < 10000; i++)
			{
				Vector2D* vec = new Vector2D();
				delete vec;
			}
			Logger::WriteMessage(("C++NativeMalloc Score (Vector2D): "  + Str(timer.Mark()) + "s").c_str());

			timer.Reset();
			for (int i = 0; i < 100; i++)
			{
				//std::vector<int, My::allocator<int>> vec2;
				MyVector(int) vec2;
				for (int j = 0; j < 1000; j++)
					vec2.push_back(1);
			}
			Logger::WriteMessage(("MyMemoryManager Score (vec int): " + Str(timer.Mark()) + "s").c_str());

			timer.Reset();
			for (int i = 0; i < 100; i++)
			{
				std::vector<int> vec2;
				for (int j = 0; j < 1000; j++)
					vec2.push_back(1);
			}
			Logger::WriteMessage(("C++NativeMalloc Score (vec int): " + Str(timer.Mark()) + "s").c_str());

			timer.Reset();
			for (int i = 0; i < 100; i++)
			{
				MyVector(Vector2D) vec2;
				for (int j = 0; j < 1000; j++)
					vec2.push_back(Vector2D());
			}
			Logger::WriteMessage(("MyMemoryManager Score (vec Vector2D): " + Str(timer.Mark()) + "s").c_str());

			timer.Reset();
			for (int i = 0; i < 100; i++)
			{
				std::vector<Vector2D> vec2;
				for (int j = 0; j < 1000; j++)
					vec2.push_back(Vector2D());
			}
			Logger::WriteMessage(("C++NativeMalloc Score (vec Vector2D): " + Str(timer.Mark()) + "s").c_str());

			char* space1 = (char*)g_pMemoryManager->Allocate(11);
			char* temp = space1;
			for (int j = 0; j < 10; j++)
			{
				*temp = ('0' + j);
				temp++;
			}
			*temp = '\0';
			wcout << space1 << endl;
			g_pMemoryManager->Delete(space1);

			g_pMemoryManager->Finalize();

			delete g_pMemoryManager;
			Logger::WriteMessage("TestMethod2 Success.");
		}
	};
}
