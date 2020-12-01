#include "ActionAddCourse.h"
#include "..\Registrar.h"
#include "../Courses/UnivCourse.h"
#include <algorithm>
#include <iostream>
#include"../Utils/Utils.h"

ActionAddCourse::ActionAddCourse(Registrar* p):Action(p)
{
}

bool ActionAddCourse::Execute()
{
	GUI* pGUI = pReg->getGUI();	
	cout << "Add button is pressed.\n"; // for debugging

	// 1) Get Course Code
	pGUI->PrintMsg("<< Add Course to plan >> Enter course Code(e.g. CIE202):");
	Course_Code code = pGUI->GetSrting();

	//Input validations
	//Transform code to UPPER
	transform(code.begin(), code.end(), code.begin(), ::toupper);

	//Check the spaces
	addExtraSpace(code);

	//Check if exists
	bool flag = 0;
	while (flag == 0) {
		for (CourseInfo c : Registrar::getAllCourses()) {
			if (c.Code == code) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			pGUI->PrintMsg("ERROR: " + code + " is an invalid course code! Enter a valid one:");
			code = pGUI->GetSrting();
			transform(code.begin(), code.end(), code.begin(), ::toupper);
			addExtraSpace(code);
		}
	}
	
	/*
	// 2) Get Year
	int year = stoi(pGUI->GetSrting());

	//Input validation
	while (year < 1 || year > 5) {
		ActionData actData = pGUI->GetUserAction("ERROR: Invalid year! Enter a valid one: ");
		year = stoi(pGUI->GetSrting());
	}

	// 3) Get Semester
	pGUI->PrintMsg("Select a semester to add course to: (0=>FALL)(1=>SPRING)(2=>SUMMER)");
	int semester = stoi(pGUI->GetSrting());
	//Input validation
	while (semester < 0 || semester > 2) {
		pGUI->PrintMsg("ERROR: Invalid semseter! Enter a valid one: ");
		semester = stoi(pGUI->GetSrting());
	}
	switch (semester) {
	case 0:
		semester = static_cast<SEMESTER>(FALL);
		break;
	case 1:
		semester = static_cast<SEMESTER>(SPRING);
		break;
	case 2:
		semester = static_cast<SEMESTER>(SUMMER);
		break;
	}
	*/
	ActionData actData = pGUI->GetUserAction("Select where you want to add your course"); 
	int x, y;
	if (actData.actType == DRAW_AREA)	//user clicked inside drawing area
	{
		int year = NULL;
		SEMESTER semester;

		//get coord where user clicked
		x = actData.x;
		y = actData.y;
		
		graphicsInfo gInfo{ x, y };
		if (x > GUI::VerticalSeparatorX && x < GUI::Year_X2) {
			// The click X is inside the boundaries
			
			//Get the Year
			for (int i = 1; i <= GUI::NumOfYrs; i++) {
				int yi = (GUI::MenuBarHeight + (i * GUI::MyFactor) + ((i - 1) * GUI::One_Year_Div));
				int yf = (GUI::MenuBarHeight + (i * GUI::MyFactor) + (i * GUI::One_Year_Div));

				if (y > yi && y < yf) {
					year = i;
					// Get the semester
					int s1 = yi + GUI::One_Semester_Div;
					int s2 = yf - GUI::One_Semester_Div;
					
					if (y > yi && y < s1) semester = FALL;
					else if (y > s1 && y < s2) semester = SPRING;
					else if (y > s2 && y < yf) semester = SUMMER;
				}
			}

		}
		else {
			//The user clicked outside the region
		}
		if (year != NULL) {
		CourseInfo chosenCourseInfo = pReg->getCourseInfo(code);
		string Title = chosenCourseInfo.Title;
		int crd = chosenCourseInfo.Credits;
		vector<Course_Code> PreReq = chosenCourseInfo.PreReqList;
		vector<Course_Code> CoReq = chosenCourseInfo.CoReqList;
		Course* pC = new Course(code, Title, crd, PreReq, CoReq);
		pC->setYear(year);
		pC->setSemester(semester);
		pC->setGfxInfo(gInfo);

		StudyPlan* pS = pReg->getStudyPlay();
		pS->AddCourse(pC, year, static_cast<SEMESTER>(semester));
		cout << code << " is added to year " << year << " semester " << semester << endl;
		}
	}

	
	//TODO:
	

	return true;
}


ActionAddCourse::~ActionAddCourse()
{
}
