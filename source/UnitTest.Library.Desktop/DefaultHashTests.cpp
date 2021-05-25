#include "pch.h"
#include <crtdbg.h>
#include <exception>
#include <CppUnitTest.h>
#include "SList.h"
#include "Vector.h"
#include "HashMap.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

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
	TEST_CLASS(DefaultHashTests)
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

		TEST_METHOD(BaseHash)
		{
			DefaultHash<float> hash_functor;

			float a = 10.f;
			float b = 20.f;
			float c = 10.f;

			Assert::AreEqual(hash_functor(a), hash_functor(a));
			Assert::AreEqual(hash_functor(b), hash_functor(b));
			Assert::AreEqual(hash_functor(c), hash_functor(c));
			Assert::AreNotEqual(hash_functor(a), hash_functor(b));
			Assert::AreNotEqual(hash_functor(b), hash_functor(c));
			Assert::AreEqual(hash_functor(a), hash_functor(c));
		}

		TEST_METHOD(IntegerHash)
		{
			DefaultHash<int> hash_functor;

			int a = 10;
			int b = 20;
			int c = 10;

			Assert::AreEqual(hash_functor(a), hash_functor(a));
			Assert::AreEqual(hash_functor(b), hash_functor(b));
			Assert::AreEqual(hash_functor(c), hash_functor(c));
			Assert::AreNotEqual(hash_functor(a), hash_functor(b));
			Assert::AreNotEqual(hash_functor(b), hash_functor(c));
			Assert::AreEqual(hash_functor(a), hash_functor(c));
		}

		TEST_METHOD(StringHash)
		{
			using namespace std::string_literals;

			std::string hello = "Hello"s;
			std::string goodbye = "Goodbye"s;
			std::string hello_copy(hello);
			DefaultHash<std::string> hash_functor;

			Assert::AreEqual(hash_functor(hello), hash_functor(hello));
			Assert::AreEqual(hash_functor(goodbye), hash_functor(goodbye));
			Assert::AreEqual(hash_functor(hello_copy), hash_functor(hello_copy));
			Assert::AreNotEqual(hash_functor(hello), hash_functor(goodbye));
			Assert::AreNotEqual(hash_functor(goodbye), hash_functor(hello_copy));
			Assert::AreEqual(hash_functor(hello), hash_functor(hello_copy));
		}

		TEST_METHOD(WideStringHash)
		{
			using namespace std::string_literals;

			std::wstring hello = L"Hello"s;
			std::wstring goodbye = L"Goodbye"s;
			std::wstring hello_copy(hello);
			DefaultHash<std::wstring> hash_functor;

			Assert::AreEqual(hash_functor(hello), hash_functor(hello));
			Assert::AreEqual(hash_functor(goodbye), hash_functor(goodbye));
			Assert::AreEqual(hash_functor(hello_copy), hash_functor(hello_copy));
			Assert::AreNotEqual(hash_functor(hello), hash_functor(goodbye));
			Assert::AreNotEqual(hash_functor(goodbye), hash_functor(hello_copy));
			Assert::AreEqual(hash_functor(hello), hash_functor(hello_copy));
		}

		TEST_METHOD(CStringHash)
		{
			// Char*
			{
				char hello[6] = "Hello";
				char goodbye[8] = "Goodbye";
				char hello_copy[6];
				strcpy_s(hello_copy, hello);
				DefaultHash<char*> hash_functor;

				Assert::AreEqual(hash_functor(hello), hash_functor(hello));
				Assert::AreEqual(hash_functor(goodbye), hash_functor(goodbye));
				Assert::AreEqual(hash_functor(hello_copy), hash_functor(hello_copy));
				Assert::AreNotEqual(hash_functor(hello), hash_functor(goodbye));
				Assert::AreNotEqual(hash_functor(goodbye), hash_functor(hello_copy));
				Assert::AreEqual(hash_functor(hello), hash_functor(hello_copy));
			}
			// Const Char*
			{
				const char* hello = "Hello";
				const char* goodbye = "Goodbye";
				char hello_copy[6];
				strcpy_s(hello_copy, hello);
				DefaultHash<const char*> hash_functor;

				Assert::AreEqual(hash_functor(hello), hash_functor(hello));
				Assert::AreEqual(hash_functor(goodbye), hash_functor(goodbye));
				Assert::AreEqual(hash_functor(hello_copy), hash_functor(hello_copy));
				Assert::AreNotEqual(hash_functor(hello), hash_functor(goodbye));
				Assert::AreNotEqual(hash_functor(goodbye), hash_functor(hello_copy));
				Assert::AreEqual(hash_functor(hello), hash_functor(hello_copy));
			}
			// Char* Const
			{
				char hello[6] = "Hello";
				char goodbye[8] = "Goodbye";
				char* const ptr_hello = hello;
				char* const ptr_goodbye = goodbye;
				char hello_copy[6];
				strcpy_s(hello_copy, hello);
				DefaultHash<char* const> hash_functor;

				Assert::AreEqual(hash_functor(ptr_hello), hash_functor(ptr_hello));
				Assert::AreEqual(hash_functor(ptr_goodbye), hash_functor(ptr_goodbye));
				Assert::AreEqual(hash_functor(hello_copy), hash_functor(hello_copy));
				Assert::AreNotEqual(hash_functor(ptr_hello), hash_functor(ptr_goodbye));
				Assert::AreNotEqual(hash_functor(ptr_goodbye), hash_functor(hello_copy));
				Assert::AreEqual(hash_functor(ptr_hello), hash_functor(hello_copy));
			}
			// Const Char* Const
			{
				const char* const hello = "Hello";
				const char* const goodbye = "Goodbye";
				char hello_copy[6];
				strcpy_s(hello_copy, hello);
				DefaultHash<const char* const> hash_functor;

				Assert::AreEqual(hash_functor(hello), hash_functor(hello));
				Assert::AreEqual(hash_functor(goodbye), hash_functor(goodbye));
				Assert::AreEqual(hash_functor(hello_copy), hash_functor(hello_copy));
				Assert::AreNotEqual(hash_functor(hello), hash_functor(goodbye));
				Assert::AreNotEqual(hash_functor(goodbye), hash_functor(hello_copy));
				Assert::AreEqual(hash_functor(hello), hash_functor(hello_copy));
			}
		}

		TEST_METHOD(FooHash)
		{
			DefaultHash<Foo> hash_functor;

			Foo a(10);
			Foo b(20);
			Foo c(10);

			Assert::AreEqual(hash_functor(a), hash_functor(a));
			Assert::AreEqual(hash_functor(b), hash_functor(b));
			Assert::AreEqual(hash_functor(c), hash_functor(c));
			Assert::AreNotEqual(hash_functor(a), hash_functor(b));
			Assert::AreNotEqual(hash_functor(b), hash_functor(c));
			Assert::AreEqual(hash_functor(a), hash_functor(c));
		}

	private:
		static _CrtMemState sStartMemState; // Static members before C++17 could only be defined separate from declaration
	};

	_CrtMemState DefaultHashTests::sStartMemState;
}
