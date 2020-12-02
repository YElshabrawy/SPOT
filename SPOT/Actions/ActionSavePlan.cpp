
#include "ActionSavePlan.h"
#include "..\Registrar.h"
#include <filesystem>
#include <string>
#include <iostream>
#include "../Utils/Utils.h"

ActionSavePlan::ActionSavePlan(Registrar* p) : Action(p) {

}
bool ActionSavePlan::Execute() {
	/*
	cout << "Save button is pressed.\n"; // for debugging
	GUI* pGUI = pReg->getGUI(); // We have a pointer to gui
	StudyPlan* pS = pReg->getStudyPlay();
	vector<AcademicYear*> plan = pS->getStudyPlanVector();

	string filePath = "./Format Files/StudyPlan.txt";
	ofstream foutput(filePath);
	for (int i = 0; i < plan.size(); i++) {
		list<Course*> *pYear = plan[i]->getListOfYears();
		if (!(pYear[0].empty())) {
			foutput << "Year " << i + 1 << ","
				<< "Semester 1,";
			string delim = "";
			for (Course* c : pYear[0]) {
				foutput << delim << c->getCode();
				delim = ",";
			}
			foutput << endl;
		}
		if (!(pYear[1].empty())) {
			foutput << "Year " << i + 1 << ","
				<< "Semester 2,";
			string delim = "";
			for (Course* c : pYear[1]) {
				foutput << delim << c->getCode();
				delim = ",";
			}
			foutput << endl;

		}
		if (!(pYear[2].empty())) {
			foutput << "Year " << i + 1 << ","
				<< "Semester 3,";
			string delim = "";
			for (Course* c : pYear[2]) {
				foutput << delim << c->getCode();
				delim = ",";
			}
			foutput << endl;
		}
	}
	foutput.close();*/
	return 1;
}
ActionSavePlan::~ActionSavePlan() {

}
