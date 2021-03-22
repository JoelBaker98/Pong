#include <windows.h>
#include "Ball.h"
#include "PongMain.h"

extern HINSTANCE hInst;

Ball::Ball()
{
	hbitmap = LoadBitmap(hInst, "Ball1");
	hbitmask = CreateBitmap(BallSize, BallSize, 0, 0, NULL);

	hdcBitmap = CreateCompatibleDC(NULL);
	hdcMask = CreateCompatibleDC(NULL);

	SelectObject(hdcBitmap, hbitmap);
	SelectObject(hdcMask, hbitmask);

	SelectObject(hdcMask, GetStockObject(BLACK_BRUSH));
	Rectangle(hdcMask, 0, 0, BallSize, BallSize);

	SelectObject(hdcMask, GetStockObject(WHITE_BRUSH));
	Ellipse(hdcMask, 0, 0, BallSize, BallSize);

	BitBlt (hdcBitmap, 0, 0, BallSize, BallSize, 
              hdcMask, 0, 0, SRCAND);

	BitBlt (hdcMask, 0, 0, BallSize, BallSize, 
			  hdcMask, 0, 0, NOTSRCCOPY);

	DeleteObject(hbitmap);
	DeleteObject(hbitmask);
}


Ball::~Ball()
{
	DeleteDC(hdcBitmap);
	DeleteDC(hdcMask);
}


void Ball::Change(char *Name)
{
	if(hdcBitmap || hdcMask)
		this->~Ball();
	hbitmap = LoadBitmap(hInst, Name);
	hbitmask = CreateBitmap(BallSize, BallSize, 0, 0, NULL);

	hdcBitmap = CreateCompatibleDC(NULL);
	hdcMask = CreateCompatibleDC(NULL);

	SelectObject(hdcBitmap, hbitmap);
	SelectObject(hdcMask, hbitmask);

	SelectObject(hdcMask, GetStockObject(BLACK_BRUSH));
	Rectangle(hdcMask, 0, 0, BallSize, BallSize);

	SelectObject(hdcMask, GetStockObject(WHITE_BRUSH));
	Ellipse(hdcMask, 0, 0, BallSize, BallSize);

	BitBlt (hdcBitmap, 0, 0, BallSize, BallSize, 
              hdcMask, 0, 0, SRCAND);

	BitBlt (hdcMask, 0, 0, BallSize, BallSize, 
			  hdcMask, 0, 0, NOTSRCCOPY);

	DeleteObject(hbitmap);
	DeleteObject(hbitmask);
}


void Ball::Draw(HDC &hdc, int X, int Y)
{
	BitBlt(hdc, X, Y, BallSize, BallSize, hdcMask, 0, 0, SRCAND);
	BitBlt(hdc, X, Y, BallSize, BallSize, hdcBitmap, 0, 0, SRCPAINT);
}