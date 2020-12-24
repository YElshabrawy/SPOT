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
					string pre_crs = preReq[i];
					bool found = 0;
					int currentYear = pCr->getYear() - 1; // its index not its number

					for (int j = currentYear; j >= 0; j--) {
						// check all the years including this year
						list<Course*>* pYr2 = plan[j]->getListOfYears(); // pointer to the year

						int semester_count;
						if (currentYear == j) {
							// in this case only loop on the number of semester
							if (pCr->getSemester() == 0)
								continue; // not to search in the same semester
							semester_count = pCr->getSemester() - 1;
						}
						else {
							// otherwise loop on all 3 semesters
							semester_count = 2;
						}

						for (int k = semester_count; k >= 0; k--) {
							// check all the semester above my semester
							for (auto iter = pYr2[k].begin(); iter != pYr2[k].end(); iter++) {
								if ((*iter)->getCode() == pre_crs) {
									// FOUND IT
									found = true;
									break;
								}
								
							}
							if (found) break;
						}
						if (found) break;
					}

					if (found) {
						// Safe!
						
						pCr->removePreReqErrors(pre_crs);
					}
					else {
						// Critical error: (Prereq not found)
						string ErrorMsg = pre_crs + " is a prerequisite to " + pCr->getCode();
						pCr->AddPreError(CRITICAL, ErrorMsg);
						//cout << "ERROR :: " <<ErrorMsg << endl;
						//cout << "Number of errors = " << pCr->getPreErrorsNumber() << endl;
					}

				}
				if (!preReq.empty()) {
					// This course has a prerequisites
					int err_num = pCr->getPreErrorsNumber(); // number of errors
					int pre_num = preReq.size(); // neber of prerequisite courses
					if (err_num == 0)
						pCr->changeBorderColor(BLACK);
					else 
						pCr->changeBorderColor(RED);

				}
			}
		}
	}
}

StudyPlan::~StudyPlan()
{
}
