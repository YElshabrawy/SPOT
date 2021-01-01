#pragma once
#include <vector>
#include<string>
#include "AcademicYear.h"
#include "../GUI/Drawable.h"

//A full study plan for as student
class StudyPlan:public Drawable
{
private:
	vector<Error> CH_Error_List; // Holds all the modirate errors for the CH issues
	vector<int> Sem_Credits;
public:
	int TotalCredits = 0,				// Total no. of credit hours for courses registred in this year 1
		TotalUnivCredits = 0,			// Univ Compulsory 2
		TotalMajorCredits = 0,		// Univ Elective 3
		TotalTrackCredits = 0,		// Track Compulsory 4
		TotalConcentrationCredits = 0,// Major Comm Compulsory 5
		TotalMinorCredits = 0,		// Major comm Elec 6
		MaxCredits = 0;
	int NumberOfConcentrations = 0,
		ConcentrationMajorCredits = 0,
		ConcentrationMinorCredits = 0;
	int TotalDoneHours=0;
	string StudentLevel;
	vector<Course_Code> CompUniCourses,
		ElectiveUniCourses,
		TrackCourses,
		CompMajorCourses,
		ElectiveMajorCourses,
		CompConcentrationCourses,
		ElectiveConcentrationCourses;


	string PlanNotes;
	vector<AcademicYear*> plan;	//plan is a list of academic years
	StudyPlan();
	bool AddCourse(Course* , int year, SEMESTER);
	bool DeleteCourse(Course* pC);
	void virtual DrawMe(GUI*) const;
	vector<AcademicYear*>* getStudyPlanVector();
	void checkPreAndCoReq();
	void checkCreditHrs(int min, int max);
	void LiveReport(GUI* pGUI,int min,int max)const;
	void FindPreAndCoReq_ITCSP(Course* pC, GUI* pGUI);
	void GenerateStudentLevel();
	virtual ~StudyPlan();
};


