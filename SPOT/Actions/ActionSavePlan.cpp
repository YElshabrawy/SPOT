
#include "ActionSavePlan.h"
#include "..\Registrar.h"
#include <filesystem>
#include <string>
#include <iostream>
#include "../Utils/Utils.h"

ActionSavePlan::ActionSavePlan(Registrar* p) : Action(p) {

}
ActionSavePlan::~ActionSavePlan() {
}

bool ActionSavePlan::Execute() {
	cout << "Save button is pressed.\n"; //for debugging
	importCourseRules();
	cout << "Course Rules are imported successfully.\n"; //for debugging

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
	cout << "Plan is saved in (" + directory + ") successfully.\n";

	return 1;
}

void ActionSavePlan::importCourseRules() {
	// To be implemented in RegRules later.
	string directory = "Format Files\\Rules.txt";
	ifstream finput(directory);
	StudyPlan* pS = pReg->getStudyPlay();

	//line 1
	string l1;
	getline(finput, l1);
	pS->MaxCredits = stoi(l1);
	cout << "Total Credits = " << pS->MaxCredits << endl;

	//line 2
	string l2;
	getline(finput, l2);
	vector<string> l2_tokens = splitString(l2, ",");
	pS->TotalUnivCredits = stoi(l2_tokens[0]);
	pS->TotalMajorCredits = stoi(l2_tokens[1]);
	cout << "Univ Compulsory = " << pS->TotalUnivCredits << endl;
	cout << "Univ Elective = " << pS->TotalMajorCredits << endl;

	//line 3
	string l3;
	getline(finput, l3);
	pS->TotalTrackCredits = stoi(l3);
	cout << "Track Compulsory = " << pS->TotalTrackCredits << endl;

	//line 4
	string l4;
	getline(finput, l4);
	vector<string> l4_tokens = splitString(l4, ",");
	pS->TotalConcentrationCredits = stoi(l4_tokens[0]);
	pS->TotalMinorCredits = stoi(l4_tokens[1]);
	cout << "Major Comm Compulsory = " << pS->TotalConcentrationCredits << endl;
	cout << "Major comm Elec = " << pS->TotalMinorCredits << endl;

	//line 5
	string l5;
	getline(finput, l5);
	pS->NumberOfConcentrations = stoi(l5);
	cout << "Number Of Concentrations = " << pS->NumberOfConcentrations << endl;

	//line 6
	string l6;
	getline(finput, l6);
	vector<string> l6_tokens = splitString(l6, ",");
	pS->ConcentrationMajorCredits = stoi(l6_tokens[0]);
	pS->ConcentrationMinorCredits = stoi(l6_tokens[1]);
	cout << "Major Conc Compulsory = " << pS->ConcentrationMajorCredits << endl;
	cout << "Major Conc Elec = " << pS->ConcentrationMinorCredits << endl;

	//line 7
	string l7;
	getline(finput, l7);
	pS->CompUniCourses = splitString(l7, ",");

	//line 8
	string l8;
	getline(finput, l8);
	pS->ElectiveUniCourses = splitString(l8, ",");

	//line 9
	string l9;
	getline(finput, l9);
	pS->TrackCourses = splitString(l9, ",");


	//line 10
	string l10;
	getline(finput, l10);
	pS->CompMajorCourses = splitString(l10, ",");


	//line 11
	string l11;
	getline(finput, l11);
	pS->ElectiveMajorCourses = splitString(l11, ",");

	finput.close();
}