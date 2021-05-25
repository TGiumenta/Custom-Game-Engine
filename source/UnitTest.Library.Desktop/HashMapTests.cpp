#include "pch.h"
#include <crtdbg.h>
#include <exception>
#include <CppUnitTest.h>
#include "Foo.h"
#include "HashMap.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace FieaGameEngine
{
	template <>
	struct DefaultHash<Foo>
	{
	public:
		size_t operator()(const Foo& key) const
		{
			return key.Data();
		}
	};
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(HashMapTests)
	{
	public:
		TEST_METHOD_INITIALIZE(ClassInitialize)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF); // Changes the way that "new" works
			_CrtMemCheckpoint(&sStartMemState);
#endif // Debug
		}

		TEST_METHOD_CLEANUP(MethodCleanup)
		{
#ifdef _DEBUG

			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leak"); // Makes this string a wide character
			}
#endif // DEBUG
		}

		TEST_METHOD(TestConstructor)
		{
			HashMap<Foo, std::string> map(5_z);
			Assert::AreEqual(0_z, map.Size());
			HashMap<Foo, std::string> def_map;
			Assert::AreEqual(0_z, map.Size());

			auto expressionA = [] { HashMap<Foo, std::string> map(0_z); };
			Assert::ExpectException<std::invalid_argument>(expressionA);
		}

		TEST_METHOD(TestCopyConstructor)
		{
			HashMap<Foo, std::string> map(5_z);
			Assert::AreEqual(0_z, map.Size());

			auto my_pair = std::make_pair(Foo(10), "Hello");
			map.Insert(my_pair);
			Assert::AreEqual(1_z, map.Size());
			auto my_next_pair = std::make_pair(Foo(49), "Goodbye");
			map.Insert(my_next_pair);

			Assert::AreEqual(2_z, map.Size());

			HashMap<Foo, std::string> copy_map(map);
			Assert::AreEqual(2_z, copy_map.Size());

			HashMap<Foo, std::string> r_copy_map(std::move(copy_map));
			Assert::AreEqual(2_z, r_copy_map.Size());
		}

		TEST_METHOD(TestLValueEqualOperator)
		{
			HashMap<Foo, std::string> map(5_z);
			auto my_pair = std::make_pair(Foo(10), "Hello");
			map.Insert(my_pair);

			HashMap<Foo, std::string> other_map;
			auto my_next_pair = std::make_pair(Foo(490), "Goodbye");
			other_map.Insert(my_next_pair);

			auto my_dead_pair = std::make_pair(Foo(9107), "Help");
			other_map.Insert(my_dead_pair);

			other_map = map;

			Assert::AreEqual(other_map.Size(), map.Size());
		}

		TEST_METHOD(TestRValueEqualOperator)
		{
			HashMap<Foo, std::string> map(5_z);
			auto my_pair = std::make_pair(Foo(10), "Hello");
			map.Insert(my_pair);
			auto my_next_pair = std::make_pair(Foo(490), "Goodbye");
			map.Insert(my_next_pair);

			size_t map_size = map.Size();

			HashMap<Foo, std::string> other_map;
			other_map = std::move(map);

			Assert::AreEqual(map_size, other_map.Size());
		}

		TEST_METHOD(TestFind)
		{
			// Non-Const
			{
				HashMap<Foo, std::string> map(5_z);
				Assert::AreEqual(0_z, map.Size());

				Foo a(10);
				auto my_pair = std::make_pair(a, "Hello");

				auto pair_tuple = map.Insert(my_pair);
				Assert::AreEqual(1_z, map.Size());
				Assert::AreEqual(std::get<1>(pair_tuple)->first, a);

				Assert::AreEqual(std::get<1>(pair_tuple), map.Find(a));

				Foo b(45);
				auto my_next_pair = std::make_pair(b, "Goodbye");
				auto next_pair_tuple = map.Insert(my_next_pair);
				Assert::AreEqual(2_z, map.Size());

				Assert::AreEqual(std::get<1>(next_pair_tuple), map.Find(b));

				Foo c(30);
				const HashMap<Foo, std::string> c_map(5_z);
				Assert::AreEqual(0_z, c_map.Size());
				Assert::AreEqual(c_map.end(), c_map.Find(c));
			}
			// Const
			{
				HashMap<Foo, std::string> map(5_z);
				Assert::AreEqual(0_z, map.Size());

				Foo a(10);
				auto my_pair = std::make_pair(a, "Hello");

				auto pair_tuple = map.Insert(my_pair);
				Assert::AreEqual(1_z, map.Size());

				Assert::AreEqual(std::get<1>(pair_tuple), map.Find(a));

				const HashMap<Foo, std::string> other_map(map);
				Assert::AreNotEqual(other_map.end(), other_map.Find(a));

				const HashMap<Foo, std::string>::ConstIterator const_it = other_map.begin();
				Assert::AreEqual(const_it->first, a);
			}
		}

		TEST_METHOD(TestInsert)
		{
			HashMap<Foo, std::string> map(5_z);
			Assert::AreEqual(0_z, map.Size());

			auto my_pair = std::make_pair(Foo(10), "Hello");

			map.Insert(my_pair);
			Assert::AreEqual(1_z, map.Size());
			map.Insert(my_pair);
			Assert::AreEqual(1_z, map.Size());

			auto my_next_pair = std::make_pair(Foo(45), "Goodbye");
			map.Insert(my_next_pair);
			Assert::AreEqual(2_z, map.Size());
		}

		TEST_METHOD(TestBracketOperator)
		{
			HashMap<Foo, std::string> map(5_z);
			Assert::AreEqual(0_z, map.Size());

			auto my_pair = std::make_pair(Foo(10), "Hello");
			map.Insert(my_pair);
			Assert::AreEqual(1_z, map.Size());

			Foo b(45);
			std::string goodbye = "Goodbye";
			std::pair<Foo, std::string> my_next_pair(b, goodbye);
			map.Insert(my_next_pair);

			Assert::AreEqual(map[b], goodbye);

			using namespace std::string_literals;

			Foo c(200);
			auto my_empty_val_pair = std::make_pair(c, "");
			Assert::AreEqual(map[c], ""s);

			const HashMap<Foo, std::string> const_map = map;
			Assert::AreEqual(const_map[c], ""s);
		}

		TEST_METHOD(TestRemove)
		{
			HashMap<Foo, std::string> map(5_z);
			Assert::AreEqual(0_z, map.Size());

			auto my_pair = std::make_pair(Foo(10), "Hello");
			map.Insert(my_pair);
			Assert::AreEqual(1_z, map.Size());

			Foo b(45);
			auto my_next_pair = std::make_pair(b, "Goodbye");
			map.Insert(my_next_pair);
			Assert::AreEqual(2_z, map.Size());

			map.Remove(b);
			Assert::AreEqual(1_z, map.Size());

			map.Remove(Foo(5000));
			Assert::AreEqual(1_z, map.Size());
		}

		TEST_METHOD(TestResize)
		{
			HashMap<Foo, std::string> map(5_z);
			Assert::AreEqual(0_z, map.Size());
			HashMap<Foo, std::string> def_map;
			Assert::AreEqual(0_z, map.Size());

			auto expressionA = [] { HashMap<Foo, std::string> map(0_z); };
			Assert::ExpectException<std::invalid_argument>(expressionA);

			auto my_pair = std::make_pair(Foo(10), "Hello");
			map.Insert(my_pair);
			map.Resize(1_z);
			Assert::AreEqual(1_z, map.BucketSize());
		}

		TEST_METHOD(TestClear)
		{
			HashMap<Foo, std::string> map(5_z);
			Assert::AreEqual(0_z, map.Size());

			auto my_pair = std::make_pair(Foo(10), "Hello");

			map.Insert(my_pair);
			Assert::AreEqual(1_z, map.Size());
			map.Clear();
			Assert::AreEqual(0_z, map.Size());
			map.Insert(my_pair);
			Assert::AreEqual(1_z, map.Size());
		}

		TEST_METHOD(TestContainsKey)
		{
			HashMap<Foo, std::string> map;
			Assert::AreEqual(0_z, map.Size());

			Foo a(10);
			const std::pair<Foo, std::string> my_pair = std::make_pair(a, "Hello");
			map.Insert(my_pair);

			Assert::AreEqual(1_z, map.Size());
			Assert::AreEqual(true, map.ContainsKey(a));
			Assert::AreEqual(false, map.ContainsKey(Foo(20)));
		}

		TEST_METHOD(TestAt)
		{
			// Non-Const
			HashMap<Foo, std::string> map;
			Assert::AreEqual(0_z, map.Size());

			Foo a(10);
			std::string hello = "Hello";

			std::pair<Foo, std::string> my_pair(a, hello);
			map.Insert(my_pair);
			Assert::AreEqual(hello, map.at(a));

			Foo b(45);
			std::string goodbye = "Goodbye";
			std::pair<Foo, std::string> my_next_pair(b, goodbye);
			map.Insert(my_next_pair);

			auto expressionA = [&map] { map.at(Foo(65)); };
			Assert::ExpectException<std::runtime_error>(expressionA);

			// Const
			const HashMap<Foo, std::string> const_map = map;
			Assert::AreEqual(hello, const_map.at(a));

			auto expressionB = [&const_map] { const_map.at(Foo(65)); };
			Assert::ExpectException<std::runtime_error>(expressionB);
		}

		TEST_METHOD(TestBegin)
		{
			// Non-Const
			{
				HashMap<Foo, std::string> map;
				Assert::AreEqual(0_z, map.Size());
				Assert::AreEqual(map.begin(), map.end());
				Assert::AreEqual(map.cbegin(), map.cend());
			}

			// Const
			{
				HashMap<Foo, std::string> map;
				auto my_pair = std::make_pair(Foo(10), "Hello");
				map.Insert(my_pair);
				auto my_next_pair = std::make_pair(Foo(45), "Goodbye");
				map.Insert(my_next_pair);

				const HashMap<Foo, std::string> const_map = map;
				Assert::AreEqual(const_map.begin(), const_map.begin());
				Assert::AreEqual(const_map.cbegin(), const_map.cbegin());

				const HashMap<Foo, std::string> empty_map;
				Assert::AreEqual(0_z, empty_map.Size());
				Assert::AreEqual(empty_map.begin(), empty_map.end());
				Assert::AreEqual(empty_map.cbegin(), empty_map.cend());
			}
		}

		TEST_METHOD(TestIncrement)
		{
			HashMap<Foo, std::string> map;
			auto my_pair = std::make_pair(Foo(10), "Hello");
			map.Insert(my_pair);
			auto my_next_pair = std::make_pair(Foo(45), "Goodbye");
			map.Insert(my_next_pair);

			// Both Prefix/Postfix
			size_t counter = 0;
			for (HashMap<Foo, std::string>::Iterator it = map.begin(); it != map.end(); ++it)
			{
				++counter;
			}
			Assert::AreEqual(counter, map.Size());
			counter = 0;
			for (HashMap<Foo, std::string>::Iterator it = map.begin(); it != map.end(); it++)
			{
				++counter;
			}
			Assert::AreEqual(counter, map.Size());

			// Const
			const HashMap<Foo, std::string> const_map(map);
			counter = 0;
			for (HashMap<Foo, std::string>::ConstIterator it = const_map.begin(); it != const_map.end(); ++it)
			{
				++counter;
			}
			Assert::AreEqual(counter, const_map.Size());
			counter = 0;
			for (HashMap<Foo, std::string>::ConstIterator it = const_map.begin(); it != const_map.end(); it++)
			{
				++counter;
			}
			Assert::AreEqual(counter, map.Size());
		}

	private:
		static _CrtMemState sStartMemState; // Static members before C++17 could only be defined separate from declaration
	};

	_CrtMemState HashMapTests::sStartMemState;
}
