﻿#include "main.h"

void _player::Init()
{
	pos.set(480, 400);
	size.set(60, 60);
	pic = Texture(Emoji(U"😇"));
	debugtxr = RenderTexture(WINDOW_SIZE,ColorF(0.1));
	debug = false;
	didSpaceDown = false;
	scr = 0;
	speed.y = 0;
	speed.x = 0;
	jumpCnt = 0;

	afterImage.clear();
	dashingTime = 0;
	dashing = false;
	dashSp = 0;
}

void _player::Update(_mapData* map)
{
	//debug
	static int upsp;
	if (KeyD.down() && KeyAlt.down())debug = !debug;
	if(KeyUp.pressed())
		speed.y = (KeyShift.pressed())? 2:1;
	Jump();
	Dash();
	Move(map);
}
void _player::Draw()
{
	//debugtxr.draw(AlphaF(0.5));
	debugtxr.clear(ColorF(0.1,0.5));
	afterImage.update();
	//Rect(pos.x, pos.y, size)
	pic.resized(size).draw(pos);//,ColorF(0.3, 0.8, 0.4, 0.5));
}

void _player::Dash()
{
	//ダッシュ中なら更新する
	if(dashing)
	{
		speed.set(20*dashSp, 0);
		afterImage.add([ef_tex=pic, p = pos, ef_size = size](double t) {
			Point ef_pos((int)p.x, (int)p.y);
			//Rect(ef_pos, ef_size)
			ef_tex.resized(ef_size).draw(ef_pos, AlphaF(0.5 - t/0.3));//ColorF(0.3, 0.8, 0.4, 0.5 - t/0.3));
			return t < 0.5;
		});
		dashingTime++;
		if(dashingTime > 18)
		{
			dashing = false;
			dashSp = 0;
			dashingTime = 0;
		}
	}
	//ダッシュしてなかったら
	else
	{
		if(KeyD.down()) dashSp = 1;
		if(KeyA.down())	dashSp = -1;
		if(dashSp!=0)
		{
			dashing = true;
			dashingTime = 0;
		}
	}
}
void _player::Jump()
{
	Print << U"{}(JumpCnt)"_fmt(jumpCnt);
	if (KeySpace.pressed())
	{
		//すでに二弾ジャンプしていない、かつ押された瞬間なら
		if (!didSpaceDown)
		{
			if(jumpCnt < 2)
			{
				speed.y = 0;
				spacePressedFrame = 0;
				didSpaceDown = true;
			}
		}else //押され続けていたら
		{
			spacePressedFrame++;
			if (spacePressedFrame < 13)
				speed.y = JUMP_POWER;
		}
	}
	else
	{
		if (didSpaceDown)
			jumpCnt++;
		spacePressedFrame = 0;
		didSpaceDown = false;
	}
}
void _player::Move(_mapData* map)
{
	int sp;
	//シフトキーで速度アップ
	if (KeyShift.pressed())
		sp = PLAYER_HIGHSPEED;
	else sp = PLAYER_SPEED;
	//左右移動
	if(!dashing)
	{
		if (KeyLeft.pressed())  speed.x = -sp;
		if (KeyRight.pressed()) speed.x =  sp;
		//if (KeyRight.down()) speed.x = 60;
	}
	Print << U"pos:" << pos;
	Print << U"sp_y({})"_fmt(speed.y);
	//横移動
	pos.x += speed.x;
	//縦移動
	pos.y -= speed.y;
	speed.y--;
	CheckMapHit(map);
	speed.x = 0;
	if (pos.y >= WINDOW_Y) Init();
	//pos.x = Clamp(pos.x, 0.0, (double)WINDOW_X - size.x);
}
//左　Hotpink
//右　Aquamarine
//上　Purple
//下　Yellow
void _player::CheckMapHit(_mapData* mapData)
{
	int x, y;
	//プレイヤーの衝突位置と次移動すべき位置
	struct _hit
	{
		bool top, bottom, left, right;
		Vec2 pos;
	}hit;
	hit.top = false;
	hit.bottom = false;
	hit.left = false;
	hit.right = false;
	hit.pos = pos;
	hit.pos.x -= scr;
	//横方向の当たり判定
	{
		//左に移動中だったら左の当たり判定
		if (speed.x < 0)
		{
			x = hit.pos.x / MAP_CHIPSIZE;
			y = (hit.pos.y + (size.y / 2)) / MAP_CHIPSIZE;
			{
				ScopedRenderTarget2D a(debugtxr);
				if (debug)
					Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
					.draw(Palette::Hotpink);
			}
			if (mapData->Get(y,x) != 0 && hit.pos != Vec2(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE))
			{
				hit.right = true;
				hit.pos.x = (x * MAP_CHIPSIZE) + MAP_CHIPSIZE;
				speed.x = hit.pos.x-pos.x;
			}
		}
		//右に移動中だったら右の当たり判定
		if (speed.x > 0)
		{
			x = (hit.pos.x + size.x) / MAP_CHIPSIZE;
			y = (hit.pos.y + (size.y / 5)) / MAP_CHIPSIZE;
			{
				ScopedRenderTarget2D a(debugtxr);
				if (debug)
					Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
					.draw(Palette::Aquamarine);
			}
			if (mapData->Get(y,x) != 0 && hit.pos != Vec2(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE))
			{
				hit.left = true;
				hit.pos.x = (x * MAP_CHIPSIZE) - size.x;
				speed.x = hit.pos.x-pos.x;
			}
		}
	}
	//縦方向の当たり判定
	{
		//上：上昇中だったら上の当たり判定
		if (speed.y > 0)
		{
			//FIXME:speed.y>1のときに上方向にすり抜ける
			//マップチップの境目にいないなら
			if ((int)hit.pos.x % MAP_CHIPSIZE == 0)
			{
				//真下
				x = (hit.pos.x + (size.x / 2)) / MAP_CHIPSIZE;
				y = hit.pos.y / MAP_CHIPSIZE;
				{
					ScopedRenderTarget2D a(debugtxr);
					if (debug)
						Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
						.draw(Palette::Purple);
				}
				if (mapData->Get(y,x) != 0)
				{
					hit.top = true;
					hit.pos.y = (y * MAP_CHIPSIZE) - size.y;
				}
			}
			else
			{
				//左上
				x = (hit.pos.x + size.x) / MAP_CHIPSIZE;
				y = hit.pos.y / MAP_CHIPSIZE;
				{
					ScopedRenderTarget2D a(debugtxr);
					if (debug)
						Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
						.draw(Palette::Purple);
				}
				if (mapData->Get(y,x) != 0)
				{
					hit.top = true;
					hit.pos.y = (y * MAP_CHIPSIZE) + MAP_CHIPSIZE;
				}
				//右上
				x = hit.pos.x / MAP_CHIPSIZE;
				y = hit.pos.y / MAP_CHIPSIZE;
				{
					ScopedRenderTarget2D a(debugtxr);
					if (debug)
						Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
						.draw(Palette::Purple);
				}
				if (mapData->Get(y,x) != 0)
				{
					hit.top = true;
					hit.pos.y = (y * MAP_CHIPSIZE) + MAP_CHIPSIZE;
				}
			}
		}
		//下：落下中だったら下の当たり判定
		if (speed.y < 0)
		{
			//マップチップの境目にいないなら
			if ((int)hit.pos.x % MAP_CHIPSIZE == 0)
			{
				//真下
				x = (hit.pos.x + (size.x / 2)) / MAP_CHIPSIZE;
				y = (hit.pos.y + size.y) / MAP_CHIPSIZE;
				{
					ScopedRenderTarget2D a(debugtxr);
					if (debug)
						Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
						.draw(Palette::Yellow);
				}
				if (mapData->Get(y,x) != 0)
				{
					hit.bottom = true;
					hit.pos.y = (y * MAP_CHIPSIZE) - size.y;
				}
			}
			else
			{
				//左下
				x = (hit.pos.x + size.x) / MAP_CHIPSIZE;
				y = (hit.pos.y + size.y) / MAP_CHIPSIZE;
				{
					ScopedRenderTarget2D a(debugtxr);
					if (debug)
						Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
						.draw(Palette::Yellow);
				}
				if (mapData->Get(y,x) != 0)
				{
					hit.bottom = true;
					hit.pos.y = (y * MAP_CHIPSIZE) - size.y;
				}
				//右下
				x = hit.pos.x / MAP_CHIPSIZE;
				y = (hit.pos.y + size.y) / MAP_CHIPSIZE;
				{
					ScopedRenderTarget2D a(debugtxr);
					if (debug)
						Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
						.draw(Palette::Yellow);
				}
				if (mapData->Get(y,x) != 0)
				{
					hit.bottom = true;
					hit.pos.y = (y * MAP_CHIPSIZE) - size.y;
				}
			}
		}
	}

	//当たり判定

	if (hit.top || hit.bottom)
		speed.y = 0;
	if (hit.left || hit.right)
	{
		speed.x = 0;
		dashingTime = 20;//次のupdateでダッシュ終了
	}
	if (hit.bottom)
		jumpCnt = 0;
	hit.pos.x += scr;
	pos = hit.pos;
	scr -= speed.x;
	if (scr > 0) scr = 0;
}
