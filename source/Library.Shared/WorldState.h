#pragma once

#include "GameTime.h"
#include "SList.h"
#include "Scope.h"

namespace FieaGameEngine
{
	class Entity;
	class Action;

	/// <summary>
	/// Handles the current state of the game by tracking the current entity that's being 
	/// processed, and the current action as well. This wraps the GameTime classes by keeping 
	/// a reference to the current GameTime.
	/// </summary>
	class WorldState final
	{
	public:
		/// <summary>
		/// Defaulted constructor
		/// </summary>
		WorldState() = default;

		/// <summary>
		/// Constructor overload which also sets the game time
		/// </summary>
		/// <param name="game_time"> The game time to set </param>
		WorldState(GameTime& game_time);

		/// <summary>
		/// Destructor which clears both create/destroy lists
		/// </summary>
		~WorldState();

		/// <summary>
		/// Retrieves the GameTime reference held by the WorldState 
		/// </summary>
		/// <returns> The GameTime that's being used by this State right now</returns>
		GameTime* GetGameTime() { return current_game_time; }
		
		/// <summary>
		/// Alters the current GameTime to the parameter
		/// </summary>
		/// <param name="new_game_time"> The new GameTime to set </param>
		void SetGameTime(GameTime& new_game_time) { current_game_time = &new_game_time; }

		/// <summary>
		/// A getter which retrieves the current entity
		/// </summary>
		/// <returns> The current entity that is being processed </returns>
		Entity* GetCurrentEntity() { return entity; }

		/// <summary>
		/// Sets the current entity to the parameter
		/// </summary>
		/// <param name="new_entity"> The entity to set </param>
		void SetCurrentEntity(Entity* new_entity) { entity = new_entity; }

		/// <summary>
		/// A getter which retrieves the current action
		/// </summary>
		/// <returns> The current action that is being processed </returns>
		Action* GetCurrentAction() { return action; }
		
		/// <summary>
		/// Sets the current action to the parameter
		/// </summary>
		/// <param name="new_action"> The action to set </param>
		void SetCurrentAction(Action* new_action) { action = new_action; }

		/// <summary>
		/// Adds an action to the queue of actions to be created later
		/// </summary>
		/// <param name="context"> The scope context used for adopt </param>
		/// <param name="class_name"> The actual class name to instantiate a factory with </param>
		void AddCreateAction(Scope& context, const std::string& class_name);
		
		/// <summary>
		/// Empties the action queue and creates all the actions in said queue
		/// </summary>
		void CreateActions();

		/// <summary>
		/// Adds an action to the queue of actions to be destroyed later
		/// </summary>
		/// <param name="context"> The scope context used for adopt </param>
		/// <param name="name"> The action to search for destruction </param>
		void AddDestroyAction(Scope& context, const std::string& name);
		
		/// <summary>
		/// Empties the action queue and destroys all the actions in said queue 
		/// </summary>
		void DestroyActions();

	private:
		void AttemptDestroyAction(Scope* context, const std::string& name);

		GameTime* current_game_time{ nullptr };
		class Entity* entity{ nullptr };
		class Action* action{ nullptr };

		struct CreateInformation {
			Scope& context;
			std::string class_name;
		};

		struct DestroyInformation {
			Scope& context;
			const std::string& name;
		};

		static SList<CreateInformation> create_actions;
		static SList<DestroyInformation> destroy_actions;
	};
}