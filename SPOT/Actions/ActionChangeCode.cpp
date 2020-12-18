#include "ActionChangeCode.h"
#include "..\Registrar.h"
#include <iostream>
#include"../Utils/Utils.h"
#include"../GUI/GUI.h"
#include <algorithm>
#include"../GUI/Drawable.h"

ActionChangeCode::ActionChangeCode(Registrar* p) :Action(p)
{
}
ActionChangeCode::~ActionChangeCode()
{
}
bool ActionChangeCode::Execute()
{
	cout << "change code button is pressed.\n"; // for debugging
	GUI* pGUI = pReg->getGUI();
	ActionData actData = pGUI->GetUserAction("Select the course you would like to change its code  ");
	int x, y;
	if (actData.actType == DRAW_AREA) {
		x = actData.x;
		y = actData.y;
		Course* pCr = pReg->interrogateCourse(x, y);
		if (pCr == nullptr)
		{
			// No course is sellected
		}
		else 
		{
			pGUI->PrintMsg("<< Add Course to plan >> Enter new course Code: ");
			Course_Code code = pGUI->GetSrting();
			//Input validations
			pReg->transformCode(code);

			//Check if exists
			bool exists;
			CourseInfo* pCInfo = pReg->inCatalog(code, exists);
			while (exists == 0) {
				pGUI->PrintMsg("ERROR: " + code + " is an invalid course code! Enter a valid one:");
				code = pGUI->GetSrting();
				
				pReg->transformCode(code);
				pCInfo = pReg->inCatalog(code, exists);
			}

			int year = pCr->getYear();
			SEMESTER sem = pCr->getSemester();
			StudyPlan* pS = pReg->getStudyPlay();
			int newx = 0, newy = 0;
			graphicsInfo anew_ginfo{ newx, newy };
			anew_ginfo= pCr->getGfxInfo();
			pS->DeleteCourse(pCr);
			//CourseInfo chosenCourseInfo = pReg->getCourseInfo(code);
			string Title = pCInfo->Title;
			int crd = pCInfo->Credits;
			vector<Course_Code> PreReq = pCInfo->PreReqList;
			vector<Course_Code> CoReq = pCInfo->CoReqList;
			Course* pnewcR = new  Course(code, Title, crd, PreReq, CoReq, year, sem);
			pnewcR->setGfxInfo(anew_ginfo);
			pS->AddCourse(pnewcR,year,sem);
			


		}
	}
	return true;
}
