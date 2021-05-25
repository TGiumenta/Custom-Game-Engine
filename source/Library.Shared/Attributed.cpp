#include "pch.h"
#include "Attributed.h"
#include "TypeManager.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Attributed)

	Attributed::Attributed(const RTTI::IdType id_type)
	{
		(*this)["this"] = this;
		Populate(id_type);
	}

	Attributed::Attributed(const Attributed& other) : 
		Scope(other)
	{
		(*this)["this"] = this;
		PointerRewrite(other.TypeIdInstance());
	}

	Attributed::Attributed(Attributed&& other) noexcept :
		Scope(std::move(other))
	{
		(*this)["this"] = this;
		PointerRewrite(other.TypeIdInstance());
	}

	Attributed& Attributed::operator=(const Attributed& other)
	{
		if (this != &other)
		{
			Scope::operator=(other);
			(*this)["this"] = this;
			PointerRewrite(other.TypeIdInstance());
		}

		return *this;
	}

	Attributed& Attributed::operator=(Attributed&& other) noexcept
	{
		if (this != &other)
		{
			Scope::operator=(std::move(other));
			(*this)["this"] = this;
			PointerRewrite(other.TypeIdInstance());
		}

		return *this;
	}

	bool Attributed::IsAttribute(const std::string& entry) const
	{
		return (Find(entry) != nullptr);
	}

	bool Attributed::IsPrescribedAttribute(const std::string& entry) const
	{
		if (entry == "this") return true;
		return (TypeManager::GetSignature(TypeIdInstance(), entry) != nullptr);
	}

	bool Attributed::IsAuxiliaryAttribute(const std::string& entry) const
	{
		return (IsAttribute(entry) && !IsPrescribedAttribute(entry));
	}

	Datum& Attributed::AppendAuxililaryAttribute(const std::string& entry)
	{
		if (IsPrescribedAttribute(entry))
		{
			throw std::invalid_argument("This entry is already a prescribed attribute");
		}

		return Append(entry);
	}

	const Vector<Scope::ScopePairType*> Attributed::PrescribedAttributes() const
	{
		Vector<Signature> signatures = TypeManager::GetSignaturesForType(TypeIdInstance());
		Vector<ScopePairType*> prescribedAttributes(signatures.Size() - + 1); // +1 for "this"

		for (size_t i = 0; i < signatures.Size(); ++i)
		{
			prescribedAttributes.PushBack(order[i]);
		}

		return prescribedAttributes;
	}

	const Vector<Scope::ScopePairType*> Attributed::AuxiliaryAttributes() const
	{
		Vector<Signature> signatures = TypeManager::GetSignaturesForType(TypeIdInstance());
		size_t auxiliaryBeginIndex = signatures.Size() + 1;
		Vector<ScopePairType*> auxiliaryAttributes(order.Size() - auxiliaryBeginIndex);

		for (size_t i = auxiliaryBeginIndex; i < order.Size(); ++i)
		{
			auxiliaryAttributes.PushBack(order[i]);
		}	

		return auxiliaryAttributes;
	}

	void Attributed::Populate(const RTTI::IdType id_type)
	{
		Vector<Signature> signatures = TypeManager::GetSignaturesForType(id_type);
		for (const Signature& signature : signatures)
		{
			assert(signature.type != Datum::DatumTypes::Unknown);

			Datum& datum = Append(signature.name);
			datum.SetType(signature.type);

			if (signature.type == Datum::DatumTypes::Table)
			{
				for (size_t i = 0_z; i < signature.size; ++i)
				{
					AppendScope(signature.name);
				}
			}
			else
			{
				void* data = reinterpret_cast<uint8_t*>(this) + signature.storage_offset;
				datum.SetStorage(data, signature.type, signature.size);
			}
		}
	}

	void Attributed::PointerRewrite(const RTTI::IdType id_type)
	{
		Vector<Signature> signatures = TypeManager::GetSignaturesForType(id_type);
		for (const Signature& signature : signatures)
		{
			assert(signature.type != Datum::DatumTypes::Unknown);

			if (signature.type != Datum::DatumTypes::Table)
			{
				Datum* datum = Find(signature.name);
				void* new_data = reinterpret_cast<uint8_t*>(this) + signature.storage_offset;
				datum->data.vp = new_data;
			}
		}
	}
}