#include "pch.h"
#include "Entity.h"
#include "Action.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Entity)

	Entity::Entity() :
		Attributed(Entity::TypeIdClass())
	{
	}

	Entity::Entity(RTTI::IdType child_id) :
		Attributed(child_id)
	{
	}

	const Vector<Signature> Entity::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name", Datum::DatumTypes::String, 1, offsetof(Entity, name)},
			{ "Children", Datum::DatumTypes::Table, 0, 0 },
			{ "Position", Datum::DatumTypes::Vector, 1, offsetof(Entity, position)},
			{ "Rotation", Datum::DatumTypes::Vector, 1, offsetof(Entity, rotation)},
			{ "Scale", Datum::DatumTypes::Vector, 1, offsetof(Entity, scale)},
			{ "Actions", Datum::DatumTypes::Table, 0, 0},
		};
	}

	void Entity::Update(WorldState& state)
	{
		state.SetCurrentEntity(this);

		Datum& actions = Actions();
		for (size_t index = 0; index < actions.Size(); ++index)
		{
			assert(actions[index].Is(Action::TypeIdClass()));
			Action& action = static_cast<Action&>(actions[index]);
			action.Update(state);
		}

		Datum& children = Children();
		for (size_t index = 0; index < children.Size(); ++index)
		{
			assert(children[index].Is(Entity::TypeIdClass()));
			Entity& entity = static_cast<Entity&>(children[index]);
			entity.Update(state);
		}
	}

	void Entity::CreateAction(const std::string& class_name, const std::string& instance_name)
	{
		Scope* context = Factory<Scope>::Create(class_name);
		assert(context != nullptr);

		Action* action = context->As<Action>();
		assert(action != nullptr);

		Adopt(*context, "Actions");
		action->SetName(instance_name);
	}

	Datum& Entity::Children()
	{
		return order.at(children_identifier)->second;
	}

	Datum& Entity::Actions()
	{
		return order.at(actions_identifier)->second;
	}
}