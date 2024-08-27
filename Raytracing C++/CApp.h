#pragma once

#include <SDL.h>
#include "Raytracer/Image.h"
#include "RayTracer/Scene.h"

class CApp
{
public:
	CApp();

	int OnExecute();
	bool OnInit();
	void OnEvent(SDL_Event* event);
	void OnLoop();
	void OnRender();
	void OnExit();

private:
	Image m_image;

	RT::Scene m_scene;

	bool isRunning;
	SDL_Window* pWindow;
	SDL_Renderer* pRenderer;
};

