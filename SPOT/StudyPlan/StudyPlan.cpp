#include "StudyPlan.h"
#include"../GUI/GUI.h"
#include"string"
#include "../Rules.h"
#include "../Utils/Utils.h"
#include <fstream>
#include <iterator>


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
	plan[year - 1]->AddCourse(pC, sem);
	TotalCredits += pC->getCredits();
	setCourseTypeCredits(pC->getType(), 0, pC->getCredits());
	checkOffering(pC->getCode(), year-1, sem);
	return true;
}
bool StudyPlan::DeleteCourse(Course* pC) {
	plan[pC->getYear() - 1]->DeleteCourse(pC, pC->getSemester());
	TotalCredits -= pC->getCredits();
	setCourseTypeCredits(pC->getType(), 1, pC->getCredits());
	// Delete the couurse offering error
	for (int i = 0; i < Course_Offering_Errors.size(); i++) 
		if (Course_Offering_Errors[i].Msg.find(pC->getCode()) != string::npos) {
			Course_Offering_Errors.erase(Course_Offering_Errors.begin() + i);
			break;
		}
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
void StudyPlan::checkProgramReq()
{
	/*cout << "TotalCredits = " << TotalCredits << endl;
	cout << "TotalMajorCredits = " << TotalMajorCredits << endl;
	cout << "TotalElectiveCredits = " << TotalElectiveCredits << endl;
	cout << "TotalMinorCredits = " << TotalMinorCredits << endl;
	cout << "TotalConcentrationCredits = " << TotalConcentrationCredits << endl;
	cout << "TotalTrackCredits = " << TotalTrackCredits << endl;
	cout << "TotalUnivCredits = " << TotalUnivCredits << endl;*/

	// Check TotalCredits
	string errMsg = "The total CHs (" + to_string(TotalCredits) + ") are less than " + to_string(pRules->TotalCHs);
	string checkMsg = "The total CHs";
	lazyCheck(TotalCredits, pRules->TotalCHs, errMsg, checkMsg);
	
	// Check TotalMajorCredits
	errMsg = "The total major CHs (" + to_string(TotalMajorCredits) + ") are less than " + to_string(pRules->ReqMajorCredits);
	checkMsg = "The total major CHs";
	lazyCheck(TotalMajorCredits, pRules->ReqMajorCredits, errMsg, checkMsg);

	// Check TotalTrackCredits
	errMsg = "The total track CHs (" + to_string(TotalTrackCredits) + ") are less than " + to_string(pRules->ReqTrackCredits);
	checkMsg = "The total track CHs";
	lazyCheck(TotalTrackCredits, pRules->ReqTrackCredits, errMsg, checkMsg);

	// Check TotalUnivCredits
	errMsg = "The total univ CHs (" + to_string(TotalUnivCredits) + ") are less than " + to_string(pRules->ReqUnivCredits);
	checkMsg = "The total univ CHs";
	lazyCheck(TotalUnivCredits, pRules->ReqUnivCredits, errMsg, checkMsg);

	// Check TotalUnivCredits
	errMsg = "The total concentration CHs (" + to_string(TotalUnivCredits) + ") are less than " + to_string(pRules->ReqUnivCredits);
	checkMsg = "The total concentration CHs";
	int ReqConcentrationCredits = 0;
	if (concentrationNumber > 0) {
		// First we need to remove the No Concentrencration error if exists :)
		string concMsg = "No concentration is selected yet!";
		for (int i = 0; i < Program_Req_Errors.size(); i++) {
			if (Program_Req_Errors[i].Msg.find(concMsg) != string::npos)
				Program_Req_Errors.erase(Program_Req_Errors.begin() + i);
		}

		// Then check for the concentration requirements
		ReqConcentrationCredits += pRules->Concentrations[concentrationNumber - 1].CompulsoryCredits;
		ReqConcentrationCredits += pRules->Concentrations[concentrationNumber - 1].ElectiveCredits;
		lazyCheck(TotalConcentrationCredits, ReqConcentrationCredits, errMsg, checkMsg);
	}
	else {
		// No concentration is selected! the user should select one!
		errMsg = "No concentration is selected yet!";
		// Check if this error already exists
		bool exists = false;
		for (int i = 0; i < Program_Req_Errors.size(); i++) {
			if (Program_Req_Errors[i].Msg.find(errMsg) != string::npos) {
				exists = true;
			}
		}
		if (!exists) {
			// The error does not exist so create it !
			Error err;
			err.Msg = errMsg;
			err.type = CRITICAL;
			Program_Req_Errors.push_back(err);
		}
	}

}
void StudyPlan::lazyCheck(int compared, int original, string errMsg, string checkMsg) {
	if (compared < original) {
		// Check if already exists. If so modify its message!
		bool exists = false;
		for (int i = 0; i < Program_Req_Errors.size(); i++) {
			if (Program_Req_Errors[i].Msg.find(checkMsg) != string::npos) {
				exists = true;
				Program_Req_Errors[i].Msg = errMsg; // modify
			}
		}
		if (!exists) {
			// The error does not exist so create it !
			Error err;
			err.Msg = errMsg;
			err.type = CRITICAL;
			Program_Req_Errors.push_back(err);
		}
	}
	else {
		// Does not exist at all!
		for (int i = 0; i < Program_Req_Errors.size(); i++) {
			if (Program_Req_Errors[i].Msg.find(checkMsg) != string::npos)
				Program_Req_Errors.erase(Program_Req_Errors.begin() + i);
		}

	}
}
void StudyPlan::checkOffering(string code, int crsYear, SEMESTER sem)
{
	int currentYearNum = 0;
	if (currentYear.empty()) {
		// The user hasen't declared an offering 
		currentYearAvailable = false;
		string errMsg = "Please choose the current year.";
		Error err;
		err.Msg = errMsg;
		err.type = CRITICAL;
		Course_Offering_Errors.push_back(err);
	} 
	else {
		currentYearAvailable = true;
		// First: remove the "Please choose the current year." error
		for (int i = 0; i < Course_Offering_Errors.size(); i++) {
			if (Course_Offering_Errors[i].Msg.find("Please choose the current year.") != string::npos)
				Course_Offering_Errors.erase(Course_Offering_Errors.begin() + i);
		}

		// Then : Check each course
		int index = 0; // default value = 0
		for (int i = 0; i < pRules->OffringsList.size(); i++) {
			if (pRules->OffringsList[i].Year == currentYear) {
				index = i;
				break;
			}
		}
		if (pRules->OffringsList.size() > index + crsYear) {


			vector<Course_Code> crss = (pRules->OffringsList[index + crsYear].Offerings)[sem];
			bool exists = false;
			for (Course_Code c : crss) {
				if (c == code) {
					exists = true;
					break;
				}
			}

			if (!exists) {
				// The current course isnt offered this year!
				Error err;
				string semName;
				switch (sem)
				{
				case FALL:
					semName = "fall";
					break;
				case SPRING:
					semName = "spring";
					break;
				case SUMMER:
					semName = "summer";
					break;
				default:
					semName = "NaN";
					break;
				}
				// This index exists
				string errMsg = code + " isn't offered in " + pRules->OffringsList[index + crsYear].Year
					+ "'s " + semName;
				err.Msg = errMsg;
				err.type = MODERATE;
				Course_Offering_Errors.push_back(err);

			}
		}
		else
			cout << "This year is not incluided in the offering files!" << endl;
			
	}

}
void StudyPlan::setMajor(Major major)
{
	this->major = major;
}
void StudyPlan::setConcentration(Concentrations concentration)
{
	this->concentrations = concentration;
}
Concentrations StudyPlan::getConcentration() const
{
	return concentrations;
}
Major StudyPlan::getMajor() const
{
	return major;
}
void StudyPlan::setCourseTypeCredits(Type type, int mode, int hours)
{
	// If mode is 0 => Add Course 
	// If mode is 1 => Delete Course 
	switch (type)
	{
	case maj:
		(mode == 0) ? TotalMajorCredits += hours : TotalMajorCredits -= hours;
		break;
	case Elective:
		(mode == 0) ? TotalElectiveCredits += hours : TotalElectiveCredits -= hours;
		break;
	case Minor:
		(mode == 0) ? TotalMinorCredits += hours : TotalMinorCredits -= hours;
		break;
	case concentration:
		(mode == 0) ? TotalConcentrationCredits += hours : TotalConcentrationCredits -= hours;
		break;
	case Track:
		(mode == 0) ? TotalTrackCredits += hours : TotalTrackCredits -= hours;
		break;
	case Uni:
		(mode == 0) ? TotalUnivCredits += hours : TotalUnivCredits -= hours;
		break;
	case NOTYPE:
		break;
	default:
		break;
	}
}
void StudyPlan::LiveReport(GUI* pGUI, int Min_Crs, int Max_Crs)
{
	int Co_Error_Number, Pre_Error_Number;
	vector<Error> Co_Errors; 
	vector<Error> Pre_Errors;
	Set_Report_Lines();
	pGUI->Report_Start = -1;
	pGUI->Report_Stop = 11;
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
						pGUI->AddCriticalErrorLines(Co_Errors[i]);
						increment_Report_Lines(2);
					}

				}
				else if (Pre_Error_Number != 0)
				{
					Pre_Errors = (*it)->getPreReqErrors();
					for (int i = 0; i < Pre_Errors.size(); i++)
					{
						pGUI->AddCriticalErrorLines(Pre_Errors[i]);
						increment_Report_Lines(2);
					}
				}
			}
		}
	}

	for (int i = 0; i < Program_Req_Errors.size(); i++)
	{
		pGUI->AddCriticalErrorLines(Program_Req_Errors[i]);
		increment_Report_Lines(2);
	}

	for (int i = 0; i < Course_Offering_Errors.size(); i++)
	{
		pGUI->AddModerateErrorLines(Course_Offering_Errors[i]);
		increment_Report_Lines(2);
	}

	for (int i = 0; i < CH_Error_List.size(); i++)
	{
		pGUI->AddModerateErrorLines(CH_Error_List[i], Sem_Credits[i], Min_Crs, Max_Crs);
		increment_Report_Lines(3);
	}
	Set_Page_Number((pGUI->Y_div/45)-2);
	pGUI->Total_Number_Pages_In_Report = (Get_Page_Number());
}
void StudyPlan::GenerateStudentLevel(GUI* pGUI)
{
	for (AcademicYear* yr : plan) {
		list<Course*>* pYr = yr->getListOfYears(); // pointer to the year
		for (int sem = FALL; sem < SEM_CNT; sem++) {
			for (auto it = pYr[sem].begin(); it != pYr[sem].end(); it++) {
				// Iterate on courses
				Course* pCr = (*it);
				if (pCr->getCoursedone() == true && pCr->getToken()==false)
				{
					pCr->setToken(true);
					int credits = pCr->getCredits();
					TotalDoneHours = TotalDoneHours + credits;

				}
			}
		}
	}
	if (TotalDoneHours <= 32)
	{
		StudentLevel = "Foundation";
	}
	else if (TotalDoneHours<=67 && TotalDoneHours >= 32)
	{
		StudentLevel = "Sophomore";
	}
	else if (TotalDoneHours <=101 && TotalDoneHours >= 67)
	{
		StudentLevel = "Junior";
	}
	else if (TotalDoneHours <=135 && TotalDoneHours >= 101)
	{
		StudentLevel = "SeniorI";
	}
	else if (TotalDoneHours <= 162 && TotalDoneHours >= 135)
	{
		StudentLevel = "SeniorII";
	}
	//pGUI->studentLevel = StudentLevel;
	cout << TotalDoneHours << endl;//debugging 
	cout << StudentLevel << endl;//debugging 
	cout << TotalDoneHours << endl;//debugging 
	cout << "Generate student level called"<<endl;//debugging
	pGUI->Student_Level =StudentLevel;
	string str = to_string(TotalDoneHours);
	pGUI->Done_Credits = str;
}
int StudyPlan::Get_Page_Number()const
{
	return No_Of_Pages;
}
void StudyPlan::increment_Report_Lines(int Number_Of_Inc)
{
	Report_Lines += Number_Of_Inc;
}
int StudyPlan::get_Report_Lines()const
{
	return Report_Lines;
}
void StudyPlan::Set_Report_Lines()
{
	Report_Lines = 0;
}
void StudyPlan::Set_Course_Type()
{
	string Code = "";
	for (AcademicYear* yr : plan)
	{
		list<Course*>* pYr = yr->getListOfYears(); // pointer to the year
		for (int sem = FALL; sem < SEM_CNT; sem++)
			for (auto it = pYr[sem].begin(); it != pYr[sem].end(); it++)
			{
				Code = (*it)->getCode();
				for (int i = 0; i < pRules->UnivCompulsoryCourses.size(); i++)
				{
					if (Code == pRules->UnivCompulsoryCourses[i])
					{

						if (Code == pRules->UnivCompulsoryCourses[i])
						{
							(*it)->Set_Type(Uni);
							if ((*it)->getColor() == BLACK)
							{
								break;
							}
							(*it)->changeColor(SLATEGREY);
							break;
						}
					}
				}
				for (int i = 0; i < pRules->UnivElectiveCourses.size(); i++)
				{
					if (Code == pRules->UnivElectiveCourses[i])
					{
						if (Code == pRules->UnivElectiveCourses[i])
						{
							(*it)->Set_Type(Elective);
							if ((*it)->getColor() == BLACK)
							{
								break;
							}
							(*it)->changeColor(FIREBRICK);
							break;
						}
					}
				}
				for (int i = 0; i < pRules->TrackCompulsoryCourses.size(); i++)
				{
					if (Code == pRules->TrackCompulsoryCourses[i])
					{
						if (Code == pRules->TrackCompulsoryCourses[i])
						{
							(*it)->Set_Type(Track);
							if ((*it)->getColor() == BLACK)
							{
								break;
							}
							(*it)->changeColor(DARKGREEN);
							break;
						}
					}
				}
				for (int i = 0; i < pRules->MajorCompulsoryCourses.size(); i++)
				{
					if (Code == pRules->MajorCompulsoryCourses[i])
					{
						if (Code == pRules->MajorCompulsoryCourses[i])
						{
							(*it)->Set_Type(maj);
							if ((*it)->getColor() == BLACK)
							{
								break;
							}
							(*it)->changeColor(GOLDENROD);
							break;
						}
					}
				}
				for (int i = 0; i < pRules->MajorElectiveCourses.size(); i++)
				{
					if (Code == pRules->MajorElectiveCourses[i])
					{
						if (Code == pRules->MajorElectiveCourses[i])
						{
							(*it)->Set_Type(Elective);
							if ((*it)->getColor() == BLACK)
							{
								break;
							}
							(*it)->changeColor(FIREBRICK);
							break;
						}
					}
					if (pRules->NumOfConcentrations != 0)
					{
						for (int i = 0; i < pRules->Concentrations[0].ConcentrationCompulsoryCourses.size(); i++)
						{
							if (Code == pRules->Concentrations[0].ConcentrationCompulsoryCourses[i])
							{
								(*it)->Set_Type(concentration);
								if ((*it)->getColor() == BLACK)
								{
									break;
								}
								(*it)->changeColor(DARKMAGENTA);
								break;
							}
						}
						for (int i = 0; i < pRules->Concentrations[0].ConcentrationElectiveCourses.size(); i++)
						{
							if (Code == pRules->Concentrations[0].ConcentrationElectiveCourses[i])
							{
								(*it)->Set_Type(Elective);
								if ((*it)->getColor() == BLACK)
								{
									break;
								}
								(*it)->changeColor(FIREBRICK);
								break;
							}
						}
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
	/*			for (int i = 0; i < pRules->Concentrations[0].ConcentrationElectiveCourses.size(); i++)
				{
					if (Code == pRules->Concentrations[0].ConcentrationElectiveCourses[i])
					{
						(*it)->Set_Type(Elective);
						if ((*it)->getColor() == BLACK)
						{
							break;
						}
						(*it)->changeColor(FIREBRICK);
						break;
					}
				}*/
			}
	}
}
void  StudyPlan::Set_Page_Number( int Number_Of_lines)
{
	No_Of_Pages=Report_Lines/ Number_Of_lines;
}
vector<int> StudyPlan::get_Sem_Credits()const
{
	return Sem_Credits;
}
void StudyPlan::Set_Plan_Rules(Rules &RegRules)
{
	pRules = &RegRules;
}
StudyPlan::StudyPlan(const StudyPlan& CopiedSP):Drawable()
{
	PlanNotes = CopiedSP.PlanNotes;
	Report_Lines = CopiedSP.Report_Lines;
	No_Of_Pages = CopiedSP.No_Of_Pages;
	TotalDoneHours = CopiedSP.TotalDoneHours;
	ConcentrationMinorCredits = CopiedSP.ConcentrationMinorCredits;
	ConcentrationMajorCredits = CopiedSP.ConcentrationMajorCredits;
	NumberOfConcentrations = CopiedSP.NumberOfConcentrations;
	MaxCredits = CopiedSP.MaxCredits;
	TotalMinorCredits = CopiedSP.TotalMinorCredits;
	TotalConcentrationCredits = CopiedSP.TotalConcentrationCredits;
	TotalTrackCredits = CopiedSP.TotalTrackCredits;
	TotalMajorCredits = CopiedSP.TotalMajorCredits;
	TotalUnivCredits = CopiedSP.TotalUnivCredits;
	TotalCredits = CopiedSP.TotalCredits;
	StudentLevel = CopiedSP.StudentLevel;
	major = CopiedSP.major;
	pRules = CopiedSP.pRules;
	for (int i = 0; i < CopiedSP.Sem_Credits.size(); i++)
	{
		Sem_Credits.push_back(CopiedSP.Sem_Credits[i]);
	}
	for (int i = 0; i < CopiedSP.CH_Error_List.size(); i++)
	{
		CH_Error_List.push_back(CopiedSP.CH_Error_List[i]);
	}
	for (int i = 0; i < CopiedSP.CompUniCourses.size(); i++)
	{
		CompUniCourses.push_back(CopiedSP.CompUniCourses[i]);
	}
	for (int i = 0; i < CopiedSP.ElectiveUniCourses.size(); i++)
	{
		ElectiveUniCourses.push_back(CopiedSP.ElectiveUniCourses[i]);
	}
	for (int i = 0; i < CopiedSP.TrackCourses.size(); i++)
	{
		TrackCourses.push_back(CopiedSP.TrackCourses[i]);
	}
	for (int i = 0; i < CopiedSP.CompMajorCourses.size(); i++)
	{
		CompMajorCourses.push_back(CopiedSP.CompMajorCourses[i]);
	}
	for (int i = 0; i < CopiedSP.ElectiveMajorCourses.size(); i++)
	{
		ElectiveMajorCourses.push_back(CopiedSP.ElectiveMajorCourses[i]);
	}
	for (int i = 0; i < CopiedSP.CompConcentrationCourses.size(); i++)
	{
		CompConcentrationCourses.push_back(CopiedSP.CompConcentrationCourses[i]);
	}
	for (int i = 0; i < CopiedSP.ElectiveConcentrationCourses.size(); i++)
	{
		ElectiveConcentrationCourses.push_back(CopiedSP.ElectiveConcentrationCourses[i]);
	}

}
StudyPlan StudyPlan::operator=(const StudyPlan& CopiedSP)
{
	PlanNotes = CopiedSP.PlanNotes;
	Report_Lines = CopiedSP.Report_Lines;
	No_Of_Pages = CopiedSP.No_Of_Pages;
	TotalDoneHours = CopiedSP.TotalDoneHours;
	ConcentrationMinorCredits = CopiedSP.ConcentrationMinorCredits;
	ConcentrationMajorCredits = CopiedSP.ConcentrationMajorCredits;
	NumberOfConcentrations = CopiedSP.NumberOfConcentrations;
	MaxCredits = CopiedSP.MaxCredits;
	TotalMinorCredits = CopiedSP.TotalMinorCredits;
	TotalConcentrationCredits = CopiedSP.TotalConcentrationCredits;
	TotalTrackCredits = CopiedSP.TotalTrackCredits;
	TotalMajorCredits = CopiedSP.TotalMajorCredits;
	TotalUnivCredits = CopiedSP.TotalUnivCredits;
	TotalCredits = CopiedSP.TotalCredits;
	StudentLevel = CopiedSP.StudentLevel;
	major = CopiedSP.major;
	pRules = CopiedSP.pRules;
	for (int i = 0; i < CopiedSP.Sem_Credits.size(); i++)
	{
		Sem_Credits.push_back(CopiedSP.Sem_Credits[i]);
	}
	for (int i = 0; i < CopiedSP.CH_Error_List.size(); i++)
	{
		CH_Error_List.push_back(CopiedSP.CH_Error_List[i]);
	}
	for (int i = 0; i < CopiedSP.CompUniCourses.size(); i++)
	{
		CompUniCourses.push_back(CopiedSP.CompUniCourses[i]);
	}
	for (int i = 0; i < CopiedSP.ElectiveUniCourses.size(); i++)
	{
		ElectiveUniCourses.push_back(CopiedSP.ElectiveUniCourses[i]);
	}
	for (int i = 0; i < CopiedSP.TrackCourses.size(); i++)
	{
		TrackCourses.push_back(CopiedSP.TrackCourses[i]);
	}
	for (int i = 0; i < CopiedSP.CompMajorCourses.size(); i++)
	{
		CompMajorCourses.push_back(CopiedSP.CompMajorCourses[i]);
	}
	for (int i = 0; i < CopiedSP.ElectiveMajorCourses.size(); i++)
	{
		ElectiveMajorCourses.push_back(CopiedSP.ElectiveMajorCourses[i]);
	}
	for (int i = 0; i < CopiedSP.CompConcentrationCourses.size(); i++)
	{
		CompConcentrationCourses.push_back(CopiedSP.CompConcentrationCourses[i]);
	}
	for (int i = 0; i < CopiedSP.ElectiveConcentrationCourses.size(); i++)
	{
		ElectiveConcentrationCourses.push_back(CopiedSP.ElectiveConcentrationCourses[i]);
	}
	int counter =0;

	for (AcademicYear* yr : CopiedSP.plan)
	{
		/*plan.push_back(new AcademicYear);*/
		list<Course*>* pYr = yr->getListOfYears();// pointer to the year
		for (int sem = FALL; sem < SEM_CNT; sem++)
		{
			counter = 0;
			for (auto it = pYr[sem].begin(); it != pYr[sem].end(); it++)
			{
				pCrs = new Course;
				pCrs = (*it);
				Course* pcrs = new Course;
				*pcrs = *pCrs;
				pcrs->setGfxInfo((*it)->getGfxInfo());
				counter++;
				AddCourse(pcrs, pcrs->getYear(), pcrs->getSemester());
				/*delete pCrs;*/

			}

		}
	}
	return (*this);
}
StudyPlan::~StudyPlan()
{
	for (AcademicYear* yr : plan) {
		list<Course*>* pYr = yr->getListOfYears(); // pointer to the year
		for (int sem = FALL; sem < SEM_CNT; sem++) {
			for (auto it = pYr[sem].begin(); it != pYr[sem].end(); it++) {
				delete (*it);
				(*it) = nullptr;
			}
		}
	}
}