#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"
#include "Timer.h"
#include "PongMain.h"
#include "DlgBox.h"
#include "Ball.h"
#include "Paddle.h"
#include "Map.h"


BOOL CALLBACK DlgSettingsProc(HWND, UINT, WPARAM, LPARAM);  //SettingsDlgBox\DlgBox.cpp
void MakeNumpadConsistant(WPARAM &wParam, LPARAM);			//SettingsDlgBox\HandleMsg's.cpp


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CheckKeyMishap(HWND &);
BOOL Collision_Detection(PaddleInfo);
void PrintFrame(HWND &);
void GetFrameRate(double &);
void CheckforCollision();
void SetBallPath(PaddleInfo, int &);
void UpdateBall();
void UpdatePaddle(PaddleInfo &, int);
void CheckifWon();
void PaintScreen(HWND &, double);


/* Global Variables */
extern DlgSettings Settings;				//Settings in Settings dialog box, declared in SettingsDlgBox\DlgBox.cpp
BOOL Focus, Pause, GameActive;				//Identifies if the focus is on the program and if it is paused
BallCrd BallCord;							//Coordinates of Ball and speed of Ball
Timer RefreshRate;							//Timer class to limit and change the FPS
PaddleInfo Paddle1, Paddle2;				//Coordinates and settings for paddles
PlayerInfo Player1, Player2;				//Players info  e.g. how many points, if they won, 
HINSTANCE hInst;							//Handle to instance of the program and DLL file
Ball Ball;
Paddle PaddleDraw1, PaddleDraw2;
/* Global Variables */


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	const TCHAR *szAppName = "Pong";
	MSG msg;
	HWND hwnd;
	WNDCLASS wndclass;
	InitCommonControls();

	msg.message = 9000000;

	hwnd = FindWindow("Pong", "Pong");
	if(hwnd) {					//If program is already running
		ShowWindow(hwnd, SW_SHOWNORMAL);
		SetForegroundWindow(hwnd);
		return 0;
	}

	wndclass.style			= CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc	= WndProc;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 500000;
	wndclass.hInstance		= hInstance;
	wndclass.hIcon			= LoadIcon(hInstance, szAppName);
	wndclass.hCursor		= LoadCursor(hInstance, szAppName);
	wndclass.hbrBackground	= NULL;
	wndclass.lpszMenuName	= szAppName;
	wndclass.lpszClassName	= szAppName;

	if(!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This Program requires Windows NT!"),
					szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("Pong"),
						WS_OVERLAPPEDWINDOW ^ (WS_MAXIMIZEBOX | WS_THICKFRAME), 
						CW_USEDEFAULT, CW_USEDEFAULT, 800, 623,
						NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);


	while(msg.message != WM_QUIT)
	{
		RefreshRate.SetStartTime();
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		PrintFrame(hwnd);
		RefreshRate.SetEndTime();
		while(RefreshRate.GetDifference() < RefreshRate.CalculateFPSChunk())
			RefreshRate.SetEndTime();
	}
	return 0;
}

