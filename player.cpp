#include "main.h"

void _player::Init()
{
	debugtxr = RenderTexture(WINDOW_SIZE,ColorF(0.1));
	debug = false;
	pos.set(480, 400);
	didSpaceDown = false;
	size.set(60, 60);
	scr = 0;
	speed_y = 0;
	speed_x = 0;
	jumpCnt = 0;
}

void _player::Update(_mapData* map)
{
	static int upsp;
	if (KeyD.down())debug = !debug;
	int sp;
	//�V�t�g�L�[�Ń_�b�V��
	if (KeyShift.pressed())
		sp = PLAYER_HIGHSPEED;
	else sp = PLAYER_SPEED;
	//���E
	if (KeyLeft.pressed())
		speed_x = -sp;
	if (KeyRight.pressed())
		speed_x = sp;
	//debug
	if(KeyUp.pressed())
		speed_y = (KeyShift.pressed())? 2:1;
	//�W�����v
	Print << U"{}(JumpCnt)"_fmt(jumpCnt);
	Print << U"{}(didSpaceDown)"_fmt(didSpaceDown);
	Print << U"{}(spacePressF)"_fmt(spacePressedFrame) << U'\n';
	if (KeySpace.pressed())
	{
		//���łɓ�e�W�����v���Ă��Ȃ��A�������ꂽ�u�ԂȂ�
		if (!didSpaceDown)
		{
			if(jumpCnt < 2)
			{
				speed_y = 0;
				spacePressedFrame = 0;
				didSpaceDown = true;
			}
		}else //�����ꑱ���Ă�����
		{
			spacePressedFrame++;
			if (spacePressedFrame < 13)
				speed_y = JUMP_POWER;
		}
	}
	else
	{
		if (didSpaceDown)
			jumpCnt++;
		spacePressedFrame = 0;
		didSpaceDown = false;
	}
	Print << U"pos:" << pos;
	Print << U"sp_y({})"_fmt(speed_y);
	//���ړ�
	pos.x += speed_x;
	//�c�ړ�
	pos.y -= speed_y;
	speed_y--;
	CheckMapHit(map);
	speed_x = 0;
	if (pos.y >= WINDOW_Y)
	{
		Init();
	}
	pos.x = Clamp(pos.x, 0.0, (double)WINDOW_X - size.x);
}
void _player::Draw()
{
	debugtxr.draw(AlphaF(0.5));
	debugtxr.clear(ColorF(0.1,0.5));
	Rect(pos.x, pos.y, size).draw(ColorF(0.3, 0.8, 0.4, 0.5));
}
//���@Hotpink
//�E�@Aquamarine
//��@Purple
//���@Yellow
void _player::CheckMapHit(_mapData* mapData)
{
	int x, y;
	//�v���C���[�̏Փˈʒu�Ǝ��ړ����ׂ��ʒu
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
	//�������̓����蔻��
	{
		//���Ɉړ����������獶�̓����蔻��
		if (speed_x < 0)
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
			}
		}
		//�E�Ɉړ�����������E�̓����蔻��
		if (speed_x > 0)
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
			}
		}
	}
	//�c�����̓����蔻��
	{
		//��F�㏸�����������̓����蔻��
		if (speed_y > 0)
		{
			//�}�b�v�`�b�v�̋��ڂɂ��Ȃ��Ȃ�
			if ((int)hit.pos.x % MAP_CHIPSIZE == 0)
			{
				//�^��
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
				//����
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
				//�E��
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
		//���F�������������牺�̓����蔻��
		if (speed_y < 0)
		{
			//�}�b�v�`�b�v�̋��ڂɂ��Ȃ��Ȃ�
			if ((int)hit.pos.x % MAP_CHIPSIZE == 0)
			{
				//�^��
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
				//����
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
				//�E��
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

	//�����蔻��

	if (hit.top || hit.bottom)
		speed_y = 0;
	if (hit.left || hit.right)
		speed_x = 0;
	if (hit.bottom)
		jumpCnt = 0;
	hit.pos.x += scr;
	pos = hit.pos;
	scr -= speed_x;
	if (scr > 0) scr = 0;
}
