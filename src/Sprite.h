#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>
#include <string>
#include <memory>
#include <deque>
#include <cmath>
#include <filesystem>
#include "Map.h"
#include "Player.h"
#include "RayCasting.h"
#include "Settings.h"

class Sprite
{

public:
	SDL_Texture* texture;
	SDL_FPoint coordinate;
	SDL_FPoint positionInScreen;
	float horizontalPositionInScreen;
	float textureScale, heightShift;
	float viewerDistance;
	float anglePlayerAndSprite;
	int textureWidth, textureHeight;
	float textureRatio;

public:
	Sprite(SDL_Renderer& renderer, SDL_FPoint position = {10.5f, 3.5f}, std::string path = "./assets/sprites/static/candlebra.png", float _scale=.7f, float shift=0.27f);
	~Sprite();

	void GetSprite(Player& player, RayCasting& caster);
	virtual void Update(SDL_Renderer& renderer, Player& player, RayCasting& caster);
};


class AnimatedSprite : public Sprite
{
public:
	std::deque<SDL_Texture*> sprites;
	std::string spritesPath;
	float totalAnimationTime;
	float previousAnimationTime;
	bool triggerAnimation;
public:
	AnimatedSprite(SDL_Renderer& renderer, SDL_FPoint position = { 11.5f, 3.5f },
		std::string path = "./assets/sprites/animated/green/0.png", float scale = .7f,
		float shift = 0.27f, float animationTime = 120);

	void GetAllSprites(SDL_Renderer& renderer);
	void TickAnimation();
	void RunAnimation(std::deque<SDL_Texture*>& listToAnimate, bool loop=true);
	void Update(SDL_Renderer& renderer, Player& player, RayCasting& caster);
};

#endif 