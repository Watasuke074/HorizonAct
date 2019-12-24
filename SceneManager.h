#pragma once
//�V�[���Ǘ�
class _sceneManager
{
private:
	int now = sceneName::main;
	DynamicTexture fadePic;
	double col = 0;
	bool isFadeActive = false;
	bool makePic = false;
	//�V�[��
	_gameMain gameMain;
public:
	int Now() { return now; }
	void Change(int n)
	{
		ScreenCapture::RequestCurrentFrame();
		if (n != -1) now = n;
		isFadeActive = true;
		col = 0;
	}
	void Update()
	{
		//�V�[���̍X�V
		switch (now)
		{
		case sceneName::main: gameMain.Update(); break;
		}
		//�t�F�[�h�A�E�g����
		if (isFadeActive)
		{
			if (col == 0)
				ScreenCapture::GetFrame(fadePic);
			fadePic.draw(0, 0, AlphaF(col));
			col += 0.05;
			if (col >= 1)
			{
				col = 0;
				isFadeActive = false;
			}
		}
	}
};