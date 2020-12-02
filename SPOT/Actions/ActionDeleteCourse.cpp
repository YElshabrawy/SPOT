#include "ActionDeleteCourse.h"
#include "..\Registrar.h"
#include <iostream>
#include"../GUI/GUI.h"
#include"../Utils/Utils.h"

ActionDeleteCourse::ActionDeleteCourse(Registrar* p) :Action(p)
{
}

ActionDeleteCourse::~ActionDeleteCourse()
{
}

bool ActionDeleteCourse::Execute() {
	cout << "Delete button is pressed.\n"; // for debugging
	GUI* pGUI = pReg->getGUI();
	
	ActionData actData = pGUI->GetUserAction("Select the course you want to delete");
	int x, y;
	if (actData.actType == DRAW_AREA) {
		x = actData.x;
		y = actData.y;
		Course_Code code = interrogateCourse(x, y, pReg);

	}

	return true;
}