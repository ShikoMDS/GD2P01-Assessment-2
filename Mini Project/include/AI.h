#pragma once

#include <iostream>

#include "Entity.h"
#include "User.h"

class AI final : public Entity
{
public:
	enum class SteeringBehaviour
	{
		Stationary,
		Wander,
		Seek,
		Flee,
		Arrival,
		Pursuit,
		Evade,
		// More steering behaviors
	};

	AI(sf::Vector2f InitialPosition, User* UserPtr);

	//----------Set/Get----------//
	void setBehaviour(SteeringBehaviour Behavior);
	void setTexture(const sf::Texture& TextureRef);
	sf::FloatRect getGlobalBounds() const;
	void setPosition(const sf::Vector2f& Position);
	SteeringBehaviour getBehaviour() const;
	void setPredictionTime(float Time);
	//----------Set/Get----------//

	//----------Window----------//
	void draw(sf::RenderWindow& Window) override;
	void update(float DeltaTime) override;
	void handleInput(sf::RenderWindow& Window) override;
	//----------Window----------//

	//----------Actions----------//
	static sf::Vector2f seekBehaviour(const sf::Vector2f& DesiredVelocity, float Speed);
	sf::Vector2f fleeBehaviour(const sf::Vector2f& DesiredVelocity, float Speed) const;
	sf::Vector2f arrivalBehaviour(const sf::Vector2f& TargetPosition, float SlowingRadius) const;
	sf::Vector2f pursuitBehaviour(const sf::Vector2f& TargetPosition, const sf::Vector2f& TargetVelocity, float Speed) const;
	sf::Vector2f evadeBehaviour(const sf::Vector2f& TargetPosition, const sf::Vector2f& TargetVelocity, float MaxSpeed) const;
	//----------Actions----------//

private:
	SteeringBehaviour MCurrentBehaviour;
	User* MUser;

	float MSpeed;

	float MPredictionTime;

	float MWanderTimer{};
	float MWanderDuration{};
	sf::Vector2f MWanderDirection;
	float MWanderSpeed{};

	void applyBorderRule();


	void initializeWanderProperties();
	static sf::Vector2f alignment(const std::vector<AI>& Neighbours);
	sf::Vector2f cohesion(const std::vector<AI>& Neighbours) const;
	sf::Vector2f separation(const std::vector<AI>& Neighbours) const;
	void updateFlocking(const std::vector<AI>& Neighbours, float DeltaTime);
};
