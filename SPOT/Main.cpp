#include "Registrar.h"
int main()
{
	vector<AcademicYear*> myYears[5];
	//Year 1
	AcademicYear y1;
	list<Course*> y1Courses[SEM_CNT];
	Course* pC1 = new Course("MATH 101", "Calcs I", 3);
	y1Courses[FALL].push_back(pC1); 
	Course* pC2 = new Course("BIOL 111", "Bio lab", 1);
	y1Courses[FALL].push_back(pC2);
	Course* pC3 = new Course("CSCI 101", "CS", 2);
	y1Courses[SPRING].push_back(pC3);
	myYears[0].push_back(&y1);

	Registrar Reg;
	Reg.Run();
}