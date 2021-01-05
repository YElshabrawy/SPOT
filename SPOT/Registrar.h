#pragma once
#include "GUI\GUI.h"
#include "Actions\Action.h"
#include "Rules.h"
#include "StudyPlan/StudyPlan.h"


//The maestro class for the application
class Registrar
{
	GUI *pGUI;	//pointer to GUI 
	Rules RegRules;	//Registration rules
	static StudyPlan *pSPlan;
	vector<StudyPlan*>List_Of_All_StudyPlans;
	StudyPlan* pS_Old;
	bool Import_Flag= false;
	bool Delete_Flag = false;
	bool Add_Flag = false;
	bool Erase_Flag = false;
	bool Drag_Flag = false;
	bool Note_Flag = false;
	bool ChangeCode_Flag = false;
	int Current_Study_Plan = 0;
	static int SPSC;
	//Updated
	//static vector<CourseInfo> allCourses;

public:
	Registrar();
	GUI* getGUI() const;
	Action* CreateRequiredAction();
	bool ExecuteAction(Action*);
	void UpdateInterface();
	StudyPlan* getStudyPlay() const;
	Course* OldpCr = nullptr;
	Course* OldpCr_For_DDOOC = nullptr;
	void Run();

	~Registrar();

	// Updated
	void createAllCourses();
	CourseInfo* inCatalog(string code, bool& exists);
	string transformCode(string& code);
	void setCourseOffering();
	void setRules();
	Course* interrogateCourse(int x, int y);
	void Registrar::importProgramReq();
	void Increment_Current_StudyPlan();
	void Decrement_Current_StudyPlan();
	void Add_To_StudyPlan(StudyPlan &pS_New);
	int GetCurrent_Study_Plan()const;
};

