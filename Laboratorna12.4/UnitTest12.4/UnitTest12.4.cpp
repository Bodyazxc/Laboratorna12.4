#include "pch.h"
#include "CppUnitTest.h"
#include "../Laboratorna12.4/Laboratorna12.4.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest124
{
	TEST_CLASS(UnitTest124)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			string testFile = "testfile.dat";
			ofstream file(testFile, ios::binary);
			file.close();

			Room testRoom = { 10, 101, {"Te", "Ba", "Ko", ""}, 3 };

			ofstream outFile(testFile, ios::binary | ios::app);
			outFile.write((char*)&testRoom, sizeof(Room));
			outFile.close();

			ifstream inFile(testFile, ios::binary);
			Room readRoom;
			inFile.read((char*)&readRoom, sizeof(Room));
			inFile.close();

			Assert::AreEqual(testRoom.phone_number, readRoom.phone_number);
			Assert::AreEqual(testRoom.room_number, readRoom.room_number);
			Assert::AreEqual(testRoom.employee_count, readRoom.employee_count);
			Assert::AreEqual(testRoom.employees[0], readRoom.employees[0]);
			Assert::AreEqual(testRoom.employees[1], readRoom.employees[1]);
			Assert::AreEqual(testRoom.employees[2], readRoom.employees[2]);
		}
	};
}
