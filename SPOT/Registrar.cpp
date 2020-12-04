#include "Registrar.h"

#include "Actions/allActions.h"
#include"Utils/Utils.h"

vector<CourseInfo> Registrar::allCourses;

Registrar::Registrar()
{
	pGUI = new GUI;	//create interface object
	pSPlan = new StudyPlan;	//create a study plan.
}

//returns a pointer to GUI
GUI* Registrar::getGUI() const
{
	return pGUI;
}

//returns the study plan
StudyPlan* Registrar::getStudyPlay() const
{
	return pSPlan;
}

Action* Registrar::CreateRequiredAction() 
{	
	ActionData actData = pGUI->GetUserAction("Pick an action from the menu");
	Action* RequiredAction = nullptr;

	switch (actData.actType)
	{
	case ADD_CRS:	//add_course action
		RequiredAction = new ActionAddCourse(this);
		break;
	case DEL_CRS:
		RequiredAction = new ActionDeleteCourse(this);
		break;
	case SAVE: // save plan
		RequiredAction = new ActionSavePlan(this);
		break;
	case ADD_Note:
		RequiredAction = new ActionAddNotes(this);
		break;
	case IMPORT_PLAN:
		RequiredAction = new ActionImportPlan(this);
		break;
	case ERASE:
		RequiredAction = new ActionEraseAll(this);
		break;
	case SWAP:
		RequiredAction = new ActionMoveCourse(this);
		break;
	case CHANGE_CODE:
		RequiredAction = new ActionChangeCode(this);
		break;
	default:
		RequiredAction = new ActionCourseInfo(this);
		break;

	//TODO: Add case for each action
	
	/*case EXIT:
		break;
		*/
	}
	return RequiredAction;
}

//Executes the action, Releases its memory, and return true if done, false if cancelled
bool Registrar::ExecuteAction(Action* pAct)
{
	bool done = pAct->Execute();
	delete pAct;	//free memory of that action object (either action is exec or cancelled)
	return done;
}

void Registrar::Run()
{
	// create all courses vector:
	createAllCourses();

	while (true)
	{
		//update interface here as CMU Lib doesn't refresh itself
		//when window is minimized then restored
		UpdateInterface();

		Action* pAct = CreateRequiredAction();
		if (pAct)	//if user doesn't cancel
		{
			if (ExecuteAction(pAct))	//if action is not cancelled
				UpdateInterface();
		}
	}
}


void Registrar::UpdateInterface()
{
	pGUI->UpdateInterface();	//update interface items
	pSPlan->DrawMe(pGUI);		//make study plan draw itself
}

Registrar::~Registrar()
{
	delete pGUI;
}

// Updated
void Registrar::createAllCourses() {
	// Create a vector of all courses //
	allCourses = createAllCoursesVector("./Format Files/All_Courses.txt");
	cout << "All Courses In (./Format Files/All_Courses.txt) Are Loaded Successfully.\n";
}

vector<CourseInfo> Registrar::getAllCourses() {
	return allCourses;
}

CourseInfo Registrar::getCourseInfo(Course_Code code) {
	for (CourseInfo c : allCourses) {
		if (c.Code == code) {
			return c;
			break;
		}
	}
}