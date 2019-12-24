#pragma once

class _player
{
private:
	Vec2 pos;
	Size size;
	bool didSpaceDown;
	int speed_x, speed_y, jumpCnt;
	int mapData[MAP_HEIGHT][MAP_WIDTH];
	bool debug;
public:
	void Init(int map[MAP_HEIGHT][MAP_WIDTH]);
	void Update(int);
	void Draw();
	void CheckMapHit(int);
};