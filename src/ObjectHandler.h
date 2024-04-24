#ifndef OBJECT_HANDLER_H
#define OBJECT_HANDLER_H

#include <SDL.h>
#include <vector>
#include "Sprite.h"
#include "Enemy.h"

Uint32 cooldownEvent(Uint32 interval, void* param);


class ObjectHandler
{
public:
	ObjectHandler(SDL_Renderer& renderer);
	~ObjectHandler();

	void Update(SDL_Renderer& renderer, Player& player, PlayerWeapon& weapon, RayCasting& caster, Map& map);
	void Draw2DRepresentation(SDL_Renderer& renderer, Player& player);
	void Reset(SDL_Renderer& renderer);
private:
	std::vector<std::unique_ptr<Sprite>> sprites;
	std::vector<std::unique_ptr<Enemy>> enemies;
};


#endif