#pragma once

#include "animation/StateMachine.h"

class CCharacter {
public:
	CCharacter();

	void Update(float dt);

	void SetIsMoving(bool move) { mIsMoving = move; }
	bool IsMoving() const { return mIsMoving; }

private:
	CStateMachine mStateMachine;

	bool mIsMoving = false;
};