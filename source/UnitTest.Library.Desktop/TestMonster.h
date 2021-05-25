#pragma once

#include "Entity.h"
#include "Factory.h"

namespace FieaGameEngine
{
	class TestMonster final : public Entity
	{
		RTTI_DECLARATIONS(TestMonster, Entity)

	public:
		TestMonster() :
			Entity::Entity(TestMonster::TypeIdClass()) {};

		~TestMonster() = default;
	};

	ConcreteFactory(TestMonster, Scope)
}