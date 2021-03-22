#include "DlgBox.h"
#include "Ball.h"
#include "Paddle.h"


extern DlgSettings TempSettings;
extern HINSTANCE hInst;
extern WNDPROC WndprocHotKeyToolTip, WndprocAppearance;


int GetTextLength(const HWND &, char *);
void MakeToolTip(const HWND &, HWND &, const HWND &, const RECT &, 
				 POINT &, POINT &, TOOLINFO &, int &);




void SetFPS(UINT NewFPS, WPARAM wParam, HWND hDlg)
{
	TempSettings.RadioFPS = LOWORD(wParam);
	CheckRadioButton(hDlg, IDC_30, IDC_UNCAPPED, TempSettings.RadioFPS);
	TempSettings.FPS = NewFPS;
}


void ProcessMsgWindow1(HWND hwndDlg, WPARAM wParam, HWND *Controls)
{
	switch(LOWORD(wParam))
	{
	case IDC_30:
		SetFPS(30, wParam, hwndDlg);
		return;

	case IDC_60:
		SetFPS(60, wParam, hwndDlg);
		return;

	case IDC_100:
		SetFPS(100, wParam, hwndDlg);
		return;

	case IDC_150:
		SetFPS(150, wParam, hwndDlg);
		return;

	case IDC_200:
		SetFPS(1000, wParam, hwndDlg);
		return;

	case IDC_UNCAPPED:
		SetFPS(0, wParam, hwndDlg);
		return;

	case IDC_SHOWFPS:
		if(TempSettings.ShowFPS == BST_CHECKED) {
			TempSettings.ShowFPS = BST_UNCHECKED;
			CheckDlgButton(hwndDlg, IDC_SHOWFPS, BST_UNCHECKED);
		} else {
			TempSettings.ShowFPS = BST_CHECKED;
			CheckDlgButton(hwndDlg, IDC_SHOWFPS, BST_CHECKED);
		}
		return;

	case IDC_MAXBALLSPEED:
		if(HIWORD(wParam) == CBN_SELCHANGE)
			TempSettings.MaxBallSpeed = SendMessage(Controls[12], CB_GETCURSEL, 0, 0) + 1;

		return;

	case IDC_MAXPADDLE1SPEED:
		if(HIWORD(wParam) == CBN_SELCHANGE) {
			TempSettings.MaxPaddle1Speed = SendMessage(Controls[13], CB_GETCURSEL, 0, 0) + 1;

			if(TempSettings.MaxPaddle1Speed == 31)
				TempSettings.MaxPaddle1Speed = 10000;
		}
		return;

	case IDC_MAXPADDLE2SPEED:
		if(HIWORD(wParam) == CBN_SELCHANGE) {
			TempSettings.MaxPaddle2Speed = SendMessage(Controls[14], CB_GETCURSEL, 0, 0) + 1;

			if(TempSettings.MaxPaddle2Speed == 31)
				TempSettings.MaxPaddle2Speed = 10000;
		}
		return;
	}
}


