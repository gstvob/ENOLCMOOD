#include "ObjectRenderer.h"

// ARRUMAR UMA FORMA DE TER O RENDERER E OUTRAS PARADAS QUE SEJAM DE FACIL ACESSO PRA GERAL
// TO TENDO QUE PASSAR O MAPA, O RENDERER, O PLAYER, LARGURA E ALTURA, MUITAS VEZES

ObjectRenderer::ObjectRenderer(SDL_Renderer& renderer, Map& map)
{
	skyTexture = LoadTexture("./assets/textures/sky.png", renderer);
	bloodTexture = LoadTexture("./assets/textures/blood_screen.png", renderer);
	endGameScreen = LoadTexture("./assets/textures/game_over.png", renderer);

	for (const auto& directory : std::filesystem::recursive_directory_iterator("./assets/digits/")) {
		if (!directory.is_directory())
			digitTextures.push_back(LoadTexture(directory.path().string(), renderer));
	}

	digitsMap = {
		{'0', digitTextures[0]},
		{'1', digitTextures[1]},
		{'%', digitTextures[2]},
		{'2', digitTextures[3]},
		{'3', digitTextures[4]},
		{'4', digitTextures[5]},
		{'5', digitTextures[6]},
		{'6', digitTextures[7]},
		{'7', digitTextures[8]},
		{'8', digitTextures[9]},
		{'9', digitTextures[10]},

	};

	skyOffset = 0.0f;
}


void ObjectRenderer::Draw(SDL_Renderer& renderer, Player& player, RayCasting& caster)
{
	
	drawSkyBox(renderer);
	drawWalls(renderer, caster);
	drawPlayerHud(renderer, player);
}

void ObjectRenderer::drawWalls(SDL_Renderer& renderer, RayCasting& caster)
{
	SDL_Rect floor = { 0, HALF_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT };
	SDL_SetRenderDrawColor(&renderer, 30, 30, 30, 255);
	SDL_RenderFillRect(&renderer, &floor);

	std::sort(caster.objectsToRender.begin(), caster.objectsToRender.end(), [](const auto& e1, const auto& e2) -> bool {
		return std::get<0>(e1) > std::get<0>(e2);
		});

	for (int i = 0; i < caster.objectsToRender.size(); i++) {
		const auto& [depth, texture, rect, x, y, width, height] = caster.objectsToRender[i];
		SDL_Rect renderQuad = { x, y, width, height };
		SDL_RenderCopy(&renderer, texture, &rect, &renderQuad);
	}
}

void ObjectRenderer::drawSkyBox(SDL_Renderer& renderer)
{
	const auto [mouseX, mouseY] = InputManager::GetInstance()->MousePos();
	skyOffset = fmod((skyOffset + .5f * mouseX), WINDOW_WIDTH);

	if (skyOffset < 0) {
		skyOffset += WINDOW_WIDTH;
	}

	auto correction = mouseX < 0 ? -1 : 1;

	SDL_Rect rect;
	SDL_Rect offsetRect;
	rect = { -skyOffset, 0, WINDOW_WIDTH, HALF_HEIGHT };
	offsetRect = { -skyOffset + WINDOW_WIDTH, 0, WINDOW_WIDTH, HALF_HEIGHT };

	SDL_RenderCopy(&renderer, skyTexture, NULL, &rect);
	SDL_RenderCopy(&renderer, skyTexture, NULL, &offsetRect);
}

void ObjectRenderer::drawPlayerHud(SDL_Renderer& renderer, Player& player)
{
	if (player.tookDamage) {
		player.tookDamage = false;
		SDL_Rect rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
		SDL_RenderCopy(&renderer, bloodTexture, NULL, &rect);
	}

	std::string digitString = std::to_string(player.health);
	for (auto i = 0; i < digitString.size(); i++) {
		SDL_Rect digitRect = { i * 64, 0, 64, 64 };
		
		SDL_RenderCopy(&renderer, digitsMap[digitString.c_str()[i]], NULL, &digitRect);
	}

	SDL_Rect percentageRect = { (digitString.size()) * 64, 0, 64, 64 };
	SDL_RenderCopy(&renderer, digitsMap['%'], NULL, &percentageRect);

	if (!player.alive) {
		SDL_Rect rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
		SDL_RenderCopy(&renderer, endGameScreen, NULL, &rect);
	}

}
