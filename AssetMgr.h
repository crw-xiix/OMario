#pragma once
#include "pch.h"
#include "Display.h"
#include "SDL_image.h"
#include <unordered_map>

class AssetMgr {
private:

public:
	static void Destroy();
	static std::unordered_map<std::string, SDL_Texture*> Images;
	static bool Load(std::string filename, std::string nick);
	static SDL_Texture* Get(std::string nick, int gridSize, int x, int y, SDL_Rect& dest);
	static SDL_Texture* GetAll(std::string nick, SDL_Rect& dest);
};

