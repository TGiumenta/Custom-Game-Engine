#pragma once

#include <glm/glm.hpp>
#include "Attributed.h"
#include "TypeManager.h"
#include "Vector.h"
#include "WorldState.h"

namespace FieaGameEngine
{
	/// <summary>
	/// A wrapper for an Attributed Scope, this class is meant to be a compile-time representation 
	/// of the data that the user intends to fill the Entity with. This is extremely similar to 
	/// a scope but the difference is its way of construction.
	/// </summary>
	class Entity : public Attributed
	{
		RTTI_DECLARATIONS(Entity, Attributed)

	public:
		/// <summary>
		/// Default constructor for object initialization
		/// </summary>
		Entity();

		/// <summary>
		/// Copy constructor which creates a new entity based on the existing other entity
		/// </summary>
		/// <param name="entity"> The original entity to copy </param>
		Entity(const Entity& entity) = default;
		
		/// <summary>
		/// Copy constructor which creates a new entity based on the existing other entity
		/// </summary>
		/// <param name="entity"> The original entity to copy </param>
		/// <remarks> Copy constructor uses rvalues instead of lvalues </remarks>
		Entity(Entity&& entity) = default;
	
		/// <summary>
		/// Default equality operator for entities
		/// </summary>
		/// <param name = "other"> The entity to equate this entity to </param>
		/// <returns> The lhs entity (this) after equalizing them </returns>
		Entity& operator=(const Entity& entity) = default;
		
		/// <summary>
		/// Default equality operator for entities
		/// </summary>
		/// <param name = "other"> The entity to equate this entity to </param>
		/// <remarks> Equality operator uses rvalues instead of lvalues </remarks>
		/// <returns> The lhs entity (this) after equalizing them </returns>
		Entity& operator=(Entity&& entity) = default;
		
		/// <summary>
		/// Virtual destructor which should be inherited by children classes
		/// </summary>
		virtual ~Entity() = default;

		/// <summary>
		/// Queries the list of prescribed attributes of entity
		/// </summary>
		/// <returns> The prescribed attributes for the class </returns>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// Called during the game loop, goes through all the entities and their children and 
		/// calls their overidden update methods
		/// </summary>
		/// <param name="state"> The state of the world for tracking </param>
		virtual void Update(WorldState& state);

		/// <summary>
		/// Creates a new action, wraps scope adopt mainly
		/// </summary>
		/// <param name="class_name"> Name of the class to create a factory of </param>
		/// <param name="instance_name"> Name of the action </param>
		void CreateAction(const std::string& class_name, const std::string& instance_name);

		/// <summary>
		/// Queries the name of the entity
		/// </summary>
		/// <returns> The name of the entity </returns>
		std::string& Name() { return name; }

		/// <summary>
		/// Changes the entities name
		/// </summary>
		/// <param name="new_name"> The name to set on the entity </param>
		void SetName(const std::string& new_name) { name = new_name; }
		
		/// <summary>
		/// Changes the entities name
		/// </summary>
		/// <param name="new_name"> The name to set on the entity </param>
		/// <remarks> Uses rvalues instead of lvalues </remarks>
		void SetName(std::string&& new_name) { name = std::move(new_name); }

		/// <summary>
		/// An accessor which returns a grouping of the position,rotation and scale
		/// </summary>
		/// <remarks> Makes a copy - please use sparingly and only if needed </remarks>
		/// <returns></returns>
		std::tuple<glm::vec4, glm::vec4, glm::vec4> Transform() { return std::make_tuple(position, rotation, scale); }

		/// <summary>
		/// Queries the position of the entity
		/// </summary>
		/// <returns> The position of the entity </returns>
		glm::vec4& Position() { return position; };
		
		/// <summary>
		/// Changes the entity's position
		/// </summary>
		/// <param name="new_position"> The position to set on the entity </param>
		void SetPosition(const glm::vec4& new_position) { position = new_position; }
		
		/// <summary>
		/// Changes the entity's position
		/// </summary>
		/// <param name="new_position"> The position to set on the entity </param>
		/// <remarks> Uses rvalues instead of lvalues </remarks>
		void SetPosition(glm::vec4&& new_position) { position = std::move(new_position); }

