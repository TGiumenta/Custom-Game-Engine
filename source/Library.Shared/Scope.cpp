#include "pch.h"
#include "Scope.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Scope)

#pragma region RuleOf6

	Scope::Scope(const size_t initial_capacity) :
		order(initial_capacity)
	{
	}

	Scope::Scope(const Scope& other)
	{
		DeepCopyScope(other);
	}

	Scope::Scope(Scope&& other) noexcept
	{
		MoveScope(std::move(other));
	}

	Scope& Scope::operator=(const Scope& other)
	{
		if (this != &other)
		{
			Clear();
			DeepCopyScope(other);
		}

		return *this;
	}

	Scope& Scope::operator=(Scope&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			MoveScope(std::move(other));
		}

		return *this;
	}

	Scope::~Scope()
	{
		Clear();
	}

#pragma endregion RuleOf6

	Datum* Scope::Find(const std::string& entry)
	{
		auto datum = map.Find(entry);
		if (datum == map.end())
		{
			return nullptr;
		}

		return &datum->second;
	}

	const Datum* Scope::Find(const std::string& entry) const
	{
		return const_cast<Scope*>(this)->Find(entry);
	}

	std::tuple<Datum*, Scope*> Scope::Search(const std::string& entry)
	{
		Datum* found = Find(entry);

		if (found)
		{
			return std::make_tuple(found, this);
		}

		if (parent == nullptr)
		{
			return std::make_tuple(nullptr, nullptr);
		}

		return parent->Search(entry);
	}

	const std::tuple<Datum*, Scope*> Scope::Search(const std::string& entry) const
	{
		return const_cast<Scope*>(this)->Search(entry);
	}

	Datum& Scope::Append(const std::string& entry, bool& entry_created)
	{
		if (entry.empty())
		{
			throw std::invalid_argument("The key is equivalent to the empty string.");
		}

		auto [was_inserted, hash_iterator] = map.Insert(ScopePairType(entry, Datum(Datum::DatumTypes::Unknown)));
		HashMap<std::string, Datum>::PairType& pair = *hash_iterator;
		entry_created = was_inserted;
		if (entry_created)
		{
			order.PushBack(&pair);
		}

		return pair.second;
	}

	Datum& Scope::Append(const std::string& entry)
	{
		bool entry_created;
		return Append(entry, entry_created);
	}

	Scope& Scope::AppendScope(const std::string& entry)
	{
		if (entry.empty())
		{
			throw std::invalid_argument("The key is equivalent to the empty string.");
		}

		auto [was_inserted, hash_iterator] = map.Insert(ScopePairType(entry, Datum(Datum::DatumTypes::Table)));
		if (!was_inserted)
		{
			if (hash_iterator->second.Type() != Datum::DatumTypes::Table)
			{
				throw std::runtime_error("Item exists but not of type scope.");
			}
		}
		else
		{
			order.PushBack(&(*hash_iterator));
		}

		Scope* child = new Scope();
		child->parent = this;
		hash_iterator->second.PushBack(*child);

		return *child;
	}

	void Scope::Adopt(Scope& child, const std::string& entry)
	{
		if (this == &child)
		{
			throw std::invalid_argument("Self-adoption");
		}

		if (child.IsAncestor(*this))
		{
			throw std::invalid_argument("Ancestor adoption");
		}

		if (entry.empty())
		{
			throw std::invalid_argument("The key is equivalent to the empty string");
		}

		auto [was_inserted, hash_iterator] = map.Insert(ScopePairType(entry, Datum(Datum::DatumTypes::Table)));
		if (!was_inserted)
		{
			// Already in map
			if (hash_iterator->second.Type() != Datum::DatumTypes::Table)
			{
				throw std::runtime_error("The datum found is not of type Table");
			}
		}
		else
		{
			order.PushBack(&(*hash_iterator));
		}

		child.Orphan();
		hash_iterator->second.PushBack(child);
		child.parent = this;
	}

	std::tuple<Datum*, size_t> Scope::FindContainedScope(const Scope& other) const
	{
		size_t index = std::numeric_limits<size_t>::max();
		Datum* found_datum = nullptr;
		ForEachNestedScopeIn([&other, &index, &found_datum](const Scope&, Datum& datum, size_t datum_index)
		{
			if (&datum.Get<Scope>(datum_index) == &other) // IS a pointer comparsion
			{
				index = datum_index;
				found_datum = &datum;
				return true;
			}			
			
			return false;
		});

		return { found_datum, index };
	}

	void Scope::Clear()
	{
		Orphan();

		ForEachNestedScopeIn([](const Scope& _parent, Datum& datum, size_t datum_index)
		{
			_parent;
			Scope& scope = datum.Get<Scope>(datum_index);
#ifdef DEBUG
			assert(scope.parent == &_parent);
#endif

			scope.parent = nullptr;
			delete &scope;
			return false;
		});

		order.Clear();
		map.Clear();
	}

	bool Scope::Equals(const RTTI* rhs) const
	{
		bool is_equal = false;
		const Scope* other = rhs->As<Scope>();
		if (other == nullptr)
		{
			return false;
		}

		const size_t size = order.Size();
		if (size == other->Size())
		{
			for (size_t index = 0; index < size; ++index)
			{
				if (order[index]->first == "this")
				{
					continue;
				}

				if (*order[index] != *other->order[index])
				{
					return false;
				}
			}

			is_equal = true;
		}

		return is_equal;
	}

	void Scope::Orphan()
	{
		if (parent != nullptr)
		{
			auto [datum, index] = parent->FindContainedScope(*this);
			assert(datum != nullptr);
			datum->RemoveAt(index);
			parent = nullptr;
		}
	}

	bool Scope::IsAncestor(const Scope& other) const
	{
		return other.IsDescendant(*this);
	}

	bool Scope::IsDescendant(const Scope& other) const
	{
		Scope* temp_parent = parent;
		while (temp_parent != nullptr)
		{
			if (temp_parent == &other)
			{
				return true;
			}

			temp_parent = temp_parent->parent;
		}

		return false;
	}

	void Scope::ForEachNestedScopeIn(NestedScopeFunction func) const
	{
		for (const auto& pair : order)
		{
			Datum& datum = pair->second;
			if (datum.Type() == Datum::DatumTypes::Table)
			{
				for (size_t datum_index = 0_z; datum_index < datum.Size(); ++datum_index)
				{
					if (func(*this, datum, datum_index))
					{
						return;
					}
				}
			}
		}
	}

	void Scope::MoveScope(Scope&& other)
	{
		order = std::move(other.order);
		map = std::move(other.map);
		parent = other.parent;

		if (other.parent)
		{
			auto [datum, index] = other.parent->FindContainedScope(other);
			assert(datum != nullptr);
			datum->SetScope(*this, index);
			other.parent = nullptr;
		}

		ForEachNestedScopeIn([this](const Scope&, Datum& datum, size_t datum_index)
		{
			Scope& child = datum.Get<Scope>(datum_index);
			child.parent = this;
			return false;
		});
	}

	void Scope::DeepCopyScope(const Scope& other)
	{
		for (auto const& element : other.order)
		{
			auto &[key, existingDatum] = *element;
			Datum& newDatum = Append(key);

			if (existingDatum.Type() == Datum::DatumTypes::Table)
			{
				newDatum.SetType(Datum::DatumTypes::Table);
				newDatum.Reserve(existingDatum.Size());

				for (size_t i = 0_z; i < existingDatum.Size(); ++i)
				{
					Scope* newScope = existingDatum.Get<Scope>(i).Clone();
					newScope->parent = this;
					newDatum.PushBack(*newScope);
				}
			}
			else
			{
				newDatum = existingDatum;
			}
		}
	}
}