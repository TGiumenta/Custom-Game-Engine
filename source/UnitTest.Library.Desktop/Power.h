#pragma once

#include "TypeManager.h"
#include "Attributed.h"
#include "Factory.h"

namespace UnitTests
{
	class Power : public FieaGameEngine::Attributed
	{
		RTTI_DECLARATIONS(Power, FieaGameEngine::Attributed)

	public:
		static const size_t ArraySize = 5;

		Power();
		Power(const Power&) = default;
		Power(Power&&) = default;
		Power& operator=(const Power&) = default;
		Power& operator=(Power&&) = default;
		~Power() = default;

		int ExternalInteger = 0;
		float ExternalFloat = 0;
		std::string ExternalString;
		glm::vec4 ExternalVector;
		glm::mat4 ExternalMatrix;

		int ExternalIntegerArray[ArraySize];
		float ExternalFloatArray[ArraySize];
		std::string ExternalStringArray[ArraySize];
		glm::vec4 ExternalVectorArray[ArraySize];
		glm::mat4 ExternalMatrixArray[ArraySize];

		gsl::owner<Scope*> Clone() const override;
		std::string ToString() const override;

		static const FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
	};

	ConcreteFactory(Power, FieaGameEngine::Scope)
}