#include "ActionCalculateGPA.h"
#include "..\Registrar.h"
#include<iterator>
#include <iostream>
#include"../GUI/GUI.h"
#include"../Utils/Utils.h"
#include <cmath>
ActionCalculateGPA::ActionCalculateGPA(Registrar* P) :Action(P)
{
}


bool ActionCalculateGPA::Execute()
{
	vector<string>GPA_OF_All_Semesters;
	StudyPlan* plan = pReg->getStudyPlay();
	vector<AcademicYear*>* pPlan = plan->getStudyPlanVector(); // pointer on the plan vector
	double DummyTotal = 0, DummyTotalDone = 0;
	for (AcademicYear* yr : *pPlan)
	{
		list<Course*>* pYr = yr->getListOfYears(); // pointer to the year
		for (int sem = FALL; sem < SEM_CNT; sem++)
		{
			DummyTotal = 0; DummyTotalDone = 0;
			for (auto it = pYr[sem].begin(); it != pYr[sem].end(); it++)
			{
				// Iterate on courses
				Course* pCr = (*it);
				if (pCr->getCoursedone() == true)
				{
					int credits=pCr->getCredits();
					double dummy = 0;
					if (pCr->getGrade() == "A")
					{
						dummy = A * credits;
					}
					else if (pCr->getGrade() == "A_MINUS")
					{
						dummy = A_MINUS * credits;
					}
					else if (pCr->getGrade() == "B_PLUS")
					{
						dummy = B_PLUS * credits;
					}
					else if (pCr->getGrade() == "B")
					{
						dummy = B * credits;
					}
					else if (pCr->getGrade() == "B_MINUS")
					{
						dummy = B_MINUS * credits;
					}
					else if (pCr->getGrade() == "C_PLUS")
					{
						dummy = C_PLUS * credits;
					}
					else if (pCr->getGrade() == "C_MINUS")
					{
						dummy = C_MINUS * credits;
					}
					else if (pCr->getGrade() == "C")
					{
						dummy = C * credits;
					}
					else if (pCr->getGrade() == "F")
					{
						dummy = F * credits;
					}
					total = total + dummy;
					totaldonehours = totaldonehours + credits;
					DummyTotal = total;
					DummyTotalDone = totaldonehours;
				}
		
			}
			if ((DummyTotal != 0) && (DummyTotalDone != 0))
			{
				string str = to_string(DummyTotal/DummyTotalDone);
				GPA_OF_All_Semesters.push_back(str.substr(0,4));
			}
			else
			{
				GPA_OF_All_Semesters.push_back("0");
			}
		}

	}
	GPA = total / totaldonehours;
	GPA = ceil(GPA * 100.0) / 100.0;
	GUI* pGUI = pReg->getGUI();
	if ((GPA!=0)&&(totaldonehours!=0))
	{
		string str = to_string(GPA);
		pGUI->GPA = str;
	}
	for(int i=0;i< GPA_OF_All_Semesters.size();i++)
	pGUI->GPA_Semesters.push_back(GPA_OF_All_Semesters[i]);
	cout << GPA<<endl;
	return true;
}
double ActionCalculateGPA::getGPA() const
{
	return GPA;
}
ActionCalculateGPA::~ActionCalculateGPA()
{
}
