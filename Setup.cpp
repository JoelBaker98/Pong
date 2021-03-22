#include "DlgBox.h"
#include <stdio.h>



extern DlgSettings TempSettings;
extern HINSTANCE hInst;
extern WNDPROC WndprocHotKeyToolTip, WndprocAppearance;


LRESULT CALLBACK ToolTipProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK HotKeyProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK Appearance(HWND, UINT, WPARAM, LPARAM);


void SetupWindow1(HWND hwndDlg, HWND *Controls)
{

	//Static Controls
	Controls[0] = CreateWindowEx(WS_EX_TRANSPARENT, WC_BUTTON, "Game Speed", BS_GROUPBOX | WS_VISIBLE | WS_CHILD,
						20, 37, 125, 115, hwndDlg, NULL, hInst, NULL);
	Controls[1] = CreateWindowEx(WS_EX_TRANSPARENT, WC_BUTTON, "Maximum Ball Speed", BS_GROUPBOX | WS_VISIBLE | WS_CHILD,
						170, 37, 120, 60, hwndDlg, NULL, hInst, NULL);
	Controls[2] = CreateWindowEx(WS_EX_TRANSPARENT, WC_BUTTON, "Maximum Paddle Speed", BS_GROUPBOX | WS_VISIBLE | WS_CHILD,
						165, 110, 130, 100, hwndDlg, NULL, hInst, NULL);
	Controls[3] = CreateWindowEx(WS_EX_TRANSPARENT, WC_STATIC, "Player1:",
					WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP,
					180, 137, 40, 20, hwndDlg, NULL, hInst, NULL);
	Controls[4] = CreateWindow("static", "Player2:",
					WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP,
					180, 175, 40, 20, hwndDlg, NULL, hInst, NULL);

	//Radio Buttons + CheckBox
	Controls[5] = CreateWindow(WC_BUTTON, "30", BS_RADIOBUTTON | WS_VISIBLE | WS_CHILD,
						30, 55, 40, 15, hwndDlg, (HMENU)IDC_30, hInst, NULL);
	Controls[6] = CreateWindow(WC_BUTTON, "60", BS_RADIOBUTTON | WS_VISIBLE | WS_CHILD,
						30, 70, 40, 15, hwndDlg, (HMENU)IDC_60, hInst, NULL);
	Controls[7] = CreateWindow(WC_BUTTON, "100 Recomended", BS_RADIOBUTTON | WS_VISIBLE | WS_CHILD,
						30, 85, 105, 15, hwndDlg, (HMENU)IDC_100, hInst, NULL);
	Controls[8] = CreateWindow(WC_BUTTON, "150", BS_RADIOBUTTON | WS_VISIBLE | WS_CHILD,
						30, 100, 40, 15, hwndDlg, (HMENU)IDC_150, hInst, NULL);
	Controls[9] = CreateWindow(WC_BUTTON, "200", BS_RADIOBUTTON | WS_VISIBLE | WS_CHILD,
						30, 115, 40, 15, hwndDlg, (HMENU)IDC_200, hInst, NULL);
	Controls[10] = CreateWindow(WC_BUTTON, "Uncapped", BS_RADIOBUTTON | WS_VISIBLE | WS_CHILD,
						30, 130, 80, 15, hwndDlg, (HMENU)IDC_UNCAPPED, hInst, NULL);
	Controls[11] = CreateWindow(WC_BUTTON, "Show FPS", BS_CHECKBOX | WS_VISIBLE | WS_CHILD | WS_TABSTOP,
						30, 170, 80, 15, hwndDlg, (HMENU)IDC_SHOWFPS, hInst, NULL);

	//Combo Box's
	Controls[12] = CreateWindow(WC_COMBOBOX, NULL, CBS_NOINTEGRALHEIGHT | CBS_DROPDOWNLIST
								| WS_VISIBLE | WS_CHILD | WS_VSCROLL,
						200, 60, 60, 157, hwndDlg, (HMENU)IDC_MAXBALLSPEED, hInst, NULL);
	Controls[13] = CreateWindow(WC_COMBOBOX, NULL, CBS_NOINTEGRALHEIGHT | CBS_DROPDOWNLIST 
								| WS_VISIBLE | WS_CHILD | WS_VSCROLL,
						230, 135, 50, 157, hwndDlg, (HMENU)IDC_MAXPADDLE1SPEED, hInst, NULL);
	Controls[14] = CreateWindow(WC_COMBOBOX, NULL, CBS_NOINTEGRALHEIGHT | CBS_DROPDOWNLIST
								| WS_VISIBLE | WS_CHILD | WS_VSCROLL,
						230, 173, 50, 157, hwndDlg, (HMENU)IDC_MAXPADDLE2SPEED, hInst, NULL);


	
	for(int c=0; Controls[c]; c++)
		SendMessage(Controls[c], WM_SETFONT, (WPARAM)GetStockObject(ANSI_VAR_FONT ), FALSE);

	InvalidateRect(Controls[12], NULL, TRUE);
	InvalidateRect(Controls[13], NULL, TRUE);
	InvalidateRect(Controls[14], NULL, TRUE);

	WndprocHotKeyToolTip = (WNDPROC) SetWindowLong(Controls[13], GWL_WNDPROC, (LONG) ToolTipProc);
	SetWindowLong(Controls[14], GWL_WNDPROC, (LONG) ToolTipProc);
}



