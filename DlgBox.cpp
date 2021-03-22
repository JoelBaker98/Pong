#include "DlgBox.h"
#include "resource.h"
#include "Timer.h"
#include "Ball.h"
#include "Paddle.h"
#include <uxtheme.h>


extern HINSTANCE hInst;
extern Timer RefreshRate;
extern Ball Ball;
extern Paddle PaddleDraw1, PaddleDraw2;


WNDPROC WndprocHotKeyToolTip, WndprocAppearance;
DlgSettings TempSettings;
DlgSettings Settings = {100, BST_UNCHECKED, IDC_100, 'Q', 'A', VK_UP, VK_DOWN, 'P', VK_SPACE, 
						"Q", "A", "UP", "DOWN", "P", "Space", 20, 10000, 10000, "Ball1", 1,
						"PADDLE1", 0, "PADDLE1", 0};


void DestroyWindowControls(HWND *Controls);
void SetupWindow1(HWND hwndDlg, HWND *Controls);
void SetupWindow2(HWND hwndDlg, HWND *Controls);
void SetupWindow3(HWND hwndDlg, HWND *Controls);
void InitWindow1(HWND, HWND *);
void InitWindow2(HWND *Controls);
void InitWindow3(HWND *Controls);
void ProcessMsgWindow1(HWND hwndDlg, WPARAM wParam, HWND *);



void SetToolTip(const HWND &hwndControl, const HWND &hwndTT, char *String)
{
	TOOLINFO ti;
	RECT rect;


	GetClientRect(hwndControl, &rect);

 
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = TTF_SUBCLASS;
	ti.hwnd = hwndControl;
	ti.hinst = hInst;
	ti.lpszText = String;
	ti.rect = rect;

	SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);
}

void CreateToolTipsWindow1(HWND *Controls, const HWND &hwnd)
{
	static HWND hwndTT;


	DestroyWindow(hwndTT);

	hwndTT = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
						TOOLTIPS_CLASS, NULL, WS_POPUP| TTS_ALWAYSTIP,	
						CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
						hwnd, NULL, hInst, NULL);
 

	SetToolTip(Controls[5], hwndTT, "Sets game speed to 30 FPS");
	SetToolTip(Controls[6], hwndTT, "Sets game speed to 60 FPS");
	SetToolTip(Controls[7], hwndTT, "Sets game speed to 100 FPS");
	SetToolTip(Controls[8], hwndTT, "Sets game speed to 150 FPS");
	SetToolTip(Controls[9], hwndTT, "Sets game speed to 200 FPS");
	SetToolTip(Controls[10], hwndTT, "Sets game speed to as many FPS that the computer can handle");
	SetToolTip(Controls[11], hwndTT, "Toggles whether to show the FPS on the screen");
	SetToolTip(Controls[12], hwndTT, "Sets the maximum ball speed in the game");
	SetToolTip(Controls[13], hwndTT, "Sets the top speed of player1's paddle");
	SetToolTip(Controls[14], hwndTT, "Sets the top speed of player2's paddle");

	SendMessage(hwndTT, TTM_SETTIPBKCOLOR, RGB(0, 255, 255), 0);
}

void CreateToolTipsWindow2(HWND *Controls, const HWND &hwnd)
{
	static HWND hwndTT;


	DestroyWindow(hwndTT);

	hwndTT = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
						TOOLTIPS_CLASS, NULL, WS_POPUP| TTS_ALWAYSTIP,	
						CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
						hwnd, NULL, hInst, NULL);
 

	SetToolTip(Controls[2], hwndTT, "Appearance of the ball");
	SetToolTip(Controls[6], hwndTT, "Appearance of player1's paddle");
	SetToolTip(Controls[8], hwndTT, "Appearance of player2's paddle");

	SendMessage(hwndTT, TTM_SETTIPBKCOLOR, RGB(0, 255, 255), 0);
}

void CreateToolTipsWindow3(HWND *Controls, const HWND &hwnd)
{
	static HWND hwndTT;


	DestroyWindow(hwndTT);

	hwndTT = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
						TOOLTIPS_CLASS, NULL, WS_POPUP| TTS_ALWAYSTIP,	
						CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
						hwnd, NULL, hInst, NULL);
 

	SetToolTip(Controls[8], hwndTT, "Key to move player1's paddle up");
	SetToolTip(Controls[9], hwndTT, "Key to move player1's paddle down");
	SetToolTip(Controls[10], hwndTT, "Key to move player2's paddle up");
	SetToolTip(Controls[11], hwndTT, "Key to move player2's paddle down");
	SetToolTip(Controls[12], hwndTT, "Key to pause and unpause the game");
	SetToolTip(Controls[13], hwndTT, "Key to serve the ball");

	SendMessage(hwndTT, TTM_SETTIPBKCOLOR, RGB(0, 255, 255), 0);
}




