#include "pch.h"
#include <crtdbg.h>
#include <exception>
#include <CppUnitTest.h>
#include <fstream>
#include "JsonParseCoordinator.h"
#include "JsonIntegerParseHelper.h"
#include "JsonTestParseHelper.h"
#include "JsonTableParseHelper.h"
#include "Scope.h"
#include "Power.h"
#include "Factory.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ParseCoordinatorTests)
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

		TEST_METHOD(TestRTTI)
		{
			using namespace std::string_literals;

			// TestParseHelper
			{
				JsonTestParseHelper helper;
				RTTI* rtti = &helper;

				Assert::IsFalse(rtti->Is("Bar"s));
				Assert::IsTrue(rtti->Is("IJsonParseHelper"s));
				Assert::IsTrue(rtti->Is("JsonTestParseHelper"s));
				Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
				Assert::IsTrue(rtti->Is(JsonTestParseHelper::TypeIdClass()));
				Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
				Assert::AreEqual(JsonTestParseHelper::TypeIdClass(), rtti->TypeIdInstance());

				Bar* b = rtti->As<Bar>();
				Assert::IsNull(b);
				JsonTestParseHelper* h = rtti->As<JsonTestParseHelper>();
				Assert::IsNotNull(h);
				RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
				Assert::IsNull(r);
				r = rtti->QueryInterface(JsonTestParseHelper::TypeIdClass());
				Assert::IsNotNull(r);

				r = rtti->QueryInterface(Bar::TypeIdClass());
				Assert::IsNull(r);
				r = rtti->QueryInterface(JsonTestParseHelper::TypeIdClass());
				Assert::IsNotNull(r);
			}
			// IntegerParseHelper
			{
				JsonIntegerParseHelper helper;
				RTTI* rtti = &helper;

				Assert::IsFalse(rtti->Is("Bar"s));
				Assert::IsTrue(rtti->Is("IJsonParseHelper"s));
				Assert::IsTrue(rtti->Is("JsonIntegerParseHelper"s));
				Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
				Assert::IsTrue(rtti->Is(JsonIntegerParseHelper::TypeIdClass()));
				Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
				Assert::AreEqual(JsonIntegerParseHelper::TypeIdClass(), rtti->TypeIdInstance());
			}
			// TableParseHelper
			{
				JsonTableParseHelper helper;
				RTTI* rtti = &helper;

				Assert::IsFalse(rtti->Is("Bar"s));
				Assert::IsTrue(rtti->Is("IJsonParseHelper"s));
				Assert::IsTrue(rtti->Is("JsonTableParseHelper"s));
				Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
				Assert::IsTrue(rtti->Is(JsonTableParseHelper::TypeIdClass()));
				Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
				Assert::AreEqual(JsonTableParseHelper::TypeIdClass(), rtti->TypeIdInstance());
			}
			// JsonParseCoordinator
			{
				JsonIntegerParseHelper::SharedData shared;
				JsonParseCoordinator coordinator(shared);
				RTTI* rtti = &coordinator;

				Assert::IsFalse(rtti->Is("Bar"s));
				Assert::IsTrue(rtti->Is("JsonParseCoordinator"s));
				Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
				Assert::IsTrue(rtti->Is(JsonParseCoordinator::TypeIdClass()));
				Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
				Assert::AreEqual(JsonParseCoordinator::TypeIdClass(), rtti->TypeIdInstance());

				Assert::IsFalse(rtti->Is("Bar"s));
				rtti = &shared;
				JsonParseCoordinator::SharedData* other_shared = rtti->As<JsonParseCoordinator::SharedData>();
				Assert::IsNotNull(other_shared);
			}
		}

		TEST_METHOD(TestJsonRead)
		{
			using namespace std::string_literals;

			JsonIntegerParseHelper::SharedData shared;
			shared.Initialize();
			JsonIntegerParseHelper integer_parse_helper;
			integer_parse_helper.Initialize();

			JsonParseCoordinator parse_coordinator(shared);
			Assert::AreEqual(&parse_coordinator, shared.GetJsonParseCoordinator());
			parse_coordinator.AddHelper(integer_parse_helper);

			/*parse_coordinator.ParseFromFile("JsonInteger.json"s);
			Assert::AreEqual(6_z, shared.data.Size());
			Assert::AreEqual(100, shared.data.Front());
			Assert::AreEqual(500, shared.data.Back());
			Assert::AreEqual(0_z, shared.Depth());*/

			parse_coordinator.RemoveHelper(integer_parse_helper);

			const JsonParseCoordinator const_parse_coordinator(shared);

			JsonIntegerParseHelper::SharedData* shared_ptr = &shared;
			const JsonIntegerParseHelper::SharedData const_shared = *const_cast<JsonIntegerParseHelper::SharedData*>(shared_ptr);
			const JsonParseCoordinator& shared_ref = *const_shared.GetJsonParseCoordinator();
			Assert::AreEqual(&const_parse_coordinator, &shared_ref);

			auto expressionA = [&parse_coordinator] { parse_coordinator.ParseFromFile("BadFile.json"); };
			Assert::ExpectException<std::runtime_error>(expressionA);
		}

		TEST_METHOD(TestJsonStreamRead)
		{
			using namespace std::string_literals;

			Json::Value root;

			std::filebuf buffer;
			buffer.open("JsonInteger.json"s, std::ios::in);
			std::istream json(&buffer);

			JsonIntegerParseHelper::SharedData shared;
			shared.Initialize();
			JsonIntegerParseHelper integer_parse_helper;
			integer_parse_helper.Initialize();

			JsonParseCoordinator parse_coordinator(shared);
			Assert::AreEqual(&parse_coordinator, shared.GetJsonParseCoordinator());
			parse_coordinator.AddHelper(integer_parse_helper);

			parse_coordinator.Parse(json);
			/*Assert::AreEqual(6_z, shared.data.Size());
			Assert::AreEqual(100, shared.data.Front());
			Assert::AreEqual(500, shared.data.Back());
			Assert::AreEqual(0_z, shared.Depth());*/

			auto expressionA = [&parse_coordinator, &integer_parse_helper] { parse_coordinator.AddHelper(integer_parse_helper); };
			Assert::ExpectException<std::runtime_error>(expressionA);

			buffer.close();
		}

		TEST_METHOD(TestJsonStringRead)
		{
			using namespace std::string_literals;

			Json::Value root;
			std::ifstream json("JsonInteger.json"s);
			json >> root;
			Json::String json_string = root.toStyledString();

			JsonIntegerParseHelper::SharedData shared;
			shared.Initialize();
			JsonIntegerParseHelper integer_parse_helper;
			integer_parse_helper.Initialize();

			JsonParseCoordinator parse_coordinator(shared);
			Assert::AreEqual(&parse_coordinator, shared.GetJsonParseCoordinator());
			parse_coordinator.AddHelper(integer_parse_helper);

			parse_coordinator.Parse(json_string);
			/*Assert::AreEqual(6_z, shared.data.Size());
			Assert::AreEqual(100, shared.data.Front());
			Assert::AreEqual(500, shared.data.Back());
			Assert::AreEqual(0_z, shared.Depth());*/

			JsonIntegerParseHelper::SharedData other_shared;
			other_shared.Initialize();
			parse_coordinator.SetSharedData(&other_shared);
			parse_coordinator.Parse(json_string);
			/*Assert::AreEqual(6_z, other_shared.data.Size());
			Assert::AreEqual(100, other_shared.data.Front());
			Assert::AreEqual(500, other_shared.data.Back());
			Assert::AreEqual(0_z, other_shared.Depth());*/
		}

		TEST_METHOD(TestNestingDepth)
		{
			using namespace std::string_literals;

			std::string json_string = R"(
				{
					"Address":
					{
						"type": "Table",
						"value":
						{
							"Street":
							{
								"type": "string",
								"value": "123 Anystreet St."
							}
						}
					}
				})"s;

			JsonTestParseHelper::SharedData shared;
			shared.Initialize();
			JsonTestParseHelper test_parse_helper;
			test_parse_helper.Initialize();

			JsonParseCoordinator parse_coordinator(shared);
			Assert::AreEqual(&parse_coordinator, shared.GetJsonParseCoordinator());
			parse_coordinator.AddHelper(test_parse_helper);

			parse_coordinator.Parse(json_string);
			Assert::AreEqual(6U, test_parse_helper.start_handler_count);
			Assert::AreEqual(6U, test_parse_helper.end_handler_count);

			// No viable helpers...never goes in
			parse_coordinator.RemoveHelper(test_parse_helper);
			shared.Initialize();
			parse_coordinator.Parse(json_string);
			Assert::AreEqual(0_z, shared.Depth());
			Assert::AreEqual(0_z, shared.max_depth);

			JsonIntegerParseHelper integer_helper;
			integer_helper.Initialize();

			// SharedData doesn't align with the helpers all the time
			parse_coordinator.AddHelper(test_parse_helper);
			parse_coordinator.AddHelper(integer_helper);
			JsonIntegerParseHelper::SharedData integer_shared;
			parse_coordinator.SetSharedData(&integer_shared);
			parse_coordinator.Parse(json_string);
			Assert::AreEqual(0_z, shared.Depth());
			Assert::AreEqual(0_z, shared.max_depth);
		}

		TEST_METHOD(TestClone)
		{
			JsonTestParseHelper::SharedData shared;
			JsonParseCoordinator coordinator(shared);
			Assert::IsFalse(coordinator.IsClone());

			JsonTestParseHelper test_helper;
			coordinator.AddHelper(test_helper);

			JsonIntegerParseHelper integer_helper;
			coordinator.AddHelper(integer_helper);

			auto clone = coordinator.Clone();
			Assert::AreNotEqual(clone, &coordinator);
			Assert::IsTrue(clone->IsClone());
			Assert::AreEqual(2_z, clone->Helpers().Size());

			Assert::ExpectException<std::exception>([&clone] { JsonTestParseHelper helper; clone->AddHelper(helper); });
			Assert::ExpectException<std::exception>([&clone] { JsonTestParseHelper::SharedData data; clone->SetSharedData(&data); });
			Assert::ExpectException<std::exception>([&clone] { JsonTestParseHelper helper; clone->RemoveHelper(helper); });

			delete clone;
		}

		TEST_METHOD(TestMoveSemantics)
		{
			JsonTestParseHelper::SharedData shared;
			JsonParseCoordinator coordinator(shared);
			Assert::IsFalse(coordinator.IsClone());

			JsonTestParseHelper test_helper;
			coordinator.AddHelper(test_helper);
			JsonIntegerParseHelper integer_helper;
			coordinator.AddHelper(integer_helper);

			JsonParseCoordinator move_coordinator(std::move(coordinator));
			Assert::AreEqual(2_z, move_coordinator.Helpers().Size());

			JsonParseCoordinator move_coordinator_other(shared);
			move_coordinator_other = std::move(move_coordinator);
			Assert::AreEqual(2_z, move_coordinator_other.Helpers().Size());
		}

		ConcreteFactory(Power, Scope)

		TEST_METHOD(TestTableParse)
		{
			using namespace std::string_literals;
			
			Scope root;
			JsonTableParseHelper::SharedData shared(root);
			shared.Initialize();
			JsonTableParseHelper table_parse_helper;
			table_parse_helper.Initialize();

			JsonParseCoordinator parse_coordinator(shared);
			Assert::AreEqual(&parse_coordinator, shared.GetJsonParseCoordinator());
			parse_coordinator.AddHelper(table_parse_helper);

			// This helper is garbage, it won't be able to parse anything
			JsonIntegerParseHelper integer_helper;
			parse_coordinator.AddHelper(integer_helper);

			PowerFactory powers_factory;

			parse_coordinator.ParseFromFile("JsonTable.json"s);
			Assert::AreEqual(7_z, shared.root->Size());
			Assert::AreEqual(0_z, shared.Depth());

			table_parse_helper.Cleanup();

			parse_coordinator.Initialize();
			JsonIntegerParseHelper::SharedData integer_shared;
			parse_coordinator.SetSharedData(&integer_shared);
			parse_coordinator.ParseFromFile("JsonTable.json"s);
			Assert::AreEqual(7_z, shared.root->Size()); // Doesn't alter any size post-clear, please don't re-use the same root
			Assert::AreEqual(0_z, shared.Depth());
		}

		TEST_METHOD(TestTableClone)
		{
			using namespace std::string_literals;

			Scope root;
			JsonTableParseHelper::SharedData shared(root);
			shared.Initialize();
			JsonTableParseHelper table_parse_helper;
			table_parse_helper.Initialize();

			JsonTableParseHelper::SharedData* shared_clone = shared.Create();
			JsonParseCoordinator parse_coordinator(*shared_clone);
			Assert::IsNull(shared.GetJsonParseCoordinator());

			JsonTableParseHelper* cloned = static_cast<JsonTableParseHelper*>(table_parse_helper.Create());
			parse_coordinator.AddHelper(*cloned);

			PowerFactory powers_factory;

			parse_coordinator.ParseFromFile("JsonTable.json"s);
			Assert::AreEqual(7_z, shared.root->Size());
			Assert::AreEqual(0_z, shared.Depth());

			delete shared_clone;
			delete cloned;
		}

	private:
		static _CrtMemState sStartMemState; // Static members before C++17 could only be defined separate from declaration
	};

	_CrtMemState ParseCoordinatorTests::sStartMemState;
}
