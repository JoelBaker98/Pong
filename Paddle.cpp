#include <windows.h>
#include "Paddle.h"
#include "PongMain.h"

extern HINSTANCE hInst;

Paddle::Paddle()
{
	hbitmap = LoadBitmap(hInst, "PADDLE1");

	hdcBitmap = CreateCompatibleDC(NULL);

	SelectObject(hdcBitmap, hbitmap);

	DeleteObject(hbitmap);
}

Paddle::~Paddle()
{
	DeleteDC(hdcBitmap);
}

void Paddle::Change(char *Name)
{
	if(hdcBitmap)
		this->~Paddle();

	hbitmap = LoadBitmap(hInst, Name);

	hdcBitmap = CreateCompatibleDC(NULL);

	SelectObject(hdcBitmap, hbitmap);

	DeleteObject(hbitmap);
}

void Paddle::Draw(HDC &hdc, int X, int Y)
{
	//BitBlt(hdc, X, Y, PaddleWidth, PaddleHeight, hdcBitmap, 0, 0, SRCPAINT);
	StretchBlt(hdc, X, Y, 15, 60, hdcBitmap, 0, 0, 15, 60, SRCCOPY);
}