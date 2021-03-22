class Paddle
{
public:
	Paddle();
	~Paddle();
	void Change(char *);
	void Draw(HDC &, int, int);

private:
	HBITMAP hbitmap;
	HDC hdcBitmap;
};