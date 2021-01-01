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
double total = 0;
int totaldonehours = 0;
double GPA;

bool ActionCalculateGPA::Execute()
{
	//enum Grade:double{A=4,Amin=3.7,B};
	const double A = 4.0;
	const double A_MINUS = 3.67;
	const double B_PLUS = 3.33;
	const double B = 3.0;
	const double B_MINUS = 2.67;
	const double C_PLUS = 2.33;
	const double C = 2.0;
	const double C_MINUS = 1.67;
	const double F = 0.0;

	StudyPlan* plan = pReg->getStudyPlay();
	vector<AcademicYear*>* pPlan = plan->getStudyPlanVector(); // pointer on the plan vector
	for (AcademicYear* yr : *pPlan)
	{
		list<Course*>* pYr = yr->getListOfYears(); // pointer to the year
		for (int sem = FALL; sem < SEM_CNT; sem++)
		{
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
				}
			}
		}
	}
	GPA = total / totaldonehours;
	GPA = ceil(GPA * 100.0) / 100.0;
	return true;
}

ActionCalculateGPA::~ActionCalculateGPA()
{
}
