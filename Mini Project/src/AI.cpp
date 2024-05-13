#include "AI.h"

AI::AI(const sf::Vector2f InitialPosition, User* UserPtr) : Entity(InitialPosition),
                                                            MCurrentBehaviour(SteeringBehaviour::Stationary),
                                                            MUser(UserPtr), MSpeed(100.0f), MPredictionTime(1.0f)
{
	initializeWanderProperties(); // Initialize wander properties for each different AI instance
}

void AI::setBehaviour(const SteeringBehaviour Behavior)
{
	MCurrentBehaviour = Behavior;
}

void AI::setTexture(const sf::Texture& TextureRef)
{
	this->MTexture = TextureRef;
	MSprite.setTexture(TextureRef);
}

sf::FloatRect AI::getGlobalBounds() const
{
	return MSprite.getGlobalBounds();
}

void AI::setPosition(const sf::Vector2f& Position)
{
	this->MPosition = Position;
}

AI::SteeringBehaviour AI::getBehaviour() const
{
	return MCurrentBehaviour;
}

void AI::setPredictionTime(const float Time)
{
	MPredictionTime = Time;
}


void AI::draw(sf::RenderWindow& Window)
{
	MSprite.setTexture(MTexture);
	MSprite.setPosition(MPosition);
	Window.draw(MSprite);
}

void AI::update(const float DeltaTime)
{
	// Call the parent class's update function
	Entity::update(DeltaTime);

	/*
	// Print debug information about the current behaviour
	switch (MCurrentBehaviour)
	{
	case SteeringBehaviour::Stationary:
	    std::cout << "Current behaviour: Stationary" << std::endl;
	    break;
	case SteeringBehaviour::Seek:
	    std::cout << "Current behaviour: Seek" << std::endl;
	    break;
	case SteeringBehaviour::Flee:
	    std::cout << "Current behaviour: Flee" << std::endl;
	    break;
	case SteeringBehaviour::Wander:
	    std::cout << "Current behaviour: Wander" << std::endl;
	    break;
	case SteeringBehaviour::Arrival:
	    std::cout << "Current behaviour: Arrival" << std::endl;
	    break;
	case SteeringBehaviour::Pursuit:
	    std::cout << "Current behaviour: Pursuit" << std::endl;
	    break;
	case SteeringBehaviour::Evade:
	    std::cout << "Current behaviour: Evade" << std::endl;
	    break;
	}
	*/

	// Implement steering behaviour with wrapping around screen borders
	if (MUser != nullptr)
	{
		// Get the position and velocity of the user character
		const sf::Vector2f TargetPosition = MUser->getPosition();
		const sf::Vector2f TargetVelocity = MUser->getVelocity();

		// Calculate the vector from AI to user character with wrapping
		sf::Vector2f desiredVelocity;
		desiredVelocity.x = TargetPosition.x - MPosition.x;
		desiredVelocity.y = TargetPosition.y - MPosition.y;

		// Wrap around screen borders
		constexpr float WindowWidth = 1600.0f;
		constexpr float WindowHeight = 900.0f;

		if (desiredVelocity.x > WindowWidth / 2)
		{
			desiredVelocity.x -= WindowWidth;
		}
		else if (desiredVelocity.x < -WindowWidth / 2)
		{
			desiredVelocity.x += WindowWidth;
		}
		if (desiredVelocity.y > WindowHeight / 2)
		{
			desiredVelocity.y -= WindowHeight;
		}
		else if (desiredVelocity.y < -WindowHeight / 2)
		{
			desiredVelocity.y += WindowHeight;
		}

		// Normalize velocity
		const float Magnitude =
			std::sqrt(desiredVelocity.x * desiredVelocity.x + desiredVelocity.y * desiredVelocity.y);
		if (Magnitude > 0)
		{
			desiredVelocity /= Magnitude;
		}

		// Set AI velocity based on current behaviour
		sf::Vector2f MVelocity;
		if (MCurrentBehaviour == SteeringBehaviour::Seek)
		{
			MVelocity = seekBehaviour(desiredVelocity, MSpeed * 3);
		}
		else if (MCurrentBehaviour == SteeringBehaviour::Flee)
		{
			MVelocity = fleeBehaviour(desiredVelocity, MSpeed * 2);
		}
		else if (MCurrentBehaviour == SteeringBehaviour::Wander)
		{
			// If wander timer is greater than wander duration, reset properties
			if (MWanderTimer > MWanderDuration)
			{
				initializeWanderProperties();
			}
			else
			{
				// Update wander timer and adjust position based on wander direction and speed
				MWanderTimer += DeltaTime;
				MVelocity = MWanderDirection * MWanderSpeed;
			}
		}
		else if (MCurrentBehaviour == SteeringBehaviour::Arrival)
		{
			constexpr float SlowingRadius = 100.0f;
			MVelocity = arrivalBehaviour(TargetPosition, SlowingRadius);
		}
		else if (MCurrentBehaviour == SteeringBehaviour::Pursuit)
		{
			// Implement Pursuit behavior
			MVelocity = pursuitBehaviour(TargetPosition, TargetVelocity, MSpeed * 3);
		}
		else if (MCurrentBehaviour == SteeringBehaviour::Evade)
		{
			// Implement evade steering behavior
			MVelocity = evadeBehaviour(TargetPosition, TargetVelocity, MSpeed);
		}

		// Update the position based on the velocity
		MPosition += MVelocity * DeltaTime;

		// Apply border rule to wrap around to the opposite side based on entity center
		applyBorderRule();
	}
}

