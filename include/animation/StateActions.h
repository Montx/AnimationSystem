#pragma once

#include <unordered_map>
#include <iostream>

namespace NDebug {
	static void TestUpdateAction(const char* mId) {
		//std::cout << mId << ": Test Update Action" << std::endl;
	}
	static void TestEntryAction(const char* mId) {
		std::cout << mId << ": Test Entry Action" << std::endl;
	}
	static void TestExitAction(const char* mId) {
		std::cout << mId << ": Test Exit Action" << std::endl;
	}
};

using StateAction = void(*)(const char* mId);

const std::unordered_map < const char*, StateAction > sStateActionsRegister = {
	{
		"TestUpdateAction",
		& NDebug::TestUpdateAction
	},
	{
		"TestEntryAction",
		& NDebug::TestEntryAction
	},
	{
		"TestExitAction",
		& NDebug::TestExitAction
	}
};