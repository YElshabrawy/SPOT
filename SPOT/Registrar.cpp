#include"Registrar.h"
#include<iostream>
#include<iterator>
#include"Actions/allActions.h"
#include"Utils/Utils.h"
#include<vector>
#include"DEFs.h"
#include<algorithm>

StudyPlan* Registrar::pSPlan = new StudyPlan;
int Registrar::SPSC = 2;
Registrar::Registrar()
{
	pGUI = new GUI;	//create interface object
	pSPlan = new StudyPlan;	//create a study plan.
	/*for (int i = 0; i < 10; i++)
	{*/
		List_Of_All_StudyPlans.push_back(pSPlan);
	/*}*/
	Current_Study_Plan = 0;
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
		Add_Flag = true;
		break;
	case CAL_GPA:	//add_course action
		RequiredAction = new ActionCalculateGPA(this);
		break;
	case DEL_CRS:
		RequiredAction = new ActionDeleteCourse(this);
		Delete_Flag = true;
		break;
	case SAVE: // save plan
		RequiredAction = new ActionSavePlan(this);
		break;
	case ADD_Note:
	{
		RequiredAction = new ActionAddNotes(this);
		Note_Flag = true;
		break;
	}
	case IMPORT_PLAN:
	{
		RequiredAction = new ActionImportPlan(this);
		Import_Flag = true;
		break;
	}
	case ERASE:
		RequiredAction = new ActionEraseAll(this);
		Erase_Flag = true;
		break;
	case SWAP:
		RequiredAction = new ActionMoveCourse(this);
		break;
	case CHANGE_CODE:
		RequiredAction = new ActionChangeCode(this);
		ChangeCode_Flag = true;
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
	case UNDO:
		RequiredAction = new ActionUndo(this);
		break;
	case REDO:
		RequiredAction = new ActionRedo(this);
		break;
	default:
	{
		if (pGUI->Last_CLick == RIGHT_CLICK)
		{
			RequiredAction = new ActionDragAndDrop(this);
			Drag_Flag = true;
			break;
		}
		else if (((pGUI->YCoord >= 10) && (pGUI->YCoord <= 30) && ((pGUI->XCoord)>=(pGUI->SideBarX1-45 + (pGUI->SideBarX2 - pGUI->SideBarX1)/2)) && (pGUI->XCoord <= (pGUI->SideBarX1-45+100+(pGUI->SideBarX2 - pGUI->SideBarX1)/2 )))&&(pGUI->Last_CLick == LEFT_CLICK))
		{
			RequiredAction = new ActionAddNotes(this);
			Note_Flag = true;
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
	if ((Delete_Flag == true)|| (Import_Flag == true) || (Add_Flag == true) || (Erase_Flag == true) || (Drag_Flag == true) || (Note_Flag == true) || (ChangeCode_Flag == true))
	{
		Add_To_StudyPlan(*pSPlan);
		Import_Flag = false;
		Delete_Flag = false;
		Add_Flag = false;
		Erase_Flag = false;
		Drag_Flag = false;
		Note_Flag = false;
		ChangeCode_Flag = false;
	}
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

	while (true)
	{

		//update interface here as CMU Lib doesn't refresh itself
		//when window is minimized then restored..
		setRules();
		pSPlan->Set_Course_Type();
		pSPlan->GenerateStudentLevel(pGUI);
		pSPlan->checkPreAndCoReq();
		pSPlan->checkCreditHrs(RegRules.SemMinCredit, RegRules.SemMaxCredit);
		importProgramReq();
		pSPlan->LiveReport(pGUI, RegRules.SemMinCredit, RegRules.SemMaxCredit);
		pGUI->Total_Number_Pages_In_Report=(pSPlan->Get_Page_Number());
		pGUI->DrawLiveReportPages((pGUI->ReportAreaHeight/15)-2, pGUI->Current_Page_Report);
		pGUI->NotesLines.clear();
		UpdateInterface();
		Action* pAct = CreateRequiredAction();
		if (pAct)	//if user doesn't cancel
		{
	
			if (ExecuteAction(pAct))
			{//if action is not cancelled
				UpdateInterface();
			}
		}
	}
}


void Registrar::UpdateInterface()
{
	/*delete pSPlan;*/
    pSPlan = List_Of_All_StudyPlans[Current_Study_Plan];
	pGUI->Notes = pSPlan->PlanNotes;
	pGUI->NotesLines.clear();
	pGUI->SegmentNotes();
	//pSPlan->checkPreAndCoReq();
	pGUI->Total_Number_Study_Plans = List_Of_All_StudyPlans.size();
	pGUI->Current_StudyPlan = Current_Study_Plan;
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
	//pSPlan->Set_Page_Number((pGUI->ReportAreaHeight / 15) - 2);
	pGUI->ReportLines.clear();
	pSPlan->DrawMe(pGUI);
	pSPlan->LiveReport(pGUI, RegRules.SemMinCredit, RegRules.SemMaxCredit);//make study plan draw itself
	pGUI->Total_Number_Pages_In_Report = (pSPlan->Get_Page_Number());
	pGUI->DrawLiveReportPages((pGUI->ReportAreaHeight / 15) - 2, pGUI->Current_Page_Report);
	pGUI->DrawNotesPages((pGUI->NotesHeight / 15) - 2, pGUI->Current_Page_Notes);
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


	finput.close();



}
void Registrar::Increment_Current_StudyPlan()
{
	if(Current_Study_Plan<(List_Of_All_StudyPlans.size()-1))
	Current_Study_Plan++;
}
void Registrar::Decrement_Current_StudyPlan()
{
	if (Current_Study_Plan > 0)
	Current_Study_Plan--;
}
void Registrar::Add_To_StudyPlan(StudyPlan &pS_New)
{
	if (Current_Study_Plan <= List_Of_All_StudyPlans.size() - 1)
	{
		StudyPlan*pS_Old = new StudyPlan;
		*pS_Old = pS_New;
		if (SPSC == 2)
		{
			List_Of_All_StudyPlans.push_back(pS_Old);
			--SPSC;
		}
		else
		{
			if (Current_Study_Plan < List_Of_All_StudyPlans.size() - 1)
				List_Of_All_StudyPlans.erase(List_Of_All_StudyPlans.begin() + Current_Study_Plan + 1, List_Of_All_StudyPlans.end() - Current_Study_Plan);
			List_Of_All_StudyPlans.insert(List_Of_All_StudyPlans.end() - SPSC, pS_Old);
		}
		Current_Study_Plan++;
		pSPlan = List_Of_All_StudyPlans[Current_Study_Plan];
	}
}
int  Registrar::GetCurrent_Study_Plan() const
{
	return Current_Study_Plan;
}