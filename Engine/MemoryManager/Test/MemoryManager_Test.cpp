#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <Engine/MemoryManager/MemoryManager.h>
#include <Engine/MemoryManager/PoolMemoryStore.h>
#include <Engine/Utils/Factory.h>
#include <chrono>
#include <new>
#include <cstdlib>
#include <iostream>
#include <thread>

using namespace Scheduler;

TEST_CASE("Engine - MemoryManager - Malloc store", "[unit][functional][memory_manager]")
{
	MemoryManager memory_manager;
	MallocMemoryStore* memory_store = memory_manager.createMallocMemoryStore();

	void* memory50 = memory_store->allocate(50);
	void* memory1000 = memory_store->allocate(1000);
	void* memory5000 = memory_store->allocate(5000);

	memory_store->deallocate(memory50);
	memory_store->deallocate(memory1000);
	memory_store->deallocate(memory5000);

	memory_store->deallocate(nullptr);

	memory_manager.destroyMemoryStore(memory_store);

}

TEST_CASE("Engine - MemoryManager - Pool store", "[unit][functional][memory_manager]")
{
	MemoryManager memory_manager;

	

	SECTION("Raw allocation")
	{
		PoolMemoryStore* pool_memory_store = memory_manager.createPoolMemoryStore(56, 10);
	
		SECTION("Allocation within initial capacity")
		{
			void* ptrs[10];
			for (size_t i = 0; i < 10; ++i)
			{
				ptrs[i] = pool_memory_store->allocate(std::rand() % 55 + 1);
			}

			for (size_t i = 0; i < 9; ++i)
			{
				REQUIRE(reinterpret_cast<char*>(ptrs[i + 1]) - reinterpret_cast<char*>(ptrs[i]) == 56);
			}

			for (size_t i = 0; i < 10; ++i)
			{
				pool_memory_store->deallocate(ptrs[i]);
			}
		}

		SECTION("Allocation outside initial capacity")
		{
			void* ptrs[100];
			for (size_t i = 0; i < 100; ++i)
			{
				ptrs[i] = pool_memory_store->allocate(std::rand() % 55 + 1);
			}

			for (size_t i = 0; i < 100; ++i)
			{
				pool_memory_store->deallocate(ptrs[i]);
			}
		}

		SECTION("Allocations with returns")
		{
			void* ptrs[10];
			for (size_t i = 0; i < 10; ++i)
			{
				ptrs[i] = pool_memory_store->allocate(std::rand() % 55 + 1);
			}

			for (size_t i = 9; i >= 5; --i)
			{
				pool_memory_store->deallocate(ptrs[i]);
			}

			for (size_t i = 5; i < 10; ++i)
			{
				ptrs[i] = pool_memory_store->allocate(std::rand() % 55 + 1);
			}

			for (size_t i = 0; i < 9; ++i)
			{
				REQUIRE(reinterpret_cast<char*>(ptrs[i + 1]) - reinterpret_cast<char*>(ptrs[i]) == 56);
			}

			for (size_t i = 0; i < 10; ++i)
			{
				pool_memory_store->deallocate(ptrs[i]);
			}
		}

		SECTION("Allocation outside chunk size")
		{
			REQUIRE(pool_memory_store->allocate(57) == nullptr);
		}

		memory_manager.destroyMemoryStore(pool_memory_store);
	}

	SECTION("Object allocations")
	{
		
		struct TestStruct
		{
			int a;
			float b;
			double c;

			TestStruct():
				a(5),b(8),c(3.14)
			{
			}
		};

		SECTION("Exact size")
		{
			PoolMemoryStore* pool_memory_store = memory_manager.createPoolMemoryStore(sizeof(TestStruct), 10);

			TestStruct* ptrs[10];
			for (size_t i = 0; i < 10; ++i)
			{
				 void* mem = pool_memory_store->allocate(sizeof(TestStruct));
				 ptrs[i] = new(mem) TestStruct();
			}

			for (size_t i = 0; i < 9; ++i)
			{
				REQUIRE(reinterpret_cast<char*>(ptrs[i + 1]) - reinterpret_cast<char*>(ptrs[i]) == sizeof(TestStruct));
			}

			for (size_t i = 0; i < 10; ++i)
			{
				REQUIRE(ptrs[i]->a == 5);
				REQUIRE(ptrs[i]->b == 8);
				REQUIRE(ptrs[i]->c == Approx(3.14));
			}

			for (size_t i = 0; i < 10; ++i)
			{
				pool_memory_store->deallocate(ptrs[i]);
			}
		}

		SECTION("Covering size")
		{
			PoolMemoryStore* pool_memory_store = memory_manager.createPoolMemoryStore(152, 10);

			TestStruct* ptrs[10];
			for (size_t i = 0; i < 10; ++i)
			{
				void* mem = pool_memory_store->allocate(sizeof(TestStruct));
				ptrs[i] = new(mem) TestStruct();
			}

			for (size_t i = 0; i < 9; ++i)
			{
				REQUIRE(reinterpret_cast<char*>(ptrs[i + 1]) - reinterpret_cast<char*>(ptrs[i]) == 152);
			}

			for (size_t i = 0; i < 10; ++i)
			{
				REQUIRE(ptrs[i]->a == 5);
				REQUIRE(ptrs[i]->b == 8);
				REQUIRE(ptrs[i]->c == Approx(3.14));
			}

			for (size_t i = 0; i < 10; ++i)
			{
				pool_memory_store->deallocate(ptrs[i]);
			}
		}

		SECTION("Using factory")
		{
			Factory<TestStruct> test_struct_factory(&memory_manager, sizeof(TestStruct), 10);

			for (size_t j = 0; j < 500; ++j)
			{
				TestStruct* ptrs[500];
				for (size_t i = 0; i < 500; ++i)
				{
					ptrs[i] = test_struct_factory.createObject<TestStruct>();
				}

				for (size_t i = 0; i < 500; ++i)
				{
					test_struct_factory.destroyObject(ptrs[i]);
				}
			}
		}

	}

	
}