void SetupWindow2(HWND hwndDlg, HWND *Controls)
{
	Controls[0] = CreateWindowEx(WS_EX_TRANSPARENT, WC_BUTTON, "Ball", 
						BS_GROUPBOX | WS_VISIBLE | WS_CHILD,
						40, 37, 230, 70, hwndDlg, NULL, hInst, NULL);
	Controls[1] = CreateWindow("static", "",
					WS_CHILD | WS_VISIBLE,
					200, 65, 16, 16, hwndDlg, (HMENU)IDC_BALLPIC, hInst, NULL);
	Controls[2] = CreateWindow(WC_COMBOBOX, NULL, CBS_NOINTEGRALHEIGHT | CBS_DROPDOWNLIST
								| WS_VISIBLE | WS_CHILD | WS_VSCROLL,
						80, 60, 60, 157, hwndDlg, (HMENU)IDC_BALLCB, hInst, NULL);
	Controls[3] = CreateWindowEx(WS_EX_TRANSPARENT, WC_BUTTON, "Player1 Paddle",
						BS_GROUPBOX | WS_VISIBLE | WS_CHILD,
						40, 110, 110, 100, hwndDlg, NULL, hInst, NULL);
	Controls[4] = CreateWindowEx(WS_EX_TRANSPARENT, WC_BUTTON, "Player2 Paddle",
						BS_GROUPBOX | WS_VISIBLE | WS_CHILD,
						160, 110, 110, 100, hwndDlg, NULL, hInst, NULL);
	Controls[5] = CreateWindow("static", "",
					WS_CHILD | WS_VISIBLE,
					120, 130, 15, 30, hwndDlg, (HMENU)IDC_PADDLE1PIC, hInst, NULL);
	Controls[6] = CreateWindow(WC_COMBOBOX, NULL, CBS_NOINTEGRALHEIGHT | CBS_DROPDOWNLIST
								| WS_VISIBLE | WS_CHILD | WS_VSCROLL,
						50, 150, 60, 157, hwndDlg, (HMENU)IDC_PADDLE1CB, hInst, NULL);
	Controls[7] = CreateWindow("static", "PADDLE1",
					WS_CHILD | WS_VISIBLE,
					240, 130, 15, 60, hwndDlg, (HMENU)IDC_PADDLE2PIC, hInst, NULL);
	Controls[8] = CreateWindow(WC_COMBOBOX, NULL, CBS_NOINTEGRALHEIGHT | CBS_DROPDOWNLIST
								| WS_VISIBLE | WS_CHILD | WS_VSCROLL,
						170, 150, 60, 157, hwndDlg, (HMENU)IDC_PADDLE2CB, hInst, NULL);


	for(int c=0; Controls[c]; c++)
		SendMessage(Controls[c], WM_SETFONT, (WPARAM)GetStockObject(ANSI_VAR_FONT ), FALSE);

	InvalidateRect(Controls[2], NULL, TRUE);
	InvalidateRect(Controls[6], NULL, TRUE);
	InvalidateRect(Controls[8], NULL, TRUE);

	WndprocAppearance = (WNDPROC) SetWindowLong(Controls[1], GWL_WNDPROC, (LONG) Appearance);
	SetWindowLong(Controls[5], GWL_WNDPROC, (LONG) Appearance);
	SetWindowLong(Controls[7], GWL_WNDPROC, (LONG) Appearance);

	WndprocHotKeyToolTip = (WNDPROC) SetWindowLong(Controls[2], GWL_WNDPROC, (LONG) ToolTipProc);
	SetWindowLong(Controls[6], GWL_WNDPROC, (LONG) ToolTipProc);
	SetWindowLong(Controls[8], GWL_WNDPROC, (LONG) ToolTipProc);

	SendMessage(Controls[1], WM_CREATE, 0, 0);
	SendMessage(Controls[5], WM_CREATE, 0, 0);
	SendMessage(Controls[7], WM_CREATE, 0, 0);
}



