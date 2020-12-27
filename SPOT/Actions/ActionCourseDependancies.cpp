#include "ActionCourseDependancies.h"
#include "../Registrar.h"
#include "../Utils/Utils.h"
#include "../Courses/UnivCourse.h"
#include <iostream>

ActionCourseDependancies::ActionCourseDependancies(Registrar* P) :Action(P)
{
}
bool ActionCourseDependancies::Execute()
{
	GUI* pGUI = pReg->getGUI();
	StudyPlan* pS = pReg->getStudyPlay(); //pointer to study plan

	for (AcademicYear* yr : pS->plan)
	{
		list<Course*>* pYr = yr->getListOfYears(); // pointer to the year
		for (int sem = FALL; sem < SEM_CNT; sem++)
		{
			for (auto it = pYr[sem].begin(); it != pYr[sem].end(); ++it)
			{
				if (*it != NULL)
				pS->FindPreAndCoReq_ITCSP(*it, pGUI);
			}
		}
	}
	return true;
}

ActionCourseDependancies::~ActionCourseDependancies()
{
}