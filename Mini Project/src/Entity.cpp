#include "Entity.h"

Entity::Entity(const sf::Vector2f InitialPosition)
	: MPosition(InitialPosition), MVelocity(0.f, 0.f)
{
}

sf::Vector2f Entity::getPosition() const
{
	return MPosition;
}

sf::Vector2f Entity::getVelocity() const
{
	return MVelocity;
}

void Entity::update(const float DeltaTime)
{
	MPosition += MVelocity * DeltaTime;
}
