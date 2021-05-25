#include "pch.h"
#include "WorldState.h"
#include "Entity.h"
#include "Action.h"

namespace FieaGameEngine
{
	SList<WorldState::CreateInformation> WorldState::create_actions;
	SList<WorldState::DestroyInformation> WorldState::destroy_actions;

	WorldState::WorldState(GameTime& game_time)
	{
		SetGameTime(game_time);
	}

	WorldState::~WorldState()
	{
		create_actions.Clear();
		destroy_actions.Clear();
	}

	void WorldState::AddCreateAction(Scope& context, const std::string& class_name)
	{
		create_actions.PushBack(CreateInformation{ context, class_name });
	}

	void WorldState::CreateActions()
	{
		for (auto& action_to_create : create_actions)
		{
			entity->CreateAction(action_to_create.class_name, "Actions");
		}

		create_actions.Clear();
	}

	void WorldState::AddDestroyAction(Scope& context, const std::string& name)
	{
		destroy_actions.PushBack(DestroyInformation{ context, name });
	}

	void WorldState::DestroyActions()
	{
		for (auto& action_to_destroy : destroy_actions)
		{
			AttemptDestroyAction(&action_to_destroy.context, action_to_destroy.name);
		}

		destroy_actions.Clear();
	}

	void WorldState::AttemptDestroyAction(Scope* context, const std::string& name)
	{
		Datum* actions = context->Find("Actions");
		for (size_t index = 0; index < actions->Size(); ++index)
		{
			Action* potential_action = static_cast<Action*>(&actions->Get<Scope>(index));
			assert(potential_action != nullptr);
			if (potential_action->Name() == name)
			{
				delete potential_action;
				actions->RemoveAt(index);
				return;
			}
		}

		Scope* parent = context->GetParent();
		if (parent) { return AttemptDestroyAction(parent, name); };
	}
}