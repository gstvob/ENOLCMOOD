#ifndef PLAYER_WEAPON_H
#define PLAYER_WEAPON_H

#include <SDL_mixer.h>
#include "Sprite.h"
#include "Settings.h"

class PlayerWeapon : public AnimatedSprite
{
public:
	int damage = 50;
	bool shooting = false;
	Mix_Chunk* gunshotSound;
public:
	PlayerWeapon(SDL_Renderer& renderer, SDL_FPoint position = { 11.5f, 3.5f },
		std::string path = "./assets/sprites/animated/weapons/shotgun/0.png", float scale = 0.28f,
		float shift = 0.27f, float animationTime = 120);
	~PlayerWeapon();

	void Draw(SDL_Renderer& renderer);
	void RunAnimation(Player& player);
	void Update(SDL_Renderer& renderer, Player& player, RayCasting& caster);
private:
	SDL_FPoint weaponPosition;
	int frameCounter;
	int totalFrames;
};

#endif