#include "animation/Character.h"

CCharacter::CCharacter() 
	: mStateMachine(*this)
{
	mStateMachine.Load(sTestStates, sTotalTestStates);
}

void CCharacter::Update(float dt) {
	mStateMachine.Update(dt);
}