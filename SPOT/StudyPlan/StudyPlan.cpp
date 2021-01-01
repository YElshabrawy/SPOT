#include "StudyPlan.h"
#include"../GUI/GUI.h"
#include"string"
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
				vector<string> coReq = pCr->getCoReq();

				// Co req Check
				for (int i = 0; i < coReq.size(); i++) {
					// For each course in the coreq
					string co_crs = coReq[i];
					bool found = false;
					int currentYr = pCr->getYear() - 1;
					SEMESTER currentSem = pCr->getSemester();
					list<Course*>* pYr2 = plan[currentYr]->getListOfYears();
					for (auto iter = pYr2[currentSem].begin(); iter != pYr2[currentSem].end(); iter++) {
						if ((*iter)->getCode() == co_crs) {
							// FOUND IT
							found = true;
							break;
						}

					}

					if (found) {
						// Safe!
						pCr->removeCoReqErrors(co_crs);
					}
					else {
						// Critical error: (Prereq not found)
						string ErrorMsg = co_crs + " is a corequisite to " + pCr->getCode();
						cout << "ERR :: " << ErrorMsg << endl;
						pCr->AddCoError(CRITICAL, ErrorMsg);
					}
				}
				if (!coReq.empty()) {
					// This course has a coreq
					int err_num = pCr->getCoErrorsNumber(); // number of errors
					int co_num = coReq.size(); // number of prerequisite courses
					if (err_num == 0)
						pCr->changeBorderColor(BLACK);
					else
						pCr->changeBorderColor(GREEN);

				}
				
				//============================================================================
				
				// Pre req Check
				for (int i = 0; i < preReq.size(); i++) {
					// For each course in the prereq
					string pre_crs = preReq[i];
					bool found = false;
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
void StudyPlan::checkCreditHrs(int min, int max)
{
	// For each semester, check if total courses chs are between the range of min n max where min and max are constants from RegRules
	//for (AcademicYear* yr : plan)
	for (int a = 0; a < plan.size(); a++) {
		AcademicYear* yr = plan[a];
		list<Course*>* pYr = yr->getListOfYears(); // pointer to the year
		for (int sem = FALL; sem < SEM_CNT - 1; sem++) {
			int semCHs = 0; // semester CHs
			string semName;
			int yearNum = a+1;
			switch (sem) {
			case 0: semName = "Fall"; break;
			case 1: semName = "Spring"; break;
			case 2: semName = "Summer"; break;
			}
			for (auto it = pYr[sem].begin(); it != pYr[sem].end(); it++) {
				semCHs += (*it)->getCredits();
				yearNum = (*it)->getYear();
			}
			// If the error does not exist create an error
			bool exists = false;
			string checkMsg = semName + " of year " + to_string(yearNum);
			for (int i = 0; i < CH_Error_List.size(); i++) {
				bool condition = CH_Error_List[i].Msg.find(checkMsg) != string::npos;
				if (condition) {
					exists = true;
				}
			}
			if (semCHs < min) {
				if (!exists) {
					// Error less than
					string errMsg = semName + " of year " + to_string(yearNum) + " has CH's ("+ to_string(semCHs) + ") less than " + to_string(min);
					Error newErr;
					newErr.type = MODERATE;
					newErr.Msg = errMsg;
					Sem_Credits.push_back(semCHs);
					CH_Error_List.push_back(newErr);
				}
				else {
					// Modify its message
					string newErrMsg = semName + " of year " + to_string(yearNum) + " has CH's (" + to_string(semCHs) + ") less than " + to_string(min);
					string checkMsg = semName + " of year " + to_string(yearNum);
					for (int i = 0; i < CH_Error_List.size(); i++) {
						bool condition = CH_Error_List[i].Msg.find(checkMsg) != string::npos;
						if (condition) {
							CH_Error_List[i].Msg = newErrMsg;
						}
					}
				}
			}
			else if (semCHs > max) {
				if (!exists) {
					// Error more than
					string errMsg = semName + " of year " + to_string(yearNum) + " has CH's (" + to_string(semCHs) + ")  more than " + to_string(max);
					Error newErr;
					newErr.type = MODERATE;
					newErr.Msg = errMsg;
					CH_Error_List.push_back(newErr);
					Sem_Credits.push_back(semCHs);
				}
				else {
					// Modify its message
					string newErrMsg = semName + " of year " + to_string(yearNum) + " has CH's (" + to_string(semCHs) + ") more than " + to_string(max);
					string checkMsg = semName + " of year " + to_string(yearNum);
					for (int i = 0; i < CH_Error_List.size(); i++) {
						bool condition = CH_Error_List[i].Msg.find(checkMsg) != string::npos;
						if (condition) {
							CH_Error_List[i].Msg = newErrMsg;
						}
					}
				}
			}
			else {
				// No err (REMOVE IT)
				string checkMsg = semName + " of year " + to_string(yearNum);
				for (int i = 0; i < CH_Error_List.size(); i++) {
					bool condition = CH_Error_List[i].Msg.find(checkMsg) != string::npos;
					if (condition) {
						CH_Error_List.erase(CH_Error_List.begin() + i);
					}
				}
				

			}
		}
	}
}
void StudyPlan::FindPreAndCoReq_ITCSP(Course* pC, GUI* pGUI)
{
	vector<string>CoReq = pC->getCoReq();
	vector<string>PreReq = pC->getPreReq();
	string Code;
	if(!((CoReq.empty())&&(PreReq.empty())))
	for (AcademicYear* yr : plan)
	{
		list<Course*>* pYr = yr->getListOfYears(); // pointer to the year
		for (int sem = FALL; sem < SEM_CNT; sem++)
		{
			for (auto it = pYr[sem].begin(); it != pYr[sem].end(); it++)
			{
				if (!((CoReq.size()) == 0))
				for (int i = 0; i < CoReq.size(); i++)
				{
					if (((*it)->getCode() == CoReq[i])&&((*it)!=NULL))
					{
						pGUI->pWind->SetPen(RED,2);
						pGUI->DrawCourse_Dependacies((*it), pC);
						break;
				    }
				}
				if (!((PreReq.size())==0))
				for (int i = 0; i < PreReq.size(); i++)
				{
					Code = (*it)->getCode();
					if ((Code== PreReq[i])&&((*it) != NULL))
					{
						pGUI->pWind->SetPen(BLUE,2);
						pGUI->DrawCourse_Dependacies((*it), pC);
						break;
					}
				}
			}
		}
	}
}
void StudyPlan::LiveReport(GUI* pGUI, int Min_Crs, int Max_Crs)const
{
	int Co_Error_Number, Pre_Error_Number,I=0;
	vector<Error> Co_Errors; 
	vector<Error> Pre_Errors;
	for (AcademicYear* yr : plan)
	{
		list<Course*>* pYr = yr->getListOfYears(); // pointer to the year
		for (int sem = FALL; sem < SEM_CNT; sem++)
		{
			for (auto it = pYr[sem].begin(); it != pYr[sem].end(); it++)
			{
				Co_Error_Number=(*it)->getCoErrorsNumber();
				Pre_Error_Number=(*it)->getPreErrorsNumber();
				if (Co_Error_Number != 0)
				{
					Co_Errors = (*it)->getCoReqErrors();
					for (int i = 0; i < Co_Errors.size(); i++)
					{
						pGUI->PrintCriticalError(Co_Errors[i],I);
						I += 2;
					}

				}
				else if (Pre_Error_Number != 0)
				{
					Pre_Errors = (*it)->getPreReqErrors();
					for (int i = 0; i < Pre_Errors.size(); i++)
					{
						pGUI->PrintCriticalError(Pre_Errors[i], I);
						I += 2;
					}
				}
			}
		}
	}

	for (int i = 0; i < CH_Error_List.size(); i++)
	{
		pGUI->PrintPrintModerateError(CH_Error_List[i], I, Sem_Credits[i], Min_Crs, Max_Crs);
		I += 3;
	}

}
void StudyPlan::Set_Course_Type()
{
	string Code;
		for (AcademicYear* yr : plan)
		{
			list<Course*>* pYr = yr->getListOfYears(); // pointer to the year
			for (int sem = FALL; sem < SEM_CNT; sem++)
			{
				for (auto it = pYr[sem].begin(); it != pYr[sem].end(); it++)
				{
					Code = (*it)->getCode();
					for (int i = 0; i < pRules->UnivCompulsoryCourses.size(); i++)
					{
						if (Code == pRules->UnivCompulsoryCourses[i])
						{
							(*it)->Set_Type(Uni);
							break;
                        }
					}
					for (int i = 0; i < pRules->UnivElectiveCourses.size(); i++)
					{
						if (Code == pRules->UnivElectiveCourses[i])
						{
							(*it)->Set_Type(Elective);
							break;
						}
					}
					for (int i = 0; i < pRules->TrackCompulsoryCourses.size(); i++)
					{
						if (Code == pRules->TrackCompulsoryCourses[i])
						{
							(*it)->Set_Type(Track);
							break;
						}
					}
					for (int i = 0; i < pRules->MajorCompulsoryCourses.size(); i++)
					{
						if (Code == pRules->MajorCompulsoryCourses[i])
						{
							(*it)->Set_Type(maj);
							break;
						}
					}
					for (int i = 0; i < pRules->MajorElectiveCourses.size(); i++)
					{
						if (Code == pRules->MajorElectiveCourses[i])
						{
							(*it)->Set_Type(Elective);
							break;
						}
					}
					//for (int i = 0; i < pRules->Concentrations[0].ConcentrationCompulsoryCourses.size(); i++)
					//{
					//	if (Code == pRules->Concentrations[0].ConcentrationCompulsoryCourses[i])
					//	{
					//		(*it)->Set_Type(concentration);
					//		break;
					//	}
					//}
					//for (int i = 0; i < pRules->Concentrations[0].ConcentrationElectiveCourses.size(); i++)
					//{
					//	if (Code == pRules->Concentrations[0].ConcentrationElectiveCourses[i])
					//	{
					//		(*it)->Set_Type(Elective);
					//		break;
					//	}
					//}
				}
			}
		}
}
void StudyPlan::setMajor(Major major)
{
	this->major = major;
}
Major StudyPlan::getMajor() const
{
	return major;
}
void StudyPlan::Set_Plan_Rules(Rules &RegRules)
{
	pRules = &RegRules;
}
StudyPlan::~StudyPlan()
{
}