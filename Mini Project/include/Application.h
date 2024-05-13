#pragma once

#include <SFML/Graphics.hpp>
#include "User.h"
#include "AI.h"

class Application
{
public:
	Application();
	~Application();

	void setCurrentBehaviour(AI::SteeringBehaviour Behaviour);

	void run();

private:
	User MUser;
	AI::SteeringBehaviour MCurrentBehaviour;

	std::vector<AI> MAiEntities;

	void drawWindow();
	void handleInput(sf::RenderWindow& Window);
};
