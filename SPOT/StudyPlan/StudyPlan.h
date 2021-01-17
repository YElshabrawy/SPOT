#pragma once
#include <vector>
#include<string>
#include<iterator>
#include "AcademicYear.h"
#include "../GUI/Drawable.h"
#include"../Rules.h"

//A full study plan for as student
class StudyPlan:public Drawable
{
private:
	vector<Error> CH_Error_List; // Holds all the modirate errors for the CH issues
	vector<Error> Program_Req_Errors; // Holds all the critical program requirements errors
	vector<Error> Course_Offering_Errors; // Holds all the modirate errors for adding a course out of offering list
	
	vector<int> Sem_Credits;
	int Report_Lines;
	Course* pCrs;
	Major major;
	Major Doublemajor;
	Concentrations concentrations;
	Concentrations Doubleconcentrations;
	bool Minor_course_flag;
	Rules* pRules;
	int concentrationNumber = 0; // Indicates which concentration is sellected (0 = no conc selected yet)

	bool currentYearAvailable; // true if user chose a current year. false if not.
	string currentYear = "2020\\2021"; // Current Year of offerings (In date)
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
	int TotalDoneHours=0;
	string StudentLevel;
	vector<Course_Code> CompUniCourses,
		ElectiveUniCourses,
		TrackCourses,
		CompMajorCourses,
		ElectiveMajorCourses,
		CompConcentrationCourses,
		ElectiveConcentrationCourses;
	int No_Of_Pages;
	string PlanNotes="";
	vector<AcademicYear*> plan;	//plan is a list of academic years
	vector<string>Minor_Course;
	static int Count;
	StudyPlan();
	bool AddCourse(Course* , int year, SEMESTER);
	bool DeleteCourse(Course* pC);
	void virtual DrawMe(GUI*) const;
	vector<AcademicYear*>* getStudyPlanVector();
	void checkPreAndCoReq();
	void Set_Course_Type();
	void checkCreditHrs(int min, int max);
	void LiveReport(GUI* pGUI,int min,int max);
	void FindPreAndCoReq_ITCSP(Course* pC, GUI* pGUI);
	void checkProgramReq();
	void setMinor_course_flag(bool cond);
	void setMajor(Major major);
	void setDoubleMajor(Major major);
	void setConcentration(Concentrations concentration);
	void setDoubleConcentration(Concentrations concentration);
	Major getMajor() const;
	Major getDoubleMajor() const;
	Concentrations getConcentration()const;
	Concentrations getDoubleConcentration() const;
	void Set_Plan_Rules(Rules& RegRules);
	void setCourseTypeCredits(Type type, int mode, int hours);
	void lazyCheck(int compared, int original, string errMsg, string checkMsg);
	void checkOffering(string code, int crsYear, SEMESTER sem);
	void GenerateStudentLevel(GUI* pGUI);
	void increment_Report_Lines(int Number_Of_Inc);
	int get_Report_Lines()const;
	vector<int> get_Sem_Credits()const;
	void Set_Report_Lines();
	void Set_Page_Number(int Number_Of_lines);
	int Get_Page_Number()const;
	StudyPlan(const StudyPlan& CopiedSP);
	StudyPlan operator=(const StudyPlan& CopiedSP);
	virtual ~StudyPlan();
};


