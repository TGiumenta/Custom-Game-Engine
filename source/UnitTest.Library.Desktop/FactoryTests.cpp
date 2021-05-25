#include "pch.h"
#include <crtdbg.h>
#include <exception>
#include <CppUnitTest.h>
#include "Foo.h"
#include "Bar.h"
#include "Scope.h"
#include "Factory.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(FactoryTests)
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

		ConcreteFactory(Foo, RTTI)

		TEST_METHOD(TestProductCreation)
		{
			using namespace std::string_literals;

			RTTI* rtti = Factory<RTTI>::Create("Foo"s);
			Assert::IsNull(rtti);

			{
				FooFactory foo_factory;
				rtti = Factory<RTTI>::Create("Foo"s);
				Assert::IsNotNull(rtti);

				Assert::AreEqual(1_z, Factory<RTTI>::Size());
				Factory<RTTI>::Clear();
				Assert::IsTrue(Factory<RTTI>::IsEmpty());

				Foo* foo = rtti->As<Foo>();
				Assert::IsNotNull(foo);

				delete foo;
			}

			rtti = Factory<RTTI>::Create("Foo"s);
			Assert::IsNull(rtti);
		}

		TEST_METHOD(TestFind)
		{
			using namespace std::string_literals;

			RTTI* rtti = Factory<RTTI>::Create("Foo"s);
			Assert::IsNull(rtti);

			FooFactory foo_factory;
			rtti = Factory<RTTI>::Create("Foo"s);
			Assert::IsNotNull(rtti);

			const Factory<RTTI>* found_foo = Factory<RTTI>::Find("Foo"s);
			Assert::IsNotNull(found_foo);
			Assert::IsTrue(Factory<RTTI>::ContainsKey("Foo"s));
			const Factory<RTTI>* found_bar = Factory<RTTI>::Find("Bar"s);
			Assert::IsNull(found_bar);
			Assert::IsFalse(Factory<RTTI>::ContainsKey("Bar"s));

			auto expressionA = [] { FooFactory bad_factory; };
			Assert::ExpectException<std::runtime_error>(expressionA);

			delete rtti;
		}

	private:
		static _CrtMemState sStartMemState; // Static members before C++17 could only be defined separate from declaration
	};

	_CrtMemState FactoryTests::sStartMemState;
}
