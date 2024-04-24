#include "Enemy.h"

Enemy::Enemy(SDL_Renderer& renderer, SDL_FPoint position, std::string path, float scale, float shift, 
	float animationTime, float attDist, float moveSpeed, float totalHealth, float attack, 
	float sizeFactor, float hitAcc)
	: AnimatedSprite(renderer, position, path, scale, shift, animationTime)
{
	attackSprites = GetAllSprites(renderer, spritesPath + "/attack");
	deathSprites = GetAllSprites(renderer, spritesPath + "/death");
	idleSprites = GetAllSprites(renderer, spritesPath + "/idle");
	painSprites = GetAllSprites(renderer, spritesPath + "/pain");
	walkSprites = GetAllSprites(renderer, spritesPath + "/walk");
	attackDistance = attDist;
	speed = moveSpeed;
	health = totalHealth;
	attackDamage = attack;
	size = sizeFactor;
	accuracy = hitAcc;
	alive = true;
	hit = false;
	hitSound = Mix_LoadWAV("./assets/sounds/npc_pain.wav");
	deathSound = Mix_LoadWAV("./assets/sounds/npc_death.wav");
	attackSound = Mix_LoadWAV("./assets/sounds/npc_attack.wav");
	nextStep = GetPositionInMap();
}

void Enemy::Update(SDL_Renderer& renderer, Player& player, PlayerWeapon& weapon, RayCasting& caster, Map& map, const std::vector<std::tuple<int, int>>& enemiesPositions)
{
	TickAnimation();
	GetSprite(player, caster);
	if (alive) {
		hasLineOfSight = HasLineOfSight(player, map);
		CheckForHit(player, weapon);

		if (hasLineOfSight) isFollowingPlayer = true;

		if (hit) {
			RunAnimation(painSprites);
			if (triggerAnimation) {
				hit = false;
			}
		} else if (isFollowingPlayer) {
			if (hasLineOfSight && viewerDistance < attackDistance) {
				RunAnimation(attackSprites);
				Attack(player, renderer);
			} else {
				RunAnimation(walkSprites);
				Move(player, map, enemiesPositions);
			}

		} else {
			RunAnimation(idleSprites);
		}
	}
	else {
		RunAnimation(deathSprites, false);
		hit = false;
	}
}

Enemy::~Enemy()
{
	for (auto* sprite : sprites) {
		SDL_DestroyTexture(sprite);
		sprite = nullptr;
	}
	for (auto* sprite : attackSprites) {
		SDL_DestroyTexture(sprite);
		sprite = nullptr;
	}
	for (auto* sprite : deathSprites) {
		SDL_DestroyTexture(sprite);
		sprite = nullptr;
	}
	for (auto* sprite : idleSprites) {
		SDL_DestroyTexture(sprite);
		sprite = nullptr;
	}
	for (auto* sprite : painSprites) {
		SDL_DestroyTexture(sprite);
		sprite = nullptr;
	}
	for (auto* sprite : walkSprites) {
		SDL_DestroyTexture(sprite);
		sprite = nullptr;
	}
	
	attackSprites.clear();
	deathSprites.clear();
	idleSprites.clear();
	painSprites.clear();
	walkSprites.clear();

	Mix_FreeChunk(hitSound);
	Mix_FreeChunk(deathSound);
	Mix_FreeChunk(attackSound);

	hitSound = nullptr;
	deathSound = nullptr;
	attackSound = nullptr;
}

void Enemy::CheckForHit(Player& player, PlayerWeapon& weapon)
{
	float halfSpriteWidth = textureWidth / 2;
	if (hasLineOfSight && player.shot && alive) {
		if (horizontalPositionInScreen > HALF_WIDTH - halfSpriteWidth &&
			horizontalPositionInScreen < HALF_WIDTH + halfSpriteWidth) {
			player.shot = false;
			hit = true;
			health -= weapon.damage;
			Mix_PlayChannel(2, hitSound, 0);
			if (health <= 0) {
				alive = false;
				totalAnimationTime = 40;
				Mix_PlayChannel(2, deathSound, 0);
			}
		}
	}
}