/* To change this function to the combo boxes you wish to use it just use the IDs of 
your controls and integrate them with this function with the if statements and thats
really all that has to be done. You can expand or shrink the color array and the 
hwndT_T array to what fits your program. You can also change the VScrollWidth constant
if your scroll bar is different than what this was used for.  */
LRESULT CALLBACK ToolTipProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	const int VScrollWidth = 19;

    static char strTT[30];
	static POINT TTPoint, OldTTPoint;
	static HWND hwndLB, Oldhwnd;
	static HWND *hwndTT, hwndTTStorage, hwndT_T[3];
	static RECT CBRect, LBRect;
	static TOOLINFO ti;
	static int TextLength;
	static bool TimerSet;
	static COLORREF color[3] = { RGB(255, 255, 255), RGB(255, 255, 255), RGB(255, 255, 255) };
	POINT CursorPnt, LBPoint, CBPoint;



	switch(message)
	{
	case WM_SETFOCUS:
		if(GetWindowLong(hwnd, GWL_ID) == IDC_MAXPADDLE1SPEED
			|| GetWindowLong(hwnd, GWL_ID) == IDC_BALLCB) 
			color[0] = RGB(178, 180, 191);
		else if(GetWindowLong(hwnd, GWL_ID) == IDC_MAXPADDLE2SPEED
			|| GetWindowLong(hwnd, GWL_ID) == IDC_PADDLE1CB)
			color[1] = RGB(178, 180, 191);
		else if(GetWindowLong(hwnd, GWL_ID) == IDC_PADDLE2CB)
			color[2] = RGB(178, 180, 191);
		break;

	case WM_KILLFOCUS:
		if(GetWindowLong(hwnd, GWL_ID) == IDC_MAXPADDLE1SPEED
			|| GetWindowLong(hwnd, GWL_ID) == IDC_BALLCB)
			color[0] = RGB(255, 255, 255);
		else if(GetWindowLong(hwnd, GWL_ID) == IDC_MAXPADDLE2SPEED
			|| GetWindowLong(hwnd, GWL_ID) == IDC_PADDLE1CB)
			color[1] = RGB(255, 255, 255);
		else if(GetWindowLong(hwnd, GWL_ID) == IDC_PADDLE2CB)
			color[2] = RGB(255, 255, 255);

		break;

	case WM_TIMER:
		GetCursorPos(&CursorPnt);
		GetClientRect (hwnd, &CBRect);

		if(SendMessage(hwnd, CB_GETDROPPEDSTATE, 0, 0) == FALSE) {
			ScreenToClient(hwnd, &CursorPnt);
			if(CursorPnt.x >= CBRect.left && CursorPnt.x <= CBRect.right-VScrollWidth &&
			   CursorPnt.y >= CBRect.top && CursorPnt.y <= CBRect.bottom)
				return 0;
			else {
				TimerSet = false;
				KillTimer(hwnd, GetWindowLong(hwnd, GWL_ID));
				SendMessage(*hwndTT, TTM_TRACKACTIVATE, (WPARAM)false, (LPARAM) &ti);
				return 0;
			}

		}
		
		LBPoint.x = LBRect.left;
		LBPoint.y = LBRect.top;
		ClientToScreen(hwndLB, &LBPoint);

		CBPoint.x = CBRect.left;
		CBPoint.y = CBRect.top;
		ClientToScreen(hwnd, &CBPoint);
		

		if(CursorPnt.x > LBPoint.x && CursorPnt.x < LBPoint.x+CBRect.right-VScrollWidth && 
		   CursorPnt.y > CBPoint.y && CursorPnt.y < CBPoint.y+LBRect.bottom+23 && 
		   LBPoint.y > CBPoint.y) {
			hwndTT = &hwndTTStorage;
			MakeToolTip(hwnd, *hwndTT, hwndLB, CBRect, TTPoint, OldTTPoint, ti, TextLength);
		}
		else if(CursorPnt.x > LBPoint.x && CursorPnt.x < LBPoint.x+CBRect.right-VScrollWidth && 
				CursorPnt.y < CBPoint.y && CursorPnt.y > CBPoint.y-LBRect.bottom &&
				LBPoint.y < CBPoint.y) {
			hwndTT = &hwndTTStorage;
			MakeToolTip(hwnd, *hwndTT, hwndLB, CBRect, TTPoint, OldTTPoint, ti, TextLength);
		} else {
			memset(&OldTTPoint, 0, sizeof(POINT));
			SendMessage(*hwndTT, TTM_TRACKACTIVATE, (WPARAM)false, (LPARAM) &ti);
		}
		return 0;

	case WM_MOUSEMOVE:

		if(GetWindowLong(hwnd, GWL_ID) == IDC_MAXPADDLE1SPEED
			|| GetWindowLong(hwnd, GWL_ID) == IDC_BALLCB) {
			if(hwndT_T[1] || hwndT_T[2]) {
				SendMessage(hwndT_T[1], TTM_TRACKACTIVATE, (WPARAM)false, (LPARAM) &ti);
				SendMessage(hwndT_T[2], TTM_TRACKACTIVATE, (WPARAM)false, (LPARAM) &ti);
			}
			hwndTT = &hwndT_T[0];
		}
		else if(GetWindowLong(hwnd, GWL_ID) == IDC_MAXPADDLE2SPEED
			|| GetWindowLong(hwnd, GWL_ID) == IDC_PADDLE1CB) {
			if(hwndT_T[0] || hwndT_T[2]) {
				SendMessage(hwndT_T[0], TTM_TRACKACTIVATE, (WPARAM)false, (LPARAM) &ti);
				SendMessage(hwndT_T[2], TTM_TRACKACTIVATE, (WPARAM)false, (LPARAM) &ti);
			}
			hwndTT = &hwndT_T[1];
		}
		else if(GetWindowLong(hwnd, GWL_ID) == IDC_PADDLE2CB) {
			if(hwndT_T[0] || hwndT_T[1]) {
				SendMessage(hwndT_T[0], TTM_TRACKACTIVATE, (WPARAM)false, (LPARAM) &ti);
				SendMessage(hwndT_T[1], TTM_TRACKACTIVATE, (WPARAM)false, (LPARAM) &ti);
			}
			hwndTT = &hwndT_T[2];
		}


		if(hwnd != Oldhwnd || TimerSet == false) {
			KillTimer(Oldhwnd, GetWindowLong(Oldhwnd, GWL_ID));
			Oldhwnd = hwnd;
			SetTimer(hwnd, GetWindowLong(hwnd, GWL_ID), 100, NULL);
			GetClientRect (hwnd, &CBRect);
			TimerSet = true;
		}

		SendMessage(hwnd, WM_TIMER, 0, 0);


		if(*hwndTT == NULL) {

			*hwndTT = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
						TOOLTIPS_CLASS, NULL, WS_POPUP| TTS_ALWAYSTIP,		
						CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
						hwnd, NULL, hInst, NULL);


 
			ti.cbSize = sizeof(TOOLINFO);
			ti.uFlags = TTF_TRACK | TTF_TRANSPARENT | TTF_ABSOLUTE;
			ti.hwnd = hwnd;
			ti.hinst = hInst;
			ti.uId = 0;
			ti.lpszText = strTT;
	

			SendMessage(*hwndTT, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);
		}
		
		else if(SendMessage(hwnd, CB_GETDROPPEDSTATE, 0, 0) == FALSE &&
				LOWORD(lParam) < CBRect.right-VScrollWidth) {

			SendMessage(hwnd, CB_GETLBTEXT, SendMessage(hwnd, CB_GETCURSEL, 0, 0), (LPARAM) ti.lpszText);
	
			if((TextLength = GetTextLength(hwnd, ti.lpszText)) < CBRect.right - VScrollWidth) {
				SendMessage(*hwndTT, TTM_TRACKACTIVATE, (WPARAM)false, (LPARAM) &ti);
				break;
			}

			memset(&TTPoint, 0, sizeof(POINT));
			ClientToScreen(hwnd, &TTPoint);

			ti.hwnd = hwnd;

			SendMessage(*hwndTT, TTM_UPDATETIPTEXT, 0, (LPARAM) &ti);
			SendMessage(*hwndTT, TTM_TRACKPOSITION, 0, MAKELONG(TTPoint.x+1, TTPoint.y+2));
			SendMessage (*hwndTT, TTM_TRACKACTIVATE, (WPARAM)true, (LPARAM) &ti);
		}

		break;

	case WM_NOTIFY:
		switch(((LPNMHDR)lParam)->code)
		{
		case TTN_SHOW:
			SendMessage(*hwndTT, WM_SETFONT, (WPARAM)GetStockObject(ANSI_VAR_FONT), FALSE);
			if(GetWindowLong(Oldhwnd, GWL_ID) == IDC_MAXPADDLE1SPEED
				|| GetWindowLong(Oldhwnd, GWL_ID) == IDC_BALLCB)
				SendMessage(*hwndTT, TTM_SETTIPBKCOLOR, color[0], 0);
			else if(GetWindowLong(Oldhwnd, GWL_ID) == IDC_MAXPADDLE2SPEED
				|| GetWindowLong(Oldhwnd, GWL_ID) == IDC_PADDLE1CB)
				SendMessage(*hwndTT, TTM_SETTIPBKCOLOR, color[1], 0);
			else if(GetWindowLong(Oldhwnd, GWL_ID) == IDC_PADDLE2CB)
				SendMessage(*hwndTT, TTM_SETTIPBKCOLOR, color[2], 0);
				
			break;
		}

		break;

	case WM_CTLCOLORLISTBOX:
		hwndLB = (HWND) lParam;
		GetClientRect((HWND) lParam, &LBRect);
		SendMessage(hwnd, WM_TIMER, 0, 0);
		break;


	case WM_COMMAND:
		TimerSet = false;
		KillTimer(hwnd, GetWindowLong(hwnd, GWL_ID));
		SendMessage (*hwndTT, TTM_TRACKACTIVATE, (WPARAM)false, (LPARAM) &ti);
		break;

	case WM_LBUTTONDOWN:
		SendMessage (*hwndTT, TTM_TRACKACTIVATE, (WPARAM)false, (LPARAM) &ti);
		break;

	case WM_DESTROY:
		DestroyWindow(hwndT_T[0]), DestroyWindow(hwndT_T[1]), DestroyWindow(hwndT_T[2]), 
		DestroyWindow(Oldhwnd), DestroyWindow(hwndTTStorage), DestroyWindow(hwndLB);
		hwndT_T[0] = hwndT_T[1] = hwndT_T[2] = hwndTTStorage = hwndLB = Oldhwnd = NULL;
		hwndTT = NULL;
		memset(&TTPoint, 0, sizeof(POINT));
		memset(&OldTTPoint, 0, sizeof(POINT));
		memset(&ti, 0, sizeof(TOOLINFO));
		TextLength = 0;
		strTT[0] = '\0';
		TimerSet = false;
		
		return 0;
	}

	return CallWindowProc(WndprocHotKeyToolTip, hwnd, message, wParam, lParam);
}



