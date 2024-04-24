#include "Application.h"

Application::Application(): window(nullptr), renderer(nullptr)
{
}

Application::~Application()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_FreeMusic(theme);

	window = nullptr;
	renderer = nullptr;
	theme = nullptr;

	delete rayCaster;
	delete gameMap;
	delete player;
	delete shotgun;
	delete objectRenderer;
	delete objectHandler;

	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool Application::Init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	} else {
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
		}
		window = SDL_CreateWindow("ENOLCMOOD", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
		SDL_ShowCursor(SDL_DISABLE);
		SDL_SetRelativeMouseMode(SDL_TRUE);
		if (window == nullptr) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		} else {
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}
				Mix_Volume(-1, 15);
				theme = Mix_LoadMUS("./assets/sounds/theme.mp3");
				bloodTexture = LoadTexture("./assets/textures/blood_screen.png", *renderer);

				gameMap = new Map(*renderer);
				player = new Player();
				rayCaster = new RayCasting();
				objectRenderer = new ObjectRenderer(*renderer, *gameMap);
				objectHandler = new ObjectHandler(*renderer);
				shotgun = new PlayerWeapon(*renderer);
			}
		}
	}

	return success;
}

// TODO
// ARRUMAR OS Destrutores
// Gerenciamento de memória ta uma merda em quase tudo. Muito alocamento dinamico desnecessário, EX: no main.cpp a application não precisa ser um ponteiro, pode ser alocado na stack
// Remover funções utilitárias das classes específicas.
//  -- Função de carregar imagens
//  -- Jogar no settings
// Generalizar algumas outras coisas.
//  -- RayCasting é um código complexo e ta duplicado
// Modularizar melhor algumas das classes. Ex: A classe da Sprite ta fazendo muita coisa numa função só
// De repente eu posso fazer uma classe TEXTURA pra ter as coisas de texturas que podem facilitar minha vida, mesma coisa com o renderer, da pra fazer um wrapper
void Application::Run()
{
	SDL_Event e;
	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;

	//Mix_PlayMusic(theme, -1);
	while (quitFlag == false) {
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();
		deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) quitFlag = true;
			if (e.type == SDL_USEREVENT && e.user.code == 0) player->Heal();
		}
		Update(deltaTime);
		Draw();
	}
}

void Application::Update(double deltaTime)
{
	InputManager::GetInstance()->Update();
	if (InputManager::GetInstance()->KeyPressed(SDL_SCANCODE_ESCAPE)) {
		quitFlag = true;
	}
	if (player->alive) {
		if (InputManager::GetInstance()->MouseButtonPressed(MOUSE_BUTTON::left) && !shotgun->shooting) {
			shotgun->shooting = true;
			player->shot = true;
			Mix_PlayChannel(3, shotgun->gunshotSound, 0);
		}
		player->Update(deltaTime, *gameMap);
		rayCaster->Update(*player, *gameMap);
		objectHandler->Update(*renderer, *player, *shotgun, *rayCaster, *gameMap);
		shotgun->Update(*renderer, *player, *rayCaster);
	} else {
		if (InputManager::GetInstance()->KeyPressed(SDL_SCANCODE_SPACE)) {
			Reset();
		}
	}
	InputManager::GetInstance()->UpdatePreviousState();
}

void Application::Draw()
{
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(renderer);
	objectRenderer->Draw(*renderer, *player, *rayCaster);
	shotgun->Draw(*renderer);
	SDL_RenderPresent(renderer);
}

void Application::Reset() {
	gameMap->Reset();
	player->Reset();
	objectHandler->Reset(*renderer);
}