bool Enemy::HasLineOfSight(Player& player, Map& map)
{

	const auto [ox, oy] = player.GetPosition();
	const auto [xMap, yMap] = player.GetPositionInMap();
	const auto [enemyX, enemyY] = GetPositionInMap();

	if (xMap == enemyX && yMap == enemyY) {
		return true;
	}
	float rayAngle = anglePlayerAndSprite;

	float enemyDistanceV = 0, enemyDistanceH = 0;
	float wallDistanceV = 0, wallDistanceH = 0;

	float sinA = sin(rayAngle);
	float cosA = cos(rayAngle);

	float yHor = sinA > 0 ? yMap + 1 : yMap - 1e-6;
	float dy = sinA > 0 ? 1 : -1;

	float depthHor = (yHor - oy) / sinA;
	float xHor = ox + depthHor * cosA;

	float deltaDepth = dy / sinA;
	float dx = deltaDepth * cosA;

	for (auto i = 0; i < MAX_DEPTH; i++) {
		std::tuple<int, int> horizontals = { (int)xHor, (int)yHor };
		if ((int)xHor == enemyX && (int)yHor == enemyY) {
			enemyDistanceH = depthHor;
			break;
		}
		if (map.mapPositions.find(horizontals) != map.mapPositions.end()) {
			wallDistanceH = depthHor;
			break;
		}
		xHor += dx;
		yHor += dy;
		depthHor += deltaDepth;
	}


	float xVert = cosA > 0 ? xMap + 1 : xMap - 1e-6;
	dx = cosA > 0 ? 1 : -1;

	float depthVert = (xVert - ox) / cosA;
	float yVert = oy + sinA * depthVert;

	deltaDepth = dx / cosA;
	dy = deltaDepth * sinA;

	for (auto i = 0; i < MAX_DEPTH; i++) {
		std::tuple<int, int> verticals = { (int)xVert, (int)yVert };
		if ((int)xVert == enemyX && (int)yVert == enemyY) {
			enemyDistanceV = depthVert;
			break;
		}
		if (map.mapPositions.find(verticals) != map.mapPositions.end()) {
			wallDistanceV = depthVert;
			break;
		}
		xVert += dx;
		yVert += dy;
		depthVert += deltaDepth;
	}

	float enemyDistance = std::max(enemyDistanceH, enemyDistanceV);
	float wallDistance = std::max(wallDistanceH, wallDistanceV);

	if ((0 < enemyDistance && enemyDistance < wallDistance) || wallDistance == 0) {
		return true;
	}
	return false;
}

std::tuple<int, int> Enemy::PathFind(
	Map& map, 
	const std::vector<std::tuple<int, int>>& enemiesPositions, 
	const std::tuple<int, int>& start, 
	const std::tuple<int, int>& goal
)
{
	std::deque<std::tuple<int, int>> queue;
	std::map<std::tuple<int, int>, std::tuple<int, int>> visited;	
	queue.push_back(start);

	visited = { 
		{start, start},
	};

	while (!queue.empty()) {
		std::tuple<int, int> currentNode = queue.front();
		queue.pop_front();
		if (currentNode == goal) {
			break;
		}

		std::vector<std::tuple<int, int>> visitableNodes = map.mapGraph[currentNode];

		for (const auto& node : visitableNodes) {
			if (
				visited.find(node) == visited.end() && 
				std::count(enemiesPositions.begin(), enemiesPositions.end(), node) <= 0
			) {
				queue.push_back(node);
				visited[node] = currentNode;
			}
		}
	}
	
	std::vector<std::tuple<int, int>> path;
	path.push_back(goal);
	std::tuple<int, int> step = visited.find(goal) != visited.end() ? visited[goal] : start;
	
	while (step != start) {
		path.push_back(step);
		step = visited[step];
	}

	nextStep = path.back();
	return path.back();
}

void Enemy::Move(Player& player, Map& map, const std::vector<std::tuple<int, int>>& enemiesPositions)
{
	std::tuple<int, int> nextPosition = PathFind(map, enemiesPositions, GetPositionInMap(), player.GetPositionInMap());
	const auto& [x, y] = nextPosition;
	if (std::count(enemiesPositions.begin(), enemiesPositions.end(), nextPosition) <= 0) {
		float angle = atan2f(y + 0.5 - coordinate.y, x + 0.5 - coordinate.x);
		float dx = cos(angle) * speed;
		float dy = sin(angle) * speed;
		if (map.mapPositions.find({ (int)(coordinate.x + dx * size), (int)coordinate.y }) == map.mapPositions.end()) {
			coordinate.x += dx;
		}
		if (map.mapPositions.find({ (int)coordinate.x, (int)(coordinate.y + dy * size) }) == map.mapPositions.end()) {
			coordinate.y += dy;
		}
	}
}

std::deque<SDL_Texture*> Enemy::GetAllSprites(SDL_Renderer& renderer, std::string path)
{
	std::deque<SDL_Texture*> loadedSprites;
	
	for (const auto& directory : std::filesystem::recursive_directory_iterator(path)) {
		if (!directory.is_directory()) {
			loadedSprites.push_back(LoadTexture(directory.path().string(), renderer));
		}
	}

	return loadedSprites;
}

void Enemy::Draw(SDL_Renderer& renderer, Player& player)
{
	const auto& [x, y] = coordinate;
	const auto& [playerX, playerY] = player.GetPosition();
	const auto& [nextX, nextY] = nextStep;
	SDL_Rect rect = { x * 100 - 25 , y * 100 - 25, 50, 50 };
	SDL_Rect nextRect = { nextX * 100 , nextY * 100, 50, 50 };

	if (hasLineOfSight) {
		SDL_RenderDrawLine(&renderer, x * 100 - 25, y * 100 - 25, playerX * 100 - 25, playerY * 100 - 25);
	}
	SDL_SetRenderDrawColor(&renderer, 0xFF, 0xFF, 0x00, 0xFF);
	SDL_RenderFillRect(&renderer, &rect);
	SDL_SetRenderDrawColor(&renderer, 0x00, 0x00, 0xFF, 0x77);
	SDL_RenderFillRect(&renderer, &nextRect);

}

void Enemy::Attack(Player& player, SDL_Renderer& renderer)
{
	if (triggerAnimation) {
		float r = ((float)rand() / (RAND_MAX));
		if (r < accuracy) {
			player.ReceiveDamage(attackDamage, renderer);
		}
		Mix_PlayChannel(2, attackSound, 0);
	}
}