		/// <summary>
		/// Queries the rotation of the entity
		/// </summary>
		/// <returns> The rotation of the entity </returns>
		glm::vec4& Rotation() { return rotation; }
	
		/// <summary>
		/// Changes the entity's rotation
		/// </summary>
		/// <param name="new_rotation"> The rotation to set on the entity </param>
		void SetRotation(const glm::vec4& new_rotation) { rotation = new_rotation; }
		
		/// <summary>
		/// Changes the entity's rotation
		/// </summary>
		/// <param name="new_rotation"> The rotation to set on the entity </param>
		/// <remarks> Uses rvalues instead of lvalues </remarks>
		void SetRotation(glm::vec4&& new_rotation) { rotation = std::move(new_rotation); }

		/// <summary>
		/// Queries the scale of the entity
		/// </summary>
		/// <returns> The scale of the entity </returns>
		glm::vec4& Scale() { return scale; }
		
		/// <summary>
		/// Changes the entity's scale
		/// </summary>
		/// <param name="new_scale"> The scale to set on the entity </param>
		void SetScale(const glm::vec4& new_scale) { scale = new_scale; }
		
		/// <summary>
		/// Changes the entity's scale
		/// </summary>
		/// <param name="new_scale"> The scale to set on the entity </param>
		/// <remarks> Uses rvalues instead of lvalues </remarks>
		void SetScale(glm::vec4&& new_scale) { scale = std::move(new_scale); }

		/// <summary>
		/// Queries the data member and returns this entity's tags
		/// </summary>
		/// <remarks> Cannot alter the tags from this method, used only for "peeking" </remarks>
		/// <returns> All of the entity's tags</returns>
		const Vector<std::string>& Tags() const { return tags; }

		/// <summary>
		/// Adds a tag to the vector. Wraps pushback
		/// </summary>
		/// <param name="tag"> The tag to add </param>
		void AddTag(const std::string& tag) { tags.PushBack(tag); }
		
		/// <summary>
		/// Adds a tag to the vector. Wraps pushback
		/// </summary>
		/// <param name="tag"> The tag to add </param>
		/// <remarks> Uses rvalues instead of lvalues </remarks>
		void AddTag(std::string&& tag) { tags.PushBack(std::move(tag)); }

		/// <summary>
		/// Questions whether the given string is a tag
		/// </summary>
		/// <param name="tag"> The tag in question </param>
		/// <returns> An indicator indicating if the tag is assigned </returns>
		bool IsTag(const std::string& tag) const { return (tags.Find(tag) != tags.end()); }
		
		/// <summary>
		/// Questions whether the given string is a tag
		/// </summary>
		/// <param name="tag"> The tag in question </param>
		/// <remarks> Uses rvalues instead of lvalues </remarks>
		bool IsTag(std::string&& tag) const { return (tags.Find(std::move(tag)) != tags.end()); }

		/// <summary>
		/// Removes a tag from the vector. Wraps remove
		/// </summary>
		/// <param name="tag"> The tag to remove </param>
		void RemoveTag(const std::string& tag) { tags.Remove(tag); }
		
		/// <summary>
		/// Removes a tag from the vector. Wraps remove
		/// </summary>
		/// <param name="tag"> The tag to remove </param>
		/// <remarks> Uses rvalues instead of lvalues </remarks>
		void RemoveTag(std::string&& tag) { tags.Remove(std::move(tag)); }

		/// <summary>
		/// Queries the order vector and finds all the children of this entity
		/// </summary>
		/// <returns> The children of this entity </returns>
		Datum& Children();

		/// <summary>
		/// Queries the order vector and finds all the actions of this entity
		/// </summary>
		/// <returns> The actions of this entity </returns>
		Datum& Actions();

	protected:
		explicit Entity(RTTI::IdType child_id);
		std::string name;
		glm::vec4 position;
		glm::vec4 rotation;
		glm::vec4 scale;

	private:
		Vector<std::string> tags;
		static const size_t children_identifier = 2;
		static const size_t actions_identifier = 6;
	};

	ConcreteFactory(Entity, Scope)
}

