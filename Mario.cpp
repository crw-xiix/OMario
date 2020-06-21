
#include "pch.h"
#include <iostream>
#include "game.h"
#include "display.h"

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "Failed to init SDL " << SDL_GetError() << "\n";
		return -1;
	}

	Display::Create(Game::DispSize * Game::Mag*Game::GridSize, Game::DispSize * Game::Mag *  Game::GridSize);

	Game game = Game();

	while (game.Running)
	{
		Display::Clear(40, 40, 200);   //sky blue
		game.ProcessEvents();
		game.Update();
		game.Render();
		Display::Present();
		SDL_Delay(11);
	}

	AssetMgr::Destroy();
}
