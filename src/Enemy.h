#ifndef ENEMY_H
#define ENEMY_H

#include <algorithm>
#include <queue>
#include "Settings.h"
#include "Sprite.h"
#include "PlayerWeapon.h"

class Enemy : public AnimatedSprite
{
public:
	std::deque<SDL_Texture*> attackSprites;
	std::deque<SDL_Texture*> deathSprites;
	std::deque<SDL_Texture*> idleSprites;
	std::deque<SDL_Texture*> painSprites;
	std::deque<SDL_Texture*> walkSprites;
	
	Mix_Chunk* hitSound;
	Mix_Chunk* deathSound;
	Mix_Chunk* attackSound;

	float attackDistance, speed, size, health, attackDamage, accuracy;
	bool alive, hit;
	bool hasLineOfSight, isFollowingPlayer;
	std::tuple<int, int> nextStep;

public:
	Enemy(SDL_Renderer& renderer, SDL_FPoint position = { 10.5f, 5.5f },
		std::string path = "./assets/sprites/enemies/soldier/0.png", float scale = .6f,
		float shift = .38f, float animationTime = 180, float attackDistance = 3.0f, float speed = 0.015f, float health = 100.0f,
		float attackDamage = 10.0f, float size = 20, float accuracy = 0.15f);
	~Enemy();

	void Update(SDL_Renderer& renderer, Player& player, PlayerWeapon& weapon, RayCasting& caster, Map& map, const std::vector<std::tuple<int, int>>& enemiesPositions);
	void CheckForHit(Player& player, PlayerWeapon& weapon);
	void Move(Player& player, Map& map, const std::vector<std::tuple<int, int>>& enemiesPositions);
	void Attack(Player& player, SDL_Renderer& renderer);
	bool HasLineOfSight(Player& player, Map& map);
	std::tuple<int, int> PathFind(Map& map, const std::vector<std::tuple<int, int>>& enemiesPositions, const std::tuple<int, int>& start, const std::tuple<int, int>& goal);
	inline std::tuple<int, int> GetPositionInMap() { return { (int)coordinate.x, (int) coordinate.y}; }

	std::deque<SDL_Texture*> GetAllSprites(SDL_Renderer& renderer, std::string path);

	void Draw(SDL_Renderer& renderer, Player& player);

};

#endif