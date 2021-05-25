#include "pch.h"
#include <crtdbg.h>
#include <exception>
#include <CppUnitTest.h>
#include "Foo.h"
#include "Datum.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	struct IncrementStrategy final
	{
		size_t operator()(size_t /*size*/, std::size_t capacity) const
		{
			return capacity * 2;
		}
	};

	TEST_CLASS(DatumTests)
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

		TEST_METHOD(TestConstructor)
		{
			Datum d;
			Assert::AreEqual(d.Type(), Datum::DatumTypes::Unknown);
		}

		TEST_METHOD(TestCopyConstructor)
		{
			Datum d(Datum::DatumTypes::Integer);
			d.PushBack(10);
			Datum e(d);
			Assert::AreEqual(e.Type(), d.Type());
			Assert::AreEqual(e.Size(), d.Size());
			Assert::AreEqual(e.Capacity(), d.Capacity());
			Assert::AreEqual(e.Front<int>(), d.Front<int>());

			Datum f(std::move(e));
			Assert::AreEqual(f.Type(), d.Type());
			Assert::AreEqual(f.Size(), d.Size());
			Assert::AreEqual(f.Capacity(), d.Capacity());
			Assert::AreEqual(f.Front<int>(), d.Front<int>());
		}
		
		TEST_METHOD(TestLValueEqualOperator)
		{
			// Datum
			{
				Datum d(Datum::DatumTypes::Integer);
				d.PushBack(10);
				d.PushBack(20);
				d.PushBack(30);
				Datum e(Datum::DatumTypes::Integer);
				e = d;
				Assert::AreEqual(e.Type(), d.Type());
				Assert::AreEqual(e.Size(), d.Size());
				Assert::AreEqual(e.Capacity(), d.Capacity());
				Assert::AreEqual(e.Front<int>(), d.Front<int>());
				Assert::AreEqual(e.Back<int>(), d.Back<int>());

				Datum g(Datum::DatumTypes::Integer);
				int arr[4] = { 5, 10, 15, 20 };
				g.SetStorage<int>(arr, 4);

				Datum h(g);
				Assert::AreEqual(h.Size(), g.Size());
				Assert::AreEqual(h.Capacity(), g.Capacity());
				Assert::AreEqual(h.Front<int>(), g.Front<int>());
				Assert::AreEqual(h.Back<int>(), g.Back<int>());

				Datum i(Datum::DatumTypes::Integer);
				i.PushBack(10);
				i = g; // Internal = External
				Assert::AreEqual(h.Size(), i.Size());
				Assert::AreEqual(h.Capacity(), i.Capacity());
				Assert::AreEqual(h.Front<int>(), i.Front<int>());
				Assert::AreEqual(h.Back<int>(), i.Back<int>());

				Datum j(Datum::DatumTypes::Integer);
				j.SetStorage<int>(arr, 4);

				i = j; // External = External
				Assert::AreEqual(i.Size(), j.Size());
				Assert::AreEqual(i.Capacity(), j.Capacity());
				Assert::AreEqual(i.Front<int>(), j.Front<int>());
				Assert::AreEqual(i.Back<int>(), j.Back<int>());

				Datum k(Datum::DatumTypes::Integer);
				k.PushBack(10);
				k.PushBack(20);
				k.PushBack(30);

				i = k; // External = Internal
				Assert::AreEqual(i.Size(), k.Size());
				Assert::AreEqual(i.Capacity(), k.Capacity());
				Assert::AreEqual(i.Front<int>(), k.Front<int>());
				Assert::AreEqual(i.Back<int>(), k.Back<int>());

				Datum l(Datum::DatumTypes::Integer);
				l.PushBack(10);
				auto expressionA = [&l, &arr] { l.SetStorage<int>(arr, 4); };
				Assert::ExpectException<std::runtime_error>(expressionA);
			}
			// Integer
			{
				Datum d(Datum::DatumTypes::Integer);
				d.PushBack(10);
				d.PushBack(20);
				d.PushBack(30);
				Datum e = d;
				Assert::AreEqual(e.Type(), d.Type());
				Assert::AreEqual(e.Size(), d.Size());
				Assert::AreEqual(e.Capacity(), d.Capacity());
				Assert::AreEqual(e.Front<int>(), d.Front<int>());
				Assert::AreEqual(e.Back<int>(), d.Back<int>());

				e = 30;
				Assert::AreEqual(e.Size(), 1_z);
				Assert::AreEqual(e.Front<int>(), 30);
			}
			// Float
			{
				Datum d(Datum::DatumTypes::Float);
				d.PushBack(10.0f);
				d.PushBack(20.0f);
				d.PushBack(30.0f);
				Datum e = d;
				Assert::AreEqual(e.Type(), d.Type());
				Assert::AreEqual(e.Size(), d.Size());
				Assert::AreEqual(e.Capacity(), d.Capacity());
				Assert::AreEqual(e.Front<float>(), d.Front<float>());
				Assert::AreEqual(e.Back<float>(), d.Back<float>());

				e = 30.0f;
				Assert::AreEqual(e.Size(), 1_z);
				Assert::AreEqual(e.Front<float>(), 30.0f);
			}
			// String
			{
				using namespace std::string_literals;

				Datum d(Datum::DatumTypes::String);
				d.PushBack("Hello"s);
				d.PushBack("Goodbye"s);
				d.PushBack("Music"s);
				Datum e = d;
				Assert::AreEqual(e.Type(), d.Type());
				Assert::AreEqual(e.Size(), d.Size());
				Assert::AreEqual(e.Capacity(), d.Capacity());
				Assert::AreEqual(e.Front<std::string>(), d.Front<std::string>());
				Assert::AreEqual(e.Back<std::string>(), d.Back<std::string>());

				e.Clear();

				e = "NewHello"s;
				Assert::AreEqual(e.Size(), 1_z);
				Assert::AreEqual(e.Front<std::string>(), "NewHello"s);
			}
			// Vector
			{
				Datum d(Datum::DatumTypes::Vector);
				d.PushBack(glm::vec4(10));
				d.PushBack(glm::vec4(20));
				d.PushBack(glm::vec4(30));
				Datum e = d;
				Assert::AreEqual(e.Type(), d.Type());
				Assert::AreEqual(e.Size(), d.Size());
				Assert::AreEqual(e.Capacity(), d.Capacity());
				Assert::AreEqual(e.Front<glm::vec4>(), d.Front<glm::vec4>());
				Assert::AreEqual(e.Back<glm::vec4>(), d.Back<glm::vec4>());

				e = glm::vec4(30);
				Assert::AreEqual(e.Size(), 1_z);
				Assert::AreEqual(e.Front<glm::vec4>(), glm::vec4(30));
			}
			// Matrix
			{
				Datum d(Datum::DatumTypes::Matrix);
				d.PushBack(glm::mat4(10));
				d.PushBack(glm::mat4(20));
				d.PushBack(glm::mat4(30));
				Datum e = d;
				Assert::AreEqual(e.Type(), d.Type());
				Assert::AreEqual(e.Size(), d.Size());
				Assert::AreEqual(e.Capacity(), d.Capacity());
				Assert::AreEqual(e.Front<glm::mat4>(), d.Front<glm::mat4>());
				Assert::AreEqual(e.Back<glm::mat4>(), d.Back<glm::mat4>());

				e = glm::mat4(30);
				Assert::AreEqual(e.Size(), 1_z);
				Assert::AreEqual(e.Front<glm::mat4>(), glm::mat4(30));
			}
			// Pointer
			{
				Foo f;
				RTTI* f_ref = &f;
				Foo g;
				RTTI* g_ref = &g;
				Foo h;
				RTTI* h_ref = &h;
				Foo i;
				RTTI* i_ref = &i;

				Datum d(Datum::DatumTypes::Pointer);
				d.PushBack(f_ref);
				d.PushBack(g_ref);
				d.PushBack(h_ref);
				Datum e = d;
				Assert::AreEqual(e.Type(), d.Type());
				Assert::AreEqual(e.Size(), d.Size());
				Assert::AreEqual(e.Capacity(), d.Capacity());
				Assert::IsTrue(e.Front<RTTI*>() == d.Front<RTTI*>());
				Assert::IsTrue(e.Back<RTTI*>() == d.Back<RTTI*>());

				e = i_ref;
				Assert::AreEqual(e.Size(), 1_z);
				Assert::IsTrue(e.Front<RTTI*>() == i_ref);
			}
			// Table
			{
				Datum d(Datum::DatumTypes::Table);

				Scope a;
				d = a;
			}
		}

		TEST_METHOD(TestRValueEqualOperator)
		{
			Datum d(Datum::DatumTypes::Integer);
			d.PushBack(10);
			d.PushBack(20);
			d.PushBack(30);
			Datum e(Datum::DatumTypes::Integer);

			const Datum::DatumTypes d_type = d.Type();
			const size_t d_size = d.Size();
			const size_t d_capacity = d.Capacity();
			const int d_front = d.Front<int>();
			const int d_back = d.Back<int>();

			e = std::move(d);
			Assert::AreEqual(e.Type(), d_type);
			Assert::AreEqual(e.Size(), d_size);
			Assert::AreEqual(e.Capacity(), d_capacity);
			Assert::AreEqual(e.Front<int>(), d_front);
			Assert::AreEqual(e.Back<int>(), d_back);
		}

		TEST_METHOD(TestType)
		{
			Datum d(Datum::DatumTypes::Integer);
			Assert::AreEqual(d.Type(), Datum::DatumTypes::Integer);
			Datum d1(Datum::DatumTypes::Float);
			Assert::AreEqual(d1.Type(), Datum::DatumTypes::Float);
			Datum d2(Datum::DatumTypes::String);
			Assert::AreEqual(d2.Type(), Datum::DatumTypes::String);
			Datum d3(Datum::DatumTypes::Vector);
			Assert::AreEqual(d3.Type(), Datum::DatumTypes::Vector);
			Datum d4(Datum::DatumTypes::Matrix);
			Assert::AreEqual(d4.Type(), Datum::DatumTypes::Matrix);
			Datum d5(Datum::DatumTypes::Pointer);
			Assert::AreEqual(d5.Type(), Datum::DatumTypes::Pointer);
			Datum d6;
			Assert::AreEqual(d6.Type(), Datum::DatumTypes::Unknown);
		}

		TEST_METHOD(TestSetType)
		{
			Datum d;
			Assert::AreEqual(d.Type(), Datum::DatumTypes::Unknown);
			d.SetType(Datum::DatumTypes::Float);
			Assert::AreEqual(d.Type(), Datum::DatumTypes::Float);

			Datum e(Datum::DatumTypes::Float);
			Assert::AreEqual(e.Type(), Datum::DatumTypes::Float);
			e.SetType(Datum::DatumTypes::Float);
			Assert::AreEqual(e.Type(), Datum::DatumTypes::Float);

			auto expressionA = [&e] { e.SetType(Datum::DatumTypes::Integer); };
			Assert::ExpectException<std::runtime_error>(expressionA);
		}

		TEST_METHOD(TestResize)
		{
			// Reducing String Size
			{
				using namespace std::string_literals;

				Datum bad;
				auto expressionA = [&bad] {bad.Resize(5_z); };
				Assert::ExpectException<std::runtime_error>(expressionA);

				Datum d(Datum::DatumTypes::String);
				d.PushBack("Hello"s);
				d.PushBack("Goodbye"s);
				d.PushBack("Music"s);
				Assert::AreEqual(3_z, d.Size());
				d.Resize(2_z);
				Assert::AreEqual(2_z, d.Size());
			}
			// Adding All Sizes
			{
				// Integer
				{
					Datum d(Datum::DatumTypes::Integer);
					d.PushBack(10);
					d.PushBack(20);
					d.PushBack(30);
					Assert::AreEqual(3_z, d.Size());
					d.Resize(4_z);
					Assert::AreEqual(4_z, d.Size());
				}
				// Float
				{
					Datum d(Datum::DatumTypes::Float);
					d.PushBack(10.0f);
					d.PushBack(20.0f);
					d.PushBack(30.0f);
					Assert::AreEqual(3_z, d.Size());
					d.Resize(4_z);
					Assert::AreEqual(4_z, d.Size());
				}
				// String
				{
					using namespace std::string_literals;

					Datum d(Datum::DatumTypes::String);
					d.PushBack("Hello"s);
					d.PushBack("Goodbye"s);
					d.PushBack("Music"s);
					Assert::AreEqual(3_z, d.Size());
					d.Resize(4_z);
					Assert::AreEqual(4_z, d.Size());
				}
				// Vector
				{
					Datum d(Datum::DatumTypes::Vector);
					d.PushBack(glm::vec4(0));
					d.PushBack(glm::vec4(1));
					d.PushBack(glm::vec4(2));
					Assert::AreEqual(3_z, d.Size());
					d.Resize(4_z);
					Assert::AreEqual(4_z, d.Size());
				}
				// Matrix
				{
					Datum d(Datum::DatumTypes::Matrix);
					d.PushBack(glm::mat4(0));
					d.PushBack(glm::mat4(1));
					d.PushBack(glm::mat4(2));
					Assert::AreEqual(3_z, d.Size());
					d.Resize(4_z);
					Assert::AreEqual(4_z, d.Size());
				}
				// Pointer
				{
					Datum d(Datum::DatumTypes::Pointer);
					Foo f;
					RTTI* empty = &f;
					Assert::IsTrue(empty->Is(Foo::TypeIdClass()));

					d.PushBack(empty);
					Assert::AreEqual(1_z, d.Size());
					d.Resize(2_z);
					Assert::AreEqual(2_z, d.Size());
				}
			}
		}

		TEST_METHOD(TestDatumEquality)
		{
			// Integer
			{
				Datum d(Datum::DatumTypes::Integer);
				d.PushBack(10);
				d.PushBack(20);
				d.PushBack(30);
				Datum e(Datum::DatumTypes::Integer);
				e.PushBack(10);
				e.PushBack(20);
				e.PushBack(30);
				Assert::IsTrue(d == e);
				e.PushBack(40);
				Assert::IsTrue(d != e);

				// Scalar
				Datum f(Datum::DatumTypes::Integer);
				f.PushBack(10);
				Assert::IsTrue(f == 10);
				f.PushBack(20);
				Assert::IsFalse(f != 10);
			}
			// Float
			{
				Datum d(Datum::DatumTypes::Float);
				d.PushBack(10.0f);
				d.PushBack(20.0f);
				d.PushBack(30.0f);
				Datum e(Datum::DatumTypes::Float);
				e.PushBack(10.0f);
				e.PushBack(20.0f);
				e.PushBack(30.0f);
				Assert::IsTrue(d == e);
				e.PushBack(40.0f);
				Assert::IsTrue(d != e);

				// Scalar
				Datum f(Datum::DatumTypes::Float);
				f.PushBack(10.0f);
				Assert::IsTrue(f == 10.0f);
				f.PushBack(20.0f);
				Assert::IsFalse(f != 10.0f);
			}
			// String
			{
				using namespace std::string_literals;

				Datum d(Datum::DatumTypes::String);
				d.PushBack("Hello"s);
				d.PushBack("Goodbye"s);
				d.PushBack("Music"s);
				Datum e(Datum::DatumTypes::String);
				e.PushBack("Hello"s);
				e.PushBack("Goodbye"s);
				e.PushBack("Music"s);
				Assert::IsTrue(d == e);
				e.PushBack("ExtraString"s);
				Assert::IsTrue(d != e);

				e.PopBack();
				Assert::IsTrue(d == e);
				e.PopBack();
				Assert::IsTrue(d != e);
				e.PushBack("WrongMusicString"s);
				Assert::IsTrue(d != e);

				// Scalar
				using namespace std::string_literals;

				Datum f(Datum::DatumTypes::String);
				f.PushBack("Hello"s);
				Assert::IsTrue(f == "Hello"s);
				f.PushBack("Goodbye"s);
				Assert::IsFalse(f != "Hello"s);
			}
			// Vector
			{
				Datum d(Datum::DatumTypes::Vector);
				d.PushBack(glm::vec4(10));
				d.PushBack(glm::vec4(20));
				d.PushBack(glm::vec4(30));
				Datum e(Datum::DatumTypes::Vector);
				e.PushBack(glm::vec4(10));
				e.PushBack(glm::vec4(20));
				e.PushBack(glm::vec4(30));
				Assert::IsTrue(d == e);
				e.PushBack(glm::vec4(40));
				Assert::IsTrue(d != e);

				// Scalar
				Datum f(Datum::DatumTypes::Vector);
				f.PushBack(glm::vec4(10));
				Assert::IsTrue(f == glm::vec4(10));
				f.PushBack(glm::vec4(20));
				Assert::IsFalse(f != glm::vec4(10));
			}
			// Matrix
			{
				Datum d(Datum::DatumTypes::Matrix);
				d.PushBack(glm::mat4(10));
				d.PushBack(glm::mat4(20));
				d.PushBack(glm::mat4(30));
				Datum e(Datum::DatumTypes::Matrix);
				e.PushBack(glm::mat4(10));
				e.PushBack(glm::mat4(20));
				e.PushBack(glm::mat4(30));
				Assert::IsTrue(d == e);
				e.PushBack(glm::mat4(40));
				Assert::IsTrue(d != e);

				// Scalar
				Datum f(Datum::DatumTypes::Matrix);
				f.PushBack(glm::mat4(10));
				Assert::IsTrue(f == glm::mat4(10));
				f.PushBack(glm::mat4(20));
				Assert::IsFalse(f != glm::mat4(10));
			}
			// Pointer
			{
				Foo f(5);
				RTTI* f_five = &f;
				Foo g(10);
				RTTI* g_ten = &g;

				Datum d(Datum::DatumTypes::Pointer);
				d.PushBack(f_five);
				Datum e(Datum::DatumTypes::Pointer);
				e.PushBack(f_five);
				Assert::IsTrue(d == e);
				e.PushBack(g_ten);
				Assert::IsTrue(d != e);
				
				e.PopBack();
				Assert::IsTrue(d == e);
				e.PopBack();
				Assert::IsTrue(d != e);
				e.PushBack(g_ten);
				Assert::IsTrue(d != e);

				// Scalar
				Datum scalar_d(Datum::DatumTypes::Pointer);
				scalar_d.PushBack(f_five);
				Assert::IsTrue(scalar_d == f_five);
				scalar_d.PushBack(g_ten);
				Assert::IsFalse(scalar_d != f_five);
			}
			// Unknown
			{
				Datum f;
				Datum g;
				Assert::IsTrue(f == g);
				Datum h(Datum::DatumTypes::Integer);
				Assert::IsTrue(f != h);
				Assert::IsTrue(g != h);
			}
		}

		TEST_METHOD(TestScopeBracketOperator)
		{
			using namespace std::string_literals;

			std::string a = "A"s;

			Scope scope;
			Scope& a_scope = scope.AppendScope(a);
			scope.AppendScope(a);
			scope.AppendScope(a);
			scope.AppendScope(a);
			scope.AppendScope(a);
			Datum d = *scope.Find(a);
			Assert::AreEqual(d[0], a_scope);

			const Datum const_d = *scope.Find(a);
			Assert::AreEqual(const_d[0], a_scope);
		}

		TEST_METHOD(TestReserve)
		{
			Datum d;
			auto expressionA = [&d] {d.Reserve(10_z); };
			Assert::ExpectException<std::runtime_error>(expressionA);
		}

		TEST_METHOD(TestSet)
		{
			// Integer
			{
				Datum d(Datum::DatumTypes::Integer);
				d.PushBack(10);
				d.PushBack(20);
				d.PushBack(30);
				Assert::AreEqual(3_z, d.Size());
				d.Set(5);
				d.Set(1, 2);
				Assert::AreEqual(5, d.Front<int>());
				Assert::AreEqual(1, d.Back<int>());

				auto expressionA = [&d] { d.Set(2, 10); };
				Assert::ExpectException<std::runtime_error>(expressionA);

				auto expressionB = [&d] { d.Set(2.0f, 10); };
				Assert::ExpectException<std::runtime_error>(expressionB);
			}
			// Float
			{
				Datum d(Datum::DatumTypes::Float);
				d.PushBack(10.0f);
				d.PushBack(20.0f);
				d.PushBack(30.0f);
				Assert::AreEqual(3_z, d.Size());
				d.Set(5.0f);
				d.Set(1.0f, 2);
				Assert::AreEqual(5.0f, d.Front<float>());
				Assert::AreEqual(1.0f, d.Back<float>());

				auto expressionA = [&d] { d.Set(2.0f, 10); };
				Assert::ExpectException<std::runtime_error>(expressionA);

				auto expressionB = [&d] { d.Set(2, 10); };
				Assert::ExpectException<std::runtime_error>(expressionB);
			}
			// String
			{
				using namespace std::string_literals;

				Datum d(Datum::DatumTypes::String);
				std::string hello = "Hello"s;
				d.PushBack(hello);
				d.PushBack("Goodbye"s);
				d.PushBack("Music"s);
				d.Set("HelloChanged"s);
				d.Set("MusicChanged"s, 2);
				Assert::AreEqual("HelloChanged"s, d.Front<std::string>());
				Assert::AreEqual("MusicChanged"s, d.Back<std::string>());

				auto expressionA = [&d] { d.Set("BadIndex"s, 10); };
				Assert::ExpectException<std::runtime_error>(expressionA);

				auto expressionB = [&d] { d.Set(2, 10); };
				Assert::ExpectException<std::runtime_error>(expressionB);
			}
			// Vector
			{
				Datum d(Datum::DatumTypes::Vector);
				d.PushBack(glm::vec4(0));
				d.PushBack(glm::vec4(1));
				d.PushBack(glm::vec4(2));
				d.Set(glm::vec4(30));
				d.Set(glm::vec4(40), 2);
				Assert::AreEqual(glm::vec4(30), d.Front<glm::vec4>());
				Assert::AreEqual(glm::vec4(40), d.Back<glm::vec4>());

				auto expressionA = [&d] { d.Set(glm::vec4(1000), 10); };
				Assert::ExpectException<std::runtime_error>(expressionA);

				auto expressionB = [&d] { d.Set(2, 10); };
				Assert::ExpectException<std::runtime_error>(expressionB);
			}
			// Matrix
			{
				Datum d(Datum::DatumTypes::Matrix);
				d.PushBack(glm::mat4(0));
				d.PushBack(glm::mat4(1));
				d.PushBack(glm::mat4(2));
				d.Set(glm::mat4(30));
				d.Set(glm::mat4(40), 2);
				Assert::AreEqual(glm::mat4(30), d.Front<glm::mat4>());
				Assert::AreEqual(glm::mat4(40), d.Back<glm::mat4>());

				auto expressionA = [&d] { d.Set(glm::mat4(1000), 10); };
				Assert::ExpectException<std::runtime_error>(expressionA);

				auto expressionB = [&d] { d.Set(2, 10); };
				Assert::ExpectException<std::runtime_error>(expressionB);
			}
			// Pointer
			{
				Datum d(Datum::DatumTypes::Pointer);
				Foo f;
				RTTI* empty_f = &f;
				Foo g;
				RTTI* empty_g = &g;
				Foo h;
				RTTI* empty_h = &h;
				
				Assert::IsTrue(empty_f->Is(Foo::TypeIdClass()));
				Assert::IsTrue(empty_g->Is(Foo::TypeIdClass()));
				Assert::IsTrue(empty_h->Is(Foo::TypeIdClass()));
				d.PushBack(empty_f);
				d.PushBack(empty_g);
				d.PushBack(empty_h);

				Foo i;
				RTTI* empty_i = &i;
				Foo j;
				RTTI* empty_j = &j;

				d.Set(empty_i);
				d.Set(empty_j, 2);

				Assert::IsTrue(empty_i == d.Front<RTTI*>());
				Assert::IsTrue(empty_j == d.Back<RTTI*>());

				Foo bad;
				RTTI* empty_bad = &bad;
				auto expressionA = [&d, &empty_bad] { d.Set(empty_bad, 10); };
				Assert::ExpectException<std::runtime_error>(expressionA);

				auto expressionB = [&d] { d.Set(2, 10); };
				Assert::ExpectException<std::runtime_error>(expressionB);
			}
		}

		TEST_METHOD(TestGet)
		{
			// Integer
			{
				Datum d(Datum::DatumTypes::Integer);
				d.PushBack(10);
				auto expressionA = [&d] { d.Get<int>(10); };
				Assert::ExpectException<std::runtime_error>(expressionA);

				Datum e(Datum::DatumTypes::Unknown);
				auto expressionB = [&e] { e.Get<int>(10); };
				Assert::ExpectException<std::runtime_error>(expressionB);

				const Datum f = d;
				Assert::AreEqual(f.Get<int>(0), d.Front<int>());
				auto expressionC = [&f] { f.Get<int>(10); };
				Assert::ExpectException<std::runtime_error>(expressionC);

				const Datum g;
				auto expressionD = [&g] { g.Get<int>(10); };
				Assert::ExpectException<std::runtime_error>(expressionD);

				Datum h(Datum::DatumTypes::Integer);
				int arr[4] = { 5, 10, 15, 20 };
				h.SetStorage<int>(arr, 4);

				auto expressionE = [&h] { h.PushBack(30); };
				Assert::ExpectException<std::runtime_error>(expressionE);
			}
			// Float
			{
				Datum d(Datum::DatumTypes::Float);
				d.PushBack(10.0f);
				auto expressionA = [&d] { d.Get<float>(10); };
				Assert::ExpectException<std::runtime_error>(expressionA);

				Datum e(Datum::DatumTypes::Unknown);
				auto expressionB = [&e] { e.Get<float>(10); };
				Assert::ExpectException<std::runtime_error>(expressionB);

				const Datum f = d;
				Assert::AreEqual(f.Get<float>(0), d.Front<float>());
				auto expressionC = [&f] { f.Get<float>(10); };
				Assert::ExpectException<std::runtime_error>(expressionC);

				const Datum g;
				auto expressionD = [&g] { g.Get<float>(10); };
				Assert::ExpectException<std::runtime_error>(expressionD);
			}
			// String
			{
				using namespace std::string_literals;

				Datum d(Datum::DatumTypes::String);
				d.PushBack("Hello"s);
				auto expressionA = [&d] { d.Get<std::string>(10); };
				Assert::ExpectException<std::runtime_error>(expressionA);

				Datum e(Datum::DatumTypes::Unknown);
				auto expressionB = [&e] { e.Get<std::string>(10); };
				Assert::ExpectException<std::runtime_error>(expressionB);

				const Datum f = d;
				Assert::AreEqual(f.Get<std::string>(0), d.Front<std::string>());
				auto expressionC = [&f] { f.Get<std::string>(10); };
				Assert::ExpectException<std::runtime_error>(expressionC);

				const Datum g;
				auto expressionD = [&g] { g.Get<std::string>(10); };
				Assert::ExpectException<std::runtime_error>(expressionD);
			}
			// Vector
			{
				Datum d(Datum::DatumTypes::Vector);
				d.PushBack(glm::vec4(10));
				auto expressionA = [&d] { d.Get<glm::vec4>(10); };
				Assert::ExpectException<std::runtime_error>(expressionA);

				Datum e(Datum::DatumTypes::Unknown);
				auto expressionB = [&e] { e.Get<glm::vec4>(10); };
				Assert::ExpectException<std::runtime_error>(expressionB);

				const Datum f = d;
				Assert::AreEqual(f.Get<glm::vec4>(0), d.Front<glm::vec4>());
				auto expressionC = [&f] { f.Get<glm::vec4>(10); };
				Assert::ExpectException<std::runtime_error>(expressionC);

				const Datum g;
				auto expressionD = [&g] { g.Get<glm::vec4>(10); };
				Assert::ExpectException<std::runtime_error>(expressionD);
			}
			// Matrix
			{
				Datum d(Datum::DatumTypes::Matrix);
				d.PushBack(glm::mat4(10));
				auto expressionA = [&d] { d.Get<glm::mat4>(10); };
				Assert::ExpectException<std::runtime_error>(expressionA);

				Datum e(Datum::DatumTypes::Unknown);
				auto expressionB = [&e] { e.Get<glm::mat4>(10); };
				Assert::ExpectException<std::runtime_error>(expressionB);

				const Datum f = d;
				Assert::AreEqual(f.Get<glm::mat4>(0), d.Front<glm::mat4>());
				auto expressionC = [&f] { f.Get<glm::mat4>(10); };
				Assert::ExpectException<std::runtime_error>(expressionC);

				const Datum g;
				auto expressionD = [&g] { g.Get<glm::mat4>(10); };
				Assert::ExpectException<std::runtime_error>(expressionD);
			}
			// Table
			{
				using namespace std::string_literals;

				Scope scope;
				scope.AppendScope("A"s);
				Datum* d = scope.Find("A"s);

				auto expressionA = [&d] { d->Get<Scope>(10); };
				Assert::ExpectException<std::runtime_error>(expressionA);

				Datum e(Datum::DatumTypes::Unknown);
				auto expressionB = [&e] { e.Get<Scope>(10); };
				Assert::ExpectException<std::runtime_error>(expressionB);

				const Datum* f = d;
				Assert::AreEqual(f->Get<Scope>(0), d->Front<Scope>());
				auto expressionC = [&f] { f->Get<Scope>(10); };
				Assert::ExpectException<std::runtime_error>(expressionC);

				const Datum g;
				auto expressionD = [&g] { g.Get<Scope>(10); };
				Assert::ExpectException<std::runtime_error>(expressionD);
			}
			// Pointer
			{
				Datum d(Datum::DatumTypes::Pointer);
				Foo foo;
				RTTI* empty_f = &foo;

				d.PushBack(empty_f);
				auto expressionA = [&d] { d.Get<RTTI*>(10); };
				Assert::ExpectException<std::runtime_error>(expressionA);

				Datum e(Datum::DatumTypes::Unknown);
				auto expressionB = [&e] { e.Get<RTTI*>(10); };
				Assert::ExpectException<std::runtime_error>(expressionB);

				const Datum f = d;
				Assert::IsTrue(f.Get<RTTI*>(0) == empty_f);
				auto expressionC = [&f] { f.Get<RTTI*>(10); };
				Assert::ExpectException<std::runtime_error>(expressionC);

				const Datum g;
				auto expressionD = [&g] { g.Get<RTTI*>(10); };
				Assert::ExpectException<std::runtime_error>(expressionD);
			}
		}

		TEST_METHOD(TestStringConverters)
		{
			// Integer
			{
				Datum d(Datum::DatumTypes::Integer);
				d.PushBack(50);
				Assert::AreEqual(d.Front<int>(), 50);
				std::string fifty = d.ToString(0);
				d.SetFromString(fifty, 0);
				Assert::AreEqual(d.Front<int>(), 50);
			}
			// Float
			{
				Datum d(Datum::DatumTypes::Float);
				d.PushBack(50.0f);
				Assert::AreEqual(d.Front<float>(), 50.0f);
				std::string fifty = d.ToString(0);
				d.SetFromString(fifty, 0);
				Assert::AreEqual(d.Front<float>(), 50.0f);
			}
			// String
			{
				using namespace std::string_literals;

				Datum d(Datum::DatumTypes::String);
				d.PushBack("Hello"s);
				Assert::AreEqual(d.Front<std::string>(), "Hello"s);
				std::string hello = d.ToString(0);
				d.SetFromString(hello, 0);
				Assert::AreEqual(d.Front<std::string>(), "Hello"s);
			}
			// Vector
			{
				Datum d(Datum::DatumTypes::Vector);
				d.PushBack(glm::vec4(10));
				Assert::AreEqual(d.Front<glm::vec4>(), glm::vec4(10));
				std::string vector_ten = d.ToString(0);
				d.SetFromString(vector_ten, 0);
				Assert::AreEqual(d.Front<glm::vec4>(), glm::vec4(10));
			}
			// Matrix
			{
				Datum d(Datum::DatumTypes::Matrix);
				d.PushBack(glm::mat4(10));
				Assert::AreEqual(d.Front<glm::mat4>(), glm::mat4(10));
				std::string matrix_ten = d.ToString(0);
				d.SetFromString(matrix_ten, 0);
				Assert::AreEqual(d.Front<glm::mat4>(), glm::mat4(10));
			}
			// Pointer
			{
				using namespace std::string_literals;

				Foo f(10);
				RTTI* f_ten = &f;

				Datum d(Datum::DatumTypes::Pointer);
				d.PushBack(f_ten);
				Assert::IsTrue(d.Front<RTTI*>() == f_ten);
				Assert::AreEqual(d.Front<RTTI*>(), f_ten);
				std::string rtti = d.ToString(0);
				Assert::AreEqual("10"s, rtti);

				RTTI* f_null = nullptr;
				d.PushBack(f_null);
				std::string null_str = d.ToString(1);
				Assert::AreEqual("nullptr"s, null_str);

				auto expressionA = [&d, &rtti] { d.SetFromString(rtti, 0); };
				Assert::ExpectException<std::runtime_error>(expressionA);

				auto expressionB = [&d, &rtti] { d.PushBackFromString(rtti); };
				Assert::ExpectException<std::runtime_error>(expressionB);
			}
			// Unknown
			{
				Datum d;
				auto expressionA = [&d] { d.ToString(0); };
				Assert::ExpectException<std::runtime_error>(expressionA);
			}
		}

		TEST_METHOD(TestPushBack)
		{
			using namespace std::string_literals;

			// Catch all the runtime errors
			Datum d(Datum::DatumTypes::Unknown);
			auto expressionA = [&d] {d.PushBack(10); };
			Assert::ExpectException<std::runtime_error>(expressionA);
			auto expressionB = [&d] {d.PushBack(10.0f); };
			Assert::ExpectException<std::runtime_error>(expressionB);

			std::string bad = "BadString";
			auto expressionC = [&d, &bad] {d.PushBack(bad); };
			Assert::ExpectException<std::runtime_error>(expressionC);
			auto expressionD = [&d] {d.PushBack(glm::vec4(10)); };
			Assert::ExpectException<std::runtime_error>(expressionD);
			auto expressionE = [&d] {d.PushBack(glm::mat4(10)); };
			Assert::ExpectException<std::runtime_error>(expressionE);

			Foo f;
			RTTI* f_empty = &f;
			auto expressionF = [&d, &f_empty] {d.PushBack(f_empty); };
			Assert::ExpectException<std::runtime_error>(expressionF);
			auto expressionG = [&d, &bad] {d.PushBack("BadString"s); };
			Assert::ExpectException<std::runtime_error>(expressionG);
		}

		TEST_METHOD(TestPopBack)
		{
			// Integer
			{
				Datum d(Datum::DatumTypes::Integer);
				d.PushBack(10);
				d.PushBack(20);
				d.PushBack(30);
				Assert::AreEqual(3_z, d.Size());
				d.PopBack();
				Assert::AreEqual(2_z, d.Size());
				d.PopBack();
				Assert::AreEqual(1_z, d.Size());
				d.PopBack();
				Assert::AreEqual(0_z, d.Size());
				Assert::AreEqual(3_z, d.Capacity());
			}
			// String
			{
				using namespace std::string_literals;

				Datum d(Datum::DatumTypes::String);
				d.PushBack("Hello"s);
				d.PushBack("Goodbye"s);
				d.PushBack("Music"s);
				Assert::AreEqual(3_z, d.Size());
				d.PopBack();
				Assert::AreEqual(2_z, d.Size());
				d.PopBack();
				Assert::AreEqual(1_z, d.Size());
				d.PopBack();
				Assert::AreEqual(0_z, d.Size());
				Assert::AreEqual(3_z, d.Capacity());
			}
		}

		TEST_METHOD(TestBack)
		{
			// Integer
			{
				Datum d(Datum::DatumTypes::Integer);
				d.PushBack(10);
				d.PushBack(20);
				d.PushBack(30);
				Assert::AreEqual(d.Back<int>(), 30);
				d.PopBack();

				const Datum e(d);
				Assert::AreEqual(e.Back<int>(), 20);
			}
			// Float
			{
				Datum d(Datum::DatumTypes::Float);
				d.PushBack(10.0f);
				d.PushBack(20.0f);
				d.PushBack(30.0f);
				Assert::AreEqual(d.Back<float>(), 30.0f);
				d.PopBack();

				const Datum e(d);
				Assert::AreEqual(e.Back<float>(), 20.0f);
			}
			// String
			{
				using namespace std::string_literals;

				Datum d(Datum::DatumTypes::String);
				d.PushBack("Hello"s);
				d.PushBack("Goodbye"s);
				d.PushBack("Music"s);
				Assert::AreEqual(d.Back<std::string>(), "Music"s);
				d.PopBack();

				const Datum e(d);
				Assert::AreEqual(e.Back<std::string>(), "Goodbye"s);
			}
			// Vector
			{
				Datum d(Datum::DatumTypes::Vector);
				d.PushBack(glm::vec4(10));
				d.PushBack(glm::vec4(20));
				d.PushBack(glm::vec4(30));
				Assert::AreEqual(d.Back<glm::vec4>(), glm::vec4(30));
				d.PopBack();

				const Datum e(d);
				Assert::AreEqual(e.Back<glm::vec4>(), glm::vec4(20));
			}
			// Matrix
			{
				Datum d(Datum::DatumTypes::Matrix);
				d.PushBack(glm::mat4(10));
				d.PushBack(glm::mat4(20));
				d.PushBack(glm::mat4(30));
				Assert::AreEqual(d.Back<glm::mat4>(), glm::mat4(30));
				d.PopBack();

				const Datum e(d);
				Assert::AreEqual(e.Back<glm::mat4>(), glm::mat4(20));
			}
			// Table
			{
				using namespace std::string_literals;

				std::string a = "A"s;
				std::string b = "B"s; 
				std::string c = "C"s;

				Scope scope;
				Scope& scope_1 = scope.AppendScope(a);
				scope.AppendScope(a);
				scope.AppendScope(a);
				scope.AppendScope(a);
				Scope& scope_5 = scope.AppendScope(a);
				Datum d = *scope.Find(a);

				Assert::AreSame(d.Back<Scope>(), scope_5);

				const Scope const_scope(scope);
				const Scope& const_scope_1 = scope_1;
				const Scope& const_scope_5 = scope_5;
				const Datum d_const = *const_scope.Find(a);
				Assert::AreEqual(d_const.Front<Scope>(), const_scope_1);
				Assert::AreEqual(d_const.Back<Scope>(), const_scope_5);
			}
			// Pointer
			{
				Datum d(Datum::DatumTypes::Pointer);
				Foo f(5);
				RTTI* f_five = &f;
				Foo g(10);
				RTTI* g_ten = &g;

				d.PushBack(f_five);
				d.PushBack(g_ten);
				Assert::AreEqual(d.Back<RTTI*>(), g_ten);
				d.PopBack();

				const Datum e(d);
				Assert::AreEqual(e.Back<RTTI*>(), f_five);
			}
		}

		TEST_METHOD(TestSetStorage)
		{
			// Integer
			{
				Datum d(Datum::DatumTypes::Integer);
				int arr[4] = { 5, 10, 15, 20 };
				d.SetStorage(arr, 4);
				auto expressionA = [&d] { d.Reserve(5); };
				Assert::ExpectException<std::runtime_error>(expressionA);
				auto expressionB = [&d] { d.Resize(5); };
				Assert::ExpectException<std::runtime_error>(expressionB);
				auto expressionC = [&d] { d.PopBack(); };
				Assert::ExpectException<std::runtime_error>(expressionC);
				auto expressionD = [&d] { d.RemoveAt(0); };
				Assert::ExpectException<std::runtime_error>(expressionD);
				auto expressionE = [&d] { d.Clear(); };
				Assert::ExpectException<std::runtime_error>(expressionE);

				Datum e(Datum::DatumTypes::Integer);
				int empty[5];
				e.SetStorage(empty, 0);
				auto expressionH= [&e] { e.Remove<int>(0); };
				Assert::ExpectException<std::runtime_error>(expressionH);

				Datum f;
				auto expressionI = [&f, &arr] { f.SetStorage(arr, 4); };
				Assert::ExpectException<std::runtime_error>(expressionI);
			}
			// Float
			{
				Datum d(Datum::DatumTypes::Float);
				float arr[4] = { 5, 10, 15, 20 };
				d.SetStorage(arr, 4);
				auto expressionA = [&d] { d.Reserve(5); };
				Assert::ExpectException<std::runtime_error>(expressionA);
				auto expressionB = [&d] { d.Resize(5); };
				Assert::ExpectException<std::runtime_error>(expressionB);
				auto expressionC = [&d] { d.PopBack(); };
				Assert::ExpectException<std::runtime_error>(expressionC);
				auto expressionD = [&d] { d.RemoveAt(0); };
				Assert::ExpectException<std::runtime_error>(expressionD);
				auto expressionE = [&d] { d.Clear(); };
				Assert::ExpectException<std::runtime_error>(expressionE);

				Datum e(Datum::DatumTypes::Float);
				float empty[5];
				e.SetStorage(empty, 0);
				auto expressionH = [&e] { e.Remove<float>(0.0f); };
				Assert::ExpectException<std::runtime_error>(expressionH);

				Datum f;
				auto expressionI = [&f, &arr] { f.SetStorage(arr, 4); };
				Assert::ExpectException<std::runtime_error>(expressionI);
			}
			// String
			{
				Datum d(Datum::DatumTypes::String);
				std::string arr[4] = { "Hello", "Goodbye", "Music", "Gaming" };
				d.SetStorage(arr, 4);
				auto expressionA = [&d] { d.Reserve(5); };
				Assert::ExpectException<std::runtime_error>(expressionA);
				auto expressionB = [&d] { d.Resize(5); };
				Assert::ExpectException<std::runtime_error>(expressionB);
				auto expressionC = [&d] { d.PopBack(); };
				Assert::ExpectException<std::runtime_error>(expressionC);
				auto expressionD = [&d] { d.RemoveAt(0); };
				Assert::ExpectException<std::runtime_error>(expressionD);
				auto expressionE = [&d] { d.Clear(); };
				Assert::ExpectException<std::runtime_error>(expressionE);

				Datum e(Datum::DatumTypes::String);
				std::string empty[5];
				e.SetStorage(empty, 0);
				auto expressionH = [&e] { e.Remove<std::string>(""); };
				Assert::ExpectException<std::runtime_error>(expressionH);

				Datum f;
				auto expressionI = [&f, &arr] { f.SetStorage(arr, 4); };
				Assert::ExpectException<std::runtime_error>(expressionI);
			}
			// Vector
			{
				Datum d(Datum::DatumTypes::Vector);
				glm::vec4 arr[4] = { glm::vec4(5), glm::vec4(10), glm::vec4(15), glm::vec4(20) };
				d.SetStorage(arr, 4);
				auto expressionA = [&d] { d.Reserve(5); };
				Assert::ExpectException<std::runtime_error>(expressionA);
				auto expressionB = [&d] { d.Resize(5); };
				Assert::ExpectException<std::runtime_error>(expressionB);
				auto expressionC = [&d] { d.PopBack(); };
				Assert::ExpectException<std::runtime_error>(expressionC);
				auto expressionD = [&d] { d.RemoveAt(0); };
				Assert::ExpectException<std::runtime_error>(expressionD);
				auto expressionE = [&d] { d.Clear(); };
				Assert::ExpectException<std::runtime_error>(expressionE);

				Datum e(Datum::DatumTypes::Vector);
				glm::vec4 empty[5];
				e.SetStorage(empty, 0);
				auto expressionH = [&e] { e.Remove<glm::vec4>(glm::vec4(0)); };
				Assert::ExpectException<std::runtime_error>(expressionH);

				Datum f;
				auto expressionI = [&f, &arr] { f.SetStorage(arr, 4); };
				Assert::ExpectException<std::runtime_error>(expressionI);
			}
			// Matrix
			{
				Datum d(Datum::DatumTypes::Matrix);
				glm::mat4 arr[4] = { glm::mat4(5), glm::mat4(10), glm::mat4(15), glm::mat4(20) };
				d.SetStorage(arr, 4);
				auto expressionA = [&d] { d.Reserve(5); };
				Assert::ExpectException<std::runtime_error>(expressionA);
				auto expressionB = [&d] { d.Resize(5); };
				Assert::ExpectException<std::runtime_error>(expressionB);
				auto expressionC = [&d] { d.PopBack(); };
				Assert::ExpectException<std::runtime_error>(expressionC);
				auto expressionD = [&d] { d.RemoveAt(0); };
				Assert::ExpectException<std::runtime_error>(expressionD);
				auto expressionE = [&d] { d.Clear(); };
				Assert::ExpectException<std::runtime_error>(expressionE);

				Datum e(Datum::DatumTypes::Matrix);
				glm::mat4 empty[5];
				e.SetStorage(empty, 0);
				auto expressionH = [&e] { e.Remove<glm::mat4>(glm::mat4(0)); };
				Assert::ExpectException<std::runtime_error>(expressionH);

				Datum f;
				auto expressionI = [&f, &arr] { f.SetStorage(arr, 4); };
				Assert::ExpectException<std::runtime_error>(expressionI);
			}
			// Pointer
			{
				Datum d(Datum::DatumTypes::Pointer);
				Foo foo[4] = { Foo(5), Foo(10), Foo(15), Foo(20) };
				RTTI* arr[4] = { &foo[0], &foo[1], &foo[2], &foo[3] };
				d.SetStorage(arr, 4);
				auto expressionA = [&d] { d.Reserve(5); };
				Assert::ExpectException<std::runtime_error>(expressionA);
				auto expressionB = [&d] { d.Resize(5); };
				Assert::ExpectException<std::runtime_error>(expressionB);
				auto expressionC = [&d] { d.PopBack(); };
				Assert::ExpectException<std::runtime_error>(expressionC);
				auto expressionD = [&d] { d.RemoveAt(0); };
				Assert::ExpectException<std::runtime_error>(expressionD);
				auto expressionE = [&d] { d.Clear(); };
				Assert::ExpectException<std::runtime_error>(expressionE);

				Datum e(Datum::DatumTypes::Pointer);
				RTTI* empty[5];
				e.SetStorage(empty, 0);
				auto expressionH = [&e, &arr] { e.Remove(arr[0]); };
				Assert::ExpectException<std::runtime_error>(expressionH);

				Datum f;
				auto expressionI = [&f, &arr] { f.SetStorage(arr, 4); };
				Assert::ExpectException<std::runtime_error>(expressionI);
			}
		}

		TEST_METHOD(TestRemove)
		{
			// Integer
			{
				Datum d(Datum::DatumTypes::Integer);
				d.PushBack(10);
				d.PushBack(30);
				d.PushBack(40);
				Assert::AreEqual(d.Size(), 3_z);
				d.Remove(20);
				Assert::AreEqual(d.Size(), 3_z);
				d.Remove(30);
				Assert::AreEqual(d.Size(), 2_z);
			}
			// Float
			{
				Datum d(Datum::DatumTypes::Float);
				d.PushBack(10.0f);
				d.PushBack(30.0f);
				d.PushBack(40.0f);
				Assert::AreEqual(d.Size(), 3_z);
				d.Remove(20.0f);
				Assert::AreEqual(d.Size(), 3_z);
				d.Remove(30.0f);
				Assert::AreEqual(d.Size(), 2_z);
			}
			// String
			{
				using namespace std::string_literals;

				Datum d(Datum::DatumTypes::String);
				d.PushBack("Hello"s);
				d.PushBack("Goodbye"s);
				d.PushBack("Music"s);
				Assert::AreEqual(d.Size(), 3_z);
				d.Remove("BadString"s);
				Assert::AreEqual(d.Size(), 3_z);
				d.Remove("Goodbye"s);
				Assert::AreEqual(d.Size(), 2_z);
			}
			// Vector
			{
				Datum d(Datum::DatumTypes::Vector);
				d.PushBack(glm::vec4(10));
				d.PushBack(glm::vec4(30));
				d.PushBack(glm::vec4(40));
				Assert::AreEqual(d.Size(), 3_z);
				d.Remove(glm::vec4(20));
				Assert::AreEqual(d.Size(), 3_z);
				d.Remove(glm::vec4(30));
				Assert::AreEqual(d.Size(), 2_z);
			}
			// Matrix
			{
				Datum d(Datum::DatumTypes::Matrix);
				d.PushBack(glm::mat4(10));
				d.PushBack(glm::mat4(30));
				d.PushBack(glm::mat4(40));
				Assert::AreEqual(d.Size(), 3_z);
				d.Remove(glm::mat4(20));
				Assert::AreEqual(d.Size(), 3_z);
				d.Remove(glm::mat4(30));
				Assert::AreEqual(d.Size(), 2_z);
			}
			// Pointer
			{
				Foo f;
				RTTI* f_empty = &f;
				Foo g;
				RTTI* g_empty = &g;
				Foo h;
				RTTI* h_empty = &h;
				Foo i;
				RTTI* i_empty = &i;

				Datum d(Datum::DatumTypes::Pointer);
				d.PushBack(f_empty);
				d.PushBack(g_empty);
				d.PushBack(h_empty);
				Assert::AreEqual(d.Size(), 3_z);
				d.Remove(i_empty);
				Assert::AreEqual(d.Size(), 3_z);
				d.Remove(g_empty);
				Assert::AreEqual(d.Size(), 2_z);
			}
		}

	private:
		static _CrtMemState sStartMemState; // Static members before C++17 could only be defined separate from declaration
	};

	_CrtMemState DatumTests::sStartMemState;
}
