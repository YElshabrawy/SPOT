#include "Course.h"
#include "../GUI/GUI.h"
#include <iostream>

// Constructors
Course::Course(Course_Code r_code, string r_title, int crd):code(r_code),Title(r_title)
{
	credits = crd;
}

Course::Course(Course_Code r_code, string r_title, int crd,
	vector<Course_Code> r_PreReq, vector<Course_Code> r_CoReq) {
	code = r_code;
	Title = r_title;
	credits = crd;
	PreReq = r_PreReq;
	CoReq = r_CoReq;

}

Course::Course()
{
	code = "";
	Title = "";
}

Course::~Course()
{
}

//Setters
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
//Getters
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

int Course::getYear() const {
	return year;
}

SEMESTER Course::getSemester() const {
	return sem;
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