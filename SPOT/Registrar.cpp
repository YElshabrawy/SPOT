#include"Registrar.h"
#include<iostream>
#include<iterator>
#include"Actions/allActions.h"
#include"Utils/Utils.h"
#include<algorithm>
using namespace std;


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
	case CAL_GPA:	//add_course action
		RequiredAction = new ActionCalculateGPA(this);
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
	case DECLARE_MAJOR:
		RequiredAction = new ActionDeclareMajor(this);//declare major photo till i get a photo for this one
		break;
	case CRS_DEP:
		RequiredAction = new ActionDDOOC(this);
		break;
	case PLAN_DEP:
		RequiredAction = new ActionCourseDependancies(this);
		break;
	default:
	{
		if (pGUI->Last_CLick == RIGHT_CLICK)
		{
			RequiredAction = new ActionDragAndDrop(this);
			break;
		}
		else if (((pGUI->YCoord >= 10) && (pGUI->YCoord <= 30) && ((pGUI->XCoord)>=(pGUI->SideBarX1-45 + (pGUI->SideBarX2 - pGUI->SideBarX1)/2)) && (pGUI->XCoord <= (pGUI->SideBarX1-45+100+(pGUI->SideBarX2 - pGUI->SideBarX1)/2 )))&&(pGUI->Last_CLick == LEFT_CLICK))
		{
			RequiredAction = new ActionAddNotes(this);
			break;
		}
		else
		{
			RequiredAction = new ActionCourseInfo(this);
			break;
		}
	}

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
	setCourseOffering();
	cout << "The year of Offerings: " << RegRules.OffringsList.Year << endl;
	//setRules();
	pSPlan->Set_Course_Type();

	while (true)
	{
		//update interface here as CMU Lib doesn't refresh itself
		//when window is minimized then restored
		pSPlan->checkPreAndCoReq();
		pSPlan->checkCreditHrs(RegRules.SemMinCredit, RegRules.SemMaxCredit);
		pSPlan->checkProgramReq();
		importProgramReq();
		pSPlan->LiveReport(pGUI, RegRules.SemMinCredit, RegRules.SemMaxCredit);
		setRules();
		pSPlan->Set_Course_Type();
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
	if (pGUI->Draw_Dependacies_Flag)
	{
		Action* pAct = new ActionCourseDependancies(this);
		ExecuteAction(pAct);
	}
	else if (pGUI->Draw_Dependacies_For_One_Course)
	{
		Action* pAct = new  ActionDDOOC(this);
		ExecuteAction(pAct);
	}
	pSPlan->DrawMe(pGUI);
	pSPlan->LiveReport(pGUI, RegRules.SemMinCredit, RegRules.SemMaxCredit);//make study plan draw itself
}

Registrar::~Registrar()
{
	delete pGUI;
}

// Updated
void Registrar::createAllCourses() {
	// Create a vector of all courses //
	string directory = "./Format Files/All_Courses.txt";
	ifstream finput(directory);
	char* token; // pointer to char (tokens)
	char* context = nullptr; // used in strtok_s function as a context
	const int size = 300; // More than the longest line
	char line[size];
	int i = 0; // for each line we have one course with index i

	vector<CourseInfo> c;

	while (finput.getline(line, size)) { // gets every line
		vector<string> tokensVector;
		token = strtok_s(line, ",", &context);
		while (token != NULL) {
			tokensVector.push_back(token);
			token = strtok_s(NULL, ",", &context);
		}
		CourseInfo newCourse;
		c.push_back(newCourse);
		c[i].Code = tokensVector[0];
		c[i].Title = tokensVector[1];
		c[i].Credits = stoi(tokensVector[2]); // string to int
		// Handle prereq
		for (string token : tokensVector) {
			bool condition = token[0] == 'P' && token[1] == 'r' && token[2] == 'e';
			if (condition) {
				vector<string> preReq;
				token.erase(0, 8);
				// Parse using string dilimeter " And "
				string delim = " And ";
				auto start = 0U;
				auto end = token.find(delim);
				while (end != string::npos) {
					preReq.push_back(token.substr(start, end - start));
					start = end + delim.length();
					end = token.find(delim, start);
				}
				preReq.push_back(token.substr(start, end));
				c[i].PreReqList = preReq;
			}
		}
		// Handle coreq
		for (string token : tokensVector) {
			bool condition = token[0] == 'C' && token[1] == 'o' && token[2] == 'r';
			if (condition) {
				vector<string> coReq;
				token.erase(0, 7);
				// Parse using string dilimeter " And "
				string delim = " And ";
				auto start = 0U;
				auto end = token.find(delim);
				while (end != string::npos) {
					coReq.push_back(token.substr(start, end - start));
					start = end + delim.length();
					end = token.find(delim, start);
				}
				coReq.push_back(token.substr(start, end));
				c[i].CoReqList = coReq;
			}
		}
		i++;
	}
	finput.close();
	RegRules.CourseCatalog = c;
	cout << "All Courses In (./Format Files/All_Courses.txt) Are Loaded Successfully.\n";
}

