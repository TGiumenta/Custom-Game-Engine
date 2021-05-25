#include "pch.h"
#include <crtdbg.h>
#include <exception>
#include <memory>
#include <CppUnitTest.h>
#include "TestReaction.h"
#include "ActionIncrement.h"
#include "EventMessageAttributed.h"
#include "ReactionAttributed.h"
#include "ActionEvent.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ReactionTests)
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

		struct AddCoinsEvent final
		{
			uint32_t coins_to_add;
		};

		class InventoryManager : public EventSubscriber
		{
		public:
			void Notify(const EventPublisher& event) override
			{
				const Event<AddCoinsEvent>* coins_event = event.As<Event<AddCoinsEvent>>();
				if (coins_event)
				{
					const AddCoinsEvent& message = coins_event->Message();
					coins += message.coins_to_add;
				}
			}

			uint32_t Coins() const { return coins; }

		private:
			uint32_t coins = 0;
		};

		TEST_METHOD(TestDerived)
		{
			using namespace std::string_literals;

			TypeManager::AddType(Action::TypeIdClass(), Action::Signatures());
			TypeManager::AddType(ActionList::TypeIdClass(), ActionList::Signatures(), Action::TypeIdClass());
			TypeManager::AddType(Reaction::TypeIdClass(), Reaction::Signatures(), ActionList::TypeIdClass());
			TypeManager::AddType(TestReaction::TypeIdClass(), TestReaction::Signatures(), Reaction::TypeIdClass());
			const auto signatures = TypeManager::GetSignaturesForType(TestReaction::TypeIdClass());
			Assert::IsFalse(signatures.IsEmpty());

			TestReaction reaction;
			Assert::IsTrue(reaction.IsPrescribedAttribute("Name"s));
			Assert::IsTrue(reaction.IsPrescribedAttribute("Count"s));
			Assert::AreEqual(0, reaction.Count());

			AddCoinsEvent coins_event{ 5 };
			Event<AddCoinsEvent> e(coins_event);
			Event<AddCoinsEvent>::Subscribe(reaction);
			Assert::AreEqual(Event<AddCoinsEvent>::Size(), 1_z);
			e.Deliver();
			Assert::AreEqual(1, reaction.Count());
			Event<AddCoinsEvent>::UnsubscribeAll();

			// Clone
			{
				auto clone = reaction.Clone();
				Assert::AreNotSame(reaction, *clone);
				Assert::AreEqual(reaction, *clone);

				delete clone;
			}

			Scope* s = Factory<Scope>::Create(TestReaction::TypeName());
			Assert::IsNull(s);

			TestReactionFactory factory;
			s = Factory<Scope>::Create(TestReaction::TypeName());
			Assert::IsNotNull(s);

			TestReaction* action = s->As<TestReaction>();
			Assert::IsNotNull(action);

			TypeManager::RemoveType(TestReaction::TypeIdClass());
			delete s;
		}

		TEST_METHOD(TestEventMessageAttributedSubtype)
		{
			using namespace std::string_literals;

			GameTime game_time;
			EventQueue queue;
			WorldState state(game_time);

			const std::string subtype = "Test!"s;
			EventMessageAttributed event(state, subtype);
			Assert::IsNotNull(&event.GetWorldState());

			Assert::AreEqual(subtype, event.Subtype());
			const std::string changed_subtype = "Changed Test!"s;
			event.SetSubtype(changed_subtype);
			Assert::AreEqual(changed_subtype, event.Subtype());
			event.SetSubtype("Last Change!"s);
			Assert::AreEqual("Last Change!"s, event.Subtype());

			const std::string const_subtype = "Test!"s;
			const EventMessageAttributed const_event(state, const_subtype);
			Assert::AreEqual(const_subtype, const_event.Subtype());
		}

		TEST_METHOD(TestReactionAttributedSubtype)
		{
			using namespace std::string_literals;

			ReactionAttributed reaction;
			reaction.SetSubtype("Test!");
			Assert::AreEqual("Test!"s, reaction.Subtype());
			const std::string different_subtype = "Different Test!"s;
			reaction.SetSubtype(different_subtype);
			Assert::AreEqual("Different Test!"s, reaction.Subtype());
			const ReactionAttributed const_reaction(reaction);
			Assert::AreEqual("Different Test!"s, const_reaction.Subtype());
			Event<EventMessageAttributed>::UnsubscribeAll();
		}

		TEST_METHOD(TestReactionsAddedToActions)
		{
			using namespace std::string_literals;

			TypeManager::AddType(Action::TypeIdClass(), Action::Signatures());
			TypeManager::AddType(ActionList::TypeIdClass(), ActionList::Signatures(), Action::TypeIdClass());

			ActionList action;
			ReactionAttributedFactory reaction_attributed_factory;

			Assert::AreEqual(0_z, action.Actions().Size());
			action.CreateAction(reaction_attributed_factory.ClassName(), "First Reaction"s);
			Assert::AreEqual(1_z, action.Actions().Size());

			GameTime game_time;
			WorldState world_state(game_time);
			action.Update(world_state);

			Event<EventMessageAttributed>::UnsubscribeAll();
		}

		TEST_METHOD(TestReactionAttributedNotify)
		{
			TypeManager::AddType(Action::TypeIdClass(), Action::Signatures());
			TypeManager::AddType(ActionList::TypeIdClass(), ActionList::Signatures(), Action::TypeIdClass());
			TypeManager::AddType(ActionIncrement::TypeIdClass(), ActionIncrement::Signatures(), Action::TypeIdClass());
			TypeManager::AddType(Reaction::TypeIdClass(), Reaction::Signatures(), ActionList::TypeIdClass());
			TypeManager::AddType(ReactionAttributed::TypeIdClass(), ReactionAttributed::Signatures(), Reaction::TypeIdClass());
			
			const auto signatures = TypeManager::GetSignaturesForType(ActionList::TypeIdClass());
			Assert::AreEqual(2_z, signatures.Size());

			ActionListFactory action_list_factory;
			ReactionAttributedFactory reaction_attributed_factory;

			using namespace std::string_literals;
			ReactionAttributed reaction;
			Assert::AreEqual(1_z, reaction.Signatures().Size());
			reaction.CreateAction("ActionList", "Test!");

			// Not an EventAttributed
			AddCoinsEvent coins_event{ 5 };
			Event<AddCoinsEvent> e(coins_event);
			Event<AddCoinsEvent>::Subscribe(reaction);
			Assert::AreEqual(Event<AddCoinsEvent>::Size(), 1_z);
			e.Deliver();
			Event<AddCoinsEvent>::UnsubscribeAll();

			GameTime game_time;
			EventQueue queue;
			WorldState state(game_time);

			const std::string subtype = "Test!"s;
			EventMessageAttributed new_event(state, subtype);
			new_event.AppendAuxililaryAttribute("ActionIncrement"s);

			reaction.SetSubtype(subtype);
			Event<EventMessageAttributed> message_attributed(new_event);
			Event<EventMessageAttributed>::Subscribe(reaction);
			message_attributed.Deliver();

			Event<EventMessageAttributed>::UnsubscribeAll();
		}

		TEST_METHOD(TestActionEvent)
		{
			using namespace std::string_literals;
			GameTime game_time;
			EventQueue queue;
			Assert::AreEqual(queue.Size(), 0_z);
			WorldState state(game_time);

			ActionEvent action_event(queue);
			Assert::AreEqual(2_z, action_event.Signatures().Size());

			// Test Accessors
			const ActionEvent const_action_event(action_event);
			Assert::AreEqual(const_action_event.Subtype(), action_event.Subtype());
			action_event.SetDelay(5);
			Assert::AreEqual(5, action_event.Delay());
			Assert::IsNotNull(const_action_event.Queue());
			Assert::IsNotNull(action_event.Queue());

			EventQueue fake_queue;
			action_event.SetQueue(fake_queue);
			Assert::IsNotNull(action_event.Queue());

			action_event.AppendAuxililaryAttribute("ActionIncrement"s);
			action_event.SetDelay(0);
			action_event.SetQueue(queue);
			action_event.Update(state);
			queue.Update(game_time);
			Assert::AreEqual(1_z, queue.Size());
		}

	private:
		static _CrtMemState sStartMemState; // Static members before C++17 could only be defined separate from declaration
	};

	_CrtMemState ReactionTests::sStartMemState;
}
