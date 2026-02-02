#include "Game.h"
#include <cassert>
#include <SFML/Graphics.hpp>
#include "Record.h"
#include <sstream>

namespace ApplesGame
{
	void InitGame(Game& game) //init_ZD
	{
		// Load resources
		assert(game.playerTexture.loadFromFile(RESOURCES_PATH + "\\Player.png"));
		assert(game.appleTexture.loadFromFile(RESOURCES_PATH + "\\Apple.png"));
		assert(game.rockTexture.loadFromFile(RESOURCES_PATH + "\\Rock.png"));
		assert(game.eatAppleSoundBuffer.loadFromFile(RESOURCES_PATH + "\\AppleEat.wav"));
		assert(game.gameOverSoundBuffer.loadFromFile(RESOURCES_PATH + "\\Death.wav"));
		assert(game.font.loadFromFile(RESOURCES_PATH + "\\Fonts\\Roboto-Regular.ttf"));

		// —оздал меню
		game.titleText.setFont(game.font);
		game.titleText.setString("ApplesGame");
		game.titleText.setCharacterSize(48);
		game.titleText.setFillColor(sf::Color::White);
		game.titleText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f - 50.f);
		CenterText(game.titleText); // выравниваю по центру

		// Ќастройки надписи "Ќачать игру"
		game.startText.setFont(game.font);
		game.startText.setString("Start game (Press Enter)");
		game.startText.setCharacterSize(24);
		game.startText.setFillColor(sf::Color::White);
		game.startText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f + 50.f);
		CenterText(game.startText);

		//параметры текста дл€ настроек
		game.settingsText.setFont(game.font);
		game.settingsText.setString("Settings (S)");
		game.settingsText.setCharacterSize(24);
		game.settingsText.setFillColor(sf::Color::White);
		game.settingsText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f + 100.f); 
		CenterText(game.settingsText);

		// Init game objects
		game.screenRect = { 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT };

		InitPlayer(game.player, game);

		// Init apples
		game.applesCount = 0;
		game.apples = new Apple[game.applesCount]; 
		for (int i = 0; i < game.applesCount; ++i)
		{
			InitApple(game.apples[i], game);
			SetApplePosition(game.apples[i], GetRandomPositionInRectangle(game.screenRect));
		}

		// Init rocks
		for (int i = 0; i < NUM_ROCKS; ++i)
		{
			InitRock(game.rocks[i], game);
		}

		// Init background
		game.background.setSize(sf::Vector2f(game.screenRect.size.x, game.screenRect.size.y));
		game.background.setFillColor(sf::Color::Cyan);
		game.background.setPosition(0.f, 0.f);

		// Init sounds
		game.eatAppleSound.setBuffer(game.eatAppleSoundBuffer);
		game.gameOverSound.setBuffer(game.gameOverSoundBuffer);

		// Init texts
		game.scoreText.setFont(game.font);
		game.scoreText.setCharacterSize(20);
		game.scoreText.setFillColor(sf::Color::Green);
		game.scoreText.setString("Score: " + std::to_string(game.score));
		game.scoreText.setPosition(10.f, 25.f);

		game.pauseText.setFont(game.font);
		game.pauseText.setCharacterSize(48);
		game.pauseText.setFillColor(sf::Color::White);
		game.pauseText.setString("\t\tpause\npress P to restart game");
		game.pauseText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f - 50.f);
		CenterText(game.pauseText);

		game.controlsHintText.setFont(game.font);
		game.controlsHintText.setCharacterSize(20);
		game.controlsHintText.setFillColor(sf::Color::White);
		game.controlsHintText.setString("Press P for pause, ESC to exit");
		game.controlsHintText.setPosition(10.f, 5.f);

		game.gameOverText.setFont(game.font);
		game.gameOverText.setCharacterSize(48);
		game.gameOverText.setFillColor(sf::Color::White);
		game.gameOverText.setString("\t\t\t\tGame Over\nPress SPACE to restart game");
		game.gameOverText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f - 50.f);
		CenterText(game.gameOverText);

		game.leadertable.setFont(game.font);
		game.leadertable.setCharacterSize(24);
		game.leadertable.setFillColor(sf::Color::White);

		HandleGameModeMenuInput;
	}



	void StartPlayingState(Game& game)
	{
		SetPlayerPosition(game.player, { SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f });
		SetPlayerSpeed(game.player, INITIAL_SPEED);
		SetPlayerDirection(game.player, PlayerDirection::Right);

		if (game.apples != nullptr)
		{
			delete[] game.apples;
			game.apples = nullptr;
		}

		// Init apples
		int appleCount = NUM_APPLES;
		if (game.gameMode & MODE_50_APPLES)
		{
			appleCount = NUM_APPLES_EXTENDED;
		}



		game.applesCount = appleCount;
		game.apples = new Apple[game.applesCount];

		for (int i = 0; i < game.applesCount; ++i)
		{
			InitApple(game.apples[i], game);
			SetApplePosition(game.apples[i], GetRandomPositionInRectangle(game.screenRect));
		}


		// Init rocks
		for (int i = 0; i < NUM_ROCKS; ++i)
		{
			SetRockPosition(game.rocks[i], GetRandomPositionInRectangle(game.screenRect));
		}

		game.score = 0;
		game.isGameFinished = false;
		game.timeSinceGameFinish = 0;
		game.scoreText.setString("Score: " + std::to_string(game.score));
	}

	void UpdatePlayingState(Game& game, float deltaTime)
	{


		// ѕроверка паузы в игре
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
		{
			game.isGamePaused = !game.isGamePaused; // ѕереключение из состо€ни€ паузы
			sf::sleep(sf::milliseconds(200)); // добавление задержки, дл€ избежани€ множественных срабатываний
		}

		// ≈сли игра находитс€ паузе, не обновл€ем игровую логику
		if (game.isGamePaused)
		{
			return;
		}

		// Check screen borders collision
		if (!DoShapesCollide(GetPlayerCollider(game.player), game.screenRect))
		{
			StartGameoverState(game);
		}

		// Handle input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			SetPlayerDirection(game.player, PlayerDirection::Right);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			SetPlayerDirection(game.player, PlayerDirection::Up);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			SetPlayerDirection(game.player, PlayerDirection::Left);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			SetPlayerDirection(game.player, PlayerDirection::Down);
		}

		UpdatePlayer(game.player, deltaTime);

		int appleCount;
		if (game.gameMode & MODE_50_APPLES)
		{
			appleCount = NUM_APPLES_EXTENDED;
		}
		else
		{
			appleCount = NUM_APPLES;
		}

		// Find player collisions with apples
		for (int i = 0; i < game.applesCount; ++i)
		{
			if (DoShapesCollide(GetPlayerCollider(game.player), GetAppleCollider(game.apples[i])))
			{
				++game.score;
				game.eatAppleSound.play();

				if (!(game.gameMode & MODE_NO_ACCELERATION))
				{
					SetPlayerSpeed(game.player, GetPlayerSpeed(game.player) + ACCELERATION);
					if (game.player.speed > MAX_SPEED)
						game.player.speed = MAX_SPEED;
				}
				if (game.gameMode & MODE_INFINITE_APPLES)
				{
					SetApplePosition(game.apples[i], GetRandomPositionInRectangle(game.screenRect));
				}
				else
				{
					//  скрываем €блоко за пределеми экрана
					SetApplePosition(game.apples[i], { -100, -100 }); // скрываем €блоко
				}
				game.scoreText.setString("Score: " + std::to_string(game.score));
			}
		}


		int targetApples;
		if (game.gameMode & MODE_50_APPLES)
		{
			targetApples = NUM_APPLES_EXTENDED;
		}
		else
		{
			targetApples = NUM_APPLES;
		}

		//win conditions
		if (!(game.gameMode & MODE_INFINITE_APPLES) && game.score >= targetApples)
		{
			game.isGameFinished = true;
			game.timeSinceGameFinish = 0.f;
		}


		// Find player collisions with rocks
		for (int i = 0; i < NUM_ROCKS; ++i)
		{
			if (DoShapesCollide(GetPlayerCollider(game.player), GetRockCollider(game.rocks[i])))
			{
				StartGameoverState(game);
			}
		}


		if (game.isGameFinished && (game.gameMode & MODE_INFINITE_APPLES))

		{
			for (auto& rec : leaderboard)
			{
				if (rec.name == "Player" && game.score > rec.score)
				{
					rec.score = game.score;
				}
			}
			SortLeaderboard(leaderboard);
		}


	}

	void StartGameoverState(Game& game)
	{
		// ѕровер€ем, если это бесконечный режим, то обновл€ем рекорд
		if (game.gameMode & MODE_INFINITE_APPLES)
		{
			for (auto& rec : leaderboard)
			{
				if (rec.name == "Player" && game.score > rec.score)
				{
					rec.score = game.score;
				}
			}
			SortLeaderboard(leaderboard);
		}

		game.score = 0;
		game.isGameFinished = true;
		game.timeSinceGameFinish = 0.f;
		game.gameOverSound.play();


	}

	void UpdateGameoverState(Game& game, float deltaTime)
	{
		// ѕроверка на нажатие клавиши ѕробела
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			//  back to mainmenu

			game.isInMainMenu = true;
		}

	}



	void CenterText(sf::Text& text)
	{
		sf::FloatRect bounds = text.getLocalBounds();
		text.setOrigin(bounds.width / 2, bounds.height / 2);
	}

	void HandleMainMenuInput(Game& game)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			game.isInMainMenu = false; // Ќачинаем игру

		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			game.isInMainMenu = false;
			game.isInSettingsMenu = true;  // меню настроек
		}
	}



	void UpdateGame(Game& game, float deltaTime, sf::Event event)
	{

		if (game.isInMainMenu)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			{
				game.isInMainMenu = false;
				StartPlayingState(game);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				game.isInMainMenu = false;
				game.isInSettingsMenu = true;
			}
		}
		else if (game.isInSettingsMenu)
		{
			HandleGameModeMenuInput(game, event);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
			{
				game.isInSettingsMenu = false;
				game.isInMainMenu = true; // возврат в главное меню
			}
		}

		else
		{
			// Update game state
			if (!game.isGameFinished)
			{
				UpdatePlayingState(game, deltaTime);
			}
			else
			{
				UpdateGameoverState(game, deltaTime);
			}
		}

	}

	void HandleGameModeMenuInput(Game& game, sf::Event event) // режимы игры
	{
		if (game.isInSettingsMenu)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::Num1: // стандартный режим (20 €блок с ускорением)
				game.gameMode = MODE_DEFAULT;
				break;

			case sf::Keyboard::Num2: // бесконечное количество €блок с ускорением
				game.gameMode = MODE_INFINITE_APPLES;
				break;

			case sf::Keyboard::Num3: // 20 €блок без ускорени€
				game.gameMode = MODE_NO_ACCELERATION;
				break;

			case sf::Keyboard::Num4: // бесконечное количество €блок без ускорени€
				game.gameMode = MODE_INFINITE_APPLES | MODE_NO_ACCELERATION;
				break;

			case sf::Keyboard::Num5:
				game.gameMode = MODE_50_APPLES; //  50 €блок
				break;

			case sf::Keyboard::B:
				game.isInSettingsMenu = false;
				game.isInMainMenu = true;
				break;
			}
		}
	}


	void DrawSettingsMenu(Game& game, sf::RenderWindow& window)
	{
		window.clear();

		// заголовок меню настроек
		sf::Text settingsTitle;
		settingsTitle.setFont(game.font);
		settingsTitle.setString("Settings");
		settingsTitle.setCharacterSize(48);
		settingsTitle.setPosition(SCREEN_WIDTH / 2.f, 50.f);
		CenterText(settingsTitle);
		window.draw(settingsTitle);

		sf::Text title;
		title.setFont(game.font);
		title.setString("Select game mode");
		title.setCharacterSize(40);
		title.setFillColor(sf::Color::White);
		title.setPosition(SCREEN_WIDTH / 2.f, 100.f);
		CenterText(title);
		window.draw(title);

		sf::Text mode1;
		mode1.setFont(game.font);
		mode1.setString("1 (20 apples with acceleration)");
		mode1.setCharacterSize(24);
		mode1.setPosition(SCREEN_WIDTH / 2.f, 200.f);
		CenterText(mode1);
		window.draw(mode1);

		sf::Text mode2;
		mode2.setFont(game.font);
		mode2.setString("2 (Infinite apples with acceleration)");
		mode2.setCharacterSize(24);
		mode2.setPosition(SCREEN_WIDTH / 2.f, 250.f);
		CenterText(mode2);
		window.draw(mode2);

		sf::Text mode3;
		mode3.setFont(game.font);
		mode3.setString("3 (20 apples without acceleration)");
		mode3.setCharacterSize(24);
		mode3.setPosition(SCREEN_WIDTH / 2.f, 300.f);
		CenterText(mode3);
		window.draw(mode3);

		sf::Text mode4;
		mode4.setFont(game.font);
		mode4.setString("4 (Infinite apples without acceleration)");
		mode4.setCharacterSize(24);
		mode4.setPosition(SCREEN_WIDTH / 2.f, 350.f);
		CenterText(mode4);
		window.draw(mode4);

		sf::Text mode5;
		mode5.setFont(game.font);
		mode5.setString("5 (50 apples with acceleration)");
		mode5.setCharacterSize(24);
		mode5.setPosition(SCREEN_WIDTH / 2.f, 400.f);
		CenterText(mode5);
		window.draw(mode5);


		//  нопка "Ќазад"
		sf::Text backText;
		backText.setFont(game.font);
		backText.setString("Back to menu ( press B)");
		backText.setCharacterSize(24);
		backText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT - 450.f);
		CenterText(backText);
		window.draw(backText);
	}

	void DrawGame(Game& game, sf::RenderWindow& window)
	{

		if (game.isInMainMenu)
		{
			// отрисовка меню
			window.draw(game.titleText);
			window.draw(game.startText);
			window.draw(game.settingsText);
		}
		else if (game.isInSettingsMenu)
		{
			DrawSettingsMenu(game, window);
		}
		else
		{
			// Draw game objects
			DrawPlayer(game.player, window);


			for (int i = 0; i < game.applesCount; ++i)
			{
				DrawApple(game.apples[i], window);
			}

			for (int i = 0; i < NUM_ROCKS; ++i)
			{
				DrawRock(game.rocks[i], window);
			}

			std::stringstream scoreText;
			scoreText << "Apples collected: " << game.score;
			game.scoreText.setString(scoreText.str());
			if (!(game.gameMode & MODE_INFINITE_APPLES))
			{
				if (game.gameMode & MODE_50_APPLES)
				{
					scoreText << " / " << NUM_APPLES_EXTENDED;
				}
				else
				{
					scoreText << " / " << NUM_APPLES;
				}
			}


			// Draw texts
			if (!game.isGameFinished)
			{
				window.draw(game.controlsHintText);
				window.draw(game.scoreText);


				if (game.isGamePaused)
				{
					window.draw(game.pauseText);

				}
			}
			else
			{
				window.draw(game.gameOverText);

				float y = 300;// позицию по вертикали
				int index = 1;// место игрока в списке
				if (game.isGameFinished && (game.gameMode & MODE_INFINITE_APPLES))
				{
					for (const auto& rec : leaderboard)
					{

						game.leadertable.setPosition(200, y);
						game.leadertable.setString(std::to_string(index) + ". " + rec.name + " - " + std::to_string(rec.score));
						window.draw(game.leadertable);

						y += 30;
						++index;
					}
				}

				window.draw(game.scoreText);
			}
		}
	}

	void DeinializeGame(Game& game)
	{
		delete[] game.apples;  // очищаем в любом случае, даже если игра перезапускалась
		game.apples = nullptr;
		game.applesCount = 0;
	}
}