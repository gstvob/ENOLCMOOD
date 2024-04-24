#include <iostream>

#include "Application.h"
#undef main

int main() {

	/*
	Game game;

	if (game->Init())
		return game->Run();
	
	return -1;
	*/

	Application* application = new Application();

	if (application->Init()) {
		application->Run();
		return 0;
	} else {
		return -1;
	}

	return -1;
}