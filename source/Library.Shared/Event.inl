#include "pch.h"
#include "Event.h"

namespace FieaGameEngine
{
	template <typename T>
	Vector<EventSubscriber*> Event<T>::subscribers;

	template<typename T>
	RTTI_DEFINITIONS(Event<T>)

	template<typename T>
	Event<T>::Event(const T& new_message) :
		EventPublisher(subscribers), message(new_message)
	{
	}

	template<typename T>
	Event<T>::Event(T&& new_message) :
		EventPublisher(subscribers), message(std::forward<T>(new_message))
	{
	}

	template<typename T>
	inline void Event<T>::Subscribe(EventSubscriber& subscriber)
	{
		if (subscribers.Find(&subscriber) == subscribers.end())
		{
			subscribers.PushBack(&subscriber);
		}
	}

	template<typename T>
	inline size_t Event<T>::Size()
	{
		return subscribers.Size();
	}

	template<typename T>
	inline bool Event<T>::Unsubscribe(EventSubscriber& subscriber)
	{
		return subscribers.Remove(&subscriber);
	}

	template<typename T>
	inline void Event<T>::UnsubscribeAll()
	{
		subscribers.Clear();
		subscribers.ShrinkToFit();
	}

	template<typename T>
	inline const T& Event<T>::Message() const
	{
		return message;
	}
}