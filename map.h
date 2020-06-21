#include "pch.h"
#include <vector>



class MapTile {
public:
	int Id = -1;
	inline MapTile(int val) {
		Id = val;
	}
};

class LevelMap {
public:
	static std::vector<MapTile> Tiles;
	static void LoadLevel();
};