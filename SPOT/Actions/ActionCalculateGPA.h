#pragma once
#include "Action.h"
//Class responsible for adding course action
class ActionCalculateGPA :
	public Action
{
public:
	ActionCalculateGPA(Registrar*);
	bool virtual Execute();
	virtual ~ActionCalculateGPA();
};