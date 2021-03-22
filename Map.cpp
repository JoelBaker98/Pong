#include <windows.h>
#include "Map.h"
#include "PongMain.h"

extern HINSTANCE hInst;

Map::Map()
{
	hbitmap = LoadBitmap(hInst, "MAP1");

	hdcBitmap = CreateCompatibleDC(NULL);

	SelectObject(hdcBitmap, hbitmap);

	DeleteObject(hbitmap);
}

Map::~Map()
{
	DeleteDC(hdcBitmap);
}

void Map::Draw(HDC &hdc)
{
	BitBlt(hdc, 0, 0, ScrWidth, ScrHeight, hdcBitmap, 0, 0, SRCPAINT);
}