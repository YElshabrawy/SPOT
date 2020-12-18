#include "ActionCourseInfo.h"
#include "..\Registrar.h"
#include <iostream>
#include"../GUI/GUI.h"
#include"../Utils/Utils.h"
#include<sstream>  

ActionCourseInfo::ActionCourseInfo(Registrar* P) :Action(P)
{
}
bool ActionCourseInfo::Execute()
{
	GUI* pGUI = pReg->getGUI();
	ActionData actData = pGUI->GetUserAction("");
	int x, y;
	if (actData.actType == DRAW_AREA) {
		x = actData.x;
		y = actData.y;
		Course* pCr = pReg->interrogateCourse(x, y);
		if (pCr == nullptr) {
			// No course is sellected
		}
		else
		{
			string Title ="Course Title: "+pCr->getTitle();
			string code = "Course Code: " + pCr->getCode();
			int credits =pCr->getCredits();
			stringstream ss;
			ss << credits;
			string String_Credits;
			ss >>String_Credits;
			pGUI->CourseTitle = Title;
			pGUI->CourseCode = code;
			pGUI->CourseCredit = "Course Credits: " + String_Credits;
		}
	}
	return true;
}

ActionCourseInfo::~ActionCourseInfo()
{
}