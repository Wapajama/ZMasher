#include "Ticker.h"
#include <iostream>
#include <string>

Ticker::Ticker(const int aFPS)
	:myFrameTime(1.f / static_cast<float>(aFPS))
{
	myTime = 0;
}


Ticker::~Ticker()
{
}

bool Ticker::Update(const float aFrameTime)
{
	if((1.f / (myTime + aFrameTime)) <
	   ((1.f / myFrameTime) * 0.25f))
	{
		myTime = myFrameTime + 0.01f;
	}
	else
	{
		myTime += aFrameTime;
	}
	if(myTime > myFrameTime)
	{
		//std::cout << "FPS: " << std::to_string(1.f / myTime) << std::endl;
		myTime -= myFrameTime;
		return true;
	}
	return false;
}

bool Ticker::NeedsMoreTime()
{
	if(myTime > myFrameTime)
	{
		return true;
	}
	return false;
}