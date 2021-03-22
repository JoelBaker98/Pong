class Map
{
public:
	Map();
	~Map();
	void Draw(HDC &);

private:
	HBITMAP hbitmap;
	HDC hdcBitmap;
};