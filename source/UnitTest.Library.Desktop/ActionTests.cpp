#include "pch.h"
#include <crtdbg.h>
#include <exception>
#include <CppUnitTest.h>
#include "Entity.h"
#include "Action.h"
#include "ActionIncrement.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include "ActionListIf.h"
#include "JsonParseCoordinator.h"
#include "JsonTableParseHelper.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ActionTests)
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

		TEST_METHOD(TestRTTIActionDerived)
		{
			using namespace std::string_literals;
			
			// ActionList
			{
				TypeManager::AddType(ActionList::TypeIdClass(), ActionList::Signatures());

				ActionList a;
				RTTI* rtti = &a;
				Assert::IsFalse(rtti->Is("Foo"s));
				Assert::IsTrue(rtti->Is("ActionList"s));
				Assert::IsTrue(rtti->Is("Scope"s));
				Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
				Assert::IsTrue(rtti->Is(ActionList::TypeIdClass()));
				Assert::IsTrue(rtti->Is(Action::TypeIdClass()));
				Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
				Assert::AreEqual(ActionList::TypeIdClass(), rtti->TypeIdInstance());

				Foo* b = rtti->As<Foo>();
				Assert::IsNull(b);

				ActionList* f = rtti->As<ActionList>();
				Assert::IsNotNull(f);
				Assert::AreEqual(&a, f);

				Attributed* fAsAttributed = rtti->As<Attributed>();
				Assert::IsNotNull(fAsAttributed);
				Assert::AreEqual(static_cast<Attributed*>(&a), fAsAttributed);

				RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
				Assert::IsNull(r);
				r = rtti->QueryInterface(ActionList::TypeIdClass());
				Assert::IsNotNull(r);
				r = rtti->QueryInterface(Action::TypeIdClass());
				Assert::IsNotNull(r);

				ActionList other_action_list;
				Assert::IsTrue(rtti->Equals(&other_action_list));

				Foo foo(10);
				Assert::IsFalse(rtti->Equals(&foo));
			}

			// ActionListIf
			{
				TypeManager::AddType(ActionListIf::TypeIdClass(), ActionListIf::Signatures());

				ActionListIf a;
				RTTI* rtti = &a;
				Assert::IsFalse(rtti->Is("Foo"s));
				Assert::IsTrue(rtti->Is("ActionListIf"s));
				Assert::IsTrue(rtti->Is("Scope"s));
				Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
				Assert::IsTrue(rtti->Is(ActionListIf::TypeIdClass()));
				Assert::IsTrue(rtti->Is(Action::TypeIdClass()));
				Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
				Assert::AreEqual(ActionListIf::TypeIdClass(), rtti->TypeIdInstance());

				Foo* b = rtti->As<Foo>();
				Assert::IsNull(b);

				ActionListIf* f = rtti->As<ActionListIf>();
				Assert::IsNotNull(f);
				Assert::AreEqual(&a, f);

				Attributed* fAsAttributed = rtti->As<Attributed>();
				Assert::IsNotNull(fAsAttributed);
				Assert::AreEqual(static_cast<Attributed*>(&a), fAsAttributed);

				RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
				Assert::IsNull(r);
				r = rtti->QueryInterface(ActionListIf::TypeIdClass());
				Assert::IsNotNull(r);
				r = rtti->QueryInterface(Action::TypeIdClass());
				Assert::IsNotNull(r);

				ActionListIf other_action_list_if;
				Assert::IsTrue(rtti->Equals(&other_action_list_if));

				Foo foo(10);
				Assert::IsFalse(rtti->Equals(&foo));
			}

			// ActionCreateAction
			{
				TypeManager::AddType(ActionCreateAction::TypeIdClass(), ActionCreateAction::Signatures());

				ActionCreateAction a;
				RTTI* rtti = &a;
				Assert::IsFalse(rtti->Is("Foo"s));
				Assert::IsTrue(rtti->Is("ActionCreateAction"s));
				Assert::IsTrue(rtti->Is("Scope"s));
				Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
				Assert::IsTrue(rtti->Is(ActionCreateAction::TypeIdClass()));
				Assert::IsTrue(rtti->Is(Action::TypeIdClass()));
				Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
				Assert::AreEqual(ActionCreateAction::TypeIdClass(), rtti->TypeIdInstance());

				Foo* b = rtti->As<Foo>();
				Assert::IsNull(b);

				ActionCreateAction* f = rtti->As<ActionCreateAction>();
				Assert::IsNotNull(f);
				Assert::AreEqual(&a, f);

				Attributed* fAsAttributed = rtti->As<Attributed>();
				Assert::IsNotNull(fAsAttributed);
				Assert::AreEqual(static_cast<Attributed*>(&a), fAsAttributed);

				RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
				Assert::IsNull(r);
				r = rtti->QueryInterface(ActionCreateAction::TypeIdClass());
				Assert::IsNotNull(r);
				r = rtti->QueryInterface(Action::TypeIdClass());
				Assert::IsNotNull(r);

				ActionCreateAction other_create_action;
				Assert::IsTrue(rtti->Equals(&other_create_action));

				Foo foo(10);
				Assert::IsFalse(rtti->Equals(&foo));
			}

			// ActionDestroyAction
			{
				TypeManager::AddType(ActionDestroyAction::TypeIdClass(), ActionDestroyAction::Signatures());

				ActionDestroyAction a;
				RTTI* rtti = &a;
				Assert::IsFalse(rtti->Is("Foo"s));
				Assert::IsTrue(rtti->Is("ActionDestroyAction"s));
				Assert::IsTrue(rtti->Is("Scope"s));
				Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
				Assert::IsTrue(rtti->Is(ActionDestroyAction::TypeIdClass()));
				Assert::IsTrue(rtti->Is(Action::TypeIdClass()));
				Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
				Assert::AreEqual(ActionDestroyAction::TypeIdClass(), rtti->TypeIdInstance());

				Foo* b = rtti->As<Foo>();
				Assert::IsNull(b);

				ActionDestroyAction* f = rtti->As<ActionDestroyAction>();
				Assert::IsNotNull(f);
				Assert::AreEqual(&a, f);

				Attributed* fAsAttributed = rtti->As<Attributed>();
				Assert::IsNotNull(fAsAttributed);
				Assert::AreEqual(static_cast<Attributed*>(&a), fAsAttributed);

				RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
				Assert::IsNull(r);
				r = rtti->QueryInterface(ActionDestroyAction::TypeIdClass());
				Assert::IsNotNull(r);
				r = rtti->QueryInterface(Action::TypeIdClass());
				Assert::IsNotNull(r);

				ActionDestroyAction other_destroy_action;
				Assert::IsTrue(rtti->Equals(&other_destroy_action));

				Foo foo(10);
				Assert::IsFalse(rtti->Equals(&foo));
			}

			// ActionIncrement
			{
				TypeManager::AddType(ActionIncrement::TypeIdClass(), ActionIncrement::Signatures());

				ActionIncrement a;
				RTTI* rtti = &a;
				Assert::IsFalse(rtti->Is("Foo"s));
				Assert::IsTrue(rtti->Is("ActionIncrement"s));
				Assert::IsTrue(rtti->Is("Scope"s));
				Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
				Assert::IsTrue(rtti->Is(ActionIncrement::TypeIdClass()));
				Assert::IsTrue(rtti->Is(Action::TypeIdClass()));
				Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
				Assert::AreEqual(ActionIncrement::TypeIdClass(), rtti->TypeIdInstance());

				Foo* b = rtti->As<Foo>();
				Assert::IsNull(b);

				ActionIncrement* f = rtti->As<ActionIncrement>();
				Assert::IsNotNull(f);
				Assert::AreEqual(&a, f);

				Attributed* fAsAttributed = rtti->As<Attributed>();
				Assert::IsNotNull(fAsAttributed);
				Assert::AreEqual(static_cast<Attributed*>(&a), fAsAttributed);

				RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
				Assert::IsNull(r);
				r = rtti->QueryInterface(ActionIncrement::TypeIdClass());
				Assert::IsNotNull(r);
				r = rtti->QueryInterface(Action::TypeIdClass());
				Assert::IsNotNull(r);

				ActionIncrement other_action_increment;
				Assert::IsTrue(rtti->Equals(&other_action_increment));

				Foo foo(10);
				Assert::IsFalse(rtti->Equals(&foo));
			}
		}

		TEST_METHOD(TestActionIncrement)
		{
			using namespace std::string_literals;

			ActionIncrement increment;
			Datum& a = increment.AppendAuxililaryAttribute("A"s);
			a = 0.0f;
			const std::string string_a = "A"s;
			increment.SetTarget(string_a);
			Assert::AreEqual("A"s, increment.Target());
			Assert::AreEqual(1.0f, increment.Step());

			GameTime game_time;
			WorldState world_state;
			world_state.SetGameTime(game_time);

			increment.Update(world_state);
			Assert::AreEqual(1.0f, a.Get<float>());
			increment.Update(world_state);
			Assert::AreEqual(2.0f, a.Get<float>());

			increment.SetStep(2.0f);
			Assert::AreEqual(2.0f, increment.Step());
			increment.Update(world_state);
			Assert::AreEqual(4.0f, a.Get<float>());

			increment.SetTarget("B"s);
			Assert::ExpectException<std::exception>([&increment, &world_state] {increment.Update(world_state); });
		}

		TEST_METHOD(TestClone)
		{
			using namespace std::string_literals;

			ActionList action;
			action.SetName("Test Action"s);
			auto clone = action.Clone();
			Assert::AreNotSame(action, *clone);
			Assert::AreEqual(action, *clone);

			delete clone;

			ActionIncrement increment;
			increment.SetName("Test ActionIncrement"s);
			auto clone_increment = increment.Clone();
			Assert::AreNotSame(increment, *clone_increment);
			Assert::AreEqual(increment, *clone_increment);

			delete clone_increment;
		}

		TEST_METHOD(TestJsonRead)
		{
			using namespace std::string_literals;
			
			TypeManager::AddType(Action::TypeIdClass(), Action::Signatures());
			TypeManager::AddType(ActionList::TypeIdClass(), ActionList::Signatures(), Action::TypeIdClass());
			TypeManager::AddType(ActionIncrement::TypeIdClass(), ActionIncrement::Signatures(), Action::TypeIdClass());		
			TypeManager::AddType(ActionDestroyAction::TypeIdClass(), ActionDestroyAction::Signatures(), Action::TypeIdClass());
			TypeManager::AddType(ActionCreateAction::TypeIdClass(), ActionCreateAction::Signatures(), Action::TypeIdClass());

			ActionListFactory action_factory;
			ActionIncrementFactory action_increment_factory;
			ActionDestroyActionFactory action_destroy_action_factory;
			ActionCreateActionFactory action_create_action_factory;

			ActionList list;
			std::string name = "List";
			list.SetName(name);
			Assert::AreEqual(list.Name(), name);

			JsonTableParseHelper::SharedData shared(list);
			JsonTableParseHelper table_parse_helper;
			table_parse_helper.Initialize();
			JsonParseCoordinator parse_coordinator(shared);
			parse_coordinator.AddHelper(table_parse_helper);

			parse_coordinator.ParseFromFile("TestAction.json"s);
			Assert::AreEqual(4_z, list.Size());

			Assert::AreEqual("Test Action"s, list.Name());
			auto& actions_datum = list.Actions();
			Assert::AreEqual(1_z, actions_datum.Size());

			Datum& a = list["X"];
			Assert::AreEqual(10.0f, a.Get<float>(0));

			GameTime game_time;
			WorldState world_state;
			world_state.SetGameTime(game_time);
			list.Update(world_state);

			Assert::AreEqual(11.0f, a.Get<float>());
			list.Update(world_state);
			Assert::AreEqual(12.0f, a.Get<float>());
		}

		TEST_METHOD(TestActionListIf)
		{
			using namespace std::string_literals;

			std::string action_list_if_name = "ActionIf";

			TypeManager::AddType(ActionList::TypeIdClass(), ActionList::Signatures());
			TypeManager::AddType(ActionListIf::TypeIdClass(), ActionListIf::Signatures());

			GameTime game_time;
			WorldState world_state(game_time);
			ActionListIf action_list_if;

			action_list_if.SetName(action_list_if_name);
			Assert::AreEqual(action_list_if.Name(), "ActionIf"s);
			action_list_if.SetName("ActionIf"s);

			Datum& a = action_list_if.AppendAuxililaryAttribute("A"s);
			a = 0.0f;
			Datum& b = action_list_if.AppendAuxililaryAttribute("B"s);
			b = 0.0f;

			ActionIncrementFactory action_increment_factory;

			ActionIncrement* then_increment = new ActionIncrement;
			ActionIncrement* else_increment = new ActionIncrement;
			action_list_if.Adopt(*then_increment, "Then"s);
			Assert::IsNotNull(&then_increment);
			then_increment->SetTarget("A"s);

			action_list_if.Adopt(*else_increment, "Else"s);
			Assert::IsNotNull(&else_increment);
			else_increment->SetTarget("B"s);
			else_increment->SetStep(-1.0f);

			action_list_if.SetCondition(true);
			Assert::IsTrue(action_list_if.Condition());
			action_list_if.Update(world_state);
			Assert::AreEqual(a.Get<float>(), 1.0f);

			action_list_if.SetCondition(false);
			Assert::IsFalse(action_list_if.Condition());
			action_list_if.Update(world_state);
			Assert::AreEqual(a.Get<float>(), 1.0f);
			Assert::AreEqual(b.Get<float>(), -1.0f);

			delete then_increment;
			delete else_increment;
		}

		TEST_METHOD(TestActionCreateAction)
		{
			using namespace std::string_literals;
			std::string create_name = "CreateA"s;
			std::string prototype_name = "ActionIncrement"s;

			TypeManager::AddType(Entity::TypeIdClass(), Entity::Signatures());

			ActionCreateActionFactory action_create_action_factory;
			ActionIncrementFactory action_increment_factory;

			GameTime game_time;
			WorldState world_state;
			world_state.SetGameTime(game_time);

			ActionCreateAction* create_action = new ActionCreateAction;
			Entity root;
			Datum& a = root.AppendAuxililaryAttribute("A"s);
			a = 0.0f;

			root.Adopt(*create_action, "Actions");

			ActionIncrement* increment = new ActionIncrement;
			increment->SetName("IncrementA"s);
			increment->SetTarget("A"s);
			increment->SetStep(1.0f);

			create_action->SetName(create_name);
			create_action->SetName("CreateA"s);
			Assert::AreEqual("CreateA"s, create_action->Name());
			create_action->SetTarget(increment->Name());
			Assert::AreEqual("IncrementA"s, create_action->Target());

			create_action->SetPrototype(prototype_name);
			create_action->SetPrototype("ActionIncrement"s);
			Assert::AreEqual(prototype_name, create_action->Prototype());

			root.Update(world_state);
			Assert::AreEqual(0.0f, a.Get<float>(0));
			world_state.CreateActions();
			root.Find("Actions")->Back<Scope>().As<ActionIncrement>()->SetTarget(increment->Target());
			root.Find("Actions")->Back<Scope>().As<ActionIncrement>()->SetStep(increment->Step());

			root.Update(world_state);
			Assert::AreEqual(1.0f, a.Get<float>(0));

			delete create_action;
			delete increment;
		}

		TEST_METHOD(TestActionDestroyAction)
		{
			using namespace std::string_literals;

			std::string increment_name = "IncrementA"s;
			std::string destroy_name = "DestroyA"s;

			TypeManager::AddType(Entity::TypeIdClass(), Entity::Signatures());

			GameTime game_time;
			WorldState world_state;
			world_state.SetGameTime(game_time);

			ActionDestroyAction* destroy_action = new ActionDestroyAction;
			Entity root;
			Datum& a = root.AppendAuxililaryAttribute("A"s);
			a = 0.0f;

			root.Adopt(*destroy_action, "Actions");

			ActionIncrement* increment = new ActionIncrement;
			increment->SetName("IncrementA"s);
			increment->SetName(increment_name);
			increment->SetTarget("A"s);
			increment->SetStep(1.0f);

			root.Adopt(*increment, "Actions");

			destroy_action->SetName(destroy_name);
			destroy_action->SetName("DestroyA"s);
			Assert::AreEqual("DestroyA"s, destroy_action->Name());
			destroy_action->SetTarget(increment->Name());
			Assert::AreEqual("IncrementA"s, destroy_action->Target());

			root.Update(world_state);
			Assert::AreEqual(1.0f, a.Get<float>(0));
			world_state.DestroyActions();
			root.Update(world_state);
			Assert::AreEqual(1.0f, a.Get<float>(0));

			delete destroy_action;
		}

		TEST_METHOD(TestNestedActionDestroyAction)
		{
			using namespace std::string_literals;

			TypeManager::AddType(Entity::TypeIdClass(), Entity::Signatures());
			TypeManager::AddType(Action::TypeIdClass(), Action::Signatures());
			TypeManager::AddType(ActionList::TypeIdClass(), ActionList::Signatures(), Action::TypeIdClass());
			TypeManager::AddType(ActionIncrement::TypeIdClass(), ActionIncrement::Signatures(), Action::TypeIdClass());
			TypeManager::AddType(ActionDestroyAction::TypeIdClass(), ActionDestroyAction::Signatures(), Action::TypeIdClass());
			TypeManager::AddType(ActionCreateAction::TypeIdClass(), ActionCreateAction::Signatures(), Action::TypeIdClass());
			Entity root;

			JsonTableParseHelper::SharedData shared(root);
			shared.Initialize();
			JsonTableParseHelper table_parse_helper;
			table_parse_helper.Initialize();
			JsonParseCoordinator parse_coordinator(shared);
			parse_coordinator.AddHelper(table_parse_helper);

			EntityFactory entity_factory;
			ActionIncrementFactory action_increment_factory;
			ActionDestroyActionFactory action_destroy_action_factory;

			GameTime game_time;
			WorldState world_state;
			world_state.SetGameTime(game_time);

			parse_coordinator.ParseFromFile("TestDestroyAction.json"s);
			Assert::AreEqual(8_z, root.Size());
			auto& actions_datum = root.Actions();
			Assert::AreEqual(1_z, actions_datum.Size());
			auto& children_datum = root.Children();
			Assert::AreEqual(2_z, children_datum.Size());

			Datum& a = root["X"];
			Assert::AreEqual(20.0f, a.Get<float>(0));
			root.Update(world_state);
			Assert::AreEqual(22.f, a.Get<float>(0));
			world_state.DestroyActions();
			root.Update(world_state);
			Assert::AreEqual(22.f, a.Get<float>(0));
		}

	private:
		static _CrtMemState sStartMemState; // Static members before C++17 could only be defined separate from declaration
	};

	_CrtMemState ActionTests::sStartMemState;
}
