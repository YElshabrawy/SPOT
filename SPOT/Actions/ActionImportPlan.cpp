#include "ActionImportPlan.h"
#include "..\Registrar.h"
#include <iostream>
#include"../GUI/GUI.h"
#include"../Utils/Utils.h"

ActionImportPlan::ActionImportPlan(Registrar* p) : Action(p)
{
}

ActionImportPlan::~ActionImportPlan()
{
}

bool ActionImportPlan::Execute() {
	cout << "Import button is pressed.\n"; // for debugging
	GUI* pGUI = pReg->getGUI();
	// First we need to erase the current plan (Until i know how to call another action from here)
	StudyPlan* pS = pReg->getStudyPlay();
	vector<AcademicYear*>* pPlan = pS->getStudyPlanVector(); // pointer on the plan vector
	for (AcademicYear* yr : *pPlan) {
		list<Course*>* pYr = yr->getListOfYears(); // pointer to the year
		for (int sem = FALL; sem < SEM_CNT; sem++) {
			pYr[sem].clear();
		}
	}
	fill(Course::numOfCoursesPerSem.begin(), Course::numOfCoursesPerSem.end(), 0);

	//string directory = "Format Files\\StudyPlan-ENV.txt";
	string directory = "Format Files\\StudyPlan-CIE.txt";

	// Start reading
	ifstream finput(directory);
	const int size = 300;
	char line[size];
	char* token; // pointer to char (tokens)
	char* context = nullptr; // used in strtok_s function as a context
	int i = 0; // for each line we have one course with index i

	while (finput.getline(line, size)) {
		cout << endl;
		vector<string> tokens;
		token = strtok_s(line, ",", &context);
		while (token != NULL) {
			tokens.push_back(token);
			token = strtok_s(NULL, ",", &context);
		}
		// Get the year
		int year = stoi(tokens[0].erase(0, 5));
		// Get the semester
		SEMESTER sem;
		if (tokens[1][0] == 'F') sem = FALL;
		else if (tokens[1][0] == 'S' && tokens[1][1] == 'p') sem = SPRING;
		else sem = SUMMER;
		// remove first two tokens
		tokens.erase(tokens.begin(), tokens.begin() + 2);
		int iter = 0;
		for (string token : tokens) {
			bool exists;
			CourseInfo* pCInfo = pReg->inCatalog(token, exists);
			pCInfo = pReg->inCatalog(token, exists);
			if (exists) {
				StudyPlan* pS = pReg->getStudyPlay();
				string title = pCInfo->Title; // FOR NOW
				int crd = pCInfo->Credits; // FOR NOW
				vector<Course_Code> PreReq = pCInfo->PreReqList; // FOR NOW
				vector<Course_Code> CoReq = pCInfo->CoReqList; // FOR NOW
				Course* pC = new Course(token, title, crd, PreReq, CoReq, year, sem);
				pS->AddCourse(pC, year, static_cast<SEMESTER>(sem));
				cout << token << " is added to year " << year << " semester " << sem << endl;
				int x = 0;
				int y = 0;
				graphicsInfo gInfo{ x, y };
				gInfo.x = gInfo.x = GUI::TitleBarWidth + (iter * CRS_WIDTH);
				gInfo.y = GUI::MenuBarHeight + GUI::MyFactor + ((year - 1) * GUI::One_Year_Div) + (sem * GUI::One_Semester_Div) +
					(GUI::MyFactor * (year - 1));
				pC->setGfxInfo(gInfo);
				iter++;
			}
			else {
				vector<Course_Code> PreReq;
				vector<Course_Code> CoReq;

				Course* pC = new Course(token, "", 0, PreReq, CoReq, year, sem);
				pS->AddCourse(pC, year, static_cast<SEMESTER>(sem));
				cout << token << " is added to year " << year << " semester " << sem << endl;
				int x = 0;
				int y = 0;
				graphicsInfo gInfo{ x, y };
				gInfo.x = gInfo.x = GUI::TitleBarWidth + (iter * CRS_WIDTH);
				gInfo.y = GUI::MenuBarHeight + GUI::MyFactor + ((year - 1) * GUI::One_Year_Div) + (sem * GUI::One_Semester_Div) +
					(GUI::MyFactor * (year - 1));
				pC->setGfxInfo(gInfo);
				iter++;
			}
		}
		i++;
	}

	finput.close();

	return true;
}