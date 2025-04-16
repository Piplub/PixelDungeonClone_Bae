#include "Tiles.h"
#include "Image.h"
#include "CommonFunction.h"

Tile::Tile(int x, int y, int code) : indX(x), indY(y), tileCode(code)
{
	frameX = (int)FrameAdapter(tileCode).x;
	frameY = (int)FrameAdapter(tileCode).y;
	image = ImageManager::GetInstance()->AddImage("PixelDungeon_Tile0", L"Image/tiles0.bmp", 256, 64,
		SAMPLE_TILE_X, SAMPLE_TILE_Y);
}

void Tile::Render(HDC hdc)
{
	if (tileCode >= 0b110) //
	{
		image->FrameRender(hdc, indX, indY, frameX, frameY);
	}
}
