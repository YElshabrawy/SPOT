
#include "ActionSavePlan.h"
#include "..\Registrar.h"
#include <filesystem>
#include <string>
#include <iostream>
#include "../Utils/Utils.h"

ActionSavePlan::ActionSavePlan(Registrar* p) : Action(p) {

}
bool ActionSavePlan::Execute() {
	cout << "Save button is pressed.\n"; //for debugging
	GUI* pGUI = pReg->getGUI();
	StudyPlan* pS = pReg->getStudyPlay(); //pointer to study plan
	vector<AcademicYear*>* pPlan = pS->getStudyPlanVector(); // pointer on the plan vector
	string directory = "Format Files\\Saved_Plan.txt";

	ofstream outFile;
	outFile.open(directory);
	int numOfYear = 0;
	for (AcademicYear* yr : *pPlan) {
		list<Course*>* pYr = yr->getListOfYears(); // pointer to the year
		for (int sem = FALL; sem < SEM_CNT; sem++) {
			int numOfCoursesPreThisSemester = Course::numOfCoursesPerSem[(numOfYear * 3) + sem];
			if (numOfCoursesPreThisSemester != 0) {
				string str_semester = "UNKNOWN";
				switch (sem)
				{
				case FALL:
					str_semester = "Fall";
					break;
				case SPRING:
					str_semester = "Spring";
					break;
				case SUMMER:
					str_semester = "Summer";
					break;
				}
				outFile << "Year " << numOfYear + 1 << ","
					<< str_semester;
				for (auto it = pYr[sem].begin(); it != pYr[sem].end(); it++) {
					string delim_comma = ",";
					outFile << delim_comma << (*it)->getCode();
					delim_comma = "";
				}
				outFile << "\n";
			}
		}
		numOfYear++;
	}
	outFile.close();

	return 1;
}
ActionSavePlan::~ActionSavePlan() {

}
