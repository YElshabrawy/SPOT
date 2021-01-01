#pragma once
#include <vector>
#include "AcademicYear.h"
#include "../GUI/Drawable.h"

//A full study plan for as student
class StudyPlan:public Drawable
{
private:
	vector<Error> CH_Error_List; // Holds all the modirate errors for the CH issues
	Major major;

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
	void checkPreAndCoReq();
	void checkCreditHrs(int min, int max);
	void FindPreAndCoReq_ITCSP(Course* pC, GUI* pGUI);
	void importProgramReq();
	void setMajor(Major major);
	Major getMajor() const;
	virtual ~StudyPlan();
};


