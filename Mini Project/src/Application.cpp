#include "Application.h"

Application::Application() : MUser(sf::Vector2f(800, 450)), MCurrentBehaviour()
{
}


Application::~Application() = default;

void Application::run()
{
	drawWindow();
}

void Application::setCurrentBehaviour(const AI::SteeringBehaviour Behaviour)
{
	MCurrentBehaviour = Behaviour;
}

void Application::drawWindow()
{
	//----------Window setup----------//
	sf::RenderWindow Window(sf::VideoMode(1600, 900), "Herta Steering Behaviours",
	                        sf::Style::Close);

	auto Image = sf::Image{};
	if (!Image.loadFromFile("resources/images/herta kuru.jpg"))
	{
		std::cout << "Image failed to load!" << std::endl;
		std::cout << "Window/task bar icon failed to load." << std::endl;
	}

	Window.setIcon(Image.getSize().x, Image.getSize().y, Image.getPixelsPtr());
	//----------Window setup----------//

	sf::Clock Clock;

	//----------Running window----------//
	while (Window.isOpen())
	{
		// Handle input events
		handleInput(Window);

		// Calculate delta time
		const float DeltaTime = Clock.restart().asSeconds();

		// Handle user input for the user character
		MUser.handleInput(Window);
		MUser.update(DeltaTime); // Update user character's position

		// Update AI entities' positions
		for (auto& Ai : MAiEntities)
		{
			Ai.update(DeltaTime);
		}

		Window.clear();
		// Draw user character
		MUser.draw(Window);

		// Draw AI entities
		for (auto& Ai : MAiEntities)
		{
			Ai.draw(Window);
		}

		Window.display();
	}
	//----------Running window----------//
}

void Application::handleInput(sf::RenderWindow& Window)
{
	sf::Event Event{};
	while (Window.pollEvent(Event))
	{
		switch (Event.type)
		{
		case sf::Event::Closed:
			Window.close();
		// Close the window if the close button is clicked - if placed elsewhere this breaks and you can't close
			break;
		//----------AI Entity Placement/Removal----------//
		case sf::Event::MouseButtonPressed:
			if (Event.mouseButton.button == sf::Mouse::Left)
			{
				// Load texture for AI entity
				sf::Texture AiTexture;
				if (!AiTexture.loadFromFile("resources/images/herta kuru.jpg"))
				{
					std::cout << "Image failed to load!" << std::endl;
					std::cout << "AI entity image failed to load." << std::endl;
				}

				// Place new AI entity with current steering behaviour at mouse position
				AI::SteeringBehaviour Behaviour = MCurrentBehaviour; // Use the currently selected steering behaviour
				AI NewAi(sf::Vector2f(Event.mouseButton.x, Event.mouseButton.y), &MUser);
				// Adjust the position to the center of the sprite
				NewAi.setPosition(sf::Vector2f(Event.mouseButton.x - AiTexture.getSize().x / 2,
				                               Event.mouseButton.y - AiTexture.getSize().y / 2));
				NewAi.setTexture(AiTexture); // Set texture for AI entity
				NewAi.setBehaviour(Behaviour); // Set steering behaviour
				MAiEntities.push_back(NewAi);
			}
			else if (Event.mouseButton.button == sf::Mouse::Right)
			{
				// Remove AI entity at mouse position
				sf::Vector2f MousePos = Window.mapPixelToCoords(sf::Vector2i(Event.mouseButton.x, Event.mouseButton.y));

				// std::remove_if to move the elements to be removed to end of the vector
				std::erase_if(MAiEntities,
				              [&](const AI& Ai) { return Ai.getGlobalBounds().contains(MousePos); });
			}
			break;
		//----------AI Entity Placement/Removal----------//
		//----------AI Steering Behaviour Cycle----------//
		case sf::Event::KeyPressed:
			if (Event.key.code == sf::Keyboard::Space)
			{
				int Index = 0;
				// Cycle through behaviours for all existing AI entities
				for (auto& Ai : MAiEntities)
				{
					switch (Ai.getBehaviour())
					{
					case AI::SteeringBehaviour::Stationary:
						Ai.setBehaviour(AI::SteeringBehaviour::Wander);
						std::cout << "Entity " << Index << ": Behaviour swapped to Wander" << std::endl;
						break;
					case AI::SteeringBehaviour::Wander:
						Ai.setBehaviour(AI::SteeringBehaviour::Seek);
						std::cout << "Entity " << Index << ": Behaviour swapped to Seek" << std::endl;
						break;
					case AI::SteeringBehaviour::Seek:
						Ai.setBehaviour(AI::SteeringBehaviour::Flee);
						std::cout << "Entity " << Index << ": Behaviour swapped to Flee" << std::endl;
						break;
					case AI::SteeringBehaviour::Flee:
						Ai.setBehaviour(AI::SteeringBehaviour::Arrival);
						std::cout << "Entity " << Index << ": Behaviour swapped to Arrival" << std::endl;
						break;
					case AI::SteeringBehaviour::Arrival:
						Ai.setBehaviour(AI::SteeringBehaviour::Pursuit);
						std::cout << "Entity " << Index << ": Behaviour swapped to Pursuit" << std::endl;
						break;
					case AI::SteeringBehaviour::Pursuit:
						Ai.setBehaviour(AI::SteeringBehaviour::Evade);
						std::cout << "Entity " << Index << ": Behaviour swapped to Evade" << std::endl;
						break;
					case AI::SteeringBehaviour::Evade:
						Ai.setBehaviour(AI::SteeringBehaviour::Stationary);
						std::cout << "Entity " << Index << ": Behaviour swapped to Stationary" << std::endl;
						break;
					}

					++Index;
				}

				switch (MCurrentBehaviour)
				{
				case AI::SteeringBehaviour::Stationary:
					MCurrentBehaviour = AI::SteeringBehaviour::Wander;
					break;
				case AI::SteeringBehaviour::Wander:
					MCurrentBehaviour = AI::SteeringBehaviour::Seek;
					break;
				case AI::SteeringBehaviour::Seek:
					MCurrentBehaviour = AI::SteeringBehaviour::Flee;
					break;
				case AI::SteeringBehaviour::Flee:
					MCurrentBehaviour = AI::SteeringBehaviour::Arrival;
					break;
				case AI::SteeringBehaviour::Arrival:
					MCurrentBehaviour = AI::SteeringBehaviour::Pursuit;
					break;
				case AI::SteeringBehaviour::Pursuit:
					MCurrentBehaviour = AI::SteeringBehaviour::Evade;
					break;
				case AI::SteeringBehaviour::Evade:
					MCurrentBehaviour = AI::SteeringBehaviour::Stationary;
					break;
				}

				std::cout << "Current AI Entity Count: " << Index << std::endl;
			}
			break;
		//----------AI Steering Behaviour Cycle----------//
		default:
			break;
		}
	}
}
