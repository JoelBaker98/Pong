class Timer
{
public:
	Timer();
	Timer(unsigned int);
	BOOL CheckSupport();
	void SetStartTime();
	void SetStartTime(__int64);
	void SetEndTime();
	void SetFPS(unsigned int);
	void SetMaxFPS();
	__int64 GetStartTime();
	__int64 GetEndTime();
	__int64 GetDifference();
	unsigned int GetFPS();
	__int64 CalculateFPSChunk();
	double CalculateFPS();

private:
	unsigned int FPS;
	__int64 StartTime;
	__int64 EndTime;
	__int64 Frequency;
};