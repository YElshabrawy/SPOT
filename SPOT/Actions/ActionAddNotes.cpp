#include "ActionAddNotes.h"
#include "../Registrar.h"
#include "../Courses/UnivCourse.h"
#include <iostream>

ActionAddNotes::ActionAddNotes(Registrar*P):Action(P)
{
}
bool ActionAddNotes::Execute()
{
	cout << "Notes button is pressed.\n"; // for debugging
	GUI* pGUI = pReg->getGUI();
	pGUI->PrintMsg("Add Notes For The Study Plan");
		pGUI->Notes = pGUI->GetSrting();
		StudyPlan::PlanNotes= StudyPlan::PlanNotes +" "+ pGUI->Notes;
		pGUI->Notes = StudyPlan::PlanNotes;
	return true;
}

ActionAddNotes::~ActionAddNotes()
{
}
