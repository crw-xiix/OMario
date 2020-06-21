#include "pch.h"
#include "game.h"
#include "map.h"

void Player::Draw(int x, int y)
{
}

void Camera::Update()
{
}

void Camera::Set(int x, int y)
{
	//Y doesn't matter here
	CamX = x - ((Game::DispSize*Game::GridSize) / 2);
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

	player.x = 40;
	player.y = 140;
	player.w = 16;
	player.h = 16;
	player.vx = 0;
	player.vy = 0;
}

void Game::Update()
{

	const uint8_t* ks = SDL_GetKeyboardState(NULL);
	player.vx = 0;
	if (ks[SDL_SCANCODE_A]) {
		player.vx = -3;
	}
	if (ks[SDL_SCANCODE_D]) {		player.vx = +3;
	}/*
	if (ks[SDL_SCANCODE_SPACE]) {
		player.vy -= 1;
	}*/

	player.vy += 0.10;


	//Collision time.

	SDL_Rect playerC = { (int)player.x, (int)player.y, player.w, player.h };
	SDL_Rect playerV = playerC;
	//Forecast the location
	playerV.x += player.vx;
	playerV.y += player.vy;
	bool touching = false;
	for (int i = 0; i < LevelMap::Tiles.size(); i++) {
		SDL_Rect block;
		block.x = (i % (MapWidth))*GridSize;
		block.y = (i / (MapWidth))*GridSize;
		block.h = GridSize;
		block.w = GridSize;
		if (LevelMap::Tiles[i].Id != 3) continue;
		SDL_Rect result;
		if (SDL_IntersectRect(&block, &playerV, &result)) {
			
			if (result.w > result.h) {
				player.vy = 0;
				touching = true;
			}
			if (result.w < result.h) {
				player.vx = 0;
			}
		}
	}
	
	player.x += player.vx;
	player.y += player.vy;


	if (touching) {
		if (ks[SDL_SCANCODE_SPACE]) {
			player.vy -= 6;
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