CourseInfo* Registrar::inCatalog(string code, bool& exists)
{
	// Returns a pointer to the right corse info.
	int index = 0;
	bool flag = 0;
	for (CourseInfo c : RegRules.CourseCatalog) {
		if (c.Code == code) {
			exists = 1;
			flag = 1;
			break;
		}
		index++;
	}
	if (flag) {
		return &(RegRules.CourseCatalog[index]);
	}
	else {
	exists = 0;
	return nullptr;
	}
}

string Registrar::transformCode(string& code)
{
	//Transform code to UPPER
	transform(code.begin(), code.end(), code.begin(), ::toupper);

	//Check the spaces
	if (!(code.find(" ") != string::npos)) {
		// Only if there is no space
		Course_Code output = "";
		for (int i = 0; i < code.length(); i++) {
			bool condition = code[i] == '0' || code[i] == '1' || code[i] == '2' || code[i] == '3' || code[i] == '4' ||
				code[i] == '5' || code[i] == '6' || code[i] == '7' || code[i] == '8' ||
				code[i] == '9';
			if (condition) {
				if (code[i + 1] != NULL && code[i + 2] != NULL) {
					output += ' ';
					output += code[i];
					output += code[i + 1];
					output += code[i + 2];
					// Assuming that all courses have 3 numbers "CODE XXX"
					break;
				}
			}
			else {
				output += code[i];
			}
		}
		code = output;
	}

	return code;
}

void Registrar::setCourseOffering()
{
	AcademicYearOfferings yearOfferings;
	char* token;
	char* context = nullptr;
	const int size = 300;
	char line[size];
	ifstream OfferingFile("Format Files\\Course_Offering.txt");
	if (!OfferingFile)
	{
		cout << "Couldn't open offering file";
	}
	else {
		cout << "Importing course offerings" << endl;

		while (OfferingFile.getline(line, size)) {
			vector<string> tokensVector;
			token = strtok_s(line, ",", &context);
			while (token != NULL) {
				tokensVector.push_back(token);
				token = strtok_s(NULL, ",", &context);
			}
			yearOfferings.Year = tokensVector[0];
			SEMESTER sem = FALL;
			int sem_num = stoi(tokensVector[1].erase(0, 9));
			switch (sem_num) {
			case 1:
				sem = FALL;
				break;
			case 2:
				sem = SPRING;
				break;
			case 3:
				sem = SUMMER;
				break;
			default:
				sem = FALL;
				break;
			}
			tokensVector.erase(tokensVector.begin(), tokensVector.begin() + 2);
			yearOfferings.Offerings[sem] = tokensVector;
		}
		OfferingFile.close();
		cout << "Done: Importing course offerings" << endl;
	}

	RegRules.OffringsList = yearOfferings;
}


//void Registrar::setRules()
//{
//	RegRules.ReqUnivCredits = pSPlan->MaxCredits;
//	RegRules.ReqMajorCredits = pSPlan->TotalMajorCredits;
//	RegRules.ReqTrackCredits = pSPlan->TotalTrackCredits;
//
//	RegRules.UnivCompulsory = pSPlan->CompUniCourses;
//	RegRules.UnivElective = pSPlan->ElectiveUniCourses;
//
//	RegRules.TrackCompulsory = pSPlan->TrackCourses;
//	RegRules.TrackElective; // was not included in the file (will be fixed later)
//
//	RegRules.MajorCompulsory = pSPlan->CompMajorCourses;
//	RegRules.MajorElective = pSPlan->ElectiveMajorCourses;
//
//	RegRules.ConcentrationCompulsory = pSPlan->CompConcentrationCourses;
//	RegRules.ConcentrationElective = pSPlan->ElectiveConcentrationCourses;
//
//
//
//}

void Registrar::setRules()
{
	pSPlan->Set_Plan_Rules(RegRules);
}


Course* Registrar::interrogateCourse(int x, int y)
{
	Course* output = nullptr;
	StudyPlan* pS = getStudyPlay(); // pointer on study plan
	vector<AcademicYear*>* pPlan = pS->getStudyPlanVector(); // pointer on the plan vector
	bool flag = 0;
	for (AcademicYear* yr : *pPlan) {
		list<Course*>* pYr = yr->getListOfYears(); // pointer to the year
		for (int sem = FALL; sem < SEM_CNT; sem++) {
			for (auto it = pYr[sem].begin(); it != pYr[sem].end(); it++) {
				int course_x = (*it)->getGfxInfo().x;
				int course_y = (*it)->getGfxInfo().y;
				bool condition = x > course_x && x < (course_x + CRS_WIDTH) &&
					y > course_y && y < (course_y + CRS_HEIGHT);
				if (condition) {
					flag = 1;
					output = (*it)->getCoursePtr();
					break;
				}
			}
			if (flag) break;
		}
		if (flag) break;
	}

	if (flag) {
		return output;
	}
	else {
		cout << "No course is sellected!" << endl;
		return nullptr;
	}
}

