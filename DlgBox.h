#include <windows.h>
#include <commctrl.h>


typedef struct
{
	unsigned int FPS;
	int ShowFPS;
	int RadioFPS;
	UINT Player1up;
	UINT Player1down;
	UINT Player2up;
	UINT Player2down;
	UINT Pause;
	UINT Serve;
	char ControlLabels[6][20];
	int MaxBallSpeed;
	int MaxPaddle1Speed;
	int MaxPaddle2Speed;
	char *BallType;
	short BallSel;
	char *Paddle1Type;
	short Paddle1Sel;
	char *Paddle2Type;
	short Paddle2Sel;
} DlgSettings;




#define IDC_30                          1000
#define IDC_60                          1001
#define IDC_100                         1002
#define IDC_150                         1003
#define IDC_200							1004
#define IDC_UNCAPPED                    1005
#define IDC_SHOWFPS                     1006
#define IDC_MAXBALLSPEED				1007
#define IDC_MAXPADDLE1SPEED				1008
#define IDC_MAXPADDLE2SPEED				1009
#define IDC_BALLPIC						2000
#define IDC_PADDLE1PIC					2001
#define IDC_PADDLE2PIC					2002
#define IDC_BALLCB						3000
#define IDC_PADDLE1CB					3001
#define IDC_PADDLE2CB					3002

#define IDC_PLAYER1UP					1100
#define IDC_PLAYER1DOWN					1101
#define IDC_PLAYER2UP					1102
#define IDC_PLAYER2DOWN					1103
#define IDC_PAUSE						1104
#define IDC_SERVE						1105
