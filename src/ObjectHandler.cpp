#include "ObjectHandler.h"


Uint32 cooldownEvent(Uint32 interval, void* param)
{
	SDL_Event event;
	SDL_UserEvent userevent;

	userevent.type = SDL_USEREVENT;
	userevent.code = 0;
	userevent.data1 = NULL;
	userevent.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);
	return(interval);
}

ObjectHandler::ObjectHandler(SDL_Renderer& renderer)
{
	sprites.push_back(std::make_unique<Sprite>(renderer));
	sprites.push_back(std::make_unique<AnimatedSprite>(renderer));
	sprites.push_back(std::make_unique<AnimatedSprite>(renderer, SDL_FPoint({ 1.5f, 1.5f })));
	sprites.push_back(std::make_unique<AnimatedSprite>(renderer, SDL_FPoint({ 1.5f, 7.5f })));
	sprites.push_back(std::make_unique<AnimatedSprite>(renderer, SDL_FPoint({ 5.5f, 3.25f })));
	sprites.push_back(std::make_unique<AnimatedSprite>(renderer, SDL_FPoint({ 5.5f, 4.75f })));
	sprites.push_back(std::make_unique<AnimatedSprite>(renderer, SDL_FPoint({ 7.5f, 2.5f })));
	sprites.push_back(std::make_unique<AnimatedSprite>(renderer, SDL_FPoint({ 7.5f, 5.5f })));
	sprites.push_back(std::make_unique<AnimatedSprite>(renderer, SDL_FPoint({ 14.5f, 1.5f })));
	sprites.push_back(std::make_unique<AnimatedSprite>(renderer, SDL_FPoint({ 14.5f, 7.5f }), "./assets/sprites/animated/red/0.png"));
	sprites.push_back(std::make_unique<AnimatedSprite>(renderer, SDL_FPoint({ 12.5f, 7.5f }), "./assets/sprites/animated/red/0.png"));
	sprites.push_back(std::make_unique<AnimatedSprite>(renderer, SDL_FPoint({ 9.5f, 7.5f }), "./assets/sprites/animated/red/0.png"));

	enemies.push_back(std::make_unique<Enemy>(renderer));
	enemies.push_back(std::make_unique<Enemy>(renderer, SDL_FPoint({ 11.5f, 4.5f })));
	enemies.push_back(std::make_unique<Enemy>(renderer, SDL_FPoint({ 11.0f, 19.0f })));
	enemies.push_back(std::make_unique<Enemy>(renderer, SDL_FPoint({ 13.5f, 6.5f })));
	enemies.push_back(std::make_unique<Enemy>(renderer, SDL_FPoint({ 2.0f, 20.0f })));
	enemies.push_back(std::make_unique<Enemy>(renderer, SDL_FPoint({ 4.0f, 29.0f })));
	enemies.push_back(std::make_unique<Enemy>(renderer, SDL_FPoint({ 5.5f, 14.5f }), 
		"./assets/sprites/enemies/caco_demon/0.png", 0.7f, 0.27f, 250, 1.0f, 0.03f, 150, 25, 20, 0.05f));
	enemies.push_back(std::make_unique<Enemy>(renderer, SDL_FPoint({ 5.5f, 16.5f }), "./assets/sprites/enemies/caco_demon/0.png", 0.7f, 0.27f, 250, 1.0f, 0.03f, 150, 25, 20, 0.05f));
	enemies.push_back(std::make_unique<Enemy>(renderer, SDL_FPoint({ 14.5f, 25.5f }), "./assets/sprites/enemies/cyber_demon/0.png", 1.0f, 0.04f, 210, 6.0f, 0.04f, 350, 15, 20, 0.25f));

	SDL_TimerID my_timer_id = SDL_AddTimer(300, cooldownEvent, nullptr);

}

ObjectHandler::~ObjectHandler()
{
	sprites.clear();
	enemies.clear();
}

void ObjectHandler::Update(SDL_Renderer& renderer, Player& player, PlayerWeapon& weapon, RayCasting& caster, Map& map)
{

	std::vector<std::tuple<int, int>> enemiesPositions;
	for (const auto& enemy : enemies) {
		if (enemy->alive)
			enemiesPositions.push_back(enemy->GetPositionInMap());
	}

	for (const auto& sprite : sprites) {
		sprite->Update(renderer, player, caster);
	}
	for (const auto& enemy : enemies) {
		enemy->Update(renderer, player, weapon, caster, map, enemiesPositions);
	}
}


void ObjectHandler::Draw2DRepresentation(SDL_Renderer& renderer, Player& player) {
	for (const auto& enemy : enemies) {
		enemy->Draw(renderer, player);
	}
}

void ObjectHandler::Reset(SDL_Renderer& renderer)
{
	//for (auto i = 0; i < enemies.size(); i++) {
	//	enemies[i].release();
	//}
	enemies.clear();
	enemies.push_back(std::make_unique<Enemy>(renderer));
	enemies.push_back(std::make_unique<Enemy>(renderer, SDL_FPoint({ 11.5f, 4.5f })));
	enemies.push_back(std::make_unique<Enemy>(renderer, SDL_FPoint({ 11.0f, 19.0f })));
	enemies.push_back(std::make_unique<Enemy>(renderer, SDL_FPoint({ 13.5f, 6.5f })));
	enemies.push_back(std::make_unique<Enemy>(renderer, SDL_FPoint({ 2.0f, 20.0f })));
	enemies.push_back(std::make_unique<Enemy>(renderer, SDL_FPoint({ 4.0f, 29.0f })));
	enemies.push_back(std::make_unique<Enemy>(renderer, SDL_FPoint({ 5.5f, 14.5f }),
		"./assets/sprites/enemies/caco_demon/0.png", 0.7f, 0.27f, 250, 1.0f, 0.03f, 150, 25, 20, 0.05f));
	enemies.push_back(std::make_unique<Enemy>(renderer, SDL_FPoint({ 5.5f, 16.5f }), "./assets/sprites/enemies/caco_demon/0.png", 0.7f, 0.27f, 250, 1.0f, 0.025f, 150, 25, 20, 0.05f));
	enemies.push_back(std::make_unique<Enemy>(renderer, SDL_FPoint({ 14.5f, 25.5f }), "./assets/sprites/enemies/cyber_demon/0.png", 1.0f, 0.04f, 210, 6.0f, 0.03f, 350, 15, 20, 0.25f));

}

