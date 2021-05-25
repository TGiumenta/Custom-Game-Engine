#pragma once

#include "TypeManager.h"
#include "Attributed.h"

namespace UnitTests
{
	class AttributedFoo : public FieaGameEngine::Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, FieaGameEngine::Attributed)

	public:
		static const size_t size = 5;

		AttributedFoo();
		AttributedFoo(const AttributedFoo&) = default;
		AttributedFoo(AttributedFoo&&) = default;
		AttributedFoo& operator=(const AttributedFoo&) = default;
		AttributedFoo& operator=(AttributedFoo&&) = default;
		~AttributedFoo() = default;

		int ExternalInteger = 0;
		float ExternalFloat = 0;
		std::string ExternalString;
		glm::vec4 ExternalVector;
		glm::mat4 ExternalMatrix;

		int ExternalIntegerArray[size];
		float ExternalFloatArray[size];
		std::string ExternalStringArray[size];
		glm::vec4 ExternalVectorArray[size];
		glm::mat4 ExternalMatrixArray[size];

		gsl::owner<Scope*> Clone() const override;
		std::string ToString() const override;

		static const FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
	};
}