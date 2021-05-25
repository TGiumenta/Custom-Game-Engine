#include "pch.h"
#include <crtdbg.h>
#include <exception>
#include <CppUnitTest.h>
#include "ToStringSpecializations.h"
#include "Foo.h"
#include "Vector.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	struct IncrementStrategy final
	{
		size_t operator()(size_t /*size*/, std::size_t capacity) const
		{
			return capacity*2;
		}
	};

	TEST_CLASS(VectorTests)
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
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			Vector<Foo> v;
			v.PushBack(a);
			Assert::IsFalse(v.IsEmpty());

			Vector<Foo> v_2{ a, b, c};
			Assert::IsFalse(v_2.IsEmpty());

			Vector<Foo> v_3(3);
		}

		TEST_METHOD(TestCopyConstructor)
		{
			Vector<Foo> v{ Foo(10), Foo(20), Foo(30) };
			Vector<Foo> v_copy = v;

			bool is_equal = true;
			Vector<Foo>::ConstIterator first_it = v.begin();
			Vector<Foo>::ConstIterator second_it = v_copy.begin();
			for (; first_it != v.end() && second_it != v_copy.end(); ++first_it, second_it++) // Use post-fix once to test it working
			{
				is_equal = (is_equal && (*first_it == *second_it));
			}
			Assert::IsTrue(is_equal);

			Vector<Foo> v_lval_copy(v);
			Vector<Foo>::Iterator third_it = v.begin();
			Vector<Foo>::Iterator fourth_it = v_lval_copy.begin();
			for (; third_it != v.end() && fourth_it != v_lval_copy.end(); ++third_it, fourth_it++) // Use post-fix once to test it working
			{
				is_equal = (is_equal && (*third_it == *fourth_it));
			}
			Assert::IsTrue(is_equal);

			Vector<Foo> v_rval_copy(std::move(v));
			Vector<Foo>::Iterator fifth_it = v_lval_copy.begin();
			Vector<Foo>::Iterator sixth_it = v_rval_copy.begin();
			for (; fifth_it != v_lval_copy.end() && sixth_it != v_rval_copy.end(); ++fifth_it, sixth_it++) // Use post-fix once to test it working
			{
				is_equal = (is_equal && (*fifth_it == *sixth_it));
			}
			Assert::IsTrue(is_equal);
		}

		TEST_METHOD(TestLValueEqualOperator)
		{
			Vector<Foo> v{ Foo(10), Foo(20) };
			Vector<Foo> v_2;
			v_2 = v;
			v_2 = v_2;					// Ensures operator= gets called with identical lists

			Assert::AreEqual(v.Size(), v_2.Size());
		}

		TEST_METHOD(TestRValueEqualOperator)
		{
			Vector<Foo> v{ Foo(10), Foo(20) };
			size_t v_size = v.Size();

			Vector<Foo> v_2;
			v_2 = std::move(v);	
			v_2 = std::move(v_2);		// Ensures operator= gets called with identical lists

			Assert::AreEqual(v_size, v_2.Size());
		}

		TEST_METHOD(TestBracketOperator)
		{
			// Non-const
			{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);
			Vector<Foo> v{ a, b, c };
			Assert::IsFalse(v.IsEmpty());

			Assert::AreEqual(v[0], a);
			Assert::AreEqual(v[2], c);

			auto expressionA = [&v] { v[3]; };
			Assert::ExpectException<std::runtime_error>(expressionA);
			}
			// Const
			{
				const Foo a(10);
				const Foo b(20);
				const Foo c(30);
				const Vector<Foo> v{ a, b, c };
				Assert::IsFalse(v.IsEmpty());

				Assert::AreEqual(v[0], a);
				Assert::AreEqual(v[2], c);

				auto expressionA = [&v] { v[3]; };
				Assert::ExpectException<std::runtime_error>(expressionA);
			}
		}

		TEST_METHOD(TestAtOperator)
		{
			// Non-const
			{
				const Foo a(10);
				const Foo b(20);
				const Foo c(30);
				Vector<Foo> v{ a, b, c };
				Assert::IsFalse(v.IsEmpty());

				Assert::AreEqual(v.at(0), a);
				Assert::AreEqual(v.at(2), c);

				auto expressionA = [&v] { v.at(3); };
				Assert::ExpectException<std::runtime_error>(expressionA);
			}
			// Const
			{
				const Foo a(10);
				const Foo b(20);
				const Foo c(30);
				const Vector<Foo> v{ a, b, c };
				Assert::IsFalse(v.IsEmpty());

				Assert::AreEqual(v.at(0), a);
				Assert::AreEqual(v.at(2), c);

				auto expressionA = [&v] { v.at(3); };
				Assert::ExpectException<std::runtime_error>(expressionA);
			}
		}

		TEST_METHOD(TestPopBack)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);
			Vector<Foo> v{ a, b, c };
			Assert::IsFalse(v.IsEmpty());

			v.PopBack();

			Assert::AreEqual(b, v.Back());
			
			v.PopBack();
			v.PopBack();

			Assert::AreEqual(0_z, v.Size());
			Assert::AreEqual(3_z, v.Capacity());
		}

		TEST_METHOD(TestIsEmpty)
		{
			Vector<Foo> v;
			Assert::IsTrue(v.IsEmpty());

			const Foo a(10);
			const Foo b(20);
			const Foo c(30);
			v.PushBack(a);
			v.PushBack(b);
			v.PushBack(c);
			Assert::IsFalse(v.IsEmpty());
		}

		TEST_METHOD(TestFront)
		{
			// Non-const
			{
				Vector<Foo> v;
				Assert::IsTrue(v.IsEmpty());
				auto expressionA = [&v] { v.Front(); };
				Assert::ExpectException<std::runtime_error>(expressionA);

				const Foo a(10);
				const Foo b(20);
				const Foo c(30);
				v.PushBack(a);
				Assert::AreEqual(a, v.Front());
				v.PushBack(b);
				Assert::AreEqual(a, v.Front());
				v.PushBack(c);
				Assert::AreEqual(a, v.Front());
			}
			// Const
			{
				const Vector<Foo> v;
				Assert::IsTrue(v.IsEmpty());
				auto expressionA = [&v] { v.Front(); };
				Assert::ExpectException<std::runtime_error>(expressionA);

				const Foo a(10);
				const Foo b(20);
				const Foo c(30);
				const Vector<Foo> v_2{ a, b, c };
				Assert::AreEqual(a, v_2.Front());
			}
		}

		TEST_METHOD(TestBack)
		{
			// Non-const
			{
				Vector<Foo> v;
				Assert::IsTrue(v.IsEmpty());
				auto expressionA = [&v] { v.Back(); };
				Assert::ExpectException<std::runtime_error>(expressionA);

				const Foo a(10);
				const Foo b(20);
				const Foo c(30);
				v.PushBack(a);
				Assert::AreEqual(a, v.Back());
				v.PushBack(b);
				Assert::AreEqual(b, v.Back());
				v.PushBack(c);
				Assert::AreEqual(c, v.Back());
			}
			// Const
			{
				const Vector<Foo> v;
				Assert::IsTrue(v.IsEmpty());
				auto expressionA = [&v] { v.Back(); };
				Assert::ExpectException<std::runtime_error>(expressionA);

				const Foo a(10);
				const Foo b(20);
				const Foo c(30);
				const Vector<Foo> v_2{ a, b, c };
				Assert::AreEqual(c, v_2.Back());
			}
		}

		TEST_METHOD(TestSize)
		{
			Vector<Foo> v;
			Assert::AreEqual(0_z, v.Size());

			const Foo a(10);
			const Foo b(20);
			const Foo c(30);
			v.PushBack(a);
			v.PushBack(b);
			v.PushBack(c);
			Assert::AreEqual(3_z, v.Size());
		}

		TEST_METHOD(TestCapacity)
		{
			Vector<Foo> v;
			Assert::AreEqual(0_z, v.Capacity());

			const Foo a(10);
			const Foo b(20);
			const Foo c(30);
			v.PushBack(a);				// Adds 1 to capacity
			v.PushBack(b);				// Adds 1 to capacity
			v.PushBack(c);				// Adds 1 to capacity
			Assert::AreEqual(3_z, v.Capacity());
		}

		TEST_METHOD(TestPushBack)
		{
			// Default Increment
			{
				Vector<Foo> v;
				Assert::AreEqual(0_z, v.Capacity());

				const Foo a(10);
				const Foo b(20);
				const Foo c(30);
				v.PushBack(a);			// Adds 1 to capacity
				v.PushBack(b);			// Adds 1 to capacity
				v.PushBack(c);			// Adds 1 to capacity
				Assert::AreEqual(3_z, v.Capacity());
				Assert::AreEqual(3_z, v.Size());
				Assert::AreEqual(a, v.Front());
				Assert::AreEqual(c, v.Back());

				v.PushBack(Foo(40));
				Assert::AreEqual(4_z, v.Size());
				Assert::AreEqual(a, v.Front());
				Assert::AreEqual(Foo(40), v.Back());

				v.PushBack(Foo(50));
				v.PushBack(Foo(60));
				v.PushBack(Foo(70));
				Assert::AreEqual(7_z, v.Size());
				Assert::AreEqual(Foo(70), v.Back());
			}
			// Custom Increment
			{
				const Foo a(10);
				const Foo b(20);
				const Foo c(30);
				
				// l-value increment strategy
				{
					Vector<Foo> v;
					v.PushBack<IncrementStrategy>(a);
					Assert::AreEqual(1_z, v.Capacity());
					v.PushBack<IncrementStrategy>(b);
					Assert::AreEqual(3_z, v.Capacity());
					v.PushBack<IncrementStrategy>(c);
					Assert::AreEqual(3_z, v.Capacity());
				}
				// r-value increment strategy
				{
					Vector<Foo> v;
					v.PushBack<IncrementStrategy>(Foo(10));
					Assert::AreEqual(1_z, v.Capacity());
					v.PushBack<IncrementStrategy>(Foo(20));
					Assert::AreEqual(3_z, v.Capacity());
					v.PushBack<IncrementStrategy>(Foo(30));
					Assert::AreEqual(3_z, v.Capacity());
				}
				{
					Vector<Foo> v{ a,b,c };
					Assert::AreEqual(3_z, v.Capacity());
					v.PushBack<IncrementStrategy>(Foo(40));
					Assert::AreEqual(9_z, v.Capacity());
					v.PushBack<IncrementStrategy>(Foo(50));
					Assert::AreEqual(9_z, v.Capacity());
				}
			}
		}

		TEST_METHOD(TestFind)
		{
			Vector<Foo> v;
			Assert::AreEqual(0_z, v.Capacity());

			const Foo a(10);
			const Foo b(20);
			const Foo c(30);
			v.PushBack(a);
			Vector<Foo>::Iterator it_b = v.PushBack(b);
			v.PushBack(c);

			Assert::AreEqual(it_b, v.Find(b));

			const Vector<Foo> v_const(v);
			Assert::AreNotEqual(v_const.end(), v_const.Find(c));
		}

		TEST_METHOD(TestResize)
		{
			Vector<Foo> v{ Foo(10), Foo(20) };
			Vector<Foo> v_2;
			v_2 = v;
			v_2 = v_2;					// Ensures operator= gets called with identical lists

			Assert::AreEqual(v.Size(), v_2.Size());
			v.Resize(1_z);
			Assert::AreEqual(1_z, v.Size());
			Assert::AreEqual(2_z, v.Capacity());
			v.ShrinkToFit();
			Assert::AreEqual(1_z, v.Capacity());
		}

		TEST_METHOD(TestClear)
		{
			Vector<Foo> v{ Foo(10), Foo(20), Foo(30) };
			Assert::AreNotEqual(0_z, v.Size());
			v.Clear();
			Assert::AreEqual(0_z, v.Size());
		}

		TEST_METHOD(TestRemove)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);
			const Foo d(40);
			const Foo e(50);
			const Foo f(60);
			Vector<Foo> v{ a,b,c };
			Assert::AreNotEqual(0_z, v.Size());

			bool found = v.Remove(b);
			Assert::IsTrue(found);
			Assert::AreEqual(v.end(), v.Find(b));

			found = v.Remove(c);
			Assert::IsTrue(found);
			Assert::AreEqual(v.end(), v.Find(c));

			Vector<Foo>::Iterator v_bad_ref;
			auto expressionA = [&v, &v_bad_ref] { v.Remove(v_bad_ref); };
			Assert::ExpectException<std::runtime_error>(expressionA);

			Vector<Foo> multi_remove;
			Vector<Foo>::Iterator a_it = multi_remove.PushBack(a);
			Vector<Foo>::Iterator b_it = multi_remove.PushBack(b);
			Vector<Foo>::Iterator c_it = multi_remove.PushBack(c);
			Vector<Foo>::Iterator d_it = multi_remove.PushBack(d);
			Vector<Foo>::Iterator e_it = multi_remove.PushBack(e);
			Vector<Foo>::Iterator f_it = multi_remove.PushBack(f);

			found = multi_remove.Remove(b_it, e_it);
			Assert::IsTrue(found);
			Assert::AreEqual(3_z, multi_remove.Size());

			Vector<Foo>::Iterator last_element = multi_remove.Find(f);
			auto expressionB = [&multi_remove, &last_element] { multi_remove.Remove(multi_remove.end(), last_element); };
			Assert::ExpectException<std::runtime_error>(expressionB);

			found = multi_remove.Remove(last_element, multi_remove.end());
			Assert::IsTrue(found);
			Assert::AreEqual(2_z, multi_remove.Size());

			Vector<Foo>::Iterator v_bad_first_ref;
			Vector<Foo>::Iterator v_bad_second_ref;
			auto expressionC = [&multi_remove, &v_bad_first_ref, &v_bad_second_ref] { multi_remove.Remove(v_bad_first_ref, v_bad_second_ref); };
			Assert::ExpectException<std::runtime_error>(expressionC);
		}
		
		TEST_METHOD(TestShrinkToFit)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);
			Vector<Foo> v{ a,b,c };
			Assert::AreNotEqual(0_z, v.Size());
			v.Remove(b);
			v.ShrinkToFit();
			Assert::AreEqual(v.Size(), v.Capacity());
			v.Clear();
			v.ShrinkToFit();
			Assert::AreEqual(v.Size(), v.Capacity());
			v.PushBack(a);
			Assert::AreEqual(a, v.Front());
		}

	private:
		static _CrtMemState sStartMemState; // Static members before C++17 could only be defined separate from declaration
	};

	_CrtMemState VectorTests::sStartMemState;
}
