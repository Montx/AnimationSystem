#pragma once

#include "animation/StateActions.h"
#include "animation/Transition.h"
#include "animation/Conditions.h"

struct SStateInfo {
	const char* mId;
	const char* mEntryActionId;
	const char* mExitActionId;
	const char* mUpdateActionId;
	std::vector<STransitionInfo> mTransitionInfos;
};


class CState {
public:
	CState(const SStateInfo& info) 
		: mId(info.mId)
	{
		mTransitions.reserve(info.mTransitionInfos.size());
		for (const STransitionInfo& transition_info : info.mTransitionInfos) {
			mTransitions.emplace_back(transition_info.mId, transition_info.mConditionInfo);
		}

		mEntryAction = sStateActionsRegister.at(info.mEntryActionId);
		mExitAction = sStateActionsRegister.at(info.mExitActionId);
		mUpdateAction = sStateActionsRegister.at(info.mUpdateActionId);
	}

	std::vector<CTransition>& GetTransitions() { return mTransitions; }

	StateAction GetEntryAction() const { return mEntryAction;  }
	StateAction GetExitAction() const { return mExitAction; }
	StateAction GetUpdateAction() const { return mUpdateAction; }

	const char* GetId() const { return mId; }

private:
	std::vector<CTransition> mTransitions;
	const char* mId;

	StateAction mEntryAction;
	StateAction mExitAction;
	StateAction mUpdateAction;
};

static SStateInfo sTestStates[] = {
	{
		"Idle",
		"TestEntryAction",
		"TestExitAction",
		"TestUpdateAction",
		{
			{ "Locomotion", { "IsMovingCondition" } }
		}
	},
	{
		"Locomotion",
		"TestEntryAction",
		"TestExitAction",
		"TestUpdateAction",
		{
			{ "Idle", { "IsMovingCondition", false } }
		}
	}
};

const int sTotalTestStates = sizeof(sTestStates) / sizeof(SStateInfo);