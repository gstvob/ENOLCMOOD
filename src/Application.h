#ifndef APPLICATION_H
#define APPLICATION_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include "InputManager.h"
#include "Settings.h"
#include "RayCasting.h"
#include "Map.h"
#include "Player.h"
#include "ObjectRenderer.h"
#include "ObjectHandler.h"
#include "PlayerWeapon.h"

class Application {

public:
	Application();
	~Application();

	bool Init();

	void Run();
	void Update(double deltaTime);
	void Draw();
	void Reset();
private: 
	SDL_Window* window;
	SDL_Renderer* renderer;
	Mix_Music* theme;
	SDL_Texture* bloodTexture;

	bool quitFlag = false;
	
	RayCasting* rayCaster;
	Map* gameMap;
	Player* player;
	ObjectRenderer* objectRenderer;
	ObjectHandler* objectHandler;
	PlayerWeapon* shotgun;

};

#endif