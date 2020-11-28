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
	cout << "Add button is pressed.\n"; // for debugging
	GUI* pGUI = pReg->getGUI();	

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
	
	// 2) Get Year
	ActionData actData = pGUI->GetUserAction("Select a year to add course to: "); 
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

	int x, y;
	if (actData.actType == DRAW_AREA)	//user clicked inside drawing area
	{
		//get coord where user clicked
		x = actData.x;
		y = actData.y;
		
		
		
		graphicsInfo gInfo{ x, y };

		//TODO: given course code, get course title, crd hours from registrar
		CourseInfo chosenCourseInfo = pReg->getCourseInfo(code);
		string Title = chosenCourseInfo.Title;
		int crd = chosenCourseInfo.Credits;
		Course* pC = new Course(code, Title, crd);
		pC->setGfxInfo(gInfo);

		//TODO: Ask registrar to add course to the year selected by the user
		//TODO: add the course to the correct year obtained from registrar

		//For the seke of demo, we will add the course to the 1st year, 1st semester
		StudyPlan* pS = pReg->getStudyPlay();
		pS->AddCourse(pC, year, static_cast<SEMESTER>(semester));
	}

	
	//TODO:
	

	return true;
}


ActionAddCourse::~ActionAddCourse()
{
}
