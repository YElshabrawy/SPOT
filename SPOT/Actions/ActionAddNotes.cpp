#include "ActionAddNotes.h"
#include "../Registrar.h"
#include <string>
#include <iostream>

ActionAddNotes::ActionAddNotes(Registrar*P):Action(P)
{
}
bool ActionAddNotes::Execute()
{

	cout << "Notes button is pressed.\n"; // for debugging
	GUI* pGUI = pReg->getGUI();
	StudyPlan* pS = pReg->getStudyPlay(); //pointer to study plan
	    if (((pGUI->YCoord >= 10) && (pGUI->YCoord <= 30) && ((pGUI->XCoord) >= (pGUI->SideBarX1 - 45 + (pGUI->SideBarX2 - pGUI->SideBarX1) / 2)) && (pGUI->XCoord <= (pGUI->SideBarX1 - 45 + 100 + (pGUI->SideBarX2 - pGUI->SideBarX1) / 2))) && (pGUI->Last_CLick == LEFT_CLICK))
		{
			if (pS->PlanNotes.size() == 0)
				return true;
			pS->PlanNotes=pGUI->GetSrting(pS->PlanNotes);
			pGUI->Notes = pS->PlanNotes;
		}
		else
		{
			StudyPlan* pS = pReg->getStudyPlay(); //pointer to study plan
			pGUI->PrintMsg("Add Notes For The Study Plan");
			pGUI->Notes = pGUI->GetSrting();
			pS->PlanNotes = pS->PlanNotes + " " + pGUI->Notes;
			pGUI->Notes = pS->PlanNotes;
		}
	return true;
}

ActionAddNotes::~ActionAddNotes()
{
}
