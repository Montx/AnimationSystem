#include "Character.h"

CCharacter::CCharacter() 
{
	mStateMachine.Load(this, sTestStates, sTotalTestStates);
}