void MakeToolTip(const HWND &hwnd, HWND &hwndTT, const HWND &hwndLB, const RECT &CBRect, 
			  POINT &TTPoint, POINT &OldTTPoint, TOOLINFO &ti, int &TextLength)
{
	int index, topindex;
	POINT CursorPnt, LBPoint;
	RECT LBRect;



	topindex = SendMessage(hwnd, CB_GETTOPINDEX, 0, 0);
	index = SendMessage(hwnd, CB_GETCURSEL, 0, 0);
	SendMessage(hwnd, CB_GETLBTEXT, index, (LPARAM) ti.lpszText);


	if((index-topindex) >= 10 || (index-topindex) < 0) {
		SendMessage(hwndTT, TTM_TRACKACTIVATE, (WPARAM)false, (LPARAM) &ti);
		return;
	}

	GetCursorPos(&CursorPnt);
	ScreenToClient(hwnd, &CursorPnt);

	if(CursorPnt.y < CBRect.bottom+1 && CursorPnt.y > CBRect.top) {
		memset(&OldTTPoint, 0, sizeof(POINT));
		SendMessage(hwndTT, TTM_TRACKACTIVATE, (WPARAM)false, (LPARAM) &ti);
		return;
	}


	GetClientRect(hwndLB, &LBRect);
	LBPoint.y = LBRect.top;
	ClientToScreen(hwndLB, &LBPoint);

	TTPoint.x = TTPoint.y = 0;
	ClientToScreen(hwnd, &TTPoint);
	TTPoint.y = LBPoint.y + (13*(index - topindex))-4;

		
	if((TTPoint.x != OldTTPoint.x) || (TTPoint.y != OldTTPoint.y))
		OldTTPoint = TTPoint;
	else
		return;


	TextLength = GetTextLength(hwnd, ti.lpszText);

	if(TextLength < LBRect.right) {
		SendMessage(hwndTT, TTM_TRACKACTIVATE, (WPARAM)false, (LPARAM) &ti);
		return;
	}


	DestroyWindow(hwndTT);

	hwndTT = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
				TOOLTIPS_CLASS, NULL, WS_POPUP| TTS_ALWAYSTIP,		
				CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
				hwnd, NULL, hInst, NULL);

	SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);
	//SendMessage(hwndTT, TTM_UPDATETIPTEXT, 0, (LPARAM) &ti);
	SendMessage(hwndTT, TTM_TRACKPOSITION, 0, MAKELONG (TTPoint.x, TTPoint.y+2));
	SendMessage(hwndTT, TTM_TRACKACTIVATE, (WPARAM)true, (LPARAM) &ti);
}



