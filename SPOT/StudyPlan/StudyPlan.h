#pragma once
#include <vector>
#include "AcademicYear.h"
#include "../GUI/Drawable.h"
#include"../Rules.h"

//A full study plan for as student
class StudyPlan:public Drawable
{
private:
	vector<Error> CH_Error_List; // Holds all the modirate errors for the CH issues
	vector<Error> Program_Req_Errors;
	vector<int> Sem_Credits;
	Major major;
	Rules* pRules;
	int concentrationNumber = 0; // Indicates which concentration is sellected (0 = no conc selected yet)
public:
	int TotalCredits = 0,				// Total no. of credit hours for courses registred in this year 1
		TotalMajorCredits = 0,		// Univ Elective 3
		TotalElectiveCredits = 0,
		TotalMinorCredits = 0,		// Major comm Elec 6
		TotalConcentrationCredits = 0,// Major Comm Compulsory 5
		TotalTrackCredits = 0,		// Track Compulsory 4
		TotalUnivCredits = 0,			// Univ Compulsory 2
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

	string PlanNotes;
	vector<AcademicYear*> plan;	//plan is a list of academic years
	StudyPlan();
	bool AddCourse(Course* , int year, SEMESTER);
	bool DeleteCourse(Course* pC);
	void virtual DrawMe(GUI*) const;
	vector<AcademicYear*>* getStudyPlanVector();
	void checkPreAndCoReq();
	void Set_Course_Type();
	void checkCreditHrs(int min, int max);
	void LiveReport(GUI* pGUI,int min,int max)const;
	void FindPreAndCoReq_ITCSP(Course* pC, GUI* pGUI);
	void checkProgramReq();
	void setMajor(Major major);
	void Set_Plan_Rules(Rules& RegRules);
	Major getMajor() const;
	void setCourseTypeCredits(Type type, int mode, int hours);
	void lazyCheck(int compared, int original, string errMsg, string checkMsg);
	virtual ~StudyPlan();
};