HWND hwndStatus;

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static int ServeDirection;
	int A[3] = { 100, 150, 240};

	switch(Message)
	{
	case WM_CREATE:
		hInst = ((LPCREATESTRUCT) lParam)->hInstance;
		PlaySound(NULL, NULL, NULL);

		ServeDirection = 1;

		Paddle1.Paddleupspeed = Paddle2.Paddleupspeed = 
		Paddle1.Paddledownspeed = Paddle2.Paddledownspeed = 1;
		Paddle1.Direction = Paddle2.Direction = STOP;

		BallCord.X = 400;
		BallCord.Y = -100;
		BallCord.YC = BallCord.XC = GameActive = Player1.Won =
		Player2.Won = Player1.Score = Player2.Score = 0;

		SetRect(&Paddle1.RectPaddle, 10, 275, 25, 325);
		SetRect(&Paddle2.RectPaddle, 768, 275, 776, 325);

		Ball.Change(Settings.BallType);
		PaddleDraw1.Change("PADDLE1");
		PaddleDraw2.Change("PADDLE1");

		hwndStatus =
		CreateWindowEx(
        WS_EX_WINDOWEDGE,           // no extended styles
        STATUSCLASSNAME,			// name of status bar class
        "Have Fun!",				// no text when first created
        SBT_TOOLTIPS |				// includes a sizing grip
        WS_CHILD | WS_VISIBLE,      // creates a child window
        0, 0, 0, 0,					// ignores size and position
        hwnd,						// handle to parent window
        (HMENU) 40,					// child window identifier
        hInst,						// handle to application instance
        NULL);

		SendMessage(hwndStatus, WM_SETFONT, (WPARAM)GetStockObject(ANSI_VAR_FONT ), FALSE);
		SendMessage(hwndStatus, SB_SETPARTS, (WPARAM) 3, (LPARAM) &A);
		SendMessage(hwndStatus, SB_SETICON, (WPARAM) 2, (LPARAM) LoadIcon(hInst, "Pong"));
		SendMessage(hwndStatus, SB_SETTIPTEXT, (WPARAM) 2, (LPARAM) "Enjoy Playing Pong!");


		return 0;

	case WM_SETFOCUS:
		Focus = TRUE;
		return 0;

	case WM_KILLFOCUS:
		Focus = FALSE;
		return 0;

	case WM_INITMENU:
		CheckKeyMishap(hwnd);
		return 0;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDM_EXIT:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			return 0;

		case IDM_NEWGAME:
			BallCord.X = 400;
			BallCord.Y = -100;
			BallCord.YC = BallCord.XC = GameActive = Player1.Won = 
			Player2.Won = Player1.Score = Player2.Score = 0;
			SetRect(&Paddle1.RectPaddle, 10, 275, 25, 325);
			SetRect(&Paddle2.RectPaddle, 768, 275, 776, 325);
			return 0;

		case IDM_SETTINGS:
			DialogBox(hInst, "Settings", hwnd, DlgSettingsProc);
			return 0;

		case IDM_ABOUT:
			MessageBox(hwnd, TEXT("Pong by Joel Baker"), TEXT("About Pong"), MB_OK);
			return 0;
		}

		return 0;

	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		if(lParam & 1073741824)  //If the key is a repeated key no need to go through switch
			return 0;

		MakeNumpadConsistant(wParam, lParam);


		if(Settings.Player1up == wParam) {
			Paddle1.Direction += UP;
			Paddle1.KeysDown[0] = TRUE;
		}
		if(Settings.Player1down == wParam) {
			Paddle1.Direction += DOWN;
			Paddle1.KeysDown[1] = TRUE;
		}
		if(Settings.Player2up == wParam) {
			Paddle2.Direction += UP;
			Paddle2.KeysDown[0] = TRUE;
		}
		if(Settings.Player2down == wParam) {
			Paddle2.Direction += DOWN;
			Paddle2.KeysDown[1] = TRUE;
		}

		if(Settings.Pause == wParam)
			Pause = !Pause;

		if(Settings.Serve == wParam) {
			if(GameActive == FALSE && Player1.Won == FALSE && Player2.Won == FALSE) {
				if(ServeDirection == 1)
					ServeDirection -= 2;
				else
					ServeDirection += 2;
				BallCord.X = 400;
				BallCord.Y = 300;
				BallCord.XC = ServeDirection;
				BallCord.YC = 1;
				BallCord.YDirection = DOWN;
				GameActive = TRUE;
			}
		}

		break;

	case WM_SYSKEYUP:
	case WM_KEYUP:
		MakeNumpadConsistant(wParam, lParam);


		if(Settings.Player1up == wParam && Paddle1.KeysDown[0] == TRUE) {
			Paddle1.Direction -= UP;
			Paddle1.KeysDown[0] = FALSE;
			Paddle1.Paddleupspeed = 1;
		}
		if(Settings.Player1down == wParam && Paddle1.KeysDown[1] == TRUE) {
			Paddle1.Direction -= DOWN;
			Paddle1.KeysDown[1] = FALSE;
			Paddle1.Paddledownspeed = 1;
		}
		if(Settings.Player2up == wParam && Paddle2.KeysDown[0] == TRUE) {
			Paddle2.Direction -= UP;
			Paddle2.KeysDown[0] = FALSE;
			Paddle2.Paddleupspeed = 1;
		}
		if(Settings.Player2down == wParam && Paddle2.KeysDown[1] == TRUE) {
			Paddle2.Direction -= DOWN;
			Paddle2.KeysDown[1] = FALSE;
			Paddle2.Paddledownspeed = 1;
		}

		break;

	case WM_PAINT:
		BeginPaint(hwnd, &ps), EndPaint(hwnd, &ps); //Needed to validate Window
		PaintScreen(hwnd, 0);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, Message, wParam, lParam);
}



void CheckKeyMishap(HWND &hwnd) 
{
		if(Paddle1.KeysDown[0] == TRUE)
			SendMessage(hwnd, WM_KEYUP, Settings.Player1up, 0);
			
		if(Paddle1.KeysDown[1] == TRUE)
			SendMessage(hwnd, WM_KEYUP, Settings.Player1down, 0);

		if(Paddle2.KeysDown[0] == TRUE)
			SendMessage(hwnd, WM_KEYUP, Settings.Player2up, 0);

		if(Paddle2.KeysDown[1] == TRUE)
			SendMessage(hwnd, WM_KEYUP, Settings.Player2down, 0);
}



