#include "pch.h"
#include <crtdbg.h>
#include <exception>
#include <CppUnitTest.h>
#include "Foo.h"
#include "Bar.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(BarTests)
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
			Bar a;
			Assert::AreEqual(0, a.Data());

			const int value = 10;
			Bar b(value);
			Assert::AreEqual(value, b.Data());
		}

		TEST_METHOD(TestCopySemantics)
		{
			Bar b(10);
			Assert::AreEqual(10, b.Data());

			Bar c;
			c = b;
			Assert::AreEqual(c.Data(), b.Data());
		}

		TEST_METHOD(TestMoveSemantics)
		{
			Bar a(10);
			Assert::AreEqual(10, a.Data());

			Bar b(std::move(a));
			auto expressionA = [&a] { a.Data(); };
			Assert::ExpectException<std::runtime_error>(expressionA);

			Bar c;
			c = std::move(b);
			auto expressionB = [&b] { b.Data(); };
			Assert::ExpectException<std::runtime_error>(expressionB);
			Assert::AreEqual(10, c.Data());
		}

	private:
		static _CrtMemState sStartMemState; // Static members before C++17 could only be defined separate from declaration
	};

	_CrtMemState BarTests::sStartMemState;
}
