#pragma once
#include "Action.h"
//Class responsible for Deleting course action
class ActionAddNotes :
	public Action
{
public:
	ActionAddNotes(Registrar*);
	bool virtual Execute();
	virtual ~ActionAddNotes();
};