BOOL Collision_Detection(PaddleInfo Paddle) 
{
  
    int LeftBall, LeftPaddle;
    int RightBall, RightPaddle;
    int TopBall, TopPaddle;
    int BottomBall, BottomPaddle;

    LeftBall = BallCord.X+1;
	RightBall = BallCord.X + BallSize-1;
	TopBall = BallCord.Y+1;
	BottomBall = BallCord.Y + BallSize-1;

	LeftPaddle = Paddle.RectPaddle.left;
    RightPaddle = Paddle.RectPaddle.right;
    TopPaddle = Paddle.RectPaddle.top;
    BottomPaddle = Paddle.RectPaddle.top + PaddleHeight;

	if(Paddle.RectPaddle.left == 15) //if left paddle
		LeftPaddle += 7;

    if (BottomBall < TopPaddle)
		return 0;
    if (TopBall > BottomPaddle)
		return 0;
    if (RightBall < LeftPaddle)
		return 0;
    if (LeftBall > RightPaddle)
		return 0;

    return 1;
}



void PrintFrame(HWND &hwnd)
{
	static double FPS;

	if(Focus == FALSE) {
		CheckKeyMishap(hwnd);
		Sleep(10);
		return;
	}

	else if(Pause == TRUE)
		return;

	GetFrameRate(FPS);
	CheckforCollision();
	UpdateBall();
	UpdatePaddle(Paddle1, Settings.MaxPaddle1Speed);
	UpdatePaddle(Paddle2, Settings.MaxPaddle2Speed);
	CheckifWon();
	PaintScreen(hwnd, FPS);
}



void GetFrameRate(double &FPS)
{
	static Timer FrameRate;

	if(Settings.ShowFPS == BST_CHECKED) {
		FrameRate.SetEndTime();
		if(FrameRate.GetDifference() > 0)
			FPS = FrameRate.CalculateFPS();
		FrameRate.SetStartTime(FrameRate.GetEndTime());
	}
}



void CheckforCollision()
{
	static int FrameSkip;

	if(BallCord.XC == 0)
		return;

	if((FrameSkip % (400/BallCord.XC)) == 0) {
		if(Collision_Detection(Paddle1))
			SetBallPath(Paddle1, FrameSkip);

		else if(Collision_Detection(Paddle2))
			SetBallPath(Paddle2, FrameSkip);
	}

	else FrameSkip = FrameSkip++ % (400/BallCord.XC);
}



void SetBallPath(PaddleInfo Paddle, int &FrameSkip)
{
	if(BallCord.YC > 0 && BallCord.YDirection != DOWN)
		BallCord.YDirection = DOWN;
	else if(BallCord.YC < 0 && BallCord.YDirection != UP)
		BallCord.YDirection = UP;


	FrameSkip = 1;
	BallCord.XC = -BallCord.XC;

	if(Paddle.Paddleupspeed > 5) {
		if(BallCord.YC > 0 && BallCord.YDirection == DOWN)
			BallCord.YC -= Paddle.Paddleupspeed/6;
		else if(BallCord.YC < 0 && BallCord.YDirection == UP)
			BallCord.YC -= Paddle.Paddleupspeed/6;
		else if(BallCord.YC == 0)
			BallCord.YC -= Paddle.Paddleupspeed/6;
		PlaySound("BallSound1", hInst, SND_ASYNC | SND_NOWAIT/* | SND_NODEFAULT*/ | SND_RESOURCE | SND_MEMORY);
	}
	else if(Paddle.Paddledownspeed > 5) {
		if(BallCord.YC > 0 && BallCord.YDirection == DOWN)
			BallCord.YC += Paddle.Paddledownspeed/6;
		else if(BallCord.YC < 0 && BallCord.YDirection == UP)
			BallCord.YC += Paddle.Paddledownspeed/6;
		else if(BallCord.YC == 0)
			BallCord.YC += Paddle.Paddledownspeed/6;
		PlaySound("BallSound1", hInst, SND_ASYNC | SND_NOWAIT/* | SND_NODEFAULT*/ | SND_RESOURCE | SND_MEMORY);
	}	
	else if(abs(BallCord.XC) < Settings.MaxBallSpeed) {
		if(BallCord.XC > 0)
			BallCord.XC++;
		else if(BallCord.XC < 0)
			BallCord.XC--;
		PlaySound("BallSound2", hInst, SND_ASYNC | SND_NOWAIT/* | SND_NODEFAULT*/ | SND_RESOURCE | SND_MEMORY);
	}
	else
		PlaySound("BallSound2", hInst, SND_ASYNC | SND_NOWAIT/* | SND_NODEFAULT*/ | SND_RESOURCE |SND_MEMORY);
}


