#include "pch.h"
#include <crtdbg.h>
#include <exception>
#include <CppUnitTest.h>
#include "SList.h"
#include "Vector.h"
#include "HashMap.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(IteratorTests)
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

		TEST_METHOD(TestIteratorConstructor)
		{
			// SList
			{
				SList<Foo> list;
				const Foo first_foo(10);
				list.PushBack(first_foo);
				SList<Foo>::Iterator it = list.begin();
				Assert::AreEqual(list.Front(), *it);
			}
			// Vector
			{
				Vector<Foo> list;
				const Foo first_foo(10);
				list.PushBack(first_foo);
				Vector<Foo>::Iterator it = list.begin();
				Assert::AreEqual(list.Front(), *it);
			}
			// HashMap
			{
				HashMap<Foo, std::string> map;
				Assert::AreEqual(0_z, map.Size());
				auto pair = std::make_pair(Foo(10), "Hello");
				auto pair_tuple = map.Insert(pair);
				Assert::AreEqual(std::get<1>(pair_tuple), map.begin());
			}
		}

		TEST_METHOD(TestIteratorEquality)
		{
			// SList
			{
				SList<Foo> first_list;
				SList<Foo> second_list;

				SList<Foo>::Iterator first_it = first_list.begin();
				SList<Foo>::Iterator second_it = second_list.begin();

				// This will ALWAYS be false
				Assert::AreNotEqual(first_it, second_it);
			}
			// Vector
			{
				Vector<Foo> first_list;
				Vector<Foo> second_list;

				Vector<Foo>::Iterator first_it = first_list.begin();
				Vector<Foo>::Iterator second_it = second_list.begin();

				// This will ALWAYS be false
				Assert::AreNotEqual(first_it, second_it);
			}
			// HashMap
			{
				HashMap<Foo, std::string> first_map;
				HashMap<Foo, std::string> second_map;

				HashMap<Foo, std::string>::Iterator first_it = first_map.begin();
				HashMap<Foo, std::string>::Iterator second_it = second_map.begin();

				// This will ALWAYS be false
				Assert::AreNotEqual(first_it, second_it);
			}
		}

		TEST_METHOD(TestIteratorIncrement)
		{
			// SList
			{
				// Prefix
				SList<Foo>::Iterator it;
				auto expressionA = [&it] { ++it; };
				Assert::ExpectException<std::runtime_error>(expressionA);
			}
			// Vector
			{
				// Prefix
				Vector<Foo>::Iterator it;
				auto expressionA = [&it] { ++it; };
				Assert::ExpectException<std::runtime_error>(expressionA);
			}
			// HashMap
			{
				// Prefix
				HashMap<Foo, std::string>::Iterator it;
				auto expressionA = [&it] { ++it; };
				Assert::ExpectException<std::runtime_error>(expressionA);
			}
		}

		TEST_METHOD(TestIteratorDecrement)
		{
			// Vector
			{
				// Prefix
				Vector<Foo>::Iterator it;
				auto expressionA = [&it] { --it; };
				Assert::ExpectException<std::runtime_error>(expressionA);

				const Foo a(10);
				const Foo b(20);
				Vector<Foo> v{ a,b };
				Vector<Foo>::Iterator end = v.end();
				Vector<Foo>::Iterator new_a = v.Find(a);
				Vector<Foo>::Iterator new_b = v.Find(b);

				Assert::AreEqual(new_b, --end);
				end--;
				Assert::AreEqual(new_a, end);
			}
		}

		TEST_METHOD(TestIteratorDereference)
		{
			// SList
			{
				SList<Foo>::Iterator it;
				auto expressionA = [&it] { *it; };
				Assert::ExpectException<std::runtime_error>(expressionA);
			}
			// Vector
			{
				// * operator
				{
					Vector<Foo>::Iterator it;
					auto expressionA = [&it] { *it; };
					Assert::ExpectException<std::runtime_error>(expressionA);

					Vector<Foo> v;
					Vector<Foo>::Iterator it_bad_data = v.begin();
					auto expressionB = [&it_bad_data] { *it_bad_data; };
					Assert::ExpectException<std::runtime_error>(expressionB);
				}
				// -> operator
				{
					Vector<Foo>::Iterator it;
					auto expressionA = [&it] { it->Data(); };
					Assert::ExpectException<std::runtime_error>(expressionA);

					Vector<Foo> v;
					Vector<Foo>::Iterator it_bad_data = v.begin();
					auto expressionB = [&it_bad_data] { it_bad_data->Data(); };
					Assert::ExpectException<std::runtime_error>(expressionB);
				}
			}
			// Hashmap
			{
				// * operator
				{
					HashMap<Foo, std::string>::Iterator it;
					auto expressionA = [&it] { *it; };
					Assert::ExpectException<std::runtime_error>(expressionA);
				}
				// -> operator
				{
					HashMap<Foo, std::string>::Iterator it;
					auto expressionA = [&it] { it->first; };
					Assert::ExpectException<std::runtime_error>(expressionA);
				}
			}
		}

		TEST_METHOD(TestConstIteratorConstructor)
		{
			SList<Foo> list;
			Foo first_foo(10);
			list.PushBack(first_foo);
			const SList<Foo>::Iterator it = list.begin();
			Assert::AreEqual(list.Front(), *it);

			SList<Foo>::ConstIterator const_it = SList<Foo>::ConstIterator(it);
			Assert::AreEqual(list.Front(), *const_it);
		}

		TEST_METHOD(TestConstIteratorEquality)
		{
			// SList 
			{
				SList<Foo> first_list;
				SList<Foo> second_list;

				SList<Foo>::ConstIterator first_it = first_list.cbegin();
				SList<Foo>::ConstIterator second_it = second_list.cbegin();

				// This will ALWAYS be false
				Assert::AreNotEqual(first_it, second_it);

				first_it = first_list.cend();
				second_it = second_list.cend();

				// This will ALWAYS be false
				Assert::AreNotEqual(first_it, second_it);
			}
			// Vector
			{
				Vector<Foo> first_vector;
				Vector<Foo> second_vector;

				Vector<Foo>::ConstIterator first_it = first_vector.cbegin();
				Vector<Foo>::ConstIterator second_it = second_vector.cbegin();

				// This will ALWAYS be false
				Assert::AreNotEqual(first_it, second_it);

				first_it = first_vector.cend();
				second_it = second_vector.cend();

				// This will ALWAYS be false
				Assert::AreNotEqual(first_it, second_it);
			}
		}

		TEST_METHOD(TestConstIteratorIncrement)
		{
			// SList
			{
				SList<Foo>::ConstIterator it;
				auto expressionA = [&it] { ++it; };
				Assert::ExpectException<std::runtime_error>(expressionA);
			}
			// Vector
			{
				Vector<Foo>::ConstIterator it;
				auto expressionA = [&it] { ++it; };
				Assert::ExpectException<std::runtime_error>(expressionA);
			}
			// HashMap
			{
				HashMap<Foo, std::string>::ConstIterator it;
				auto expressionA = [&it] { ++it; };
				Assert::ExpectException<std::runtime_error>(expressionA);
			}
		}

		TEST_METHOD(TestConstIteratorDecrement)
		{
			// Vector
			{
				Vector<Foo>::ConstIterator it;
				auto expressionA = [&it] { --it; };
				Assert::ExpectException<std::runtime_error>(expressionA);

				const Foo a(10);
				const Foo b(20);
				const Vector<Foo> v{ a,b };
				Vector<Foo>::ConstIterator end = v.end();
				Vector<Foo>::ConstIterator new_a = v.Find(a);
				Vector<Foo>::ConstIterator new_b = v.Find(b);

				--end;
				Assert::AreEqual(new_b, end);
				end--;
				Assert::AreEqual(new_a, end);
			}
		}

		TEST_METHOD(TestConstIteratorDereference)
		{
			// SList
			{
				SList<Foo>::ConstIterator it;
				auto expressionA = [&it] { *it; };
				Assert::ExpectException<std::runtime_error>(expressionA);
			}
			// Vector
			{
				// * operator
				{
					Vector<Foo>::ConstIterator it;
					auto expressionA = [&it] { *it; };
					Assert::ExpectException<std::runtime_error>(expressionA);

					Vector<Foo> v;
					Vector<Foo>::ConstIterator it_bad_data = v.begin();
					auto expressionB = [&it_bad_data] { *it_bad_data; };
					Assert::ExpectException<std::runtime_error>(expressionB);
				}

				// -> operator
				{
					Vector<Foo>::ConstIterator it;
					auto expressionA = [&it] { it->Data(); };
					Assert::ExpectException<std::runtime_error>(expressionA);

					Vector<Foo> v;
					Vector<Foo>::ConstIterator it_bad_data = v.begin();
					auto expressionB = [&it_bad_data] { it_bad_data->Data(); };
					Assert::ExpectException<std::runtime_error>(expressionB);
				}
			}
			// Hashmap
			{
				// * operator
				{
					HashMap<Foo, std::string>::ConstIterator it;
					auto expressionA = [&it] { *it; };
					Assert::ExpectException<std::runtime_error>(expressionA);
				}
				// -> operator
				{
					HashMap<Foo, std::string>::ConstIterator it;
					auto expressionA = [&it] { it->first; };
					Assert::ExpectException<std::runtime_error>(expressionA);
				}
			}
		}

	private:
		static _CrtMemState sStartMemState; // Static members before C++17 could only be defined separate from declaration
	};

	_CrtMemState IteratorTests::sStartMemState;
}
