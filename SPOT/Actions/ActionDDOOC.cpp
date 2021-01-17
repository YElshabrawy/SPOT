#include "ActionDDOOC.h"
#include "..\Registrar.h"
#include <iostream>
#include"../GUI/GUI.h"
#include"../Utils/Utils.h"
ActionDDOOC::ActionDDOOC(Registrar* P) :Action(P)
{
}
bool  ActionDDOOC::Execute()
{

	GUI* pGUI = pReg->getGUI();
	StudyPlan* pS = pReg->getStudyPlay();
	if (pGUI->Draw_Dependacies_For_One_Course_Flag)
	{
		pGUI->Draw_Dependacies_For_One_Course_Flag = false;
		pGUI->Draw_Dependacies_For_One_Course = false;
		return true;
	}
	if (pGUI->Draw_Dependacies_For_One_Course)
    {
	pS->FindPreAndCoReq_ITCSP(pReg->OldpCr_For_DDOOC, pGUI);
	return true;
    }
	ActionData actData = pGUI->GetUserAction("Select the course you want to see its Dependancies");
	int x, y;
	x = actData.x;
	y = actData.y;
	Course* pCr = pReg->interrogateCourse(x, y);
	if (pCr == nullptr) {
		pGUI->Draw_Dependacies_For_One_Course = false;
		return true;
	}
	else
	{
		pReg->OldpCr_For_DDOOC = pCr;
		pGUI->Draw_Dependacies_For_One_Course = true;
		pS->FindPreAndCoReq_ITCSP(pCr, pGUI);
	}
	return true;
}
ActionDDOOC::~ActionDDOOC()
{

}
