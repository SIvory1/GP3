#pragma once
#include <SDL/SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <string>
using namespace std;


class DisplayGame
{
public:
	DisplayGame();
	~DisplayGame();
	void InitalizeDisplay();
	void ChangeBuffer();
	void ClearDisplay(float r, float g, float b, float a);

	float getX();
	float getY();

private:

	void ReturnError(std::string errorMessage);

	SDL_Window* m_window; //holds pointer to out window
	int m_screenX;
	int m_screenY;
	SDL_GLContext m_glContext;
};

