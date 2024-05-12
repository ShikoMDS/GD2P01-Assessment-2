#include "Entity.h"

Entity::Entity(const sf::Vector2f InitialPosition)
	: MPosition(InitialPosition)
{
}

sf::Vector2f Entity::getPosition() const
{
	return MPosition;
}
