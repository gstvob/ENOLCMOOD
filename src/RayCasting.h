#ifndef RAYCASTING_H
#define RAYCASTING_H

#include <math.h>
#include <SDL.h>
#include <vector>
#include "Map.h"
#include "Player.h"
#include "Settings.h"

class RayCasting 
{
public:
	std::vector<std::tuple<float, SDL_Texture*, SDL_Rect, float, float, float, float>> objectsToRender;

public:
	RayCasting();
	~RayCasting();

	void ObjectsToRender(Map& map);
	void RayCast(Player& player, Map& map);
	void Update(Player& player, Map& map);
	void Draw(SDL_Renderer& render, Map& map);

private:
	std::vector<std::tuple<float, float, int, float>> raycastingResults;
};


#endif