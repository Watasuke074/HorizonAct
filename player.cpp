#include "main.h"

void _player::Init(int map[MAP_HEIGHT][MAP_WIDTH])
{
	for (auto i:step(MAP_HEIGHT))
		for (auto j:step(MAP_WIDTH))
			mapData[i][j] = map[i][j];
	debug = false;
	pos.set(460, 400);
	didSpaceDown = false;
	size.set(60, 60);
	speed_y = 0;
	speed_x = 0;
	jumpCnt = 0;
}

void _player::Update()
{
	if (KeyD.down())debug = !debug;
	int sp;
	//�V�t�g�L�[�Ń_�b�V��
	if (KeyShift.pressed())
		sp = PLAYER_HIGHSPEED;
	else sp = PLAYER_SPEED;
	//���E
	if (KeyLeft.pressed())
	{
		//�E�Ɉړ����������猸��������
		if (speed_x > 0)
			speed_x -= 2;
		else speed_x = -sp;
	}else if (KeyRight.pressed())
	{
		if (speed_x < 0)
			speed_x += 2;
		else speed_x = sp;
	}else
	{
		if (speed_x < 0)speed_x += 1;
		if (speed_x > 0)speed_x -= 1;
	}
	//�W�����v
	if (KeySpace.pressed())
	{
		//�����ꂽ�u�ԁA���łɓ�e�W�����v���Ă��Ȃ��Ȃ�
		if (jumpCnt < 2 && !didSpaceDown)
		{
			speed_y = 0;
			jumpSp = 0;
			didSpaceDown = true;
		}
		//�����ꑱ���Ă�����
		else
		{
			jumpSp += JUMP_POWER;
			if ( jumpSp < (JUMP_POWER*7) )
				speed_y += JUMP_POWER;
		}
	}
	else
	{
		if (didSpaceDown)
			jumpCnt++;
		didSpaceDown = false;
	}
	Print << pos;
	Print << speed_y;
	//���ړ�
	pos.x += speed_x;
	//�c�ړ�
	pos.y -= speed_y;
	speed_y--;
	CheckMapHit();
	//speed_x = 0;

	//�ړ�����
	if (pos.y >= 900 - size.y)
	{
		jumpCnt = 0;
		pos.y = 900 - size.y;
		speed_y = 0;
	}
	pos.x = Clamp(pos.x, 0.0, (double)WINDOW_X - size.x);
}
void _player::Draw()
{
	Rect(pos.x, pos.y, size).draw(ColorF(0.3, 0.8, 0.4, 0.5));
}
//���@Hotpink
//�E�@Aquamarine
//��@Purple
//���@Yellow
void _player::CheckMapHit()
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
			if (debug)
				Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
				.draw(Palette::Hotpink);
			if (mapData[y][x] != 0 && hit.pos != Vec2(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE))
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
			if (debug)
				Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
				.draw(Palette::Aquamarine);
			if (mapData[y][x] != 0 && hit.pos != Vec2(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE))
			{
				hit.left = true;
				hit.pos.x = (x * MAP_CHIPSIZE) - size.x;
			}
		}
	}
	//�c�����̓����蔻��
	{
		//���F�������������牺�̓����蔻��
		if (speed_y < 0)
		{
			//�}�b�v�`�b�v�̋��ڂɂ��Ȃ��Ȃ�
			if ((int)hit.pos.x % MAP_CHIPSIZE == 0)
			{
				//�^��
				x = (hit.pos.x + (size.x / 2)) / MAP_CHIPSIZE;
				y = (hit.pos.y + size.y) / MAP_CHIPSIZE;
				if (debug)
					Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
					.draw(Palette::Yellow);
				if (mapData[y][x] != 0)
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
				if (debug)
					Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
					.draw(Palette::Yellow);
				if (mapData[y][x] != 0)
				{
					hit.bottom = true;
					hit.pos.y = (y * MAP_CHIPSIZE) - size.y;
				}
				//�E��
				x = hit.pos.x / MAP_CHIPSIZE;
				y = (hit.pos.y + size.y) / MAP_CHIPSIZE;
				if (debug)
					Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
					.draw(Palette::Yellow);
				if (mapData[y][x] != 0)
				{
					hit.bottom = true;
					hit.pos.y = (y * MAP_CHIPSIZE) - size.y;
				}
			}
		}
		//��F�㏸�����������̓����蔻��

		if (speed_y > 0)
		{
			//�}�b�v�`�b�v�̋��ڂɂ��Ȃ��Ȃ�
			if ((int)hit.pos.x % MAP_CHIPSIZE == 0)
			{
				//�^��
				x = (hit.pos.x + (size.x / 2)) / MAP_CHIPSIZE;
				y = hit.pos.y / MAP_CHIPSIZE;
				if (debug)
					Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
					.draw(Palette::Purple);
				if (mapData[y][x] != 0)
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
				if (debug)
					Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
					.draw(Palette::Purple);
				if (mapData[y][x] != 0)
				{
					hit.top = true;
					hit.pos.y = (y * MAP_CHIPSIZE) + MAP_CHIPSIZE;
				}
				//�E��
				x = hit.pos.x / MAP_CHIPSIZE;
				y = hit.pos.y / MAP_CHIPSIZE;
				if (debug)
					Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
					.draw(Palette::Purple);
				if (mapData[y][x] != 0)
				{
					hit.top = true;
					hit.pos.y = (y * MAP_CHIPSIZE) + MAP_CHIPSIZE;
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
}
/*
void _player::CheckMapHit(int scr)
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
	hit.pos.x += scr;
	//�������̓����蔻��
	{
		//���Ɉړ����������獶�̓����蔻��
		if (speed_x < 0)
		{
			x = pos.x / MAP_CHIPSIZE;
			y = (pos.y + (size.y / 2)) / MAP_CHIPSIZE;
			if (debug)
				Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
				.draw(Palette::Hotpink);
			if (mapData[y][x] != 0 && pos != Vec2(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE))
			{
				hit.right = true;
				pos.x = (x * MAP_CHIPSIZE) + MAP_CHIPSIZE;
			}
		}
		//�E�Ɉړ�����������E�̓����蔻��
		if (speed_x > 0)
		{
			x = (pos.x + size.x) / MAP_CHIPSIZE;
			y = (pos.y + (size.y / 5)) / MAP_CHIPSIZE;
			if (debug)
				Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
				.draw(Palette::Aquamarine);
			if (mapData[y][x] != 0 && pos != Vec2(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE))
			{
				hit.left = true;
				pos.x = (x * MAP_CHIPSIZE) - size.x;
			}
		}
	}
	//�c�����̓����蔻��
	{
		//���F�������������牺�̓����蔻��
		if (speed_y < 0)
		{
			//�}�b�v�`�b�v�̋��ڂɂ��Ȃ��Ȃ�
			if ((int)pos.x % MAP_CHIPSIZE == 0)
			{
				//�^��
				x = (pos.x + (size.x / 2)) / MAP_CHIPSIZE;
				y = (pos.y + size.y) / MAP_CHIPSIZE;
				if (debug)
					Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
					.draw(Palette::Yellow);
				if (mapData[y][x] != 0)
				{
					hit.bottom = true;
					pos.y = (y * MAP_CHIPSIZE) - size.y;
				}
			}
			else
			{
				//����
				x = (pos.x + size.x) / MAP_CHIPSIZE;
				y = (pos.y + size.y) / MAP_CHIPSIZE;
				if (debug)
					Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
					.draw(Palette::Yellow);
				if (mapData[y][x] != 0)
				{
					hit.bottom = true;
					pos.y = (y * MAP_CHIPSIZE) - size.y;
				}
				//�E��
				x = pos.x / MAP_CHIPSIZE;
				y = (pos.y + size.y) / MAP_CHIPSIZE;
				if (debug)
					Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
					.draw(Palette::Yellow);
				if (mapData[y][x] != 0)
				{
					hit.bottom = true;
					pos.y = (y * MAP_CHIPSIZE) - size.y;
				}
			}
		}
		//��F�㏸�����������̓����蔻��

		if (speed_y > 0)
		{
			//�}�b�v�`�b�v�̋��ڂɂ��Ȃ��Ȃ�
			if ((int)pos.x % MAP_CHIPSIZE == 0)
			{
				//�^��
				x = (pos.x + (size.x / 2)) / MAP_CHIPSIZE;
				y = pos.y / MAP_CHIPSIZE;
				if (debug)
					Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
					.draw(Palette::Purple);
				if (mapData[y][x] != 0)
				{
					hit.top = true;
					pos.y = (y * MAP_CHIPSIZE) - size.y;
				}
			}
			else
			{
				//����
				x = (pos.x + size.x) / MAP_CHIPSIZE;
				y = pos.y / MAP_CHIPSIZE;
				if (debug)
					Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
					.draw(Palette::Purple);
				if (mapData[y][x] != 0)
				{
					hit.top = true;
					pos.y = (y * MAP_CHIPSIZE) + MAP_CHIPSIZE;
				}
				//�E��
				x = pos.x / MAP_CHIPSIZE;
				y = pos.y / MAP_CHIPSIZE;
				if (debug)
					Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
					.draw(Palette::Purple);
				if (mapData[y][x] != 0)
				{
					hit.top = true;
					pos.y = (y * MAP_CHIPSIZE) + MAP_CHIPSIZE;
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
	//pos = hit.pos;
}
*/