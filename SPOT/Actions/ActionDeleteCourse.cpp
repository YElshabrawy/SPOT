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
	pGUI->Draw_Dependacies_Flag = false;
	pGUI->Draw_Dependacies_For_One_Course = false;
	ActionData actData = pGUI->GetUserAction("Select the course you want to delete");
	int x, y;
	if (actData.actType == DRAW_AREA) {
		x = actData.x;
		y = actData.y;
		Course* pCr = pReg->interrogateCourse(x, y);
		if (pCr == nullptr) {
			pReg->Not_Worth_Saving_Flag = true;
			// No course is sellected
		}
		else {
			StudyPlan* pS = pReg->getStudyPlay();
			pCr->setUnknownCrs(false);
			pS->DeleteCourse(pCr);
			cout << pCr->getCode() << " is deleted." << endl;
			// Update the graphics info of the other courses in the same semester and year
			int year = pCr->getYear();
			SEMESTER sem = pCr->getSemester();
			vector<AcademicYear*>* pPlan = pS->getStudyPlanVector(); // pointer on the plan vector
			list<Course*>* pYr = (*pPlan)[year-1]->getListOfYears();
			int new_x =0;
			int new_y=0;
			graphicsInfo new_gInfo{ new_x, new_y };

			for (int sem = FALL; sem < SEM_CNT; sem++) {
				int iter = 0;
				for (auto it = pYr[sem].begin(); it != pYr[sem].end(); it++) {
					// itertate over all courses
					//int iter = pCr->numOfCoursesPerSem[(3 * (year - 1)) + sem] - 1;
					new_gInfo.x = GUI::TitleBarWidth + (iter * CRS_WIDTH);
					new_gInfo.y = GUI::MenuBarHeight + GUI::MyFactor + ((year - 1) * GUI::One_Year_Div) + (sem * GUI::One_Semester_Div) +
						(GUI::MyFactor * (year - 1));
					(*it)->setGfxInfo(new_gInfo);
					iter++;
				}
			}
		}
	}

	return true;
}