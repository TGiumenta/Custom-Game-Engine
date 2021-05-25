#include "pch.h"
#include "AttributedFoo.h"

namespace UnitTests
{
	RTTI_DEFINITIONS(AttributedFoo)

	using namespace FieaGameEngine;

	AttributedFoo::AttributedFoo() :
		Attributed(AttributedFoo::TypeIdClass())
	{
	}

	const Vector<Signature> AttributedFoo::Signatures()
	{
		return Vector<Signature>
		{
			{ "ExternalInteger", Datum::DatumTypes::Integer, 1, offsetof(AttributedFoo, ExternalInteger)},
			{ "ExternalFloat", Datum::DatumTypes::Float, 1, offsetof(AttributedFoo, ExternalFloat)},
			{ "ExternalString", Datum::DatumTypes::String, 1, offsetof(AttributedFoo, ExternalString)},
			{ "ExternalVector", Datum::DatumTypes::Vector, 1, offsetof(AttributedFoo, ExternalVector)},
			{ "ExternalMatrix", Datum::DatumTypes::Matrix, 1, offsetof(AttributedFoo, ExternalMatrix)},
			{ "ExternalIntegerArray", Datum::DatumTypes::Integer, size, offsetof(AttributedFoo, ExternalIntegerArray)},
			{ "ExternalFloatArray", Datum::DatumTypes::Float, size, offsetof(AttributedFoo, ExternalFloatArray)},
			{ "ExternalStringArray", Datum::DatumTypes::String, size, offsetof(AttributedFoo, ExternalStringArray)},
			{ "ExternalVectorArray", Datum::DatumTypes::Vector, size, offsetof(AttributedFoo, ExternalVectorArray)},
			{ "ExternalMatrixArray", Datum::DatumTypes::Matrix, size, offsetof(AttributedFoo, ExternalMatrixArray)},
			{ "NestedScope", Datum::DatumTypes::Table, 0, 0 },
			{ "NestedScopeArray", Datum::DatumTypes::Table, 5, 0 },
		};
	}

	gsl::owner<Scope*> AttributedFoo::Clone() const 
	{ 
		return new AttributedFoo(*this); 
	}

	std::string AttributedFoo::ToString() const
	{
		using namespace std::string_literals;
		return "AttributedFoo"s;
	}
}