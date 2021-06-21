#include "animation/Transition.h"
#include "animation/Character.h"

CTransition::CTransition(const char* target_state_id, const SConditionInfo& condition_info)
	: mTargetStateId(target_state_id)
	, mCondition(sConditionsRegister.at(condition_info.mId))
	, mEvaluation(condition_info.evaluation) {}

bool CTransition::IsTriggered(const CCharacter& user) const {
	return mCondition ? mCondition(user) == mEvaluation : false;
}