#pragma once

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

namespace globals
{
	bool Running = true;
	bool Paused = false;

	SDL_Window* window;
	SDL_Renderer* rend;

	Uint32 mouseState;
	int mouseX;
	int mouseY;
}