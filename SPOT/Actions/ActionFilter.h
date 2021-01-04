#pragma once
#include "Action.h"
#include"../GUI/CMUgraphicsLib/colors.h"
class ActionFilter :
	public Action
{
public:
	ActionFilter(Registrar*);
	bool virtual Execute();
	virtual ~ActionFilter();
	void Filteringyear(int inputyear);
	void Filteringsemester(int inputsem);
	void Filteringcolour(color col);
	//void FilteringStatus(string s);
	void UNFiltering();
};
