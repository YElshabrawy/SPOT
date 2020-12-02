#pragma once
#include <string>
using namespace std;

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
		ITM_DELETE,
		ITM_UNDO,
		ITM_REDO,
		ITM_SAVE,
		ITM_IMPORT,
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
	//Some constants for GUI
	static const int	WindWidth = 1600, WindHeight = 880,	//Window width and height
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
		Year_X2 = 1300,
		YearImgMidSubtractor = 45,
		//Separators
		VerticalSeparatorX = 20,
		//Semesters
		One_Year_Div = (Y_div - (NumOfYrs * MyFactor)) / NumOfYrs,
		One_Semester_Div = One_Year_Div / 3,
		SemesterMidFactor = 20,
		//Side Bar
		SideBarX1 = Year_X2 + 10,
		SideBarX2 = WindWidth - 20,
		// Add Notes
		NotesY1 = MenuBarHeight + MyFactor,
		NotesHeight = 250,
		// Course Info
		CourseInfoY1 = NotesY1 + NotesHeight + MyFactor,
		CourseInfoHeight = 250;


	GUI();
	void CreateMenu() const;
	void ClearDrawingArea() const;
	void ClearStatusBar() const;	//Clears the status bar

	//output functions
	void PrintMsg(string msg) const;		//prints a message on status bar

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

