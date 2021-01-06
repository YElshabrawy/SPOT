#include "ActionDeclareMinor.h"
#include "..\Registrar.h"
#include <iostream>
#include"../GUI/GUI.h"
#include"../Utils/Utils.h"
#include<sstream>  
#include <algorithm>


ActionDeclareMinor::ActionDeclareMinor(Registrar* P) :Action(P)
{
}
bool ActionDeclareMinor::Execute()
{
	cout << "Bgrb a call" << endl;

	GUI* pGUI = pReg->getGUI();
	char c;//define a character c 
	keytype key = NO_KEYPRESS;
	static int count = 0;//count to check the user enters 5 courses start from negative one caue i increment first therefore i could store in poition zero in an array
	static int counter = 0;//to check if the course is present in the plan 
	int TNOMCTBA = 0;//total number of minor credits to be added
	bool done = false;
	static string Minor = "No Minor";
	string MinorAlternatives[6] = { "ENV 357","ENV 359","ENV 319","CIE 202","ENV 303","ENV 357" };//sample
	//bool flag;
	static string MinorCourses[5] = {};
	int dummy=0;

	key = pGUI->pWind->GetKeyPress(c);
	cout << Minor << endl;//debugging
	if (count == 0)
	{
		pGUI->PrintMsg("Input 5 Minor Courses ");
	}
	else if (count == 1)
	{
		pGUI->PrintMsg(" 4 Courses to be in a minor ");

	}
	else if (count == 2)
	{
		pGUI->PrintMsg(" 3 Courses to be in a minor ");
	}
	else if (count == 3)
	{
		pGUI->PrintMsg(" 2 Courses to be in a minor ");

	}
	else if (count == 4)
	{
		pGUI->PrintMsg(" 1 Courses to be in a minor ");

	}
	else if (count == 5)
	{
		pGUI->GetUserAction(" Congratulations You are now in a Minor ");
		cout << MinorCourses[0] << endl;
		cout << MinorCourses[1] << endl;
		cout << MinorCourses[2] << endl;
		cout << MinorCourses[3] << endl;
		cout << MinorCourses[4] << endl;
		cout << "Minor Already Declared"<<endl;//to be changed
		cout << Minor<<endl;
		return true;
	}

	string coursecode = pGUI->GetSrting();
	//turn the code from small to capital
	for_each(coursecode.begin(), coursecode.end(), [](char& c)
		{
			c = ::toupper(c);
		});

	//Input validations
	if (coursecode == " ")//to exit when the user presses escape
	{
		return true;
	}
	pReg->transformCode(coursecode);

	//Check if exists
	bool exists;
	CourseInfo* pCInfo = pReg->inCatalog(coursecode, exists);
	while (exists == 0)
	{
		pGUI->PrintMsg("ERROR: " + coursecode + " is an invalid course code! Enter a valid one:");
		coursecode = pGUI->GetSrting();
		if (coursecode == " ")//to exit when the user presses escape
			return true;
		pReg->transformCode(coursecode);
		pCInfo = pReg->inCatalog(coursecode, exists);
	}
	//if the course exists we loop through all thecourses in the studyplan
	StudyPlan* plan = pReg->getStudyPlay();
	vector<AcademicYear*>* pPlan = plan->getStudyPlanVector(); // pointer on the plan vector
	for (AcademicYear* yr : *pPlan)
	{
		if (counter > 1)
		{
			pGUI->GetUserAction("You can't have more than 1 common course in the minor");
			cout<<"You can't have more than 1 common course in the minor" << endl;
			break;

		}
		list<Course*>* pYr = yr->getListOfYears(); // pointer to the year
		for (int i = 0; i < SEM_CNT; i++)
		{
			for (auto it = pYr[i].begin(); it != pYr[i].end(); it++)
			{
				// Iterate on courses
				Course* pCr = (*it);
				//if code was found in the studyplan
				if (coursecode == pCr->getCode())
				{
					//increment the static counter by 1
					counter = counter + 1;
					if (counter <= 1)
					{
						count = count + 1;
					}
					//flag = false;
					return true;
				}
			}
		}
	}
	//youssef's part from add course
	/*if (counter <= 1)
	{*/
		ActionData actData = pGUI->GetUserAction("Select where you want to add your course");
		int x, y;
		if (actData.actType == DRAW_AREA)	//user clicked inside drawing area
		{
			int year = NULL;
			SEMESTER semester = FALL;

			//get coord where user clicked
			x = actData.x;
			y = actData.y;

			graphicsInfo gInfo{ x, y };
			if (x > GUI::VerticalSeparatorX && x < GUI::Year_X2)
			{
				// The click X is inside the boundaries

				//Get the Year
				for (int i = 1; i <= GUI::NumOfYrs; i++)
				{
					int yi = (GUI::MenuBarHeight + (i * GUI::MyFactor) + ((i - 1) * GUI::One_Year_Div));
					int yf = (GUI::MenuBarHeight + (i * GUI::MyFactor) + (i * GUI::One_Year_Div));

					if (y > yi && y < yf)
					{
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
			else
			{
				//The user clicked outside the region
			}
			//add the course
			if (year != NULL && pCInfo != nullptr)
			{
				StudyPlan* pS = pReg->getStudyPlay();
				string Title = pCInfo->Title;
				int crd = pCInfo->Credits;
				vector<Course_Code> PreReq = pCInfo->PreReqList;
				vector<Course_Code> CoReq = pCInfo->CoReqList;
				Course* pC = new Course(coursecode, Title, crd, PreReq, CoReq, year, semester);
				pS->AddCourse(pC, year, static_cast<SEMESTER>(semester));
				cout << coursecode << " is added to year " << year << " semester " << semester << endl;
				pC->setYear(year);
				pC->setSemester(semester);
				int iter = pC->numOfCoursesPerSem[(3 * (year - 1)) + semester] - 1;
				gInfo.x = GUI::TitleBarWidth + (iter * CRS_WIDTH);
				gInfo.y = GUI::MenuBarHeight + GUI::MyFactor + ((year - 1) * GUI::One_Year_Div) + (semester * GUI::One_Semester_Div) +
					(GUI::MyFactor * (year - 1));
				pC->setGfxInfo(gInfo);
				count = count + 1;// increment the course count by 1
				done = true;
				pReg->UpdateInterface();


			}
		}
	//}
	////if more than 1 course exists in the studypaln
	//else
	//{	
	//	pGUI->PrintMsg("You can't have more than 1 common course in your minor and your study plan ");
	//	cout << "You can't have more than 1 common course in your minor and your study plan" << endl;
	//	return true;
	//}
	//store the course name in the declared array
	MinorCourses[count - 1] = coursecode;
	cout << MinorCourses[0] << endl;


	if (count == 5)
	{
		for (int j = 0; j < 5; j++)
		{
			for (int i = 0; i < 6; i++)
			{
				if (MinorCourses[j] == MinorAlternatives[i])
				{
					dummy += 1;
					break;
				}
			}
		}
		if (dummy == 5)
		{
			Minor = "Environmental";
		}
	}
	while (key != 4 && done == false)
	{
		key = pGUI->pWind->GetKeyPress(c);
	}
	return true;
}
ActionDeclareMinor::~ActionDeclareMinor()
{

}
