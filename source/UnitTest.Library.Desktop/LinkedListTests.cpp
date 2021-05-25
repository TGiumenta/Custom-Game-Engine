#include "pch.h"
#include <crtdbg.h>
#include <exception>
#include <CppUnitTest.h>
#include "Bar.h"
#include "SList.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTests
{
	struct BarEquality final
	{
		bool operator()(const Bar& lhs, const Bar& rhs) const
		{
			return lhs.Data() == rhs.Data();
		}
	};
}

namespace FieaGameEngine
{
	template<>
	struct DefaultEquality<Bar>
	{
		bool operator()(const Bar& lhs, const Bar& rhs) const
		{
			return lhs.Data() == rhs.Data();
		}
	};
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(LinkedListTests)
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

		TEST_METHOD(TestDefaultConstructor)
		{
			SList<Foo> list;
			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(TestCopyConstructor)
		{
			SList<Foo> first_list;
			const Foo first_foo(5);
			first_list.PushBack(first_foo);

			bool is_equal = true;
			SList<Foo> second_list = first_list;
			SList<Foo>::ConstIterator first_it = first_list.begin();
			SList<Foo>::ConstIterator second_it = second_list.begin();
			for (; first_it != first_list.end() && second_it != second_list.end(); ++first_it, second_it++) // Use post-fix once to test it working
			{
				is_equal = (is_equal && (*first_it == *second_it));
			}

			SList<Foo> third_list;
			const Foo second_foo(10);
			third_list.PushFront(second_foo);
			const Foo third_foo(15);
			third_list.PushFront(third_foo);
			Foo fourth_foo(20);
			third_list.PushFront(fourth_foo);

			SList<Foo> fourth_list = third_list;
			SList<Foo>::Iterator third_it = third_list.begin();
			SList<Foo>::Iterator fourth_it = fourth_list.begin();
			for (; third_it != third_list.end() && fourth_it != fourth_list.end(); ++third_it, fourth_it++) // Use post-fix once to test it working
			{
				is_equal = (is_equal && (*third_it == *fourth_it));
			}
			Assert::IsTrue(is_equal);

			SList<Foo> fifth_list;
			SList<Foo> sixth_list = fifth_list;
			Assert::AreEqual(sixth_list.Size(), fifth_list.Size());
		}

		TEST_METHOD(TestMoveSemantics)
		{
			const int first_value = 10;
			SList<Foo> first_list;
			Foo first_foo(first_value);
			first_list.PushBack(first_foo);

			SList<Foo> second_list(std::move(first_list));
			Assert::AreEqual(first_value, second_list.Front().Data());
			auto expressionA = [&first_list] { first_list.Front().Data(); };
			Assert::ExpectException<std::runtime_error>(expressionA);

			SList<Foo> third_list;
			third_list = std::move(second_list);
			Assert::AreEqual(first_value, third_list.Front().Data());
			auto expressionB = [&second_list] { second_list.Front().Data(); };
			Assert::ExpectException<std::runtime_error>(expressionB);
		}

		TEST_METHOD(TestLValueEqualOperator)
		{
			SList<Foo> first_list;
			const Foo first_foo(5);
			first_list.PushBack(first_foo);
			const Foo second_foo(10);
			first_list.PushBack(second_foo);
			Foo third_foo(20);
			first_list.PushBack(third_foo);

			SList<Foo> second_list;
			second_list = first_list;
			second_list = second_list;	// Ensures operator= gets called with identical lists
			Assert::AreEqual(first_list.Size(), second_list.Size());

			first_list.Clear();
			second_list = first_list;
			Assert::AreEqual(first_list.Size(), second_list.Size());
		}

		TEST_METHOD(TestPushFront)
		{
			SList<Foo> list;
			const Foo first_foo(10);
			list.PushFront(first_foo);
			Assert::AreEqual(first_foo, list.Front());

			const Foo second_foo(20);
			list.PushFront(second_foo);

			Assert::AreEqual(second_foo, list.Front());
			Assert::AreEqual(first_foo, list.Back());
		}

		TEST_METHOD(TestPushFrontRValues)
		{
			SList<Foo> list;
			list.PushFront(Foo(5));
			Assert::IsFalse(list.IsEmpty());

			list.PushFront(Foo(1));
			Assert::AreEqual(1, list.Front().Data());
		}

		TEST_METHOD(TestPopFront)
		{
			SList<Foo> list;
			const Foo test_foo(10);
			list.PushFront(test_foo);
			Assert::IsFalse(list.IsEmpty());
			list.PopFront();
			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(TestPushBack)
		{
			SList<Foo> list;
			const Foo first_foo(35);
			list.PushBack(first_foo);
			Assert::IsFalse(list.IsEmpty());
			Assert::AreEqual(first_foo, list.Back());

			const Foo second_foo(40);
			list.PushBack(second_foo);
			Assert::AreEqual(first_foo, list.Front());
			Assert::AreEqual(second_foo, list.Back());
		}

		TEST_METHOD(TestPushBackRValues)
		{
			SList<Foo> list;
			list.PushBack(Foo(35));
			Assert::IsFalse(list.IsEmpty());

			list.PushBack(Foo(40));
			Assert::AreEqual(40, list.Back().Data());
		}

		TEST_METHOD(TestIsEmpty)
		{
			SList<Foo> list;
			const Foo test_foo(400);
			list.PushFront(test_foo);
			Assert::IsFalse(list.IsEmpty());
			list.PopBack();
			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(TestFront)
		{
			SList<Foo> list;

			const Foo first_foo(2);
			list.PushFront(first_foo);
			Assert::AreEqual(first_foo, list.Front());

			const Foo second_foo = Foo(4);
			list.PushFront(second_foo);
			Assert::AreEqual(second_foo, list.Front());

			const Foo third_foo = Foo(6);
			list.PushFront(third_foo);
			Assert::AreEqual(third_foo, list.Front());

			const Foo fourth_foo = Foo(459);
			list.PushBack(fourth_foo);
			Assert::AreEqual(third_foo, list.Front());

			SList<Foo> empty_list;
			auto expressionA = [&empty_list] { empty_list.Front(); };
			Assert::ExpectException<std::runtime_error>(expressionA);
		}

		TEST_METHOD(TestFrontConst)
		{
			SList<Foo> first_list;
			const Foo first_foo(10);
			first_list.PushFront(first_foo);
			Assert::IsFalse(first_list.IsEmpty());

			const SList<Foo> second_list = first_list;
			Assert::AreEqual(first_list.Front(), second_list.Front());

			const SList<Foo> empty_const_list;
			auto expressionA = [&empty_const_list] { empty_const_list.Front(); };
			Assert::ExpectException<std::runtime_error>(expressionA);
		}

		TEST_METHOD(TestBack)
		{
			SList<Foo> list;

			const Foo first_foo(2);
			list.PushBack(first_foo);
			Assert::AreEqual(first_foo, list.Back());

			const Foo second_foo(4);
			list.PushBack(second_foo);
			Assert::AreEqual(second_foo, list.Back());

			Foo third_foo = Foo(6);
			list.PushBack(third_foo);
			Assert::AreEqual(third_foo, list.Back());

			const Foo fourth_foo = Foo(459);
			list.PushFront(fourth_foo);
			Assert::AreEqual(third_foo, list.Back());

			SList<Foo> empty_list;
			auto expressionA = [&empty_list] { empty_list.Back(); };
			Assert::ExpectException<std::runtime_error>(expressionA);
		}

		TEST_METHOD(TestBackConst)
		{
			SList<Foo> first_list;
			const Foo first_foo(10);
			first_list.PushFront(first_foo);
			Assert::IsFalse(first_list.IsEmpty());

			const SList<Foo> second_list = first_list;
			Assert::AreEqual(first_list.Back(), second_list.Back());

			const SList<Foo> empty_const_list;
			auto expressionA = [&empty_const_list] { empty_const_list.Back(); };
			Assert::ExpectException<std::runtime_error>(expressionA);
		}

		TEST_METHOD(TestSize)
		{
			SList<Foo> list;
			Assert::AreEqual(0_z, list.Size());

			const Foo first_foo(10);
			list.PushFront(first_foo);
			const Foo second_foo(20);
			list.PushBack(second_foo);
			const Foo third_foo(30);
			list.PushFront(third_foo);
			const Foo fourth_foo(40);
			list.PushBack(fourth_foo);
			const Foo fifth_foo(50);
			list.PushFront(fifth_foo);

			Assert::AreEqual(5_z, list.Size());
			list.Clear();
			Assert::AreEqual(0_z, list.Size());
		}

		TEST_METHOD(TestClear)
		{
			SList<Foo> list;
			list.Clear();
			Assert::AreEqual(0_z, list.Size());

			const Foo first_foo(10);
			list.PushFront(first_foo);
			Assert::AreEqual(1_z, list.Size());

			list.Clear();
			Assert::AreEqual(0_z, list.Size());
		}

		TEST_METHOD(TestPopBack)
		{
			SList<Foo> list;

			const Foo first_foo(10);
			list.PushFront(first_foo);
			Assert::IsFalse(list.IsEmpty());
			list.PopBack();
			Assert::IsTrue(list.IsEmpty());

			const Foo second_foo(5);
			list.PushFront(second_foo);
			const Foo third_foo(15);
			list.PushFront(third_foo);
			const Foo fourth_foo(25);
			list.PushFront(fourth_foo);
			Assert::IsFalse(list.IsEmpty());

			list.PopBack();
			
			Assert::AreEqual(2_z, list.Size());
			Assert::AreEqual(third_foo, list.Back());
		}

		TEST_METHOD(TestInsertAfter)
		{
			SList<Foo> list;
			const Foo first_foo(5);
			list.PushFront(first_foo);
			const Foo second_foo(10);
			list.PushBack(second_foo);

			const Foo third_foo(15);
			SList<Foo>::Iterator it_ref = list.PushBack(third_foo);

			const Foo fourth_foo(20);
			list.PushBack(fourth_foo);
			const Foo fifth_foo(25);
			list.PushBack(fifth_foo);
			const Foo sixth_foo(30);
			list.PushBack(sixth_foo);

			const Foo seventh_foo(35);
			SList<Foo>::Iterator it_new_ref = list.InsertAfter(seventh_foo, it_ref);
			Assert::AreEqual(7_z, list.Size());

			// Covering incorrect list case
			SList<Foo> list_empty;
			SList<Foo>::Iterator it_bad_ref = list_empty.begin();
			const Foo eighth_foo(40);
			auto expressionA = [&list, &eighth_foo, &it_bad_ref] { list.InsertAfter(eighth_foo, it_bad_ref); };
			Assert::ExpectException<std::runtime_error>(expressionA);

			// Inserting at the tail case
			SList<Foo>::Iterator it_back_ref = list.InsertAfter(eighth_foo, list.end());
			Assert::AreEqual(8_z, list.Size());
		}

		TEST_METHOD(TestFind)
		{
			// Iterator
			// Foo
			{
				const Foo a(10);
				const Foo b(20);
				const Foo c(30);

				SList<Foo> list;
				SList<Foo>::Iterator end = list.end();
				SList<Foo>::Iterator find = list.Find(a);
				Assert::AreEqual(list.end(), list.Find(a));
				list.PushFront(a);
				Assert::AreNotEqual(list.end(), list.Find(a));
				list.Clear();
				Assert::AreEqual(list.end(), list.Find(a));
				list.PushFront(a);
				list.PushFront(b);
				list.PushFront(c);
				Assert::AreNotEqual(list.end(), list.Find(b));
				Assert::AreNotEqual(list.end(), list.Find(c));
			}
			// Bar
			{
				const Bar a(10);
				const Bar b(20);
				const Bar c(30);

				SList<Bar> list;
				SList<Bar>::Iterator end = list.end();
				SList<Bar>::Iterator find = list.Find(a);
				Assert::AreEqual(list.end(), list.Find(a));
				list.PushFront(a);
				Assert::AreNotEqual(list.end(), list.Find(a));
				list.Clear();
				Assert::AreEqual(list.end(), list.Find(a));
				list.PushFront(a);
				list.PushFront(b);
				list.PushFront(c);
				Assert::AreNotEqual(list.end(), list.Find(b));
				Assert::AreNotEqual(list.end(), list.Find(c));
			}

			// ConstIterator
			// Foo
			{
				const Foo a(10);
				const Foo b(20);
				const Foo c(30);

				SList<Foo> list;
				list.PushBack(a);
				list.PushBack(b);
				list.PushBack(c);

				const SList<Foo> empty_const_list;
				SList<Foo>::ConstIterator end = empty_const_list.cend();
				SList<Foo>::ConstIterator find = empty_const_list.Find(a);
				Assert::AreEqual(empty_const_list.end(), empty_const_list.Find(a));

				const SList<Foo> const_list(list);
				Assert::AreNotEqual(const_list.end(), const_list.Find(a));
				Assert::AreNotEqual(const_list.end(), const_list.Find(b));
			}
		}

		TEST_METHOD(TestRemove)
		{
			SList<Foo> list;
			Foo first_foo(5);
			list.PushFront(first_foo);
			Foo second_foo(10);
			SList<Foo>::Iterator it_second_ref = list.PushBack(second_foo);

			Foo third_foo(15);
			list.PushBack(third_foo);

			// Removing Tail
			Assert::IsTrue(list.Remove(third_foo));

			// Removing nothing
			Assert::IsFalse(list.Remove(third_foo));

			list.PushBack(third_foo);
			list.Remove(second_foo);
			Assert::AreEqual(2_z, list.Size());

			SList<Foo>::Iterator it_bad_ref;
			auto expressionA = [&list, &it_bad_ref] { list.Remove(it_bad_ref); };
			Assert::ExpectException<std::runtime_error>(expressionA);
		}

	private:
		static _CrtMemState sStartMemState; // Static members before C++17 could only be defined separate from declaration
	};

	_CrtMemState LinkedListTests::sStartMemState;
}
