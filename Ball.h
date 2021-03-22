class Ball
{
public:
	Ball();
	~Ball();
	void Change(char *);
	void Draw(HDC &, int, int);

private:
	HBITMAP hbitmap;
	HBITMAP hbitmask;
	HDC hdcBitmap;
	HDC hdcMask;
};