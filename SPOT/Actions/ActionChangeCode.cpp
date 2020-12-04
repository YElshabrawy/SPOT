#include "ActionChangeCode.h"
#include "..\Registrar.h"
#include <iostream>
#include"../GUI/GUI.h"
#include"../Utils/Utils.h"
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
		Course* pCr = interrogateCourse(x, y, pReg);
		if (pCr == nullptr)
		{
			// No course is sellected
		}
		else 
		{
			pGUI->PrintMsg("<< Add Course to plan >> Enter new course Code: ");
			Course_Code code = pGUI->GetSrting();
			//Input validations
		//Transform code to UPPER
			transform(code.begin(), code.end(), code.begin(), ::toupper);

			//Check the spaces
			addExtraSpace(code);

			//Check if exists
			bool flag = 0;
			while (flag == 0) {
				for (CourseInfo c : Registrar::getAllCourses()) {
					if (c.Code == code) {
						flag = 1;
						break;
					}
				}
				if (flag == 0) {
					pGUI->PrintMsg("ERROR: " + code + " is an invalid course code! Enter a valid one:");
					code = pGUI->GetSrting();
					transform(code.begin(), code.end(), code.begin(), ::toupper);
					addExtraSpace(code);
				}
			}
			int year = pCr->getYear();
			SEMESTER sem = pCr->getSemester();
			StudyPlan* pS = pReg->getStudyPlay();
			int newx = 0, newy = 0;
			graphicsInfo anew_ginfo{ newx, newy };
			anew_ginfo= pCr->getGfxInfo();
			pS->DeleteCourse(pCr);
			CourseInfo chosenCourseInfo = pReg->getCourseInfo(code);
			string Title = chosenCourseInfo.Title;
			int crd = chosenCourseInfo.Credits;
			vector<Course_Code> PreReq = chosenCourseInfo.PreReqList;
			vector<Course_Code> CoReq = chosenCourseInfo.CoReqList;
			Course* pnewcR = new  Course(code, Title, crd, PreReq, CoReq, year, sem);
			pnewcR->setGfxInfo(anew_ginfo);
			pS->AddCourse(pnewcR,year,sem);
			


		}
	}
	return true;
}
