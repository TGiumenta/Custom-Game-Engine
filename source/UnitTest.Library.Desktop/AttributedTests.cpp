#include "pch.h"
#include <crtdbg.h>
#include <exception>
#include <CppUnitTest.h> 
#include "Attributed.h"
#include "AttributedFoo.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(AttributedTests)
	{
	public:
		TEST_METHOD_INITIALIZE(ClassInitialize)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF); // Changes the way that "new" works
			_CrtMemCheckpoint(&sStartMemState);
#endif // Debug
		}

		TEST_METHOD_CLEANUP(ClassCleanup)
		{
#ifdef _DEBUG
			TypeManager::Clear();
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leak"); // Makes this string a wide character
			}
#endif // DEBUG
		}

		TEST_METHOD(TestRTTIInheritence)
		{
			using namespace std::string_literals;
			TypeManager::AddType(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());

			AttributedFoo a;
			RTTI* rtti = &a;
			Assert::IsFalse(rtti->Is("Foo"s));
			Assert::IsTrue(rtti->Is("AttributedFoo"s));
			Assert::IsTrue(rtti->Is("Attributed"s));
			Assert::IsTrue(rtti->Is("Scope"s));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Attributed::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(AttributedFoo::TypeIdClass(), rtti->TypeIdInstance());

			Foo* b = rtti->As<Foo>();
			Assert::IsNull(b);

			AttributedFoo* f = rtti->As<AttributedFoo>();
			Assert::IsNotNull(f);
			Assert::AreEqual(&a, f);

			Attributed* fAsAttributed = rtti->As<Attributed>();
			Assert::IsNotNull(fAsAttributed);
			Assert::AreEqual(static_cast<Attributed*>(&a), fAsAttributed);

			RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(AttributedFoo::TypeIdClass());
			Assert::IsNotNull(r);
			r = rtti->QueryInterface(Attributed::TypeIdClass());
			Assert::IsNotNull(r);

			AttributedFoo other_foo;
			Assert::IsTrue(rtti->Equals(&other_foo));

			Foo foo(10);
			Assert::IsFalse(rtti->Equals(&foo));

			Assert::AreEqual("AttributedFoo"s, r->ToString());
		}

		TEST_METHOD(TestConstructor)
		{
			TypeManager::AddType(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
			AttributedFoo a;

			Scope* clone = a.Clone();
			Assert::IsTrue(a.Equals(clone));

			delete clone;
		}

		TEST_METHOD(TestCopyConstructor)
		{
			TypeManager::AddType(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
			AttributedFoo a;
			AttributedFoo b(a);

			Assert::IsTrue(a.Equals(&b));

			b.AppendAuxililaryAttribute("Hello");
			Assert::IsFalse(a.Equals(&b));
		}

		TEST_METHOD(TestMoveConstructor)
		{
			TypeManager::AddType(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
			
			AttributedFoo a;
			AttributedFoo a_copy(a);
			AttributedFoo b(std::move(a));

			Assert::IsTrue(a_copy.Equals(&b));

			b.AppendAuxililaryAttribute("Hello");
			Assert::IsFalse(a_copy.Equals(&b));
		}

		TEST_METHOD(TestLValueEqualityOperator)
		{
			TypeManager::AddType(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
			AttributedFoo a;
	
			AttributedFoo b;
			b = a;

			Assert::IsTrue(a.Equals(&b));

			b.AppendAuxililaryAttribute("Hello");
			Assert::IsFalse(a.Equals(&b));
		}

		TEST_METHOD(TestRValueEqualityOperator)
		{
			TypeManager::AddType(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
			AttributedFoo a;
			AttributedFoo a_copy(a);
			AttributedFoo b;
			b = std::move(a);
			Assert::IsTrue(a_copy.Equals(&b));

			b.AppendAuxililaryAttribute("Hello");
			Assert::IsFalse(a_copy.Equals(&b));
		}

		TEST_METHOD(TestAccessorMethods)
		{
			TypeManager::AddType(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
			AttributedFoo a;

			auto& attributes = a.Attributes();
			for (const auto& attribute : attributes)
			{
				Assert::IsTrue(a.IsAttribute(attribute->first));
			}

			Assert::IsFalse(a.IsAttribute(""));

			auto& prescribed_attributes = a.PrescribedAttributes();
			for (const auto& attribute : prescribed_attributes)
			{
				Assert::IsTrue(a.IsPrescribedAttribute(attribute->first));
			}

			a.AppendAuxililaryAttribute("Hello");
			a.AppendAuxililaryAttribute("Goodbye");
			a.AppendAuxililaryAttribute("Music");
			auto& auxiliary_attributes = a.AuxiliaryAttributes();
			for (const auto& attribute : auxiliary_attributes)
			{
				Assert::IsTrue(a.IsAuxiliaryAttribute(attribute->first));
			}
		}

		TEST_METHOD(TestAppendAuxiliaryAttribute)
		{
			TypeManager::AddType(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
			AttributedFoo a;
			AttributedFoo b(a);

			Assert::IsTrue(a.Equals(&b));

			b.AppendAuxililaryAttribute("Hello");
			Assert::IsFalse(a.Equals(&b));

			Assert::IsTrue(a.IsPrescribedAttribute("this"));
			Assert::IsFalse(a.IsAuxiliaryAttribute("this"));
			Assert::IsFalse(a.IsPrescribedAttribute("Hello"));

			auto expressionA = [&a] { a.AppendAuxililaryAttribute("ExternalInteger"); };
			Assert::ExpectException<std::invalid_argument>(expressionA);
		}

	private:
		static _CrtMemState sStartMemState; // Static members before C++17 could only be defined separate from declaration
	};

	_CrtMemState AttributedTests::sStartMemState;
}