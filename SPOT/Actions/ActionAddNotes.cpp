#include "ActionAddNotes.h"
#include "../Registrar.h"
#include <string>
#include "../Utils/Utils.h"
#include "../Courses/UnivCourse.h"
#include <iostream>
#include <fstream>

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
		string directory = "Format Files\\StudyPlan_Notes.txt";
		ofstream outFile;
		outFile.open(directory);
		if (outFile)
		{	
			outFile << StudyPlan::PlanNotes;
			outFile.close();
		}
	return true;
}

ActionAddNotes::~ActionAddNotes()
{
}
