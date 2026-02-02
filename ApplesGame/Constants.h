#pragma once
#include <string>


namespace ApplesGame
{
	enum GameModes// битовые маски (bit-mask)
	{
		MODE_DEFAULT = 0,          // 0000
		MODE_INFINITE_APPLES = 1,  // 0001
		MODE_NO_ACCELERATION = 2,   // 0010
		MODE_50_APPLES = 4          // 0100
	};


	const std::string RESOURCES_PATH = "Resources/";
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	const float INITIAL_SPEED = 100.f; 
	const float PLAYER_SIZE = 25.f;
	const float ACCELERATION = 20.f; 
	const int NUM_APPLES = 20;
	const float APPLE_SIZE = 25.f;
	const int NUM_ROCKS = 5;
	const float ROCK_SIZE = 10.f;
	const int NUM_APPLES_EXTENDED = 50;
	const float MAX_SPEED = 400.f;

}