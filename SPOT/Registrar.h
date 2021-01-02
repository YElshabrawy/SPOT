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
	StudyPlan *pSPlan;
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

	void importProgramReq();
	Rules const* getRegRules() const;
	void setCatalogCoursesType();

};

