#pragma once

#include "Entity.h"

class User : public Entity
{
public:
	User(sf::Vector2f InitialPosition);

	void handleInput(sf::RenderWindow& Window) override;
	void update(float DeltaTime) override;
	void draw(sf::RenderWindow& Window) override;

private:
	float MSpeed;
	sf::IntRect MFrameRect; // Rectangle defining the current frame of animation
	sf::Clock MAnimationClock; // Clock for controlling animation frame rate
	float MAnimationSpeed; // Speed of animation (frames per second)
	unsigned int MCurrentFrame; // Index of the current frame
	unsigned int MNumFrames; // Total number of frames in the animation
};