int GetTextLength(const HWND &hwnd, char *Text)
{
	HDC hdc;
	SIZE TextSize;

	hdc = GetDC(hwnd);
	SelectObject(hdc, GetStockObject(ANSI_VAR_FONT));
	GetTextExtentPoint32(hdc, Text, strlen(Text), &TextSize);
	DeleteDC(hdc);

	return TextSize.cx;
}



void MakeNumpadConsistant(WPARAM &wParam, LPARAM lParam)
{
	char KeyName[20];

	GetKeyNameText(lParam, KeyName, 19);

	if(KeyName[0] == 'N' && KeyName[1] == 'u' && KeyName[2] == 'm')  //If key is on NumPad
		switch(wParam)
		{
		case 45:
			wParam = 96;
			return;
		case 35:
			wParam = 97;
			return;
		case 40:
			wParam = 98;
			return;
		case 34:
			wParam = 99;
			return;
		case 37:
			wParam = 100;
			return;
		case 12:
			wParam = 101;
			return;
		case 39:
			wParam = 102;
			return;
		case 36:
			wParam = 103;
			return;
		case 38:
			wParam = 104;
			return;
		case 33:
			wParam = 105;
			return;
		case 46:
			wParam = 110;
			return;
		}
}



LRESULT CALLBACK HotKeyProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char Label[20];


	switch (message)
	{
	case WM_GETDLGCODE:
		if(wParam == VK_RETURN || wParam == VK_ESCAPE || wParam == VK_TAB) {
			switch(wParam)
			{
			case VK_RETURN:
				strcpy(Label, "Enter");
				SetWindowText(hwnd, "Enter");
				break;
			case VK_ESCAPE:
				strcpy(Label, "Escape");
				SetWindowText(hwnd, "Escape");
				break;
			case VK_TAB:
				strcpy(Label, "Tab");
				SetWindowText(hwnd, "Tab");
				break;
			}

			switch(GetWindowLong(hwnd, GWL_ID))
			{
			case IDC_PLAYER1UP:
				TempSettings.Player1up = wParam;
				strcpy(TempSettings.ControlLabels[0], Label);
				break;
			case IDC_PLAYER1DOWN:
				TempSettings.Player1down = wParam;
				strcpy(TempSettings.ControlLabels[1], Label);
				break;
			case IDC_PLAYER2UP:
				TempSettings.Player2up = wParam;
				strcpy(TempSettings.ControlLabels[2], Label);
				break;
			case IDC_PLAYER2DOWN:
				TempSettings.Player2down = wParam;
				strcpy(TempSettings.ControlLabels[3], Label);
				break;
			case IDC_PAUSE:
				TempSettings.Pause = wParam;
				strcpy(TempSettings.ControlLabels[4], Label);
				break;
			case IDC_SERVE:
				TempSettings.Serve = wParam;
				strcpy(TempSettings.ControlLabels[5], Label);
				break;
			}
			SetFocus(GetParent(hwnd));
		}
		break;

	case WM_CHAR:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
		return 0;

	case WM_LBUTTONDOWN:
		SetFocus(hwnd);
		return 0;

	case WM_SYSKEYDOWN:
	case WM_KEYDOWN :
		GetKeyNameText(lParam, Label, 19);
		SetWindowText(hwnd, Label);
		MakeNumpadConsistant(wParam, lParam);

		if(strcmp(Label, "Delete") == 0)
			SetWindowText(hwnd, "DDELETE");	 //For some reason it doesn't show with 1 D


		switch(GetWindowLong(hwnd, GWL_ID))
		{
		case IDC_PLAYER1UP:
			TempSettings.Player1up = wParam;
			strcpy(TempSettings.ControlLabels[0], Label);
			break;
		case IDC_PLAYER1DOWN:
			TempSettings.Player1down = wParam;
			strcpy(TempSettings.ControlLabels[1], Label);
			break;
		case IDC_PLAYER2UP:
			TempSettings.Player2up = wParam;
			strcpy(TempSettings.ControlLabels[2], Label);
			break;
		case IDC_PLAYER2DOWN:
			TempSettings.Player2down = wParam;
			strcpy(TempSettings.ControlLabels[3], Label);
			break;
		case IDC_PAUSE:
			TempSettings.Pause = wParam;
			strcpy(TempSettings.ControlLabels[4], Label);
			break;
		case IDC_SERVE:
			TempSettings.Serve = wParam;
			strcpy(TempSettings.ControlLabels[5], Label);
			break;
		}

		SetFocus(GetParent(hwnd));
		break;
	}

	return CallWindowProc(WndprocHotKeyToolTip, hwnd, message, wParam, lParam);
}



