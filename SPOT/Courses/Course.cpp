#include "Course.h"
#include "../GUI/GUI.h"
#include <iostream>
#include<iterator>
vector<int> Course::numOfCoursesPerSem((GUI::NumOfYrs *3));
// Constructors
/*Course::Course(Course_Code r_code, string r_title, int crd):code(r_code),Title(r_title)
{
	credits = crd;
	numberOfErrors.Critical_errors = 0;
	numberOfErrors.Moderate_errors = 0;
}*/

Course::Course(Course_Code r_code, string r_title, int crd,
	vector<Course_Code> r_PreReq, vector<Course_Code> r_CoReq,
	int r_year, SEMESTER r_sem) {
	MyColor = MYCYAN;
	MyBorderColor = BLACK;
	UnknownCRS = 0;
	code = r_code;
	Title = r_title;
	credits = crd;
	PreReq = r_PreReq;
	CoReq = r_CoReq;
	year = r_year;
	sem = r_sem;
	//Number of years per semester
	numOfCoursesPerSem[(3 * (year - 1)) + sem]++;
	
}

Course::Course()
{
	code = "";
	Title = "";
	Done = false;
}

Course::~Course()
{
}

//Setters
void Course::setCoursedone(bool Case)
{
	Done = Case;
}
void Course::setCoursepending(bool Case)
{
	pending = Case;
}
void Course::setCourseinprogress(bool Case)
{
	inprogress = Case;
}
void Course::setCode(Course_Code i_code) {
	code = i_code;
}
void Course::setTitle(string i_title) {
	Title = i_title;
}
void Course::setCredits(int i_cr) {
	credits = i_cr;
}
void Course::setPreReq(vector<string> vector) {
	copy(vector.begin(), vector.end(), back_inserter(PreReq)); // copy a vector to a list
}
void Course::setCoReq(vector<string> vector) {
	copy(vector.begin(), vector.end(), back_inserter(CoReq)); // copy a vector to a list
}
void Course::setYear(int inputYear) {
	year = inputYear;
}
void Course::setSemester(SEMESTER inputSem) {
	sem = inputSem;
}
void Course::changeColor(color newColor)
{
	MyColor = newColor;
}
void Course::changeBorderColor(color newColor)
{
	MyBorderColor = newColor;
}
void Course::setUnknownCrs(bool unknown)
{
	UnknownCRS = unknown;
}
void Course::AddPreError(ErrorType Type, string msg)
{
	bool flag = true;
	if (!Prereq_Error_List.empty()) {
		// The list is not empty we first make sure the error message is not repeated
		for (int i = 0; i < Prereq_Error_List.size(); i++) {
			if (msg == Prereq_Error_List[i].Msg) {
				flag = false;
				break;
			}
		}
	}
	if (flag) {
		Error newErr;
		newErr.type = Type;
		newErr.Msg = msg;
		Prereq_Error_List.push_back(newErr);
	}
}
void Course::AddCoError(ErrorType Type, string msg)
{
	bool flag = true;
	if (!Coreq_Error_List.empty()) {
		// The list is not empty we first make sure the error message is not repeated
		for (int i = 0; i < Coreq_Error_List.size(); i++) {
			if (msg == Coreq_Error_List[i].Msg) {
				flag = false;
				break;
			}
		}
	}
	if (flag) {
		Error newErr;
		newErr.type = Type;
		newErr.Msg = msg;
		Coreq_Error_List.push_back(newErr);
	}
}
void Course::removePreReqErrors(string code)
{
	for (int i = 0; i < Prereq_Error_List.size(); i++) {
		if (Prereq_Error_List[i].Msg.find(code) != string::npos) {
			// found
			Prereq_Error_List.erase(Prereq_Error_List.begin()+i);
		}
	}
}

void Course::removeCoReqErrors(string code)
{
	for (int i = 0; i < Coreq_Error_List.size(); i++) {
		if (Coreq_Error_List[i].Msg.find(code) != string::npos) {
			// found
			Coreq_Error_List.erase(Coreq_Error_List.begin() + i);
		}
	}
}

//Getters

bool Course::getCoursedone()const
{
	return Done;
}
bool Course::getCoursepending()const
{
	return pending;
}
bool Course::getCourseinprogress()const
{
	return inprogress;
}
Course_Code Course::getCode() const
{
	return code;
}

string Course::getTitle() const
{
	return Title;
}

int Course::getCredits() const
{
	return credits;
}

vector<string> Course::getPreReq() const
{
	return PreReq;
}

vector<string> Course::getCoReq()
{
	return CoReq;
}

int Course::getYear() const {
	return year;
}

SEMESTER Course::getSemester() const {
	return sem;
}

color Course::getColor() const
{
	return MyColor;
}

color Course::getBorderColor() const
{
	return MyBorderColor;
}

bool Course::isUnknown() const
{
	return UnknownCRS;
}

int Course::getNumOfCrsPerSem(int year, SEMESTER sem)
{
	return numOfCoursesPerSem[(3 * (year - 1)) + sem];
}

int Course::getPreErrorsNumber() const
{
	return Prereq_Error_List.size();
}

int Course::getCoErrorsNumber() const
{
	return Coreq_Error_List.size();
}

void Course::DrawMe(GUI* pG) const
{
	pG->DrawCourse(this);
}

void Course::printCourse() const {
	cout << "Displaying the course info.\n"
		<< "Course ID: " << code << endl
		<< "Course Title: " << Title << endl
		<< "CH: " << credits << endl
		<< "Year: " << year << endl
		<< "Sem: " << sem <<endl
		<< "Prerequisites: ";
	for (Course_Code i : PreReq) {
		cout << i << " ";
	}
	cout << endl << "Corequesites: ";
	for (Course_Code i : CoReq) {
		cout << i << " ";
	}
	cout << endl << endl;
}

Course* Course::getCoursePtr() {
	return this;
}