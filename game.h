
#include "pch.h"
#include "assetmgr.h"

class Player {
public:
	float x, y;
	float vx, vy;
	int w, h;
	void Draw(int x, int y);
};

class Camera {
public:
	float tx=40, ty=20;
	float CamX=0, CamY=0;
	
	Camera() = default;
	void Update();
	void Set(int x, int y);  ///Game pixels, not display pixels

};

class Collide {
public:
	enum class Where {Top, Right, Bottom, Left, None};
//	Where collide(Player player, vector<Object> objects);
};

class Game {
public:
	static const int DispSize = 20;
	static const int Mag = 3;
	static const int GridSize = 16;
	static const int MapWidth = 100;
	static const int MapHeight = 16;
	Camera camera;
	Player player;
	Game();
	void Update();
	void Render();
	void ProcessEvents();
	bool Running = true;
};