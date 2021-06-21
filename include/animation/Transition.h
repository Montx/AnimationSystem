#pragma once

#include "animation/Conditions.h"

#include <vector>

class CCharacter;

struct STransitionInfo {
	const char* mId;
	const SConditionInfo& mConditionInfo;
};

class CTransition {
public:
	CTransition(const char* target_state_id, const SConditionInfo& condition_info);

	bool IsTriggered(const CCharacter& user) const;

	const char* GetTargetStateId() const { return mTargetStateId; }

private:
	const char* mTargetStateId;
	ConditionTest mCondition;
	bool mEvaluation;
};