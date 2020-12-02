#pragma once
#include <string>
#include <list>
using namespace std;
#include "..\DEFs.h"

#include "../GUI/Drawable.h"
#include<vector>

//Base class for all types of courses
class Course : public Drawable
{
private:
	Course_Code code;	//course code: e.g. "CIE202". This is the course ID
	string Title;		//course title: e.g. "Fundamentals of Computer Programming"
	int credits;	//no. of course credits
	string type;	//Univ, track, or major.....etc.
	bool Done;		//Done or not yet?
	vector<Course_Code> PreReq;	//list of prerequisites
	vector<Course_Code> CoReq;	//list of prerequisites
	//Curret year and semester
	int year;
	SEMESTER sem;
public:
	static vector<int> numOfCoursesPerSem;
	//Constructors
	Course();
	Course(Course_Code r_code,string r_title, int crd);
	Course(Course_Code r_code, string r_title, int crd,
		vector<Course_Code> r_PreReq, vector<Course_Code> r_CoReq,int r_year, SEMESTER r_sem);
	// Setters
	void setCode(Course_Code i_code);
	void setTitle(string i_title);
	void setCredits(int i_cr);
	void setPreReq(vector<string> vector);
	void setCoReq(vector<string> vector);
	void setYear(int inputYear);
	void setSemester(SEMESTER inputSem);

	//Getters
	string getTitle() const;
	string getCode() const;
	int getCredits() const;
	int getYear() const;
	SEMESTER getSemester() const;

	void DrawMe(GUI*) const;
	virtual ~Course();

	void printCourse() const;
	

};
