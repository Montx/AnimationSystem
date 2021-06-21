#include "animation/Conditions.h"
#include "animation/Character.h"

namespace NConditions {

	bool IsMovingCondition(const CCharacter& user) {
		return user.IsMoving();
	}

	bool IsNotMovingCondition(const CCharacter& user) {
		return !user.IsMoving();
	}
}