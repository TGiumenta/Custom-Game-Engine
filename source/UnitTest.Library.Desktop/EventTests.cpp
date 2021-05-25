#include "pch.h"
#include <crtdbg.h>
#include <exception>
#include <memory>
#include <CppUnitTest.h>
#include "EventPublisher.h"
#include "Event.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EventTests)
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
			Event<AddCoinsEvent>::UnsubscribeAll();
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

		TEST_METHOD(TestEventRTTIDerived)
		{
			using namespace std::string_literals;

			InventoryManager manager;

			AddCoinsEvent coins_event{ 5 };
			Event<AddCoinsEvent> e(coins_event);

			RTTI* rtti = &e;
			Assert::IsFalse(rtti->Is("Foo"s));
			Assert::IsTrue(rtti->Is("Event<T>"s));
			Assert::IsTrue(rtti->Is("EventPublisher"s));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Event<AddCoinsEvent>::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(Event<AddCoinsEvent>::TypeIdClass(), rtti->TypeIdInstance());

			Foo* b = rtti->As<Foo>();
			Assert::IsNull(b);

			Event<AddCoinsEvent>* f = rtti->As<Event<AddCoinsEvent>>();
			Assert::IsNotNull(f);
			Assert::IsTrue(&e == f);

			Event<AddCoinsEvent>* fAsEventAddCoinsEvent = rtti->As<Event<AddCoinsEvent>>();
			Assert::IsNotNull(fAsEventAddCoinsEvent);
			Assert::IsTrue(static_cast<Event<AddCoinsEvent>*>(&e) == fAsEventAddCoinsEvent);

			RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(Event<AddCoinsEvent>::TypeIdClass());
			Assert::IsNotNull(r);

			Foo foo(10);
			Assert::IsFalse(rtti->Equals(&foo));
		}

		TEST_METHOD(TestEventSubscription)
		{
			InventoryManager manager;

			AddCoinsEvent coins_event{ 5 };
			Event<AddCoinsEvent> e(coins_event);
			Event<AddCoinsEvent>::Subscribe(manager);
			Assert::AreEqual(Event<AddCoinsEvent>::Size(), 1_z);
			
			Assert::AreEqual(manager.Coins(), 0U);
			e.Deliver();
			Assert::AreEqual(manager.Coins(), 5U);

			// Adding a duplicate event does nothing
			Event<AddCoinsEvent> e_copy(AddCoinsEvent{ 10 });
			Event<AddCoinsEvent>::Subscribe(manager);
			Assert::AreEqual(Event<AddCoinsEvent>::Size(), 1_z);

			Event<AddCoinsEvent>::Unsubscribe(manager);
			Assert::AreEqual(Event<AddCoinsEvent>::Size(), 0_z);
		}

		TEST_METHOD(TestEventQueue)
		{
			InventoryManager manager;
			GameTime game_time;
			EventQueue q;

			Assert::IsTrue(q.IsEmpty());
			Assert::AreEqual(q.Size(), 0_z);

			AddCoinsEvent coins_event{ 5 };
			Event<AddCoinsEvent>::Subscribe(manager);

			std::shared_ptr<EventPublisher> e = std::make_shared<Event<AddCoinsEvent>>(coins_event);
			q.Enqueue(e, game_time);
			Assert::AreEqual(q.Size(), 0_z);
			Assert::AreEqual(manager.Coins(), 0U);
			q.Send(e);
			Assert::AreEqual(manager.Coins(), 5U);
			q.Send(e);
			Assert::AreEqual(manager.Coins(), 10U);
			
			q.Update(game_time);
			Assert::AreEqual(manager.Coins(), 10U);
			game_time.SetCurrentTime(game_time.CurrentTime() + std::chrono::minutes(1));
			q.Update(game_time);
			Assert::AreEqual(manager.Coins(), 15U);

			q.Clear();
			Assert::AreEqual(q.Size(), 0_z);
		}

	private:
		static _CrtMemState sStartMemState; // Static members before C++17 could only be defined separate from declaration
	};

	_CrtMemState EventTests::sStartMemState;
}
