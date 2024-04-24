#include "PlayerWeapon.h"

PlayerWeapon::PlayerWeapon(SDL_Renderer& renderer, SDL_FPoint pos, std::string path, float scale, float shift, float animationTime)
	: AnimatedSprite(renderer, pos, path, scale, shift, animationTime)
{
	textureWidth = textureWidth * textureScale;
	textureHeight = textureHeight * textureScale;
	weaponPosition.x = (HALF_WIDTH - (int)(textureWidth / 2));
	weaponPosition.y = (WINDOW_HEIGHT - textureHeight);
	shooting = false;
	frameCounter = 0;
	totalFrames = sprites.size();
	gunshotSound = Mix_LoadWAV("./assets/sounds/shotgun.wav");
}

PlayerWeapon::~PlayerWeapon()
{
	Mix_FreeChunk(gunshotSound);
	delete gunshotSound;
}

void PlayerWeapon::Draw(SDL_Renderer& renderer)
{
	SDL_Rect renderPosition = { weaponPosition.x, weaponPosition.y, textureWidth, textureHeight };
	SDL_RenderCopy(&renderer, sprites[0], NULL, &renderPosition);
}


void PlayerWeapon::RunAnimation(Player& player)
{
	player.shot = false;
	if (triggerAnimation) {
		sprites.pop_front();
		sprites.push_back(texture);
		texture = sprites[0];
		frameCounter += 1;
		if (frameCounter >= totalFrames) {
			shooting = false;
			frameCounter = 0;
		}
	}
}


void PlayerWeapon::Update(SDL_Renderer& renderer, Player& player, RayCasting& caster)
{

	if (shooting) {
		RunAnimation(player);
		TickAnimation();
	}
}

