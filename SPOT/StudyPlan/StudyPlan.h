#pragma once
#include <vector>
#include "AcademicYear.h"
#include "../GUI/Drawable.h"

//A full study plan for as student
class StudyPlan:public Drawable
{

public:
	int TotalCredits=0,				// Total no. of credit hours for courses registred in this year
	    TotalUnivCredits=0,			// Univ Compulsory
		TotalMajorCredits=0,		// Univ Elective
		TotalTrackCredits=0,		// Track Compulsory
		TotalConcentrationCredits=0,// Major Comm Compulsory
		TotalMinorCredits=0;		// Major comm Elec
	int NumberOfConcentrations = 0,
		ConcentrationMajorCredits = 0,
		ConcentrationMinorCredits = 0;
	vector<Course_Code> CompUniCourses,
		ElectiveUniCourses,
		TrackCourses,
		CompMajorCourses,
		ElectiveMajorCourses,
		CompConcentrationCourses,
		ElectiveConcentrationCourses;

	static string PlanNotes;
	vector<AcademicYear*> plan;	//plan is a list of academic years
	StudyPlan();
	bool AddCourse(Course* , int year, SEMESTER);
	bool DeleteCourse(Course* pC);
	void virtual DrawMe(GUI*) const;
	vector<AcademicYear*>* getStudyPlanVector();
	
	virtual ~StudyPlan();
};


