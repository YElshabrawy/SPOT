#pragma once
#include "Action.h"
//Class responsible for saving study plan
class ActionSavePlan : public Action
{
public:
	ActionSavePlan(Registrar*);
	bool virtual Execute();
	virtual ~ActionSavePlan();
};

