#pragma once
#include "Action.h"
//Class responsible for adding course action
class ActionDeclareMinor :
	public Action
{
private:
public:
	ActionDeclareMinor(Registrar*);
	bool virtual Execute();
	virtual ~ActionDeclareMinor();
};


