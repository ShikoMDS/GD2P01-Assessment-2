#pragma once

#include <SFML/Graphics.hpp>

class Entity
{
public:
	Entity(sf::Vector2f InitialPosition);

	virtual void handleInput(sf::RenderWindow& Window) = 0;
	virtual void update(float DeltaTime) = 0;
	virtual void draw(sf::RenderWindow& Window) = 0;
	sf::Vector2f getPosition() const;

protected:
	sf::Vector2f MPosition;
	sf::Texture MTexture;
	sf::Sprite MSprite;
};
