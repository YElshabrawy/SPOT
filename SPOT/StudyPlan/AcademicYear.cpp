#include "AcademicYear.h"
#include "../GUI/GUI.h"
int AcademicYear::TotalYears = 1;

AcademicYear::AcademicYear()
{
	TotalYears++;
}


AcademicYear::~AcademicYear()
{
}

//Adds a course to this year in the spesified semester
bool AcademicYear::AddCourse(Course* pC, SEMESTER sem)
{
	//TODO:
	//This function still needs many checks to be compelete
	YearCourses[sem].push_back(pC);
	TotalCredits += pC->getCredits();
	if (pC->CCC_Flag == true)
	{
		pC->CCC_Flag = false;
		pC->Distance_Flag = true;
		return true;
	}
	else
	{
		/*pC->Distance_Flag = false;*/
		return true;
	}
}

bool AcademicYear::DeleteCourse(Course* pC, SEMESTER sem) {

	YearCourses[sem].remove(pC);
	TotalCredits -= pC->getCredits();
	Course::numOfCoursesPerSem[(3 * (pC->getYear() - 1)) + sem]--;
	if (pC->CCC_Flag == true)
	{
		pC->CCC_Flag = false;
		return true;
	}

	pC->Distance_Flag = false;
		for (int sem = FALL; sem < SEM_CNT; sem++)
			for (auto it = YearCourses[sem].begin(); it != YearCourses[sem].end(); ++it)
			{
				(*it)->Distance_Flag = false;
			}

	return true;
}

void AcademicYear::DrawMe(GUI* pGUI) const
{
	int Counter = 0;
	pGUI->DrawAcademicYear(this);
	//Draw all semesters inside this year by iterating on each semester list
	//to get courses and draw each course
	for (int sem = FALL; sem < SEM_CNT; sem++)
	{
		Counter = 0;
		for (auto it = YearCourses[sem].begin(); it != YearCourses[sem].end(); ++it)
		{
			
			if ((Counter == 0)&& (*it != NULL))
			{
				graphicsInfo gInfo_Old = (*it)->getGfxInfo();
				int new_x = 60;
				int new_y = gInfo_Old.y;
				graphicsInfo New_gInfo{ new_x, new_y };
				(*it)->Distance_Flag = true;
				(*it)->setGfxInfo(New_gInfo);
				
			}
			else if((Counter>0)&&((!(*it)->Distance_Flag))&&(*it != NULL))
			{
				graphicsInfo gInfo_Old = (*it)->getGfxInfo();
				int new_x = gInfo_Old.x+40*Counter;
				int new_y = gInfo_Old.y;
				graphicsInfo New_gInfo{ new_x, new_y };
				(*it)->setGfxInfo(New_gInfo);
				(*it)->Distance_Flag = true;
			}
			if (((*it)->DrawMe_Flag))
			{
				(*it)->DrawMe(pGUI); //call DrawMe for each course in this semester
			}  
			Counter++;
		}
	}
}

list<Course*>* AcademicYear::getListOfYears() {
	//list<Course*>* ptr = new list<Course*>;
	//return ptr;
	return YearCourses;
}