void SetupWindow3(HWND hwndDlg, HWND *Controls)
{
	/* Static Controls */
	Controls[0] = CreateWindowEx(WS_EX_TRANSPARENT, WC_BUTTON, "Paddles",
						BS_GROUPBOX | WS_VISIBLE | WS_CHILD,
						40, 37, 230, 125, hwndDlg, NULL, hInst, NULL);
	Controls[1] = CreateWindow("static", "Player1 Up:",
					WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP,
					60, 55, 70, 20, hwndDlg, NULL, hInst, NULL);
	Controls[2] = CreateWindow("static", "Player1 Down:",
					WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP,
					60, 80, 70, 20, hwndDlg, NULL, hInst, NULL);
	Controls[3] = CreateWindow("static", "Player2 Up:",
					WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP,
					60, 105, 70, 20, hwndDlg, NULL, hInst, NULL);
	Controls[4] = CreateWindow("static", "Player2 Down:",
					WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP,
					60, 130, 70, 20, hwndDlg, NULL, hInst, NULL);
	Controls[5] = CreateWindowEx(WS_EX_TRANSPARENT, WC_BUTTON, "Miscellaneous",
						BS_GROUPBOX | WS_VISIBLE | WS_CHILD,
						40, 170, 230, 100, hwndDlg, NULL, hInst, NULL);
	Controls[6] = CreateWindow("static", "Pause:",
					WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP,
					60, 200, 70, 20, hwndDlg, NULL, hInst, NULL);
	Controls[7] = CreateWindow("static", "Serve:",
					WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP,
					60, 230, 70, 20, hwndDlg, NULL, hInst, NULL);

	/* Edit Control Boxes */
	Controls[8] = CreateWindow(WC_EDIT, NULL,
					WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_UPPERCASE,
					140, 55, 100, 18, hwndDlg, (HMENU)IDC_PLAYER1UP, hInst, NULL);
	Controls[9] = CreateWindow(WC_EDIT, NULL,
					WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_UPPERCASE,
					140, 80, 100, 18, hwndDlg, (HMENU)IDC_PLAYER1DOWN, hInst, NULL);
	Controls[10] = CreateWindow(WC_EDIT, NULL,
					WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_UPPERCASE,
					140, 105, 100, 18, hwndDlg, (HMENU)IDC_PLAYER2UP, hInst, NULL);
	Controls[11] = CreateWindow(WC_EDIT, NULL,
					WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_UPPERCASE,
					140, 130, 100, 18, hwndDlg, (HMENU)IDC_PLAYER2DOWN, hInst, NULL);
	Controls[12] = CreateWindow(WC_EDIT, NULL,
					WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_UPPERCASE,
					140, 200, 100, 18, hwndDlg, (HMENU)IDC_PAUSE, hInst, NULL);
	Controls[13] = CreateWindow(WC_EDIT, NULL,
					WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_UPPERCASE,
					140, 230, 100, 18, hwndDlg, (HMENU)IDC_SERVE, hInst, NULL);


	//Set HotKeyProc as default winproc for these control boxes
	WndprocHotKeyToolTip = (WNDPROC) SetWindowLong(Controls[8], GWL_WNDPROC, (LONG) HotKeyProc);
	for(int i=9; i <= 13; i++)
		SetWindowLong (Controls[i], GWL_WNDPROC, (LONG) HotKeyProc);



	for(int c=0; Controls[c]; c++)
		SendMessage(Controls[c], WM_SETFONT, (WPARAM)GetStockObject(ANSI_VAR_FONT), FALSE);
}



