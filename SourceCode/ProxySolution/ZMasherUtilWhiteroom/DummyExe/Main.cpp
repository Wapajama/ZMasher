//#include "../ZMasherUtilWhiteroom/ZMVector.h"
//#include "../ZMasherUtilWhiteroom/ZMMatrix44.h"

#include <ZMMatrix44.h>
#include <StackArray.h>
#include <MemoryContainer.h>
#include <MemoryBucket.h>
#include <string>
#include <stdio.h>
#include <cstdio>

using namespace ZMasher;
int main()
{
	MemoryBucket test;

	char* long_ass_sentence = reinterpret_cast<char*>( test.GetData<char[1024]>());

	sprintf_s(long_ass_sentence, 1024, "One study indicates that anywhere from 5–7% of the world population is affected by severe claustrophobia, but only a small percentage of these people receive some kind of treatment for the disorder.");

	return 0;
}