void AI::handleInput(sf::RenderWindow& Window)
{
	// Implement input handling logic for AI
}


sf::Vector2f AI::seekBehaviour(const sf::Vector2f& DesiredVelocity, const float Speed)
{
	return DesiredVelocity * Speed;
}

sf::Vector2f AI::fleeBehaviour(const sf::Vector2f& DesiredVelocity, const float Speed) const
{
	// To flee, we want to move away from the target position, invert the desired velocity
	// Normalize to maintain direction but reverse
	sf::Vector2f OppositeDirection = -DesiredVelocity;

	// Normalize the opposite direction vector
	const float Magnitude = std::sqrt(
		OppositeDirection.x * OppositeDirection.x + OppositeDirection.y * OppositeDirection.y);
	if (Magnitude > 0)
	{
		OppositeDirection /= Magnitude;
	}

	// Set the AI's velocity to move in the opposite direction
	return OppositeDirection * Speed;
}

sf::Vector2f AI::arrivalBehaviour(const sf::Vector2f& TargetPosition, const float SlowingRadius) const
{
	// Calculate desired velocity towards the target
	sf::Vector2f DesiredVelocity = TargetPosition - MPosition;

	// Calculate distance to target
	const float Distance = std::sqrt(DesiredVelocity.x * DesiredVelocity.x + DesiredVelocity.y * DesiredVelocity.y);

	// If the distance is within slowing radius, slow down
	if (Distance < SlowingRadius)
	{
		const float SpeedFactor = Distance / SlowingRadius;
		DesiredVelocity *= SpeedFactor;
	}

	// Return desired velocity
	return DesiredVelocity;
}

sf::Vector2f AI::pursuitBehaviour(const sf::Vector2f& TargetPosition, const sf::Vector2f& TargetVelocity,
                                  const float Speed) const
{
	// Calculate the vector from AI to user future position
	const sf::Vector2f TargetFuturePosition = TargetPosition + TargetVelocity;
	sf::Vector2f DesiredVelocity = TargetFuturePosition - MPosition;

	// Normalize desired velocity
	const float Magnitude = std::sqrt(DesiredVelocity.x * DesiredVelocity.x + DesiredVelocity.y * DesiredVelocity.y);
	if (Magnitude > 0)
	{
		DesiredVelocity /= Magnitude;
	}

	// Set the AI's velocity to move in the desired direction with the given speed
	return DesiredVelocity * Speed;
}

sf::Vector2f AI::evadeBehaviour(const sf::Vector2f& TargetPosition, const sf::Vector2f& TargetVelocity,
                                const float MaxSpeed) const
{
	// Calculate the future position of the target based on its current position and velocity
	const sf::Vector2f FutureTargetPosition = TargetPosition + TargetVelocity * MPredictionTime;

	// Calculate the vector from AI to the future position of the target
	sf::Vector2f DesiredVelocity;
	DesiredVelocity.x = MPosition.x - FutureTargetPosition.x;
	DesiredVelocity.y = MPosition.y - FutureTargetPosition.y;

	// Normalize desired velocity
	const float Magnitude = std::sqrt(DesiredVelocity.x * DesiredVelocity.x + DesiredVelocity.y * DesiredVelocity.y);
	if (Magnitude > 0)
	{
		DesiredVelocity /= Magnitude;
	}

	// Scale the desired velocity by the AI's speed
	DesiredVelocity *= MaxSpeed * 3;

	// Return the desired velocity
	return DesiredVelocity;
}