BOOL CALLBACK DlgSettingsProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndTab, Controls[30];
	static HBRUSH BackGround;
	static int Tab;
	TCITEM tie;
	RECT Rect;




	switch(message)
	{
	case WM_INITDIALOG:
		TempSettings = Settings;
		Tab = 0;
		SetupWindow1(hwndDlg, Controls);
		InitWindow1(hwndDlg, Controls);
		CreateToolTipsWindow1(Controls, hwndDlg);
		BackGround = CreateSolidBrush(RGB(240, 240, 240));
		GetClientRect(hwndDlg, &Rect);
		hwndTab = CreateWindow(WC_TABCONTROL, "",
						WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,// | BS_OWNERDRAW,
						0, 0, Rect.right, 23,
						hwndDlg, NULL, hInst, NULL);
		
		tie.mask = TCIF_TEXT | TCIF_IMAGE;
		tie.iImage = -1;
		tie.pszText = "Game Speed";
		TabCtrl_InsertItem(hwndTab, 0, &tie);
		tie.pszText = "Appearance";
		TabCtrl_InsertItem(hwndTab, 1, &tie);
		tie.pszText = "Controls";
		TabCtrl_InsertItem(hwndTab, 2, &tie);
		SendMessage(hwndTab, WM_SETFONT, (WPARAM)GetStockObject(ANSI_VAR_FONT ), TRUE);
		TabCtrl_SetMinTabWidth(hwndTab, 98);
		TabCtrl_SetPadding(hwndTab, 23, 4);

		return TRUE;


	case WM_COMMAND:		
		switch(LOWORD(wParam))
		{
		case ID_OK:
			Settings = TempSettings;

			if(Settings.FPS > 0)
				RefreshRate.SetFPS(Settings.FPS);	
			else 
				RefreshRate.SetMaxFPS();

			Ball.Change(Settings.BallType);
			PaddleDraw1.Change(Settings.Paddle1Type);
			PaddleDraw2.Change(Settings.Paddle2Type);

			DeleteObject(BackGround);
			DestroyWindowControls(Controls);
			DestroyWindow(hwndTab);
			EndDialog(hwndDlg, 0);
			return TRUE;

		case ID_CANCEL:
			DeleteObject(BackGround);
			DestroyWindowControls(Controls);
			DestroyWindow(hwndTab);
			EndDialog(hwndDlg, 0);
			return TRUE;
		}
		
		if(Tab == 0)
			ProcessMsgWindow1(hwndDlg, wParam, Controls);
	
		return TRUE;

	case WM_CTLCOLOREDIT:
		if(Tab == 2) {
			HPEN Pen;
			Pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
			SelectObject((HDC) wParam, Pen);
			GetClientRect((HWND) lParam, &Rect);
			Rectangle((HDC) wParam, 0, 0, Rect.right, Rect.bottom);
			SetTextColor((HDC) wParam, RGB(0, 70, 213));
			DeleteObject(Pen);
		}
		return wParam;

	case WM_CTLCOLORSTATIC:
		if (IsAppThemed()) {
			SetBkColor((HDC)wParam, RGB(240, 240, 240));
			return (int)BackGround;  //If XP Theme is Enabled
		}
		
		return TRUE;

	case WM_NOTIFY:
		switch(((LPNMHDR)lParam)->code)
		{
		case TCN_SELCHANGE:
			Tab = TabCtrl_GetCurSel(hwndTab);
			if(Tab == 0) {
				DestroyWindowControls(Controls);
				SetupWindow1(hwndDlg, Controls);
				InitWindow1(hwndDlg, Controls);
				CreateToolTipsWindow1(Controls, hwndDlg);
			}
			else if(Tab == 1) {
				DestroyWindowControls(Controls);
				SetupWindow2(hwndDlg, Controls);
				InitWindow2(Controls);
				CreateToolTipsWindow2(Controls, hwndDlg);
			} 
			else if(Tab == 2) {
				DestroyWindowControls(Controls);
				SetupWindow3(hwndDlg, Controls);
				InitWindow3(Controls);
				CreateToolTipsWindow3(Controls, hwndDlg);
			}
			return TRUE;
		}
	}
	return FALSE;
}



void DestroyWindowControls(HWND *Controls)
{
	for(int c=0; Controls[c]; c++)
		DestroyWindow(Controls[c]);
}
