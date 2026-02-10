#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "Constants.h"
#include "Math.h"
#include "Player.h"
#include "Apple.h"
#include "Rock.h"

namespace ApplesGame
{
	struct Game
	{
		Rectangle screenRect;
		

		Player player;
		int applesCount = NUM_APPLES;
		Apple* apples=nullptr;
		Rock rocks[NUM_ROCKS];

		// Global game data
		int score = 0;
		bool isGameFinished = false;
		bool isGamePaused = false;
		float timeSinceGameFinish = 0.f;
		bool isInMainMenu = true;
		bool isInSettingsMenu = false;
		unsigned int gameMode = MODE_DEFAULT;


		// Resources
		sf::Texture playerTexture;
		sf::Texture appleTexture;
		sf::Texture rockTexture;

		sf::SoundBuffer eatAppleSoundBuffer;
		sf::SoundBuffer gameOverSoundBuffer;
		sf::Sound eatAppleSound;
		sf::Sound gameOverSound;
		
		sf::RectangleShape background;
		sf::Font font;
		sf::Text scoreText;
		sf::Text pauseText;
		sf::Text controlsHintText;
		sf::Text gameOverText;
		sf::Text leadertable;

		//МЕНЮ
		 // Добавляем флаг меню
		sf::Text titleText;       // текст "Apples game"
		sf::Text startText;       // текст "Начать игру (Enter)"
		sf::Text settingsText;  //кнопка "Настройки"

	};

	void InitGame(Game& game);
	void StartPlayingState(Game& game);
	void UpdatePlayingState(Game& game, float deltaTime);
	void StartGameoverState(Game& game);
	void UpdateGameoverState(Game& game, float deltaTime);
	void CenterText(sf::Text& text);
	void HandleMainMenuInput(Game& game);
	void UpdateGame(Game& game, float deltaTime, sf::Event event);
	void HandleGameModeMenuInput(Game& game, sf::Event event);
	void DrawSettingsMenu(Game& game, sf::RenderWindow& window);
	void DrawGame(Game& game, sf::RenderWindow& window);
	void DeinializeGame(Game& game);
}
