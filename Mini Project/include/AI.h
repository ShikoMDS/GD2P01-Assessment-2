#pragma once

#include "Entity.h"
#include "Behaviour.h"

class AI : public Entity
{
public:
	AI(sf::Vector2f InitialPosition);

	void setBehaviour(Behaviour Behaviour);
	void setTexture(const sf::Texture& Texture);
	sf::FloatRect getGlobalBounds() const; // Function to get the global bounds of the sprite
	void setPosition(const sf::Vector2f& Position); // Function to set the position of the AI
	void handleInput(sf::RenderWindow& Window) override;
	void update(float DeltaTime) override;
	void draw(sf::RenderWindow& Window) override;

private:
	Behaviour MCurrentBehaviour;
};
