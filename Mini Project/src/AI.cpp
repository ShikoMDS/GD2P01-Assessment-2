#include "AI.h"

AI::AI(const sf::Vector2f InitialPosition)
	: Entity(InitialPosition), MCurrentBehaviour(Behaviour::Seek)
{
	// Initialize AI properties
}

void AI::setBehaviour(const Behaviour Behaviour)
{
	MCurrentBehaviour = Behaviour;
	// Implement behaviour-specific logic here
}

void AI::setTexture(const sf::Texture& Texture)
{
	this->MTexture = Texture;
	MSprite.setTexture(Texture);
}

sf::FloatRect AI::getGlobalBounds() const
{
	return MSprite.getGlobalBounds();
}

void AI::setPosition(const sf::Vector2f& Position)
{
	this->MPosition = Position;
}

void AI::handleInput(sf::RenderWindow& Window)
{
	// Implement input handling logic for AI
}

void AI::update(float DeltaTime)
{
	// Implement AI behaviour update logic based on currentBehaviour
}

void AI::draw(sf::RenderWindow& Window)
{
	MSprite.setTexture(MTexture); // Set the texture for drawing
	MSprite.setPosition(MPosition);
	Window.draw(MSprite); // Draw the sprite
}
