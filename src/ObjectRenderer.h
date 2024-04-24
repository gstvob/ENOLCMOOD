#ifndef OBJECT_RENDERER_H
#define OBJECT_RENDERER_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <algorithm>
#include <map>
#include <filesystem>
#include "Map.h"
#include "RayCasting.h"
#include "InputManager.h"
#include "Settings.h"

class ObjectRenderer {

public:
	ObjectRenderer(SDL_Renderer& renderer, Map& map);
	~ObjectRenderer() = default;

	void Draw(SDL_Renderer& renderer, Player& player, RayCasting& caster);
private:
	SDL_Texture* skyTexture;
	SDL_Texture* bloodTexture;
	SDL_Texture* endGameScreen;
	std::vector<SDL_Texture*> digitTextures;
	std::map<char, SDL_Texture*> digitsMap;
	int skyOffset;

private:
	void drawWalls(SDL_Renderer& renderer, RayCasting& caster);
	void drawSkyBox(SDL_Renderer& renderer);
	void drawPlayerHud(SDL_Renderer& renderer, Player& player);
};


#endif