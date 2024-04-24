#include "Sprite.h"

Sprite::Sprite(SDL_Renderer& renderer, SDL_FPoint position, std::string path, float scale, float shift) : coordinate(position)
{
	texture = LoadTexture(path, renderer);
	SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);
	textureRatio = (float)textureWidth / (float)textureHeight;
	textureScale = scale;
	heightShift = shift;
}

Sprite::~Sprite()
{
	SDL_DestroyTexture(texture);
	texture = nullptr;
}

void Sprite::GetSprite(Player& player, RayCasting& caster)
{
	const auto [px, py] = player.GetPosition();
	float dx = coordinate.x - px;
	float dy = coordinate.y - py;

	anglePlayerAndSprite = atan2f(dy, dx);
	float delta = anglePlayerAndSprite - player.GetAngle();

	if ((dx > 0 && player.GetAngle() > M_PI) || (dx < 0 && dy < 0)) {
		delta += 2.0f * M_PI;
	}

	float deltaRays = delta / DELTA_ANGLE;
	horizontalPositionInScreen = (HALF_NUM_RAYS + deltaRays) * SCALE;

	viewerDistance = hypotf(dx, dy);
	viewerDistance = viewerDistance * cosf(delta);

	// DAQUI PRA BAIXO DA PRA JOGAR PRA UMA OUTRA FUNÇÃO.

	float windowWidth = HALF_WIDTH * 2;

	float halfTextureWidth = (float)textureWidth / 2;
	if ((-halfTextureWidth < horizontalPositionInScreen && horizontalPositionInScreen < (windowWidth + halfTextureWidth)) && viewerDistance > 0.5f) {
		float projection = SCREEN_DIST / viewerDistance * textureScale;
		float projectionWidth = projection * textureRatio, projectionHeight = projection;

		positionInScreen.x = horizontalPositionInScreen - (int)(projectionWidth / 2);
		positionInScreen.y = HALF_HEIGHT - (int)(projectionHeight / 2) + (projectionHeight * heightShift);

		SDL_Rect clipRect = { 0, 0, textureWidth, textureHeight };
		caster.objectsToRender.push_back({ viewerDistance, texture, clipRect, positionInScreen.x, positionInScreen.y, projectionWidth, projectionHeight });
	}
}


void Sprite::Update(SDL_Renderer& renderer, Player& player, RayCasting& caster)
{
	GetSprite(player, caster);
}




AnimatedSprite::AnimatedSprite(SDL_Renderer& renderer, SDL_FPoint pos, std::string path, float scale, float shift, float animationTime) 
	: Sprite(renderer, pos, path, scale, shift)
{
	totalAnimationTime = animationTime;
	const size_t lastSeparator = path.rfind('/');
	if (std::string::npos != lastSeparator)
	{
		spritesPath = path.substr(0, path.rfind('/'));
	}
	previousAnimationTime = SDL_GetTicks();
	triggerAnimation = false;
	GetAllSprites(renderer);
}

void AnimatedSprite::RunAnimation(std::deque<SDL_Texture*>& listToAnimate, bool loop)
{
	if (triggerAnimation) {
		SDL_Texture* previousFrame = listToAnimate.size() > 0 ? listToAnimate[0] : texture;

		if (listToAnimate.size() > 0)
			listToAnimate.pop_front();

		if (loop) {
			listToAnimate.push_back(previousFrame);
		}
		texture = listToAnimate.size() > 0 ? listToAnimate[0] : texture;
	}
}

void AnimatedSprite::Update(SDL_Renderer& renderer, Player& player, RayCasting& caster)
{
	Sprite::Update(renderer, player, caster);
	TickAnimation();
	RunAnimation(sprites);
}

void AnimatedSprite::TickAnimation()
{
	float currentTick = SDL_GetTicks();
	triggerAnimation = false;
	if (currentTick - previousAnimationTime > totalAnimationTime) {
		previousAnimationTime = currentTick;
		triggerAnimation = true;
	}
}

void AnimatedSprite::GetAllSprites(SDL_Renderer& renderer)
{
	for (const auto& directory : std::filesystem::recursive_directory_iterator(spritesPath)) {
		if (!directory.is_directory())
			sprites.push_back(LoadTexture(directory.path().string(), renderer));
	}
}

