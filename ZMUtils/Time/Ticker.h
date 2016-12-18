#pragma once

class Ticker
{
public:
	Ticker(const int aFPS);
	~Ticker();

	//true = should update, false = shouldn't update
	bool Update(const float aFrameTime);
	//use this on areas in case of FPS drop
	bool NeedsMoreTime();
	inline float GetFrameTime();
private:

	const float myFrameTime;
	float myTime;

};
inline float Ticker::GetFrameTime()
{
	return myFrameTime;
}