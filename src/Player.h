#ifndef PLAYER_H
#define PLAYER_H

#include <math.h>
#include <map>
#include <tuple>

#include "InputManager.h"
#include "Settings.h"
#include "Map.h"
#include <SDL.h>
#include <SDL_mixer.h>

// DA PRA JGOAR UMAS COISAS PRA FORA O USO DE TUPLAS É INTERESSANTE, MAS VER SE O SDL_FPoint não pode ser mais util

class Player {

public:
	bool shot = false;
	bool tookDamage = false;
	bool alive = true;
	int health;

public:
	Player();
	~Player();

	void HandleMovement(float deltaTime, Map& map);
	void Update(float deltaTime, Map& map);
	void Draw(SDL_Renderer& renderer);
	void ReceiveDamage(float damage, SDL_Renderer& renderer);
	void Reset();
	void Heal();
	inline float GetAngle() { return lookAngle; }
	inline std::tuple<float, float> GetPosition() { return playerPosition; }
	inline std::tuple<int, int> GetPositionInMap() { return { (int)std::get<0>(playerPosition), (int)std::get<1>(playerPosition) }; }

private:
	std::tuple<float, float> playerPosition;
	float lookAngle;
	float movementSpeed;
	float rotationSpeed;
	Mix_Chunk* woundedSound;
};

#endif