void DrawBall(const HWND &hwnd, const HWND &hwndCB, Ball &Ball)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch(TempSettings.BallSel = (short)SendMessage(hwndCB, CB_GETCURSEL, 0, 0))
	{
	case 0: TempSettings.BallType = "Ball";
			break;
	case 1: TempSettings.BallType = "Ball1";
			break;
	case 2: TempSettings.BallType = "Ball2";
			break;
	}
			
	Ball.Change(TempSettings.BallType);

	hdc = BeginPaint(hwnd, &ps);
	Ball.Draw(hdc, 0, 0);
	EndPaint(hwnd, &ps);

	DeleteDC(hdc);
}


void DrawPaddle(const HWND &hwnd, const HWND &hwndCB, Paddle &Paddle)
{
	HDC hdc;
	PAINTSTRUCT ps;

	if(GetWindowLong(hwnd, GWL_ID) == IDC_PADDLE1PIC) {
		switch(TempSettings.Paddle1Sel = (short)SendMessage(hwndCB, CB_GETCURSEL, 0, 0))
		{
		case 0: TempSettings.Paddle1Type = "PADDLE1";
				break;
		case 1: TempSettings.Paddle1Type = "PADDLE2";
				break;
		}
		
		Paddle.Change(TempSettings.Paddle1Type);
	} else {
		switch(TempSettings.Paddle2Sel = (short)SendMessage(hwndCB, CB_GETCURSEL, 0, 0))
		{
		case 0: TempSettings.Paddle2Type = "PADDLE1";
				break;
		case 1: TempSettings.Paddle2Type = "PADDLE2";
				break;
		}
		
		Paddle.Change(TempSettings.Paddle2Type);
	}

	hdc = BeginPaint(hwnd, &ps);
	Paddle.Draw(hdc, 0, 0);
	EndPaint(hwnd, &ps);

	DeleteDC(hdc);
}


