#include "StudyPlan.h"
#include"../GUI/GUI.h"
string StudyPlan::PlanNotes = "";


StudyPlan::StudyPlan()
{
	//By default, the study plan starts with 5 years
	//More year can be added
	for (int i = 0; i < GUI::NumOfYrs; i++)
		plan.push_back(new AcademicYear);
}

//adds a course to the study plan in certain year, semester
//year idetifies year number to add course to 1=first, 2 = 2nd,....
bool StudyPlan::AddCourse(Course* pC, int year, SEMESTER sem)
{
	//TODO: add all requried checks to add the course 
	// DONE IN ACTIONADDCOURSE

	plan[year - 1]->AddCourse(pC, sem);
	return true;
}

bool StudyPlan::DeleteCourse(Course* pC) {
	plan[pC->getYear() - 1]->DeleteCourse(pC, pC->getSemester());
	return true;
}

void StudyPlan::DrawMe(GUI* pGUI) const
{
	//Plan draws all year inside it.
	for (int i = 0; i < plan.size(); i++)
		plan[i]->DrawMe(pGUI);
}

vector<AcademicYear*>* StudyPlan::getStudyPlanVector() {
	return &plan;
}

void StudyPlan::checkPreAndCoReq()
{
	// For each crs
	for (AcademicYear* yr : plan) {
		list<Course*>* pYr = yr->getListOfYears(); // pointer to the year
		for (int sem = FALL; sem < SEM_CNT; sem++) {
			for (auto it = pYr[sem].begin(); it != pYr[sem].end(); it++) {
				// Iterate on courses
				Course* pCr = (*it);
				vector<string> preReq = pCr->getPreReq();

				for (int i = 0; i < preReq.size(); i++) {
					// For each course in the prereq
					bool found = 0;

					for (int j = pCr->getYear(); j > 0; j--) {
						// check all the years including this year
						for (int k = pCr->getSemester() - 1; k >= 0; k--) {
							// check all the semester above my semester
							if (preReq[i] == pCr->getCode()) {
								// FOUND IT
								found = 1;
								break;
							}
						}
						if (found) break;
					}
					
					if (found) {
						pCr->changeColor(MYCYAN);
					}
					else {
						pCr->changeColor(GREENYELLOW);
					}
				}
			}
		}
	}
}

StudyPlan::~StudyPlan()
{
}
