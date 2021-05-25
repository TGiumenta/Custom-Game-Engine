#pragma once

#include "Action.h"
#include "GameTime.h"
#include "Factory.h"
#include "Event.h"

namespace FieaGameEngine
{
	class ActionEvent : public Action
	{
		RTTI_DECLARATIONS(ActionEvent, Action)

	public:
		/// <summary>
		/// Default constructor for object initialization
		/// </summary>
		ActionEvent();

		/// <summary>
		/// Default constructs an actionevent including setting the queue, subtype, and delay
		/// </summary>
		/// <param name="new_queue"> The queue to set </param>
		ActionEvent(EventQueue& new_queue);

		/// <summary>
		/// Copy constructor which creates a new actionevent based on the existing other actionevent
		/// </summary>
		/// <param name="other"> The original actionevent to copy </param>
		ActionEvent(const ActionEvent& other) = default;

		/// <summary>
		/// Move constructor which creates a new actionevent based on the existing other actionevent
		/// </summary>
		/// <param name="other"> The original actionevent to copy and move </param>
		ActionEvent(ActionEvent&& other) noexcept = default;

		/// <summary>
		/// Default equality operator for actionevent
		/// </summary>
		/// <param name = "other"> The actionevent to equate this actionevent to </param>
		/// <returns> The lhs action (this) after equalizing them </returns>
		ActionEvent& operator=(const ActionEvent& other) = default;
		
		/// <summary>
		/// Default equality operator for actionevent
		/// </summary>
		/// <param name = "other"> The actionevent to equate this actionevent to </param>
		/// <remarks> Uses rvalues instead of lvalues </remarks>
		/// <returns> The lhs actionevent (this) after equalizing them </returns>
		ActionEvent& operator=(ActionEvent&& other) noexcept = default;

		/// <summary>
		/// Destructor defaulted but meant to be overridden by all its children
		/// </summary>
		virtual ~ActionEvent() = default;

		/// <summary>
		/// Queries the list of prescribed attributes of actionevent
		/// </summary>
		/// <returns> The prescribed attributes for the class </returns>
		const Vector<Signature> Signatures();

		/// <summary>
		/// Create an attributed event, assign its world and subtype, copy all auxiliary 
		/// parameters into the event and queue the event with the given delay
		/// </summary>
		/// <param name="state"> The state of the world </param>
		virtual void Update(WorldState& state) override;

		/// <summary>
		/// Queries this subtype
		/// </summary>
		/// <returns> The subtype of this actionevent </returns>
		inline std::string& Subtype() { return subtype; }

		/// <summary>
		/// Queries this subtype
		/// </summary>
		/// <returns> The subtype of this actionevent </returns>
		inline const std::string& Subtype() const { return subtype; }

		/// <summary>
		/// Queries this delay
		/// </summary>
		/// <returns> The delay of this actionevent </returns>
		inline int32_t Delay() { return delay; }

		/// <summary>
		/// Sets the delay
		/// </summary>
		/// <param name="new_delay"> The delay to set </param>
		inline void SetDelay(int32_t new_delay) { delay = new_delay; }
		
		/// <summary>
		/// Queries the current queue
		/// </summary>
		/// <returns> The current queue </returns>
		inline EventQueue* Queue() { return queue; }

		/// <summary>
		/// Queries the current queue
		/// </summary>
		/// <returns> The current queue </returns>
		inline const EventQueue* Queue() const { return queue; }

		/// <summary>
		/// Sets the current queue
		/// </summary>
		/// <param name="new_queue"> The queue to set </param>
		inline void SetQueue(EventQueue& new_queue) { queue = &new_queue; }

	protected:
		std::string subtype;
		int32_t delay;
		EventQueue* queue;
	};

	ConcreteFactory(ActionEvent, Scope)
}
