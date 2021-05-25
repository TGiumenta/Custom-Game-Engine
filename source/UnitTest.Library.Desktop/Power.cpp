#include "pch.h"
#include "Power.h"

namespace UnitTests
{
	RTTI_DEFINITIONS(Power)

		using namespace FieaGameEngine;

	Power::Power() :
		Attributed(Power::TypeIdClass())
	{
	}

	const Vector<Signature> Power::Signatures()
	{
		return Vector<Signature>
		{
			{ "ExternalInteger", Datum::DatumTypes::Integer, 1, offsetof(Power, ExternalInteger)},
			{ "ExternalFloat", Datum::DatumTypes::Float, 1, offsetof(Power, ExternalFloat) },
			{ "ExternalString", Datum::DatumTypes::String, 1, offsetof(Power, ExternalString) },
			{ "ExternalVector", Datum::DatumTypes::Vector, 1, offsetof(Power, ExternalVector) },
			{ "ExternalMatrix", Datum::DatumTypes::Matrix, 1, offsetof(Power, ExternalMatrix) },
			{ "ExternalIntegerArray", Datum::DatumTypes::Integer, ArraySize, offsetof(Power, ExternalIntegerArray) },
			{ "ExternalFloatArray", Datum::DatumTypes::Float, ArraySize, offsetof(Power, ExternalFloatArray) },
			{ "ExternalStringArray", Datum::DatumTypes::String, ArraySize, offsetof(Power, ExternalStringArray) },
			{ "ExternalVectorArray", Datum::DatumTypes::Vector, ArraySize, offsetof(Power, ExternalVectorArray) },
			{ "ExternalMatrixArray", Datum::DatumTypes::Matrix, ArraySize, offsetof(Power, ExternalMatrixArray) },
			{ "NestedScope", Datum::DatumTypes::Table, 0, 0 },
			{ "NestedScopeArray", Datum::DatumTypes::Table, 5, 0 },
		};
	}

	gsl::owner<Scope*> Power::Clone() const
	{
		return new Power(*this);
	}

	std::string Power::ToString() const
	{
		using namespace std::string_literals;
		return "Powers"s;
	}
}