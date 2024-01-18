#pragma once

#include "globals.hpp"

class Square
{
public:
	Square(int _x, int _y, bool is_activated)
	{
		rect.x = _x;
		rect.y = _y;

		rect.h = 10;
		rect.w = 10;

		activated = is_activated;
	}
	Square() = default;

	void SetActiv(bool value)
	{
		activated = value;
	}

	bool Is_Activ() const
	{
		return activated;
	}

	SDL_Rect rect;

private:
	bool activated;
};

std::vector<Square> Map;