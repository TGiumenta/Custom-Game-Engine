#pragma once
#include <CppUnitTest.h>
#include "Foo.h"
#include "Bar.h"
#include "SList.h"
#include "Vector.h"
#include "HashMap.h"
#include "Datum.h"
#include "Scope.h"
#include "AttributedFoo.h"
#include "JsonParseCoordinator.h"
#include "ActionList.h"
#include "ActionListIf.h"
#include "ActionIncrement.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include "Event.h"
#include "TestReaction.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Foo>(const Foo& t)
	{
		//std::wstringstream _s;	
		//_s << t.Data(); 
		//return _s.str();

		RETURN_WIDE_STRING(t.Data());
	}

	template<>
	inline std::wstring ToString<Bar>(const Bar& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}

	template<> 
	inline std::wstring ToString<SList<Foo>::Iterator>(const SList<Foo>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (std::runtime_error) { return L"end()"; }
	}

	template<>
	inline std::wstring ToString<SList<Bar>::Iterator>(const SList<Bar>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (std::runtime_error) { return L"end()"; }
	}

	template<>
	inline std::wstring ToString<SList<Foo>::ConstIterator>(const SList<Foo>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (std::runtime_error) { return L"end()"; }
	}

	template<>
	inline std::wstring ToString<Vector<Foo>::Iterator>(const Vector<Foo>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (std::runtime_error) { return L"end()"; }
	}

	template<>
	inline std::wstring ToString<Vector<Foo>::ConstIterator>(const Vector<Foo>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (std::runtime_error) { return L"end()"; }
	}

	template<>
	inline std::wstring ToString<HashMap<Foo, std::string>::PairType>(const HashMap<Foo, std::string>::PairType& t)
	{
		try
		{
			using namespace std::string_literals;
			std::wstringstream s;
			s << L"("s << ToString(t.first) << L","s << ToString(t.second) << L")"s;
			return s.str();
		}
		catch (std::runtime_error) { return L"Bad pair"; }
	}

	template<>
	inline std::wstring ToString<HashMap<Foo, std::string>::Iterator>(const HashMap<Foo, std::string>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (std::runtime_error) { return L"end()"; }
	}

	template<>
	inline std::wstring ToString<HashMap<Foo, std::string>::ConstIterator>(const HashMap<Foo, std::string>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (std::runtime_error) { return L"end()"; }
	}	

	template<>
	inline std::wstring ToString<Datum::DatumTypes>(const Datum::DatumTypes& t)
	{
		try
		{
			return ToString(Datum::type_to_name_map.at(t));
		}
		catch (std::runtime_error) { return L"Bad DatumType name"; }
	}

	template<>
	inline std::wstring ToString<glm::vec4>(const glm::vec4& t)
	{
		try
		{
			using namespace std::string_literals;
			std::wstringstream s;
			s << L"("s << ToString(t.x) << L","s << ToString(t.y) << L","s << ToString(t.z) << L","s << ToString(t.w) << L")"s ;
			return s.str();
		}
		catch (std::runtime_error) { return L"Bad Vector4"; }
	}

	template<>
	inline std::wstring ToString<glm::mat4>(const glm::mat4& t)
	{
		try
		{
			using namespace std::string_literals;
			std::wstringstream s;
			s << L"("s << ToString(t[0].x) << L","s << ToString(t[0].y) << L","s << ToString(t[0].z) << L","s << ToString(t[0].w) << L") "s
				<< L"("s << ToString(t[1].x) << L","s << ToString(t[1].y) << L","s << ToString(t[1].z) << L","s << ToString(t[1].w) << L") "s
				<< L"("s << ToString(t[2].x) << L","s << ToString(t[2].y) << L","s << ToString(t[2].z) << L","s << ToString(t[2].w) << L") "s
				<< L"("s << ToString(t[3].x) << L","s << ToString(t[3].y) << L","s << ToString(t[3].z) << L","s << ToString(t[3].w) << L")"s;
			return s.str();
		}
		catch (std::runtime_error) { return L"Bad Matrix4"; }
	}

	template<>
	inline std::wstring ToString<RTTI>(RTTI* t)
	{
		try
		{
			std::string basic_string = t->ToString();
			std::wstring wide_basic_string(basic_string.length(), L' '); // Make room for characters
			std::copy(basic_string.begin(), basic_string.end(), wide_basic_string.begin());
			return wide_basic_string;
		}
		catch (std::runtime_error) { return L"Bad RTTI pointer"; }
	}

	template<>
	inline std::wstring ToString<Datum>(const Datum& t)
	{
		try
		{
			using namespace std::string_literals;
			std::wstringstream s;
			s << L"("s << ToString(t.Type()) << L","s << ToString(t.Size()) << L","s << ToString(t.Capacity()) << L")"s;
			return s.str();
		}
		catch (std::runtime_error) { return L"Bad Datum"; }
	}

	template<>
	inline std::wstring ToString<Datum>(Datum* t)
	{
		try
		{
			using namespace std::string_literals;
			std::wstringstream s;
			s << L"(Type: "s << ToString(t->Type()) << L", Size: "s 
				<< ToString(t->Size()) << L", Capacity: "s 
				<< ToString(t->Capacity()) << L")"s;
			return s.str();
		}
		catch (std::runtime_error) { return L"Bad Datum*"; }
	}

	template<>
	inline std::wstring ToString<Datum>(const Datum* t)
	{
		try
		{
			using namespace std::string_literals;
			std::wstringstream s;
			s << L"(Type: "s << ToString(t->Type()) << L", Size: "s
				<< ToString(t->Size()) << L", Capacity: "s
				<< ToString(t->Capacity()) << L")"s;
			return s.str();
		}
		catch (std::runtime_error) { return L"Bad Datum*"; }
	}

	template<>
	inline std::wstring ToString<Scope>(Scope* t)
	{
		try
		{
			using namespace std::string_literals;
			std::wstringstream s;
			s << L"(Type: "s << ToString(t->TypeName()) << L", Size: "s
				<< ToString(t->Size()) << L")"s;
			return s.str();
		}
		catch (std::runtime_error) { return L"Bad Scope"; }
	}

	template<>
	inline std::wstring ToString<Scope>(const Scope& t)
	{
		try
		{
			using namespace std::string_literals;
			std::wstringstream s;
			s << L"(Type: "s << ToString(t.TypeName()) << L", Size: "s
				<< ToString(t.Size()) << L")"s;
			return s.str();
		}
		catch (std::runtime_error) { return L"Bad Scope Ref"; }
	}

	template<>
	inline std::wstring ToString<Attributed>(Attributed* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<AttributedFoo>(AttributedFoo* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonParseCoordinator::SharedData>(JsonParseCoordinator::SharedData* t)
	{
		try
		{
			using namespace std::string_literals;
			std::wstringstream s;
			s << L"(Depth: "s << ToString(t->Depth()) << L")"s;
			return s.str();
		}
		catch (std::runtime_error) { return L"Bad JsonParseCoordinator::SharedData Ref"; }
	}

	template<>
	inline std::wstring ToString<JsonParseCoordinator::SharedData>(const JsonParseCoordinator::SharedData* t)
	{
		try
		{
			using namespace std::string_literals;
			std::wstringstream s;
			s << L"(Depth: "s << ToString(t->Depth()) << L")"s;
			return s.str();
		}
		catch (std::runtime_error) { return L"Bad JsonParseCoordinator::SharedData Ref"; }
	}

	template<>
	inline std::wstring ToString<JsonParseCoordinator>(JsonParseCoordinator* t)
	{
		try
		{
			using namespace std::string_literals;
			std::wstringstream s;
			s << L"(FileName: "s << ToString(t->GetFileName()) << L", SharedData: "s
				<< ToString(t->GetSharedData()) << L")"s;
			return s.str();
		}
		catch (std::runtime_error) { return L"Bad JsonParseCoordinator Ref"; }
	}

	template<>
	inline std::wstring ToString<JsonParseCoordinator>(const JsonParseCoordinator* t)
	{
		try
		{
			using namespace std::string_literals;
			std::wstringstream s;
			s << L"(FileName: "s << ToString(t->GetFileName()) << L", SharedData: "s
				<< ToString(t->GetSharedData()) << L")"s;
			return s.str();
		}
		catch (std::runtime_error) { return L"Bad JsonParseCoordinator Ref"; }
	}

	template<>
	inline std::wstring ToString<ActionList>(ActionList* t)
	{
		try
		{
			using namespace std::string_literals;
			std::wstringstream s;
			s << L"(Name: "s << ToString(t->Name()) << L")"s;
			return s.str();
		}
		catch (std::runtime_error) { return L"Bad ActionList Ref"; }
	}

	template<>
	inline std::wstring ToString<ActionList>(const ActionList& t)
	{
		try
		{
			using namespace std::string_literals;
			std::wstringstream s;
			s << L"(Name: "s << ToString(t.Name()) << L")"s;
			return s.str();
		}
		catch (std::runtime_error) { return L"Bad ActionList Ref"; }
	}

	template<>
	inline std::wstring ToString<ActionListIf>(ActionListIf* t)
	{
		try
		{
			using namespace std::string_literals;
			std::wstringstream s;
			s << L"(Name: "s << ToString(t->Name()) << L")"s;
			return s.str();
		}
		catch (std::runtime_error) { return L"Bad ActionListIf Ref"; }
	}

	template<>
	inline std::wstring ToString<ActionCreateAction>(ActionCreateAction* t)
	{
		try
		{
			using namespace std::string_literals;
			std::wstringstream s;
			s << L"(Name: "s << ToString(t->Name()) << L")"s;
			return s.str();
		}
		catch (std::runtime_error) { return L"Bad ActionCreateAction Ref"; }
	}

	template<>
	inline std::wstring ToString<ActionDestroyAction>(ActionDestroyAction* t)
	{
		try
		{
			using namespace std::string_literals;
			std::wstringstream s;
			s << L"(Name: "s << ToString(t->Name()) << L")"s;
			return s.str();
		}
		catch (std::runtime_error) { return L"Bad ActionDestroyAction Ref"; }
	}

	template<>
	inline std::wstring ToString<ActionIncrement>(ActionIncrement* t)
	{
		try
		{
			using namespace std::string_literals;
			std::wstringstream s;
			s << L"(Name: "s << ToString(t->Name()) << L")"s;
			return s.str();
		}
		catch (std::runtime_error) { return L"Bad ActionIncrement Ref"; }
	}

	template<>
	inline std::wstring ToString<ActionIncrement>(const ActionIncrement& t)
	{
		try
		{
			using namespace std::string_literals;
			std::wstringstream s;
			s << L"(Name: "s << ToString(t.Target()) << L")"s;
			return s.str();
		}
		catch (std::runtime_error) { return L"Bad ActionIncrement Ref"; }
	}

	template<>
	inline std::wstring ToString<TestReaction>(const TestReaction& t)
	{
		try
		{
			using namespace std::string_literals;
			std::wstringstream s;
			s << L"(Count: "s << ToString(t.Count()) << L")"s;
			return s.str();
		}
		catch (std::runtime_error) { return L"Bad TestReaction Ref"; }
	}
}