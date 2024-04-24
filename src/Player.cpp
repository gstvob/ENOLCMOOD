#include "Player.h"
//
Player::Player() : playerPosition({ 1.5f, 5.0f }), lookAngle(1.0f), movementSpeed(0.004f), rotationSpeed(0.002f), health(100)
{
	woundedSound = Mix_LoadWAV("./assets/sounds/player_pain.wav");
}

Player::~Player()
{
	Mix_FreeChunk(woundedSound);
	woundedSound = nullptr;
}


void Player::HandleMovement(float deltaTime, Map& map)
{
	float sin_a = sin(lookAngle);
	float cos_a = cos(lookAngle);
	float dx = 0.0f, dy = 0.0f;

	float speed = movementSpeed * deltaTime;
	float speed_sin = speed * sin_a, speed_cos = speed * cos_a;

	if (InputManager::GetInstance()->KeyDown(SDL_SCANCODE_W)) {
		dx += speed_cos;
		dy += speed_sin;
	}
	if (InputManager::GetInstance()->KeyDown(SDL_SCANCODE_S)) {
		dx += -speed_cos;
		dy += -speed_sin;
	}
	if (InputManager::GetInstance()->KeyDown(SDL_SCANCODE_A)) {
		dx += speed_sin;
		dy += -speed_cos;
	}
	if (InputManager::GetInstance()->KeyDown(SDL_SCANCODE_D)) {
		dx += -speed_sin;
		dy += speed_cos;
	}

	float playerSizeFactor = PLAYER_SIZE / deltaTime;
	if (map.mapPositions.find({ (int)(std::get<0>(playerPosition) + dx * playerSizeFactor), (int)std::get<1>(playerPosition) }) == map.mapPositions.end()) {
		std::get<0>(playerPosition) += dx;
	}
	if (map.mapPositions.find({ (int)std::get<0>(playerPosition), (int)(std::get<1>(playerPosition) + dy * playerSizeFactor) }) == map.mapPositions.end()) {
		std::get<1>(playerPosition) += dy;
	}

	const auto [mouseX, mouseY] = InputManager::GetInstance()->MousePos();
	lookAngle += 0.0003 * mouseX * deltaTime;
	
	lookAngle = fmod(lookAngle, (2.0f * M_PI));
	if (lookAngle < 0) {
		lookAngle += 2.0f * M_PI;
	}
}

void Player::Update(float deltaTime, Map& map)
{
	HandleMovement(deltaTime, map);
}

void Player::Draw(SDL_Renderer& renderer)
{
	const auto [x, y] = playerPosition;
	SDL_Rect rect = {  x * 100 - 25 , y * 100 - 25, 50, 50 };
	SDL_SetRenderDrawColor(&renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(&renderer, &rect);
}

void Player::ReceiveDamage(float damage, SDL_Renderer& renderer)
{
	health = health - damage >= 0 ? health - damage : 0; 
	tookDamage = true;
	Mix_PlayChannel(1, woundedSound, 0);
	if (health <= 0) {
		alive = false;
	}

}

void Player::Reset()
{
	shot = false;
	tookDamage = false;
	alive = true;
	health = 100;
	playerPosition = { 1.5f, 5.0f };
	lookAngle = 1.0f;
}

void Player::Heal()
{
	if (health < 100 && health > 0) {
		health += 1;
	}
}
