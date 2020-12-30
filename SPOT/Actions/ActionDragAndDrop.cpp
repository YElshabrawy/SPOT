#include "ActionDragAndDrop.h"
#include "..\Registrar.h"
#include <iostream>
#include"../GUI/GUI.h"
#include"../Utils/Utils.h"


ActionDragAndDrop::ActionDragAndDrop(Registrar* p) :Action(p)
{
}

ActionDragAndDrop::~ActionDragAndDrop()
{
}

bool ActionDragAndDrop::Execute()
{
	GUI* pGUI = pReg->getGUI();
	pGUI->Draw_Dependacies_Flag = false;
	pGUI->Draw_Dependacies_For_One_Course = false;
	int x, y,flag=0;
	x = pGUI->XCoord;
	y = pGUI->YCoord;
	Course* pCr = pReg->interrogateCourse(x, y);
	int Course_X, Course_Y;
	if (pCr == nullptr)
		return true;
	graphicsInfo gInfo_Old=pCr->getGfxInfo();

	image systemimage= "GUI\\Images\\Menu\\test.jpg";
	while (true)
	{
		if (pGUI->Last_CLick == LEFT_CLICK)
		{
			break;
		}
		if (pCr == nullptr) {
			break;
			return true;
		}
		else
		{
			pGUI->pWind->GetMouseCoord(x, y);
			if (flag == 0)
			{
				pCr->DrawMe_Flag = false;
				pReg->UpdateInterface();
				pGUI->pWind->StoreImage(systemimage, 0, 0, pGUI->WindWidth, pGUI->WindHeight);
				flag = 1;
			}
			else
			{
				pGUI->DrawCourse(pCr, x, y);
			}
				pGUI->pWind->DrawImage(systemimage, 0, 0, pGUI->WindWidth, pGUI->WindHeight);
				pGUI->pWind->UpdateBuffer();
			StudyPlan* pS = pReg->getStudyPlay();
			int oldYear = pCr->getYear();
			int newYear = NULL;
			SEMESTER oldSem = pCr->getSemester();
			SEMESTER newSem;
			if (pGUI->pWind->GetMouseClick(x, y))
			{
				if (x > GUI::VerticalSeparatorX && x < GUI::Year_X2) {
					// The click X is inside the boundaries
					//Get the Year
					for (int i = 1; i <= GUI::NumOfYrs; i++) {
						int yi = (GUI::MenuBarHeight + (i * GUI::MyFactor) + ((i - 1) * GUI::One_Year_Div));
						int yf = (GUI::MenuBarHeight + (i * GUI::MyFactor) + (i * GUI::One_Year_Div));

						if (y > yi && y < yf) {
							newYear = i;
							// Get the semester
							int s1 = yi + GUI::One_Semester_Div;
							int s2 = yf - GUI::One_Semester_Div;

							if (y > yi && y < s1) newSem = FALL;
							else if (y > s1 && y < s2) newSem = SPRING;
							else if (y > s2 && y < yf) newSem = SUMMER;
						}
					}
				}
				else {
					//The user clicked outside the region
				}
				if (newYear != NULL) {
					// Delete crs
					pS->DeleteCourse(pCr);
					int year = pCr->getYear();
					SEMESTER sem = pCr->getSemester();
					vector<AcademicYear*>* pPlan = pS->getStudyPlanVector(); // pointer on the plan vector
					list<Course*>* pYr = (*pPlan)[year - 1]->getListOfYears();
					int new_x = y;
					int new_y = x;
					graphicsInfo gInfo{ new_x, new_y };

					for (int sem = FALL; sem < SEM_CNT; sem++) {
						int iter = 0;
						for (auto it = pYr[sem].begin(); it != pYr[sem].end(); it++) {
							// itertate over all courses
							//int iter = pCr->numOfCoursesPerSem[(3 * (year - 1)) + sem] - 1;
							gInfo.x = GUI::TitleBarWidth + (iter * CRS_WIDTH);
							gInfo.y = GUI::MenuBarHeight + GUI::MyFactor + ((year - 1) * GUI::One_Year_Div) + (sem * GUI::One_Semester_Div) +
								(GUI::MyFactor * (year - 1));
							(*it)->setGfxInfo(gInfo);
							iter++;
						}
					}
					// add it again :)
					pS->AddCourse(pCr, newYear, newSem);
					pCr->numOfCoursesPerSem[(3 * (newYear - 1)) + newSem]++;
					pCr->setYear(newYear);
					pCr->setSemester(newSem);
					int iter = pCr->numOfCoursesPerSem[(3 * (newYear - 1)) + newSem] - 1;
					gInfo.x = GUI::TitleBarWidth + (iter * CRS_WIDTH);
					gInfo.y = GUI::MenuBarHeight + GUI::MyFactor + ((newYear - 1) * GUI::One_Year_Div) + (newSem * GUI::One_Semester_Div) +
						(GUI::MyFactor * (newYear - 1));
					pCr->setGfxInfo(gInfo);
					pCr->Distance_Flag = false;
					pCr->DrawMe_Flag = true;
					pGUI->pWind->UpdateBuffer();
					pGUI->UpdateInterface();
				}
				//if (pReg->OldpCr != pCr)
				//	pReg->OldpCr = pCr;
				//if (pReg->OldpCr == nullptr)
				//	pReg->OldpCr = pCr;
				return true;
			}
		}
	}
}