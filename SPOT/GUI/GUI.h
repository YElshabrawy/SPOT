#pragma once
#include <string>
using namespace std;
#include<iostream>
#include "..\DEFs.h"
#include "CMUgraphicsLib\CMUgraphics.h"

class Course;
class AcademicYear;
//user interface class
class GUI
{

	enum MENU_ITEM //The items of the menu (you should add more items)
	{
		//Note: Items are ordered here as they appear in the menu
		//If you want to change the menu items order, just change the order here
		ITM_ADD,		//Add a new course
		ITM_DELETE,		//Delete a course
		//ITM_UNDO,		//Undo last action
		//ITM_REDO,		//Redo last action
		ITM_SAVE,		//Save the current splan
		ITM_IMPORT,		//Import a pre-made plan or a default study plan
		ITM_SWAP,
		ITM_EXCHANGE,
		ITM_Note,		//Add notes
		ITM_ERASE,		//Clear all the data (courses)
		ITM_MAJOR,
		ITM_EXIT,		//Exit item

		ITM_CNT			//no. of menu items ==> This should be the last line in this enum

	};

	color DrawColor = BLACK;		//Drawing color
	color FillColor = MYCYAN;		//Filling color (for courses)
	color HiColor = RED;			//Highlighting color
	color ConnColor = GREEN;		//Connector color
	color MsgColor = BLACK;			//Messages color
	color BkGrndColor = LIGHTGRAY;	//Background color
	color StatusBarColor = WHITE;//statusbar color
	color CourseCodeColor = WHITE;
	string WindTitle = "Study-Plan Organizational Tool (SPOT)";

	window* pWind;
public:
	//Some constants for GUI (My default = 1600 x 880 with aspect ratio 20:11)
	static const int	WindWidth = 1600, WindHeight = WindWidth * (11.0 / 20.0),	//Window width and height
		wx = 15, wy = 15,		//Window starting coordinates
		StatusBarHeight = 60,	//Status Bar Height
		MenuBarHeight = 51,		//Menu Bar Height (distance from top of window to bottom line of menu bar)
		MenuItemWidth = 51,		//Width of each item in the menu
		MenuItemWidthGap = 20,  //Gap width between items
		NumOfYrs = 5, // total years (to be implemented dynamically)
		Y_div = (WindHeight - StatusBarHeight - MenuBarHeight), // 1 div from 5 equal divisions in y
		MyFactor = 4, // used in some refinings
		//Title Bar
		TitleBarWidth = 60,
		TitleBarY2 = Y_div + MenuBarHeight - 4,
		//Years Rectangle
		Year_X1 = TitleBarWidth,
		Year_X2 = WindWidth * 0.8125,
		//Separators
		VerticalSeparatorX = 20,
		//Semesters
		One_Year_Div = (Y_div - (NumOfYrs * MyFactor)) / NumOfYrs,
		One_Semester_Div = One_Year_Div / 3,
		YearImgMidSubtractor = (One_Year_Div - 50) / 2.0,
		SemesterMidFactor = 20,
		//Side Bar
		SideBarX1 = Year_X2 + 10,
		SideBarX2 = WindWidth - 20,
		/*// Add Notes
		NotesY1 = MenuBarHeight + MyFactor,
		NotesHeight = 250,
		// Course Info
		CourseInfoY1 = NotesY1 + NotesHeight + MyFactor,
		CourseInfoHeight = 250,
		myNotesFactor = WindWidth * (1.0 / 16.0),
		courseInfoFactor = WindWidth * (13.0/320.0),*/
		// Add Notes
		NotesY1 = MenuBarHeight + MyFactor,
		NotesHeight = 250,
		NotesY2 = NotesHeight - NotesHeight * 0.65,
		NotesX1 = WindWidth - (WindWidth - Year_X2 - 10),
		string_Max_Width = ((WindWidth - NotesX1) / 10) - 1,
		myNotesFactor = (SideBarX2 - SideBarX1 - 40) / 2.0,
		courseInfoFactor = (SideBarX2 - SideBarX1 - 95) / 2.0,
		InfoX1 = WindWidth - (WindWidth - Year_X2 - 10),
		InfoY1 = NotesY2 + WindHeight * 0.36;
	// Course Info
	int CourseInfoY1 = NotesY1 + NotesHeight + MyFactor,
		CourseInfoHeight = 250;
	static string Notes, CourseTitle, CourseCode, CourseCredit;



	GUI();
	void CreateMenu() const;
	void ClearDrawingArea() const;
	void ClearStatusBar() const;	//Clears the status bar

	//output functions
	void PrintMsg(string msg) const;		//prints a message on status bar
	void PrintNotes() const;
	void PrintCourseInfo()const;
	void DrawNoteArea() const;
	void DrawInfoArea() const;

	//Drawing functions
	void DrawCourse(const Course* );
	void DrawAcademicYear(const AcademicYear*);
	void UpdateInterface() const;
	
	//input functions
	ActionData GUI::GetUserAction(string msg = "") const;
	string GetSrting() const;
	static int getYDivStartingPos();

	//Dimention getters
	static int getMenuBarHeight();
	static int getY_div();

	
	

	~GUI();
};

