#include "Tiles.h"
#include "Image.h"
#include "CommonFunction.h"

Tile::Tile(int x, int y, int code) : indX(x), indY(y), tileCode(code)
{

	ConvertToState();

	frameX = (int)FrameAdapter(tileCode).x;
	frameY = (int)FrameAdapter(tileCode).y;
	image = ImageManager::GetInstance()->AddImage("PixelDungeon_Tile0", L"Image/tiles0.bmp", 256, 64,
		SAMPLE_TILE_X, SAMPLE_TILE_Y);
}

void Tile::ConvertToState()
{
	isEmpty = (tileCode & 0b100) == 0;
	shouldRender = (tileCode & 0b010) != 0;
	blocked = (tileCode & 0b001) != 0;
}

void Tile::ConvertToCode()
{
	isEmpty ? ClearBit(tileCode, 2) : SetBit(tileCode, 2);
	shouldRender ? SetBit(tileCode, 1) : ClearBit(tileCode, 1);
	blocked ? SetBit(tileCode, 0) : ClearBit(tileCode, 0);
}

void Tile::Render(HDC hdc)
{
	if (tileCode >= 0b110) //
	{
		image->FrameRender(hdc, indX, indY, frameX, frameY);
	}
}
