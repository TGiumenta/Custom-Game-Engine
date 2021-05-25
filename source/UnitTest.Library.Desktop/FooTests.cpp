#include "pch.h"
#include <crtdbg.h>
#include <exception>
#include <CppUnitTest.h>
#include "Foo.h"
#include "Bar.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(FooTests)
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

		TEST_METHOD(Test_RTTI)
		{
			Foo f;
			RTTI* r = &f;

			Assert::IsTrue(r->Is(Foo::TypeIdClass())); // Are you a Foo? 
			Assert::IsFalse(r->Is(Bar::TypeIdClass())); // Are you a Bar? 

			Foo* f1 = r->As<Foo>();
			Assert::IsNotNull(f1);
			Bar* b1 = r->As<Bar>();		// Treat me as a bar. But the cast fails, sets to null
			Assert::IsNull(b1);

			using namespace std::string_literals;
			
			Bar b;
			RTTI* r1 = &b; r1;
			Assert::AreEqual(b.ToString(), "RTTI"s); // ToString not overloaded

			RTTI* r2 = &b;
			Assert::IsTrue(r2->Equals(r1));
		}

		TEST_METHOD(TestConstructor)
		{
			Foo a;
			Assert::AreEqual(0, a.Data());

			const int value = 10;
			Foo b(value);
			Assert::AreEqual(value, b.Data());
		}

		TEST_METHOD(TestCopySemantics)
		{
			const int value = 10;
			Foo a(value);
			Foo b = a;							// Or Foo b(a), this is initialization which invokes the copy constructor
			Assert::AreEqual(value, b.Data());	// Can't do this since the destructor on b deleted the pointer to data
			Assert::AreEqual(a, b);				// CAN'T DO THIS unless you overload == operator

			Foo c; // Declaration and definition
			c = a; // Invokes assignment operator
			Assert::AreEqual(value, c.Data());

			Foo d;
			Assert::AreNotEqual(c, d);

			Foo e = a;
			bool foo_equality = (e != a);
			Assert::IsFalse(foo_equality);
		}

		TEST_METHOD(TestMoveSemantics)
		{
			const int value = 10;
			Foo a(value);

			Foo b(std::move(a));
			Assert::AreEqual(value, b.Data());

			auto expressionA = [&a] { a.Data(); };
			Assert::ExpectException<std::runtime_error>(expressionA);

			Foo c(0);
			Assert::AreEqual(0, c.Data());
			c = std::move(b);
			Assert::AreEqual(value, c.Data());

			auto expressionB = [&b] { b.Data(); };
			Assert::ExpectException<std::runtime_error>(expressionB);

			auto expressionC = [&c, &b] { c != b; };
			Assert::ExpectException<std::runtime_error>(expressionC);

			auto expressionD = [&b] { b.ToString(); };
			Assert::ExpectException<std::runtime_error>(expressionD);
		}

		TEST_METHOD(TestData)
		{
			const int value = 10;
			Foo a(value);
			Foo b;
			b = a;
			b = std::move(a);

			auto expressionA = [&a] { a.Data(); };
			Assert::ExpectException<std::runtime_error>(expressionA);
			auto expressionB = [&a] { a.ToString(); };
			Assert::ExpectException<std::runtime_error>(expressionA);

			Assert::AreEqual(value, b.Data());
		}

	private:
		static _CrtMemState sStartMemState; // Static members before C++17 could only be defined separate from declaration
	};

	_CrtMemState FooTests::sStartMemState;
}