void Registrar::importProgramReq()
{
	// First according to the major select the file you want to read from
	string majorName;
	switch (pSPlan->getMajor())
	{
	case CIE:
		majorName = "CIE"; break;
	case ENV:
		majorName = "ENV"; break;
	case NANENG:
		majorName = "NANENG"; break;
	case REE:
		majorName = "REE"; break;
	case SPC:
		majorName = "SPC"; break;
	case BMS:
		majorName = "BMS"; break;
	case PEU:
		majorName = "PEU"; break;
	case MATSCI:
		majorName = "MATSCI"; break;
	case NANSCI:
		majorName = "NANSCI"; break;
	default:
		majorName = "CIE"; break;
	}
	string directory = "Format Files\\Requirements\\" + majorName + "-Requirements.txt";
	// Read from this file
	ifstream finput(directory);

	//line 1 (Total Cr)
	string l1;
	getline(finput, l1);
	RegRules.TotalCHs = stoi(l1);
	cout << "Total Credits = " << RegRules.TotalCHs << endl;

	//line 2 (Univ comp and major Cr)
	string l2;
	getline(finput, l2);
	vector<string> l2_tokens = splitString(l2, ",");
	RegRules.UnivCompulsoryCredits = stoi(l2_tokens[0]);
	RegRules.UnivElectiveCredits = stoi(l2_tokens[1]);
	cout << "Univ Compulsory = " << RegRules.UnivCompulsoryCredits << endl;
	cout << "Univ Elective = " << RegRules.UnivElectiveCredits << endl;

	//line 3 (Track comp)
	string l3;
	getline(finput, l3);
	RegRules.ReqTrackCredits = stoi(l3);
	cout << "Track Compulsory = " << RegRules.ReqTrackCredits << endl;

	//line 4 (Major comp and major Cr)
	string l4;
	getline(finput, l4);
	vector<string> l4_tokens = splitString(l4, ",");
	RegRules.MajorCompulsoryCredits = stoi(l4_tokens[0]);
	RegRules.MajorElectiveCredits = stoi(l4_tokens[1]);
	cout << "Major Compulsory = " << RegRules.MajorCompulsoryCredits << endl;
	cout << "Major Elective = " << RegRules.MajorElectiveCredits << endl;

	//line 5 (Num of Concentrations)
	string l5;
	getline(finput, l5);
	RegRules.NumOfConcentrations = stoi(l5);
	int numOfConc = stoi(l5); // Will be needed in line 6 & last lines
	cout << "Number Of Concentrations = " << RegRules.NumOfConcentrations << endl;

	//line 6 (Concentrations CHs "Comp and Elective")
	string l6;
	getline(finput, l6);
	vector<string> l6_tokens = splitString(l6, ","); // will contain num * 2 elements!
	for (int i = 0; i < numOfConc; i++) {
		Concentration c;
		c.ID = i + 1;
		c.CompulsoryCredits = stoi(l6_tokens[2 * i]);
		c.ElectiveCredits = stoi(l6_tokens[(2 * i) + 1]);
		RegRules.Concentrations.push_back(c);
		cout << "For concentration " << c.ID << ") Comp CH = " << c.CompulsoryCredits << " and Elective CH = "
			<< c.ElectiveCredits << endl;
	}

	//line 7 (Univ Comp Crs)
	string l7;
	getline(finput, l7);
	RegRules.UnivCompulsoryCourses = splitString(l7, ",");

	//line 8 (Univ Elective Crs)
	string l8;
	getline(finput, l8);
	RegRules.UnivElectiveCourses = splitString(l8, ",");

	//line 9 (Track Comp Crs only)
	string l9;
	getline(finput, l9);
	RegRules.TrackCompulsoryCourses = splitString(l9, ",");


	//line 10 (Major comp crs)
	string l10;
	getline(finput, l10);
	RegRules.MajorCompulsoryCourses = splitString(l10, ",");


	//line 11 (major elective crs)
	string l11;
	getline(finput, l11);
	RegRules.MajorElectiveCourses = splitString(l11, ",");

	// The rest of lines in case there is a concentration
		// The rest of lines in case there is a concentration
	for (int i = 0; i < numOfConc; i++) {

		// find the concentration
		int index;
		for (int j = 0; j < RegRules.Concentrations.size(); j++) {
			if (RegRules.Concentrations[j].ID == (i + 1)) {
				index = j;
				break;
			}
		}
		
		// Compulsory
		string L;
		getline(finput, L);
		RegRules.Concentrations[index].ConcentrationCompulsoryCourses = splitString(L, ",");

		// Elective
		string M;
		getline(finput, M);
		RegRules.Concentrations[index].ConcentrationElectiveCourses = splitString(M, ",");
		
	}

	finput.close();

}

Rules const* Registrar::getRegRules() const
{
	return 	&RegRules;
}

}

