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
	int x, y;
	x = pGUI->XCoord;
	y = pGUI->YCoord;
		Course* pCr = pReg->interrogateCourse(x, y);
		if (pCr == nullptr) {
			return true;
		}
		else
		{

			pCr->changeColor(BLACK);
			if((pReg->OldpCr!=nullptr)&&(pReg->OldpCr != pCr))
			pReg->OldpCr->changeColor(MYCYAN);
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
		if (pReg->OldpCr != pCr)
		pReg->OldpCr = pCr;
		if (pReg->OldpCr == nullptr)
			pReg->OldpCr = pCr;
	return true;
}

ActionCourseInfo::~ActionCourseInfo()
{
}