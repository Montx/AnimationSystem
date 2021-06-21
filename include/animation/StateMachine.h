#pragma once

#include "animation/State.h"
#include "animation/Character.h"

#include <vector>

class CStateMachine {
public:
	CStateMachine(const CCharacter& user)
		: mUser(user)
		, mInitialState(nullptr)
		, mCurrentState(mInitialState)
	{}

	void Load(SStateInfo states[], int total_states) {
		mStates.reserve(total_states);

		for (int i = 0; i < total_states; ++i) {
			mStates.emplace_back(states[i]);
		}

		mInitialState = &mStates[0];
		mCurrentState = mInitialState;
	}

	void Update(float dt) {

		for (CTransition& transition : mCurrentState->GetTransitions()) {
			if (!transition.IsTriggered(mUser)) 
				continue;

			for (CState& state : mStates) {
				if (strcmp(state.GetId(), transition.GetTargetStateId()) == 0) {
					mCurrentState->GetExitAction()(mCurrentState->GetId());
					mCurrentState = &state;
					mCurrentState->GetEntryAction()(mCurrentState->GetId());
					return;
				}
			}
		}

		mCurrentState->GetUpdateAction()(mCurrentState->GetId());
	}

private:
	const CCharacter& mUser;

	CState* mInitialState;
	CState* mCurrentState;

	std::vector<CState> mStates;
};