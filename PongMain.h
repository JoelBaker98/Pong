#define BallSize 16    //the balls height and width
#define PaddleWidth 15
#define PaddleHeight 60
#define ID_TIMER 1
#define ScrWidth 794
#define ScrHeight 548



enum PaddleDirections
{
	DOWN = -1, STOP, UP
};


typedef struct {
	int X;
	int Y;
	int XC;
	int YC;
	int YDirection;
} BallCrd;


typedef struct {
	int Paddleupspeed;
	int	Paddledownspeed;
	signed char Direction;
	RECT RectPaddle;
	BOOL KeysDown[2];
} PaddleInfo;


typedef struct {
	int Score;
	BOOL Won;
} PlayerInfo;


