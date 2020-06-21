#include "pch.h"
#include "game.h"
#include "map.h"
#include <vector>


void Player::Draw(int x, int y)
{
}

void Camera::Update()
{
}

void Camera::Set(int x, int y)
{
	tx = x; ty = y;
	//Y doesn't matter here - just center up
	CamX = x - ((Game::DispSize*Game::GridSize) / 2);
	//bounds
	if (CamX < 0) CamX = 0;
	if (CamX > (Game::MapWidth - Game::DispSize)*Game::GridSize) {
		CamX = ((Game::MapWidth - Game::DispSize)*Game::GridSize);
	}
}

Game::Game()
{
	AssetMgr::Load("custom.png", "BKG");
	AssetMgr::Load("50365.png", "CHR");

	LevelMap::LoadLevel();

	player.x = 47+32;
	player.y = 140;
	player.w = 16;
	player.h = 16;
	player.vx = 0;
	player.vy = 0;
}

int min(float a, float b) {
	if (a < b) return a;
	return b;
}
int counter = 0;

void Game::Update()
{
	if (Paused) return;
	const uint8_t* ks = SDL_GetKeyboardState(NULL);
	player.vx = 0;
	if (ks[SDL_SCANCODE_A]) {
		player.vx = -3;
	}
	if (ks[SDL_SCANCODE_D]) {		player.vx = +3;
	}
	//Gravity
	player.vy += 0.40;
	//Collision time.


	//Will have to fix this code so it's solid
	SDL_Rect playerC = { (int)player.x, (int)player.y, player.w, player.h };
	
	bool touching = false;
	SDL_Rect *blocks = NULL;
	SDL_Rect result;
	std::vector<SDL_Rect> collides;
	int collideW = 0;
	int collideH = 0;
	bool hit = false;

	//We can always assume, it's not in collision
	for (int i = 0; i < LevelMap::Tiles.size(); i++) {
		SDL_Rect block;
		block.x = (i % (MapWidth))*GridSize;
		block.y = (i / (MapWidth))*GridSize;
		block.h = GridSize;
		block.w = GridSize;
		if (LevelMap::Tiles[i].Id != 3) continue;
		SDL_Rect result;
		SDL_Rect playerV = playerC;
		//Forecast the location
		playerV.x += player.vx;
		playerV.y += player.vy;
		if (SDL_IntersectRect(&block, &playerV, &result)) {
			collideW += result.w;
			collideH += result.h;
			collides.push_back(result);
			hit = true;
			int playerFoot = player.y + player.h;
			if (hit) {
				//Check if we are falling
				if (player.vy > 0) {
					if (playerFoot <= block.y) {
						player.vy = 0;
						//Something is under us.
						/*
						counter++;
						int distance = (block.y - playerFoot);
						std::cout << "VelB:" << player.vy << "\n";
						player.vy = min(player.vy, distance);
						player.y = (int)player.y;
						std::cout << "VelA:" << player.vy << "\n\n";
						//Paused = true;
						if (counter >= 1) {
							int bp = 0;
						}
						*/

					}
				}
			}
		}
	}
	player.x += player.vx;
	player.y += player.vy;

	if (touching) {
		if (ks[SDL_SCANCODE_SPACE]) {
			player.vy -= 7;
		}
	}
	camera.Set(player.x, player.y);
}

void Game::Render()
{
	SDL_Rect src, dest;
	camera.Set(player.x, player.y);
	for (int x = 0; x < MapWidth; x++) {
		dest.w = GridSize * Mag;
		dest.h = GridSize * Mag;
		dest.x = ((x)* GridSize)*Mag - (camera.CamX*Mag);

		for (int y = 0; y < MapHeight; y++) {
			dest.y = y * GridSize*Mag;
			
			int mapValue = LevelMap::Tiles[((y * MapWidth) + x)].Id;
			if (mapValue == -1) continue;
			int mapX = mapValue % 26;
			int mapY = mapValue / 26;
			//Draw dude
			SDL_Texture *tex = AssetMgr::Get("BKG", GridSize, mapX, mapY, src);
			Display::DrawTexture(tex, &src, &dest);
		}
	}
	dest.x = ((int)(player.x)) *Mag -(camera.CamX*Mag);
	dest.y = ((int)(player.y)) *Mag;

	SDL_Texture *tex = AssetMgr::Get("CHR", GridSize, 5,2, src);
	Display::DrawTexture(tex, &src, &dest);
}

void Game::ProcessEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			Running = false;
		}
	}
}
