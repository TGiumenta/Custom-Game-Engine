#include "pch.h"
#include "TestReaction.h"

namespace UnitTests
{
	RTTI_DEFINITIONS(TestReaction)

	TestReaction::TestReaction() :
		Reaction(TestReaction::TypeIdClass())
	{
	}

	const FieaGameEngine::Vector<FieaGameEngine::Signature> TestReaction::Signatures()
	{
		return FieaGameEngine::Vector<FieaGameEngine::Signature>
		{
			{ "Count", FieaGameEngine::Datum::DatumTypes::Integer, 1, offsetof(TestReaction, count)}
		};
	}

	void TestReaction::Notify(const FieaGameEngine::EventPublisher& /*event*/)
	{
		++count;
	}

	gsl::owner<TestReaction*> TestReaction::Clone() const
	{
		return new TestReaction(*this);
	}

	int32_t TestReaction::Count() const
	{
		return count;
	}
}