#pragma once

#include <iostream>
#include "Entity.h"

class User final : public Entity
{
public:
	explicit User(sf::Vector2f InitialPosition);

	void handleInput(sf::RenderWindow& Window) override;
	void update(float DeltaTime) override;
	void draw(sf::RenderWindow& Window) override;

private:
	float MSpeed;
	sf::IntRect MFrameRect; // Rectangle defining the current frame of animation
	sf::Clock MAnimationClock; // Clock for controlling animation frame rate
	float MAnimationSpeed; // Speed of animation (fps)
	unsigned int MCurrentFrame; // Index of current frame
	unsigned int MNumFrames; // Number of frames in animation
};
