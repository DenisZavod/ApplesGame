#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "Constants.h"


namespace ApplesGame
{
	struct Game;

	struct Apple
	{
		Position2D position;
		sf::Sprite sprite;
	};

	void InitApple(Apple& apple, const Game& game);
	void SetApplePosition(Apple& apple, const Position2D& position);
	Circle GetAppleCollider(const Apple& apple);
	void DrawApple(Apple& apple, sf::RenderWindow& window);
}
