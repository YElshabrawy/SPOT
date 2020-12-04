#include "ActionEraseAll.h"
#include "..\Registrar.h"
#include <iostream>
#include"../GUI/GUI.h"
#include"../Utils/Utils.h"

ActionEraseAll::ActionEraseAll(Registrar* p) :Action(p)
{
}

ActionEraseAll::~ActionEraseAll()
{
}

bool ActionEraseAll::Execute() {
	cout << "Erase all is pressed.\n";
	StudyPlan* pS = pReg->getStudyPlay();
	vector<AcademicYear*>* pPlan = pS->getStudyPlanVector(); // pointer on the plan vector
	for (AcademicYear* yr : *pPlan) {
		list<Course*>* pYr = yr->getListOfYears(); // pointer to the year
		for (int sem = FALL; sem < SEM_CNT; sem++) {
			pYr[sem].clear();
		}
	}
	fill(Course::numOfCoursesPerSem.begin(), Course::numOfCoursesPerSem.end(), 0);

	return true;
}