LRESULT CALLBACK Appearance(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static Ball Ball;
	static Paddle Paddle;
	int ID;
	HWND hwndCB;


	switch(message)
	{
	case WM_CREATE:
		SetTimer(hwnd, GetWindowLong(hwnd, GWL_ID), 10, NULL);
		break;

	case WM_DESTROY:
		KillTimer(hwnd, GetWindowLong(hwnd, GWL_ID));
		break;

	case WM_TIMER:
		ID = GetWindowLong(hwnd, GWL_ID);
		hwndCB = GetWindow(hwnd, GW_HWNDNEXT);
		if(SendMessage(hwndCB, CB_GETDROPPEDSTATE, 0, 0) == FALSE)
			if(ID == IDC_BALLPIC && SendMessage(hwndCB, CB_GETCURSEL, 0, 0) != TempSettings.BallSel
				|| ID == IDC_PADDLE1PIC && SendMessage(hwndCB, CB_GETCURSEL, 0, 0) != TempSettings.Paddle1Sel
				|| ID == IDC_PADDLE2PIC && SendMessage(hwndCB, CB_GETCURSEL, 0, 0) != TempSettings.Paddle2Sel)
				InvalidateRect(hwnd, NULL, TRUE);
		
		return 0;

	case WM_PAINT:
		ID = GetWindowLong(hwnd, GWL_ID);
		hwndCB = GetWindow(hwnd, GW_HWNDNEXT);

		if(ID == IDC_BALLPIC)
			DrawBall(hwnd, hwndCB, Ball);
		else if(ID == IDC_PADDLE1PIC)
			DrawPaddle(hwnd, hwndCB, Paddle);
		else if(ID == IDC_PADDLE2PIC)
			DrawPaddle(hwnd, hwndCB, Paddle);

		return 0;
	}

	return CallWindowProc(WndprocAppearance, hwnd, message, wParam, lParam);
}