TEST_CASE("Performance", "[!mayfail]")
{
	MemoryManager memory_manager;

	struct TestStruct
	{
		int a;
		float b;
		double c;

		TestStruct() :
			a(5), b(8), c(3.14)
		{
		}
	};

	SECTION("Performance [debug only - no asserts]")
	{
		Factory<TestStruct> test_struct_factory(&memory_manager, sizeof(TestStruct), 10001);

		TestStruct* ptrs[10000];

		size_t total_pool_allocations_time = 0;
		size_t total_pool_deallocations_time = 0;
		for (size_t i = 0; i < 100; ++i)
		{
			auto start = std::chrono::high_resolution_clock::now();
			for (size_t j = 0; j < 10000; ++j)
			{
				ptrs[j] = test_struct_factory.createObject<TestStruct>();
			}
			auto end = std::chrono::high_resolution_clock::now();
			total_pool_allocations_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

			start = std::chrono::high_resolution_clock::now();
			for (size_t j = 0; j < 10000; ++j)
			{
				test_struct_factory.destroyObject(ptrs[j]);
			}
			end = std::chrono::high_resolution_clock::now();
			total_pool_deallocations_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		}

		float average_pool_allocation_time = total_pool_allocations_time / 100.0f / 10000.0f;
		float average_pool_deallocation_time = total_pool_deallocations_time / 100.0f / 10000.0f;

		INFO("Average pool allocation time: " << average_pool_allocation_time << " ns");
		INFO("Average pool deallocation time: " << average_pool_deallocation_time << " ns");

		size_t total_new_allocations_time = 0;
		size_t total_delete_deallocations_time = 0;
		for (size_t i = 0; i < 100; ++i)
		{
			auto start = std::chrono::high_resolution_clock::now();
			for (size_t j = 0; j < 10000; ++j)
			{
				ptrs[j] = new TestStruct();
			}
			auto end = std::chrono::high_resolution_clock::now();
			total_new_allocations_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

			start = std::chrono::high_resolution_clock::now();
			for (size_t j = 0; j < 10000; ++j)
			{
				delete ptrs[j];
			}
			end = std::chrono::high_resolution_clock::now();
			total_delete_deallocations_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		}

		float average_new_allocation_time = total_new_allocations_time / 100.0f / 10000.0f;
		float average_delete_deallocation_time = total_delete_deallocations_time / 100.0f / 10000.0f;

		INFO("Average new allocation time: " << average_new_allocation_time << " ns");
		INFO("Average delete deallocation time: " << average_delete_deallocation_time << " ns");

		//REQUIRE(false);
	}
/*
	SECTION("Thread safety [debug only - no asserts]")
	{

		Factory<TestStruct> test_struct_factory(&memory_manager, sizeof(TestStruct), 10001);

		TestStruct* ptrs[10000];


		size_t total_pool_allocations_time = 0;
		size_t total_pool_deallocations_time = 0;

		#pragma omp parallel for
		for (int k = 0; k < 4; ++k)
		{

			for (size_t i = 0; i < 100; ++i)
			{
				auto start = std::chrono::high_resolution_clock::now();
				for (size_t j = k * 2500; j < (k + 1) * 2500; ++j)
				{
					ptrs[j] = test_struct_factory.createObject<TestStruct>();
				}
				auto end = std::chrono::high_resolution_clock::now();
				total_pool_allocations_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

				start = std::chrono::high_resolution_clock::now();
				for (size_t j = k * 2500; j < (k + 1) * 2500; ++j)
				{
					test_struct_factory.destroyObject(ptrs[j]);
				}
				end = std::chrono::high_resolution_clock::now();
				total_pool_deallocations_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
			}
		}

		float average_pool_allocation_time = total_pool_allocations_time / 100.0f / 10000.0f;
		float average_pool_deallocation_time = total_pool_deallocations_time / 100.0f / 10000.0f;

		INFO("Average pool allocation time: " << average_pool_allocation_time << " ns");
		INFO("Average pool deallocation time: " << average_pool_deallocation_time << " ns");


		size_t total_new_allocations_time = 0;
		size_t total_delete_deallocations_time = 0;

		#pragma omp parallel for
		for (int k = 0; k < 4; ++k)
		{
			for (size_t i = 0; i < 100; ++i)
			{
				auto start = std::chrono::high_resolution_clock::now();
				for (size_t j = k * 2500; j < (k + 1) * 2500; ++j)
				{
					ptrs[j] = new TestStruct();
				}
				auto end = std::chrono::high_resolution_clock::now();
				total_new_allocations_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

				start = std::chrono::high_resolution_clock::now();
				for (size_t j = k * 2500; j < (k + 1) * 2500; ++j)
				{
					delete ptrs[j];
				}
				end = std::chrono::high_resolution_clock::now();
				total_delete_deallocations_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
			}
		}

		float average_new_allocation_time = total_new_allocations_time / 100.0f / 10000.0f;
		float average_delete_deallocation_time = total_delete_deallocations_time / 100.0f / 10000.0f;

		INFO("Average new allocation time: " << average_new_allocation_time << " ns");
		INFO("Average delete deallocation time: " << average_delete_deallocation_time << " ns");

		REQUIRE(false);
	}
	*/
}