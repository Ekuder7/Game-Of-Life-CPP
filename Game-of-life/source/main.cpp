#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <thread>

#include "square.hpp"

bool FindIfSquareIsActiveByCoords(const std::vector<Square>& squares, int x, int y)
{
	for (const Square& square : squares)
	{
		if (square.rect.x == x && square.rect.y == y)
		{
			return square.Is_Activ();
		}
	}

	return false;
}

void SetSquareActiveByCoords(std::vector<Square>& squares, int x, int y, bool val)
{
	for (Square& square : squares)
	{
		if (square.rect.x == x && square.rect.y == y)
		{
			return square.SetActiv(val);
		}
	}
}

void UpdateRule()
{
	std::vector<Square> TempMap;

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(350));

		while (globals::Paused)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}

		TempMap = Map;

		for (Square& i : Map)
		{
			int neighbors = 0;

			//[i][ ][ ]
			//[ ][H][ ]
			//[ ][ ][ ]
			if (FindIfSquareIsActiveByCoords(TempMap, i.rect.x - 10, i.rect.y - 10))
			{
				++neighbors;
			}

			//[ ][i][ ]
			//[ ][H][ ]
			//[ ][ ][ ]
			if (FindIfSquareIsActiveByCoords(TempMap, i.rect.x, i.rect.y - 10))
			{
				++neighbors;
			}

			//[ ][ ][i]
			//[ ][H][ ]
			//[ ][ ][ ]
			if (FindIfSquareIsActiveByCoords(TempMap, i.rect.x + 10, i.rect.y - 10))
			{
				++neighbors;
			}

			//[ ][ ][ ]
			//[ ][H][i]
			//[ ][ ][ ]
			if (FindIfSquareIsActiveByCoords(TempMap, i.rect.x + 10, i.rect.y))
			{
				++neighbors;
			}

			//[ ][ ][ ]
			//[ ][H][ ]
			//[ ][ ][i]
			if (FindIfSquareIsActiveByCoords(TempMap, i.rect.x + 10, i.rect.y + 10))
			{
				++neighbors;
			}

			//[ ][ ][ ]
			//[ ][H][ ]
			//[ ][i][ ]
			if (FindIfSquareIsActiveByCoords(TempMap, i.rect.x, i.rect.y + 10))
			{
				++neighbors;
			}

			//[ ][ ][ ]
			//[ ][H][ ]
			//[i][ ][ ]
			if (FindIfSquareIsActiveByCoords(TempMap, i.rect.x - 10, i.rect.y + 10))
			{
				++neighbors;
			}

			//[ ][ ][ ]
			//[i][H][ ]
			//[ ][ ][ ]
			if (FindIfSquareIsActiveByCoords(TempMap, i.rect.x - 10, i.rect.y))
			{
				++neighbors;
			}

			if (i.Is_Activ())
			{
				if (neighbors < 2 || neighbors > 3)
				{
					i.SetActiv(false);
				}
			}
			else
			{
				if (neighbors == 3)
				{
					i.SetActiv(true);
				}
			}
		}

		std::cout << "Tick" << std::endl;
	}
}

using namespace globals;

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "Init error! " << SDL_GetError() << std::endl;
	}

	window = SDL_CreateWindow("Game of life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);
	rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// MAP
	for (int i = 0; i < 100; ++i)
	{
		for (int j = 0; j < 100; ++j)
		{
			Map.push_back(Square(j * 10, i * 10, false));
		}
	}

	std::thread UpdateRuleThread(UpdateRule);
	UpdateRuleThread.detach();

	while (Running)
	{
		{
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
				case SDL_QUIT:
					Running = false;
					break;

				case SDL_KEYDOWN:
					switch (event.key.keysym.scancode)
					{
					case SDL_SCANCODE_P:
						Paused = true;
						break;
					case SDL_SCANCODE_R:
						Paused = false;
						break;
					default:
						break;
					}
				}
			}
		}

		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);

		mouseState = SDL_GetMouseState(&mouseX, &mouseY);
		mouseX = mouseX / 10;
		mouseY = mouseY / 10;

		if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			SetSquareActiveByCoords(Map, mouseX * 10, mouseY * 10, true);
		}
		else if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT))
		{
			SetSquareActiveByCoords(Map, mouseX * 10, mouseY * 10, false);
		}

		for (Square& i : Map)
		{
			if (i.Is_Activ())
			{
				SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
			}
			else
			{
				SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
			}
			SDL_RenderFillRect(rend, &i.rect);
		}

		SDL_RenderPresent(rend);

		// 60FPS
		SDL_Delay(1000 / 60);
	}

	return 0;
}