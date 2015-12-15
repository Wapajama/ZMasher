#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ZMasherUtilWhiteroom/ZMVector.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ZMasher;

namespace Tester
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(Constuctors)
		{
			Vector3f();

			Vector3f threeVector(3.f, 3.f, 3.f);
			Vector3f(__m128());

			//auto invalidConstructArgument = [this]
			//{
			//	Vector3f test(nullptr);
			//};
			//Assert::ExpectException<int>(invalidConstructArgument);
			
			float* vectorThree = new float[3];

			vectorThree[0] = 1.f;
			vectorThree[1] = 1.f;
			vectorThree[2] = 1.f;

			Vector3f testVectorthree(vectorThree);

			Assert::IsTrue(Length(testVectorthree) < Length(threeVector));
			
		}
	
	};
}