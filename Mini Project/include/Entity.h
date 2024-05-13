#pragma once

#include <SFML/Graphics.hpp>

class Entity
{
public:
	explicit Entity(sf::Vector2f InitialPosition);

	sf::Vector2f getPosition() const;
	sf::Vector2f getVelocity() const;

	virtual void handleInput(sf::RenderWindow& Window) = 0;
	virtual void update(float DeltaTime);
	virtual void draw(sf::RenderWindow& Window) = 0;

protected:
	~Entity() = default;

	sf::Vector2f MPosition;
	sf::Vector2f MVelocity;
	sf::Texture MTexture;
	sf::Sprite MSprite;
};