void AI::applyBorderRule()
{
	constexpr float WindowWidth = 1600.0f;
	constexpr float WindowHeight = 900.0f;
	const float HalfWidth = MSprite.getGlobalBounds().width / 2.0f;
	const float HalfHeight = MSprite.getGlobalBounds().height / 2.0f;

	// Wrap around screen borders
	if (MPosition.x + HalfWidth < 0)
	{
		MPosition.x = WindowWidth + HalfWidth; // Wrap around to the right side
	}
	else if (MPosition.x - HalfWidth > WindowWidth)
	{
		MPosition.x = -HalfWidth; // Wrap around to the left side
	}
	if (MPosition.y + HalfHeight < 0)
	{
		MPosition.y = WindowHeight + HalfHeight; // Wrap around to the bottom side
	}
	else if (MPosition.y - HalfHeight > WindowHeight)
	{
		MPosition.y = -HalfHeight; // Wrap around to the top side
	}
}

void AI::initializeWanderProperties()
{
	// Initialize wander timer
	MWanderTimer = static_cast<float>(rand() % 10 + 1); // Random wait time between 1 and 10 seconds

	// Initialize wander duration
	MWanderDuration = static_cast<float>(rand() % 5 + 2); // Random duration between 2 and 7 seconds

	// Generate random direction for wander behavior
	const float Angle = static_cast<float>(rand() % 360) * 3.14159265f / 180.f;
	MWanderDirection.x = std::cos(Angle);
	MWanderDirection.y = std::sin(Angle);
	const float Magnitude =
		std::sqrt(MWanderDirection.x * MWanderDirection.x + MWanderDirection.y * MWanderDirection.y);
	if (Magnitude > 0)
	{
		MWanderDirection /= Magnitude;
	}

	// Initialize wander speed
	MWanderSpeed = static_cast<float>(rand() % 126 + 75); // Random speed between 75 and 200 units per second
}

sf::Vector2f AI::alignment(const std::vector<AI>& Neighbours)
{
	sf::Vector2f AverageVelocity;
	for (const auto& Neighbour : Neighbours)
	{
		AverageVelocity += Neighbour.getVelocity();
	}
	if (!Neighbours.empty())
	{
		AverageVelocity /= static_cast<float>(Neighbours.size());
	}
	return AverageVelocity;
}

sf::Vector2f AI::cohesion(const std::vector<AI>& Neighbours) const
{
	sf::Vector2f CenterOfMass;
	for (const auto& Neighbour : Neighbours)
	{
		CenterOfMass += Neighbour.getPosition();
	}
	if (!Neighbours.empty())
	{
		CenterOfMass /= static_cast<float>(Neighbours.size());
	}
	const sf::Vector2f DesiredVelocity = CenterOfMass - MPosition;
	return DesiredVelocity;
}

sf::Vector2f AI::separation(const std::vector<AI>& Neighbours) const
{
	sf::Vector2f SeparationVelocity;
	for (const auto& Neighbour : Neighbours)
	{
		sf::Vector2f Direction = MPosition - Neighbour.getPosition();
		const float Distance = std::sqrt(Direction.x * Direction.x + Direction.y * Direction.y);
		if (Distance > 0)
		{
			SeparationVelocity += Direction / Distance; // Inverse proportion to distance
		}
	}
	return SeparationVelocity;
}

void AI::updateFlocking(const std::vector<AI>& Neighbours, const float DeltaTime)
{
	// Calculate individual flocking behaviours
	const sf::Vector2f AlignmentForce = alignment(Neighbours);
	const sf::Vector2f CohesionForce = cohesion(Neighbours);
	const sf::Vector2f SeparationForce = separation(Neighbours);

	// Apply weights to behaviours
	constexpr float AlignmentWeight = 1.0f;
	constexpr float CohesionWeight = 1.0f;
	constexpr float SeparationWeight = 1.0f;

	// Get total force
	const sf::Vector2f TotalForce = AlignmentForce * AlignmentWeight + CohesionForce * CohesionWeight + SeparationForce
		* SeparationWeight;

	// Update velocity based on total force
	MVelocity += TotalForce * DeltaTime;

	// Update position based on velocity
	MPosition += MVelocity * DeltaTime;

	// Apply border rule to wrap around to the opposite side based on entity center
	applyBorderRule();
}
