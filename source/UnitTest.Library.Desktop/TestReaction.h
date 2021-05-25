#pragma once

#include "Reaction.h"
#include "Factory.h"
#include "Vector.h"
#include "TypeManager.h"

namespace UnitTests
{
	class TestReaction final : public FieaGameEngine::Reaction
	{
		RTTI_DECLARATIONS(TestReaction, Reaction)

	public:
		TestReaction();
		TestReaction(const TestReaction& other) = default;
		TestReaction(TestReaction&& other) noexcept = default;
		TestReaction& operator=(const TestReaction& other) = default;
		TestReaction& operator=(TestReaction&& other) noexcept = default;
		~TestReaction() = default;

		static const FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

		void Notify(const FieaGameEngine::EventPublisher& event) override;
		gsl::owner<TestReaction*> Clone() const override;

		int32_t Count() const;
	private:
		int32_t count;
	};

	ConcreteFactory(TestReaction, FieaGameEngine::Scope)
}

