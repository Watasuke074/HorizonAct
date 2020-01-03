#pragma once

//FPS�̌v�Z�E�Œ�
class _fpsManager
{
private:
	const int limitFPS = 60;
	int cnt;
	int startTime;
	int progressTime;
	double fps;
	Font font;

public:
	_fpsManager()
	{
		font = Font(20);
		cnt = 0;
		startTime = Time::GetMicrosec();
		progressTime = 0;
		fps = 0;
	}
	void Update()
	{
		//�w��FPS�ɂȂ�悤�ɑҋ@
		progressTime = Time::GetMicrosec() - startTime;
		int wait = cnt * 1000 / limitFPS - progressTime;
		if (wait > 0) System::Sleep(wait);

		//FPS�̌v�Z
		if (cnt == 0) startTime = Time::GetMicrosec();
		if (cnt == limitFPS)
		{
			int time = Time::GetMicrosec();
			fps = 1000.0 / ((time - startTime) / (double)limitFPS);
			cnt = 0;
			startTime = time;
		}
		cnt++;
		//FPS�̕\��
		Print << fps;
		font(U"FPS[{:0>4.2f}]"_fmt(fps)).draw(Arg::bottomRight(WINDOW_X,WINDOW_Y));
	}
};

//�}�b�v�̃f�[�^�Ǘ�
class _mapData
{
private:
	Grid<int> map;
public:
	_mapData() { Init(0, 0); }
	void Init(int i,int j){ map = Grid<int>(i, j, 0); }
	int Width() { return map.width(); }
	int Height() { return map.height(); }
	void Set(int y, int x, int n)
	{
		if ( (y<0||y>map.width()) || (x<0 || x>map.height()) )
			return;
		map[y][x] = n;
		Print<<map[y][x]<<U"|"<<n;
	}
	int Get(int y, int x)
	{
		if ( (y<0||y>map.width()) || (x<0 || x>map.height()) )
			return 0;
		return map[y][x];
	}
};

//�v���C���[
class _player
{
private:
	Vec2 pos;
	Size size;
	bool didSpaceDown;
	int speed_x, speed_y, jumpCnt, jumpSp;
	int scr;
	bool debug;
public:
	void Init();
	void Update(_mapData*);
	void Draw();
	void CheckMapHit(_mapData*);
	int GetScr() { return scr; }
};