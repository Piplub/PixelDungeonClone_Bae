#pragma once
#include "config.h"


class Image;

class Tile {
protected:
	//Index Value
	int indX;
	int indY;

	// TileCode
	int tileCode;
	/*
	0b100: not empty
	0b010: this tile should be rendered
	0b001: blocked(wall)
	*/

	bool isEmpty;
	bool shouldRender;
	bool blocked;

	int frameX{ 0 };
	int frameY{ 0 };

	Image* image{ nullptr };

public:
	Tile(int x, int y, int code);
	virtual ~Tile() {};

	void ConvertToState();
	void ConvertToCode();
	void Render(HDC hdc);

	inline bool GetIsEmpty() { return isEmpty; }
	inline bool GetShouldRender() { return shouldRender; }
	inline bool GetBlocked() { return blocked; }
};

inline POINT FrameAdapter(int tileCode)
{
	POINT p;

	switch (tileCode) {
	case 0b000:
		p.x = 0;
		p.y = 0;
		break;
	case 0b110:
		p.x = 1;
		p.y = 0;
		break;
	case 0b111:
		p.x = 4;
		p.y = 0;
		break;
	}

	return p;
}