void InitWindow1(HWND hwndDlg, HWND *Controls)
{
	char Label[3];

	CheckRadioButton(hwndDlg, IDC_30, IDC_UNCAPPED, TempSettings.RadioFPS);
	CheckDlgButton(hwndDlg, IDC_SHOWFPS, TempSettings.ShowFPS);


	for(int c=1; c<31; c++) {
		sprintf(Label, "%d", c);
		SendMessage(Controls[12], CB_ADDSTRING, 0, (LPARAM) Label);
		SendMessage(Controls[13], CB_ADDSTRING, 0, (LPARAM) Label);
		SendMessage(Controls[14], CB_ADDSTRING, 0, (LPARAM) Label);
	}

	SendMessage(Controls[13], CB_ADDSTRING, 0, (LPARAM) "Uncapped");
	SendMessage(Controls[14], CB_ADDSTRING, 0, (LPARAM) "Uncapped");


	SendMessage(Controls[12], CB_SETCURSEL, TempSettings.MaxBallSpeed-1, 0);
	
	if(TempSettings.MaxPaddle1Speed-1 > 30)
		SendMessage(Controls[13], CB_SETCURSEL, 30, 0);
	else
		SendMessage(Controls[13], CB_SETCURSEL, TempSettings.MaxPaddle1Speed-1, 0);

	if(TempSettings.MaxPaddle2Speed-1 > 30)
		SendMessage(Controls[14], CB_SETCURSEL, 30, 0);
	else
		SendMessage(Controls[14], CB_SETCURSEL, TempSettings.MaxPaddle2Speed-1, 0);
}



void InitWindow2(HWND *Controls)
{
	SendMessage(Controls[2], CB_ADDSTRING, 0, (LPARAM) "Penny Shine");
	SendMessage(Controls[2], CB_ADDSTRING, 0, (LPARAM) "Soccer Ball");
	SendMessage(Controls[2], CB_ADDSTRING, 0, (LPARAM) "Blue Sky");


	SendMessage(Controls[6], CB_ADDSTRING, 0, (LPARAM) "Shiny Silver");
	SendMessage(Controls[6], CB_ADDSTRING, 0, (LPARAM) "Dark Blue");
	SendMessage(Controls[8], CB_ADDSTRING, 0, (LPARAM) "Shiny Silver");
	SendMessage(Controls[8], CB_ADDSTRING, 0, (LPARAM) "Dark Blue");



	SendMessage(Controls[2], CB_SETCURSEL, TempSettings.BallSel, 0);
	SendMessage(Controls[6], CB_SETCURSEL, TempSettings.Paddle1Sel, 0);
	SendMessage(Controls[8], CB_SETCURSEL, TempSettings.Paddle2Sel, 0);
}



void InitWindow3(HWND *Controls)
{
	SetWindowText(Controls[8], TempSettings.ControlLabels[0]);
	SetWindowText(Controls[9], TempSettings.ControlLabels[1]);
	SetWindowText(Controls[10], TempSettings.ControlLabels[2]);
	SetWindowText(Controls[11], TempSettings.ControlLabels[3]);
	SetWindowText(Controls[12], TempSettings.ControlLabels[4]);
	SetWindowText(Controls[13], TempSettings.ControlLabels[5]);
}