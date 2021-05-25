#include "pch.h"
#include <crtdbg.h>
#include <exception>
#include <CppUnitTest.h>
#include "Scope.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace FieaGameEngine
{
	template <>
	struct DefaultHash<Foo>
	{
	public:
		size_t operator()(const Foo& key) const
		{
			return key.Data();
		}
	};
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ScopeTests)
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

		TEST_METHOD(TestRTTIInheritence)
		{
			using namespace std::string_literals;

			Scope scope;

			RTTI* rtti = &scope;
			Assert::IsFalse(rtti->Is("Foo"s));
			Assert::IsTrue(rtti->Is("Scope"s));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Scope::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(Scope::TypeIdClass(), rtti->TypeIdInstance());

			Foo* b = rtti->As<Foo>();
			Assert::IsNull(b);

			Scope* s = rtti->As<Scope>();
			Assert::IsNotNull(s);
			Assert::AreEqual(&scope, s);
			RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(Scope::TypeIdClass());
			Assert::IsNotNull(r);

			Scope other_scope;
			Assert::IsTrue(rtti->Equals(&other_scope));

			Foo foo(10);
			Assert::IsFalse(rtti->Equals(&foo));
		}

		TEST_METHOD(TestConstructor)
		{
			Scope scope;
			Assert::AreEqual(0_z, scope.Size());
			Assert::IsNull(scope.GetParent());

			Scope scope2(5);
			Assert::AreEqual(0_z, scope2.Size());
			Assert::IsNull(scope2.GetParent());
		}

		TEST_METHOD(TestCopyConstructor)
		{
			using namespace std::string_literals;

			Scope scope;
			Assert::AreEqual(0_z, scope.Size());
			Assert::IsNull(scope.GetParent());

			std::string a = "A"s;
			std::string b = "B"s;
			std::string c = "C"s;

			scope.Append(a);
			Assert::AreEqual(1_z, scope.Size());

			Scope copy(scope);
			Assert::AreEqual(1_z, copy.Size());

			Scope& bScope = scope.AppendScope(b);
			Assert::AreEqual(2_z, scope.Size());
			Assert::AreEqual(&scope, bScope.GetParent());

			scope.AppendScope(c);
			Scope next_copy(scope);
			Assert::AreEqual(scope, next_copy);
		}

		TEST_METHOD(TestMoveConstructor)
		{
			using namespace std::string_literals;
			std::string a = "A"s;
			std::string b = "B"s;
			std::string c = "C"s;

			Scope scope;
			Assert::AreEqual(0_z, scope.Size());
			Assert::IsNull(scope.GetParent());

			scope.Append(a);
			Assert::AreEqual(1_z, scope.Size());
			Scope& bScope = scope.AppendScope(b);
			Assert::AreEqual(2_z, scope.Size());
			Assert::AreEqual(&scope, bScope.GetParent());

			scope.AppendScope(c);

			// HAS to be heap allocated, this is not allowed.
			//Scope next_copy(std::move(scope)); 

			Scope* next_moved = new Scope(std::move(bScope));
			Assert::IsNull(bScope.GetParent());
			Assert::AreEqual(next_moved->GetParent(), &scope);

			auto [datum, found_index] = scope.FindContainedScope(bScope);
			Assert::IsNull(datum);
			Assert::AreEqual(std::numeric_limits<size_t>::max(), found_index);

			std::tie(datum, found_index) = scope.FindContainedScope(*next_moved);
			Assert::IsNotNull(datum);
			Assert::AreNotEqual(std::numeric_limits<size_t>::max(), found_index);

			delete &bScope;
		}

		TEST_METHOD(TestAssignment)
		{
			using namespace std::string_literals;

			Scope scope;
			std::string a = "A"s;
			std::string b = "B"s;
			std::string c = "C"s;
			std::string d = "D"s;
			std::string e = "E"s;

			Datum& aDatum = scope.Append(a);
			Assert::AreEqual(1_z, scope.Size());
			aDatum = scope.Append(a);
			Assert::AreEqual(1_z, scope.Size());
			Assert::AreEqual(&aDatum, scope.Find(a));
			Assert::AreSame(aDatum, scope[0]);
			Assert::AreSame(aDatum, scope[a]);

			Datum& bDatum = scope.Append(b);
			Assert::AreEqual(2_z, scope.Size());
			Assert::AreEqual(&bDatum, scope.Find(b));
			Assert::AreSame(bDatum, scope[1]);
			Assert::AreSame(bDatum, scope[b]);

			Scope copy;
			copy = scope;
			Assert::AreEqual(2_z, scope.Size());
			Assert::AreEqual(bDatum, copy[1]);
			Assert::AreEqual(bDatum, copy[b]); 

			Scope& dScope = scope.AppendScope(d); dScope;
			scope.AppendScope(e);
			Scope* move_copy = new Scope(); move_copy;
			*move_copy = std::move(dScope);
			auto [datum, found_index] = scope.FindContainedScope(dScope);
			Assert::IsNull(datum);
			Assert::AreEqual(std::numeric_limits<size_t>::max(), found_index);
			Assert::IsTrue(scope.IsAncestor(*move_copy));

			delete &dScope;
		}

		TEST_METHOD(TestFind)
		{
			using namespace std::string_literals;

			Scope scope;
			std::string a = "A"s;
			std::string b = "B"s;
			std::string c = "C"s;
			std::string d = "D"s;

			Datum& aDatum = scope.Append(a); aDatum;
			Datum& bDatum = scope.Append(b); bDatum;
			Datum& cDatum = scope.Append(c); cDatum;

			const Scope const_scope(scope);

			Assert::AreEqual(&const_scope[0], const_scope.Find(a));
			Assert::AreEqual(&const_scope[1], const_scope.Find(b));
			Assert::AreEqual(&const_scope[2], const_scope.Find(c));

			Assert::IsNull(const_scope.Find(d));
		}

		TEST_METHOD(TestSearch)
		{
			using namespace std::string_literals;

			Scope scope;
			std::string a = "A"s;
			std::string b = "B"s;
			std::string c = "C"s;

			scope.Append(a);
			auto [found_datum, found_scope] = scope.Search(a);
			Assert::IsNotNull(found_scope);

			Scope& aScope = scope.AppendScope(b);
			Assert::AreEqual(&scope, aScope.GetParent());
			std::tie(found_datum, found_scope) = scope.Search(a);
			Assert::AreEqual(aScope.GetParent(), found_scope);

			std::tie(found_datum, found_scope) = scope.Search(c);
			Assert::IsNull(found_datum);
			Assert::IsNull(found_scope);

			Scope* child = new Scope;
			Assert::IsNull(child->GetParent());

			scope.Adopt(*child, c);
			Assert::AreEqual(&scope, child->GetParent());
			std::tie(found_datum, found_scope) = child->Search(c);
			Assert::IsNotNull(found_datum);
			Assert::IsNotNull(found_scope);

			const Scope const_scope(scope);
			std::tie(found_datum, found_scope) = const_scope.Search(a);
			Assert::IsNotNull(found_datum);
			Assert::AreEqual(scope, *found_scope);
		}

		TEST_METHOD(TestAppend)
		{
			using namespace std::string_literals;

			Scope scope;
			std::string a = "A"s;
			std::string b = "B"s;
			std::string c = "C"s;

			Datum& aDatum = scope.Append(a);
			Assert::AreEqual(1_z, scope.Size());
			aDatum = scope.Append(a);
			Assert::AreEqual(1_z, scope.Size());
			Assert::AreEqual(&aDatum, scope.Find(a));
			Assert::AreSame(aDatum, scope[0]);
			Assert::AreSame(aDatum, scope[a]);

			Datum& bDatum = scope.Append(b);
			Assert::AreEqual(2_z, scope.Size());
			Assert::AreEqual(&bDatum, scope.Find(b));
			Assert::AreSame(bDatum, scope[1]);
			Assert::AreSame(bDatum, scope[b]);

			Assert::IsNull(scope.Find(c));
			auto expressionA = [&scope] { scope[2]; };
			Assert::ExpectException<std::runtime_error>(expressionA);

			const Scope copy_scope(scope);

			Assert::AreEqual(scope[0], copy_scope[0]);
			Assert::AreEqual(scope[1], copy_scope[1]);
			auto expressionB = [&copy_scope] { copy_scope[2]; };
			Assert::ExpectException<std::runtime_error>(expressionB);

			auto expressionC = [&scope] { scope.Append(""s); };
			Assert::ExpectException<std::invalid_argument>(expressionC);
		}

		TEST_METHOD(TestAppendScope)
		{
			using namespace std::string_literals;

			Scope scope;
			std::string a = "A"s;
			std::string b = "B"s;
			std::string c = "C"s;
			std::string d = "D"s;
			std::string e = "E"s;

			Scope& aScope = scope.AppendScope(a);
			Assert::AreEqual(1_z, scope.Size());
			Assert::AreEqual(&scope, aScope.GetParent());
			Datum* aScopeDatum = scope.Find(a);
			Assert::IsNotNull(aScopeDatum);
			Assert::AreEqual(Datum::DatumTypes::Table, aScopeDatum->Type());
			Assert::AreSame(aScope, aScopeDatum->Get<Scope>());
			Assert::AreEqual(aScopeDatum, &(scope[0]));

			Scope& bScope = scope.AppendScope(b);
			Assert::AreEqual(2_z, scope.Size());
			Assert::AreEqual(&scope, bScope.GetParent());
			Datum* bScopeDatum = scope.Find(b);
			Assert::IsNotNull(bScopeDatum);
			Assert::AreEqual(Datum::DatumTypes::Table, bScopeDatum->Type());
			Assert::AreSame(bScope, bScopeDatum->Get<Scope>());
			Assert::AreEqual(bScopeDatum, &(scope[1]));

			Scope& cScope = scope.AppendScope(c);
			Assert::AreEqual(3_z, scope.Size());
			Assert::AreEqual(&scope, cScope.GetParent());
			Datum* cScopeDatum = scope.Find(c);
			Assert::IsNotNull(cScopeDatum);
			Assert::AreEqual(Datum::DatumTypes::Table, cScopeDatum->Type());
			Assert::AreSame(cScope, cScopeDatum->Get<Scope>());
			Assert::AreEqual(cScopeDatum, &(scope[2]));

			Scope& dScope = cScope.AppendScope(d);
			Assert::AreEqual(1_z, cScope.Size());
			Assert::AreEqual(&scope, dScope.GetParent()->GetParent());

			auto expressionA = [&scope] { scope.AppendScope(""s); };
			Assert::ExpectException<std::invalid_argument>(expressionA);

			scope.Append(e);
			auto expressionB = [&scope, &e] { scope.AppendScope(e); };
			Assert::ExpectException<std::runtime_error>(expressionB);
		}

		TEST_METHOD(TestAdopt)
		{
			using namespace std::string_literals;

			Scope* child = new Scope;
			Assert::IsNull(child->GetParent());

			Scope scope;
			scope.Adopt(*child, "A"s);
			Assert::AreEqual(&scope, child->GetParent());

			Assert::ExpectException<std::exception>([&scope, child] { scope.Adopt(*child, ""s); });

			scope.Append("NonTableDatum") = 10;
			Assert::ExpectException<std::exception>([&scope] {Scope s; scope.Adopt(s, "NonTableDatum"s); });

			// Bad Adoptions
			Assert::ExpectException<std::exception>([&scope] {Scope s; scope.Adopt(scope, "A"s); });

			Assert::ExpectException<std::exception>([&scope, &child] {child->Adopt(scope, "A"s); });

			Assert::IsTrue(child->IsDescendant(scope));
			Assert::IsTrue(scope.IsAncestor(*child));
		}

		TEST_METHOD(TestEquality)
		{
			using namespace std::string_literals;

			Scope scope;
			std::string a = "A"s;
			std::string b = "B"s;
			std::string c = "C"s;

			Datum& aDatum = scope.Append(a);
			Assert::AreEqual(1_z, scope.Size());
			Assert::AreEqual(&aDatum, scope.Find(a));
			Assert::AreSame(aDatum, scope[0]);
			Assert::AreSame(aDatum, scope[a]);

			Datum& bDatum = scope.Append(b);
			Assert::AreEqual(2_z, scope.Size());
			Assert::AreEqual(&bDatum, scope.Find(b));
			Assert::AreSame(bDatum, scope[1]);
			Assert::AreSame(bDatum, scope[b]);

			Scope copy(scope);
			Assert::AreEqual(scope, copy);

			Scope bad;
			bad.Append(b);
			bad.Append(a);
			Assert::AreNotEqual(scope, bad);
			Assert::IsTrue(scope != bad);

			Scope other_scope;
			other_scope.Append(a);
			other_scope.AppendScope(c);
			other_scope.AppendScope(b);

			Assert::AreNotEqual(scope, other_scope);
		}

		TEST_METHOD(TestFindContainedScope)
		{
			using namespace std::string_literals;

			Scope scope;
			std::string a = "A"s;
			std::string b = "B"s;
			std::string c = "C"s;
			std::string d = "D"s;

			scope.Append(a);
			Scope& bScope = scope.AppendScope(b);
			Assert::AreEqual(2_z, scope.Size());
			Assert::AreEqual(&scope, bScope.GetParent());

			Scope& cScope = scope.AppendScope(c);
			Assert::AreEqual(3_z, scope.Size());
			Assert::AreEqual(&scope, cScope.GetParent());

			auto [datum, index] = scope.FindContainedScope(cScope);
			Assert::IsNotNull(datum);
			Assert::AreEqual(0_z, index);

			std::tie(datum, index) = scope.FindContainedScope(bScope);
			Assert::IsNotNull(datum);
			Assert::AreEqual(0_z, index);

			Scope& dScope = cScope.AppendScope(d);
			Assert::AreEqual(3_z, scope.Size());
			Assert::AreEqual(1_z, cScope.Size());
			Assert::AreEqual(&scope, cScope.GetParent());
			Assert::AreEqual(&cScope, dScope.GetParent());

			std::tie(datum, index) = scope.FindContainedScope(dScope);
			Assert::IsNull(datum);
			Assert::AreEqual(std::numeric_limits<size_t>::max(), index);
		}

		TEST_METHOD(TestOrphan)
		{
			using namespace std::string_literals;

			Scope scope;
			Scope& aScope = scope.AppendScope("A"s);
			Assert::AreEqual(&scope, aScope.GetParent());

			aScope.Orphan();
			Assert::IsTrue(aScope.GetParent() == nullptr);

			delete& aScope;
		}

		TEST_METHOD(TestAncestry)
		{
			using namespace std::string_literals;

			Scope scope;
			Scope& aScope = scope.AppendScope("A"s);
			Assert::AreEqual(&scope, aScope.GetParent());

			Assert::IsTrue(aScope.IsDescendant(scope));

			Scope other_scope;

			Assert::IsFalse(aScope.IsDescendant(other_scope));
		}

	private:
		static _CrtMemState sStartMemState; // Static members before C++17 could only be defined separate from declaration
	};

	_CrtMemState ScopeTests::sStartMemState;
}
