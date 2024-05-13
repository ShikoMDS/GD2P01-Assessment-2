#include "User.h"

User::User(const sf::Vector2f InitialPosition)
	: Entity(InitialPosition), MSpeed(500.0f), MAnimationSpeed(10.0f), MCurrentFrame(0)
{
	if (!MTexture.loadFromFile("resources/images/herta kuru ss.jpg"))
	{
		std::cout << "Image failed to load!" << std::endl;
		std::cout << "Failed to load user animation sequence." << std::endl;
	}
	MSprite.setTexture(MTexture);
	MSprite.setScale(1.0f, 1.0f);

	// Define the rectangle for the initial frame of animation
	MFrameRect = sf::IntRect(0, 0, 32, 32);

	// Calculate the number of frames based on the texture width and frame width
	MNumFrames = MTexture.getSize().x / MFrameRect.width;
}

void User::handleInput(sf::RenderWindow& Window)
{
	static sf::Clock Clock;

	const float DeltaTime = Clock.restart().asSeconds(); // Get time elapsed since last frame
	const float MoveDistance = MSpeed * DeltaTime; // Calculate distance to move based on speed and time

	// Normalize diagonal movement to ensure consistent speed
	const float DiagonalFactor = 1.0f / std::sqrt(2.0f); // 1/sqrt(2) for diagonal movement normalization

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		MPosition -= sf::Vector2f(MoveDistance, MoveDistance) * DiagonalFactor;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		MPosition -= sf::Vector2f(-MoveDistance, MoveDistance) * DiagonalFactor;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		MPosition += sf::Vector2f(-MoveDistance, MoveDistance) * DiagonalFactor;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		MPosition += sf::Vector2f(MoveDistance, MoveDistance) * DiagonalFactor;
	}
	// Cardinal directions
	else
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			MPosition.y -= MoveDistance;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			MPosition.x -= MoveDistance;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			MPosition.y += MoveDistance;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			MPosition.x += MoveDistance;
		}
	}

	// Apply border rule to wrap around to the opposite side based on entity center
	const sf::Vector2u WindowSize = Window.getSize();
	const float HalfEntityWidth = MSprite.getGlobalBounds().width / 2.0f;
	const float HalfEntityHeight = MSprite.getGlobalBounds().height / 2.0f;

	if (MPosition.x + HalfEntityWidth < 0)
	{
		MPosition.x = WindowSize.x + HalfEntityWidth; // Wrap around to the right side
	}
	else if (MPosition.x - HalfEntityWidth > WindowSize.x)
	{
		MPosition.x = -HalfEntityWidth; // Wrap around to the left side
	}
	if (MPosition.y + HalfEntityHeight < 0)
	{
		MPosition.y = WindowSize.y + HalfEntityHeight; // Wrap around to the bottom side
	}
	else if (MPosition.y - HalfEntityHeight > WindowSize.y)
	{
		MPosition.y = -HalfEntityHeight; // Wrap around to the top side
	}
}

void User::update(float DeltaTime)
{
	// Update animation frame based on elapsed time
	const float FrameTime = 1.0f / MAnimationSpeed; // Time per frame
	if (MAnimationClock.getElapsedTime().asSeconds() > FrameTime)
	{
		MCurrentFrame = (MCurrentFrame + 1) % MNumFrames; // Increment frame index (loop)
		MAnimationClock.restart(); // Restart animation clock
	}

	// Calculate the rectangle for the current frame of animation
	const int FrameWidth = MTexture.getSize().x / MNumFrames; // Calculate frame width
	MFrameRect.left = MCurrentFrame * FrameWidth; // Adjust based on the sprite sheet layout
	MFrameRect.width = FrameWidth; // Set frame width
}

void User::draw(sf::RenderWindow& Window)
{
	MSprite.setTextureRect(MFrameRect);
	MSprite.setPosition(MPosition);
	Window.draw(MSprite);
}
