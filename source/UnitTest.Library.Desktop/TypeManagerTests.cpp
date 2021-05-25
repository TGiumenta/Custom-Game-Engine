#include "pch.h"
#include <crtdbg.h>
#include <exception>
#include <CppUnitTest.h> 
#include "Attributed.h"
#include "AttributedFoo.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	class Monster : public Attributed
	{
		RTTI_DECLARATIONS(Monster, Attributed)

	public:
		Monster() : Attributed(Monster::TypeIdClass())
		{};

		virtual ~Monster() override {};

		static Vector<Signature> Signatures()
		{
			return {};
		}

	private:
		std::string monster_name;
		int health;
	};

	RTTI_DEFINITIONS(Monster)

	TEST_CLASS(TypeManagerTests)
	{
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
			TypeManager::Clear();
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leak"); // Makzxes this string a wide character
			}
#endif // DEBUG
		}

		TEST_METHOD(TestAddType)
		{
			TypeManager::AddType(Monster::TypeIdClass(), Monster::Signatures());
			Monster m;
			Assert::AreEqual(1_z, TypeManager::Size());
			Assert::IsTrue(TypeManager::ContainsType(Monster::TypeIdClass()));

			auto expressionA = [] {TypeManager::AddType(Monster::TypeIdClass(), Monster::Signatures()); };
			Assert::ExpectException<std::runtime_error>(expressionA);
		}

		TEST_METHOD(TestRemoveType)
		{
			TypeManager::AddType(Monster::TypeIdClass(), Monster::Signatures());
			Monster m;
			Assert::AreEqual(1_z, TypeManager::Size());
			Assert::IsTrue(TypeManager::ContainsType(Monster::TypeIdClass()));

			TypeManager::AddType(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
			AttributedFoo a;
			Assert::AreEqual(2_z, TypeManager::Size());
			Assert::IsTrue(TypeManager::ContainsType(AttributedFoo::TypeIdClass()));

			TypeManager::RemoveType(AttributedFoo::TypeIdClass());
			Assert::AreEqual(1_z, TypeManager::Size());
			Assert::IsFalse(TypeManager::ContainsType(AttributedFoo::TypeIdClass()));
		}

	private:
		static _CrtMemState sStartMemState; // Static members before C++17 could only be defined separate from declaration
	};

	_CrtMemState TypeManagerTests::sStartMemState;
}