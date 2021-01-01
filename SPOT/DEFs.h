#pragma once

//This header file contains some defenitions to be used all over the application
#include <string>
using namespace std;
typedef string Course_Code;


//Semesters
enum SEMESTER
{
	FALL,
	SPRING,
	SUMMER,
	SEM_CNT	//number of semesters 
};


//All possible actions
enum ActionType
{
	ADD_CRS,	//Add a course to study plan
	DEL_CRS,	//Delete a course from study plan
	ADD_Note,   // Add notes
	IMPORT_PLAN, //Import plan
	ERASE,		 //Remove all courses
	SWAP,
	COURSE_INFO,
	CHANGE_CODE,	//changes course code
	DECLARE_MAJOR,
	CRS_DEP,
	PLAN_DEP,
	CAL_GPA,

	SAVE,		//Save a study plan to file
	LOAD,		//Load a study plan from a file

	UNDO,		//Undo the last Action preformed
	REDO,		//Redo the last Action canceled

	EXIT,		//Exit the application

	STATUS_BAR,	//A click on the status bar
	MENU_BAR,	//A click on an empty place in the menu bar
	DRAW_AREA,	//A click in the drawing area

	CANCEL,		//ESC key is pressed

	//TODO: Add more action types

};

//to sotre data related to the last action
struct ActionData
{
	ActionType actType;
	int x, y;
	
};

//Errors
enum ErrorType {
	CRITICAL,
	MODERATE
};
struct Error
{
	ErrorType type;
	string Msg;
};

struct NumOfErrors {
	int Critical_errors;
	int Moderate_errors;
};
