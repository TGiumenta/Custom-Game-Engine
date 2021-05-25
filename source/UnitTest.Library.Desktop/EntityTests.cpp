#include "pch.h"
#include <crtdbg.h>
#include <exception>
#include <CppUnitTest.h>
#include "Foo.h"
#include "Bar.h"
#include "Entity.h"
#include "JsonParseCoordinator.h"
#include "JsonTableParseHelper.h"
#include "Factory.h"
#include "Action.h"
#include "Power.h"
#include "TestMonster.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EntityTests)
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

			std::string monster_name;
			TestMonster monster;

			RTTI* rtti = &monster;
			Assert::IsFalse(rtti->Is("Foo"s));
			Assert::IsTrue(rtti->Is("Entity"s));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Entity::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(TestMonster::TypeIdClass(), rtti->TypeIdInstance());

			Foo* b = rtti->As<Foo>();
			Assert::IsNull(b);

			Entity* e = rtti->As<Entity>();
			Assert::IsNotNull(e);
			RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(Entity::TypeIdClass());
			Assert::IsNotNull(r);

			Entity other_entity;
			Assert::IsTrue(rtti->Equals(&other_entity));

			Foo foo(10);
			Assert::IsFalse(rtti->Equals(&foo));
		}

		TEST_METHOD(TestJsonRead)
		{
			using namespace std::string_literals;

			TypeManager::AddType(Entity::TypeIdClass(), Entity::Signatures());
			TypeManager::AddType(TestMonster::TypeIdClass(), TestMonster::Signatures());
			Entity root;

			JsonTableParseHelper::SharedData shared(root);
			shared.Initialize();
			JsonTableParseHelper table_parse_helper;
			table_parse_helper.Initialize();
			JsonParseCoordinator parse_coordinator(shared);
			Assert::AreEqual(&parse_coordinator, shared.GetJsonParseCoordinator());
			parse_coordinator.AddHelper(table_parse_helper);

			EntityFactory entity_factory;
			TestMonsterFactory test_monster_factory;

			parse_coordinator.ParseFromFile("TestEntity.json"s);
			Assert::AreEqual(7_z, root.Size());
			Assert::AreEqual("Test World"s, root.Name());
			Assert::IsNotNull(&root.Children());
		}

		TEST_METHOD(TestTags)
		{
			using namespace std::string_literals;

			TypeManager::AddType(Entity::TypeIdClass(), Entity::Signatures());
			Entity root;

			// Lvalue
			{
				std::string world = "World"s;
				std::string actor = "Actor"s;


				root.AddTag(world);
				Assert::AreEqual(1_z, root.Tags().Size());
				Assert::IsTrue(root.IsTag(world));
				Assert::IsFalse(root.IsTag(actor));
				root.RemoveTag(actor); // Prove that this doesn't explode
				Assert::AreEqual(1_z, root.Tags().Size());
				Assert::IsTrue(root.IsTag(world));
				root.RemoveTag(world);
				Assert::AreEqual(0_z, root.Tags().Size());
			}
			// Rvalue
			{
				root.AddTag("World");
				Assert::AreEqual(1_z, root.Tags().Size());
				Assert::IsTrue(root.IsTag("World"));
				Assert::IsFalse(root.IsTag("Actor"));
				root.RemoveTag("Actor"); // Prove that this doesn't explode
				Assert::AreEqual(1_z, root.Tags().Size());
				Assert::IsTrue(root.IsTag("World"));
				root.RemoveTag("World");
				Assert::AreEqual(0_z, root.Tags().Size());
			}
		}

		TEST_METHOD(TestTransform)
		{
			using namespace std::string_literals;

			TypeManager::AddType(Entity::TypeIdClass(), Entity::Signatures());
			TypeManager::AddType(Action::TypeIdClass(), Action::Signatures());
			TypeManager::AddType(TestMonster::TypeIdClass(), TestMonster::Signatures());

			Entity root;

			Assert::AreEqual(glm::vec4(0,0,0,0), root.Position());
			Assert::AreEqual(glm::vec4(0,0,0,0), root.Rotation());
			Assert::AreEqual(glm::vec4(0,0,0,0), root.Scale());

			JsonTableParseHelper::SharedData shared(root);
			shared.Initialize();
			JsonTableParseHelper table_parse_helper;
			table_parse_helper.Initialize();
			JsonParseCoordinator parse_coordinator(shared);
			Assert::AreEqual(&parse_coordinator, shared.GetJsonParseCoordinator());
			parse_coordinator.AddHelper(table_parse_helper);

			EntityFactory entity_factory;
			TestMonsterFactory test_monster_factory;

			parse_coordinator.ParseFromFile("TestEntity.json"s);
			Assert::AreEqual(7_z, root.Size());

			WorldState state;
			Assert::IsNull(state.GetGameTime());
			GameTime time;
			state.SetGameTime(time);
			Assert::IsNotNull(state.GetGameTime());
			root.Update(state);

			// LValue
			{
				glm::vec4 position(50.f, 100.f, 200.f, 300.f);
				glm::vec4 rotation(180.f, 275.f, -30.f, 0.f);
				glm::vec4 scale(1.5f, 1.f, 1.f, 0.f);

				root.SetPosition(position);
				Assert::AreNotEqual(root.Position(), glm::vec4(0.f, 0.f, 0.f, 0.f));
				Assert::AreEqual(root.Position(), glm::vec4(50.f, 100.f, 200.f, 300.f));

				root.SetRotation(rotation);
				Assert::AreNotEqual(root.Rotation(), glm::vec4(50.f, 100.f, 200.f, 300.f));
				Assert::AreEqual(root.Rotation(), glm::vec4(180.f, 275.f, -30.f, 0.f));

				root.SetScale(scale);
				Assert::AreNotEqual(root.Scale(), glm::vec4(50.f, 100.f, 200.f, 300.f));
				Assert::AreEqual(root.Scale(), glm::vec4(1.5f, 1.0f, 1.0f, 0.f));

				auto [current_pos, current_rot, current_scale] = root.Transform();
				Assert::AreEqual(position, current_pos);
				Assert::AreEqual(rotation, current_rot);
				Assert::AreEqual(scale, current_scale);
			}
			// RValue
			{
				root.SetPosition(glm::vec4(50.f, 100.f, 200.f, 300.f));
				Assert::AreNotEqual(root.Position(), glm::vec4(0.f, 0.f, 0.f, 0.f));
				Assert::AreEqual(root.Position(), glm::vec4(50.f, 100.f, 200.f, 300.f));

				root.SetRotation(glm::vec4(180.f, 275.f, -30.f, 0.f));
				Assert::AreNotEqual(root.Rotation(), glm::vec4(50.f, 100.f, 200.f, 300.f));
				Assert::AreEqual(root.Rotation(), glm::vec4(180.f, 275.f, -30.f, 0.f));

				root.SetScale(glm::vec4(1.5f, 1.0f, 1.0f, 0.f));
				Assert::AreNotEqual(root.Scale(), glm::vec4(50.f, 100.f, 200.f, 300.f));
				Assert::AreEqual(root.Scale(), glm::vec4(1.5f, 1.0f, 1.0f, 0.f));
			}
		}

		TEST_METHOD(TestWorldStateInteractions)
		{
			using namespace std::string_literals;

			WorldState state;
			Assert::IsNull(state.GetGameTime());
			GameTime time;
			state.SetGameTime(time);
			Assert::IsNotNull(state.GetGameTime());

			TypeManager::AddType(Entity::TypeIdClass(), Entity::Signatures());
			Entity root;

			Assert::IsNull(state.GetCurrentEntity());
			Assert::IsNull(state.GetCurrentAction());
			root.Update(state);
			Assert::IsNotNull(state.GetCurrentEntity());
			Assert::IsNull(state.GetCurrentAction());

			std::string music = "Music"s;

			root.SetName("Test!"s);
			Assert::AreEqual(root.Name(), "Test!"s);
			root.SetName(music);
			Assert::AreNotEqual(root.Name(), "Test!"s);
		}

	private:
		static _CrtMemState sStartMemState; // Static members before C++17 could only be defined separate from declaration
	};

	_CrtMemState EntityTests::sStartMemState;
}