void UpdateBall()
{
	if(BallCord.X < -BallSize) {
		Player2.Score++;
		BallCord.X = 400;
		BallCord.Y = -100;
		BallCord.YC = 0;
		BallCord.XC = 0;
		GameActive = FALSE;
	}

	else if(BallCord.X > ScrWidth) {
		Player1.Score++;
		BallCord.X = 400;
		BallCord.Y = -100;
		BallCord.YC = 0;
		BallCord.XC = 0;
		GameActive = FALSE;
	}

	if(BallCord.Y < 12) {
		if(GameActive == TRUE)
			PlaySound("BallSound1", hInst, SND_ASYNC | SND_NOWAIT/* | SND_NODEFAULT*/ | SND_RESOURCE | SND_MEMORY);
		BallCord.YC = -BallCord.YC;
		BallCord.YDirection = DOWN;
	}

	if(BallCord.Y > ScrHeight-BallSize-12) {
		if(GameActive == TRUE)
			PlaySound("BallSound1", hInst, SND_ASYNC | SND_NOWAIT/* | SND_NODEFAULT*/ | SND_RESOURCE | SND_MEMORY);
		BallCord.YC = -BallCord.YC;
		BallCord.YDirection = UP;
	}

	BallCord.X += BallCord.XC;
	BallCord.Y += BallCord.YC;
}



void UpdatePaddle(PaddleInfo &Paddle, int TopSpeed)
{
	switch(Paddle.Direction)
	{
	case STOP:
		Paddle.Paddleupspeed = Paddle.Paddledownspeed = 1;
		break;

	case UP:
		if(Paddle.RectPaddle.top > 12) {
			Paddle.Paddleupspeed = min(TopSpeed, Paddle.Paddleupspeed+1);
			Paddle.RectPaddle.top -= Paddle.Paddleupspeed;
		}
		if(Paddle.RectPaddle.top <= 12) {
			Paddle.RectPaddle.top = 12;
			Paddle.Paddleupspeed = 1;
		}
		break;

	case DOWN:
		if(Paddle.RectPaddle.top < 476) {
			Paddle.Paddledownspeed = min(TopSpeed, Paddle.Paddledownspeed+1);
			Paddle.RectPaddle.top += Paddle.Paddledownspeed;
		}
		if(Paddle.RectPaddle.top >= 476) {
			Paddle.RectPaddle.top = 476;
			Paddle.Paddledownspeed = 1;
		}
		break;
	}
}



void CheckifWon()
{
	if(Player1.Score == 3) {
		Player1.Won = TRUE;
	}

	else if(Player2.Score == 3) {
		Player2.Won = TRUE;
	}
}



void PaintScreen(HWND &hwnd, double FPS)
{
	static Map Map;
	static double FPSShow;
	static HDC hdc, hdcMemScr;
	HBITMAP hbScreen;
	TCHAR Text[10];


	if(FPS != 0) //If not called from WM_PAINT in WndProc
		FPSShow = FPS;

	hdc = GetDC(hwnd);
	hdcMemScr = CreateCompatibleDC(hdc);

	hbScreen = CreateCompatibleBitmap(hdc, ScrWidth, ScrHeight);

	SelectObject(hdcMemScr, hbScreen);


	Map.Draw(hdcMemScr);
	Ball.Draw(hdcMemScr, BallCord.X, BallCord.Y);
	PaddleDraw1.Draw(hdcMemScr, Paddle1.RectPaddle.left, Paddle1.RectPaddle.top);
	PaddleDraw2.Draw(hdcMemScr, Paddle2.RectPaddle.left, Paddle2.RectPaddle.top);


	SetBkMode(hdcMemScr, TRANSPARENT);
	SetTextColor(hdcMemScr, RGB(255, 0, 0));

	if(Settings.ShowFPS == BST_CHECKED) {
		sprintf(Text, "%7.1f", FPSShow);
		TextOut(hdcMemScr, ScrWidth/2-15, 15, Text, 7);
	}

	sprintf(Text, "%3d", Player1.Score);
	TextOut(hdcMemScr, 60, 20, Text, 3);
	sprintf(Text, "%3d", Player2.Score);
	TextOut(hdcMemScr, 700, 20, Text, 3);

	if(Player1.Won == TRUE)
		TextOut(hdcMemScr, 350, 300, "Player1 Wins!!!", 15);
	else if(Player2.Won == TRUE)
		TextOut(hdcMemScr, 350, 300, "Player2 Wins!!!", 15);

	BitBlt(hdc, 0, 0, ScrWidth, ScrHeight, hdcMemScr, 0, 0, SRCCOPY);


	DeleteObject(hbScreen);
	DeleteDC(hdc);
	DeleteDC(hdcMemScr);
}