#pragma once

#include <unordered_map>

class CCharacter;

struct SConditionInfo {
	const char* mId;
	bool evaluation = true;
};

namespace NConditions {

	extern bool IsMovingCondition(const CCharacter& user);

	extern bool IsNotMovingCondition(const CCharacter& user);
}

using ConditionTest = bool(*)(const CCharacter& user);

const std::unordered_map < const char*, ConditionTest > sConditionsRegister = {
	{
		"IsMovingCondition",
		& NConditions::IsMovingCondition
	},
	{
		"IsNotMovingCondition",
		& NConditions::IsNotMovingCondition
	}
};