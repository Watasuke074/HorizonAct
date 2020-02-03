﻿#include "main.h"

void Main()
{
	double resize = 0.8;
	Scene::SetBackground(ColorF(0.1));
	Graphics::SetTargetFrameRateHz(60);
	Window::SetStyle(WindowStyle::Sizable);
	//Window::Resize(WINDOW_X * resize, WINDOW_Y * resize);
	Window::Maximize();
	Scene::Resize(WINDOW_SIZE);

	_sceneManager scene;
	while (System::Update())
	{
		if (KeyLControl.pressed())
		{
			Stopwatch t;
			t.restart();
			while(t < 1s);
		}
		ClearPrint();
		scene.Update();
	}
}
