#include "GUI.h"
#include "../Courses/Course.h"
#include "../StudyPlan/AcademicYear.h"
#include <sstream>
#include <iostream>
#include"../Registrar.h"
using namespace std;
string GUI::Notes = "";

GUI::GUI()
{ 
	pWind = new window(WindWidth, WindHeight,wx,wy);
	pWind->ChangeTitle(WindTitle);
	ClearDrawingArea();
	ClearStatusBar();
	CreateMenu();
}


//Clears the status bar
void GUI::ClearDrawingArea() const
{
	pWind->SetBrush(BkGrndColor);
	pWind->SetPen(BkGrndColor);
	pWind->DrawRectangle(0, MenuBarHeight, WindWidth, WindHeight -StatusBarHeight);

}

void GUI::ClearStatusBar() const
{
	pWind->SetBrush(StatusBarColor);
	pWind->SetPen(StatusBarColor);
	pWind->DrawRectangle(0, WindHeight - StatusBarHeight, WindWidth, WindHeight);
}

void GUI::CreateMenu() const
{
	pWind->SetBrush(StatusBarColor);
	pWind->SetPen(StatusBarColor);
	pWind->DrawRectangle(0, 0, WindWidth, MenuBarHeight);

	//You can draw the menu icons any way you want.

	//First prepare List of images paths for menu item
	string MenuItemImages[ITM_CNT];
	MenuItemImages[ITM_ADD] = "GUI\\Images\\Menu\\menu_add.jpg";
	MenuItemImages[ITM_DELETE] = "GUI\\Images\\Menu\\menu_delete.jpg";
	MenuItemImages[ITM_UNDO] = "GUI\\Images\\Menu\\menu_undo.jpg";
	MenuItemImages[ITM_REDO] = "GUI\\Images\\Menu\\menu_redo.jpg";
	MenuItemImages[ITM_SAVE] = "GUI\\Images\\Menu\\menu_save.jpg";
	MenuItemImages[ITM_IMPORT] = "GUI\\Images\\Menu\\menu_import.jpg";
	MenuItemImages[ITM_Note] = "GUI\\Images\\Menu\\menu_notes.jpg";
	MenuItemImages[ITM_EXIT] = "GUI\\Images\\Menu\\menu_quit.jpg";

	//TODO: Prepare image for each menu item and add it to the list

	//Draw menu items one image at a time
	for (int i = 0; i < ITM_CNT; i++) {
		pWind->DrawImage(MenuItemImages[i], i*MenuItemWidth +(MenuItemWidthGap*i), 0, MenuItemWidth, MenuBarHeight);
	}
}

////////////////////////    Output functions    ///////////////////

//Prints a message on the status bar
void GUI::PrintMsg(string msg) const
{
	ClearStatusBar();	//Clear Status bar to print message on it
						// Set the Message offset from the Status Bar
	int MsgX = 25;
	int MsgY = StatusBarHeight - 10;

	// Print the Message
	pWind->SetFont(20, BOLD , BY_NAME, "Arial");
	pWind->SetPen(MsgColor);
	pWind->DrawString(MsgX, WindHeight - MsgY, msg);
}

//////////////////////////////////////////////////////////////////////////
void GUI::UpdateInterface() const
{
	
	pWind->SetBuffering(true);
	//Redraw everything
	CreateMenu();
	ClearStatusBar();
	ClearDrawingArea();
	PrintNotes();
	DrawNoteArea(); 
	DrawInfoArea();
	pWind->UpdateBuffer();
	pWind->SetBuffering(false);

}

////////////////////////    Drawing functions    ///////////////////
void GUI::DrawCourse(const Course* pCrs)
{
	if (pCrs->isSelected())
		pWind->SetPen(HiColor, 1);
	else
	pWind->SetPen(DrawColor, 1);
	pWind->SetBrush(FillColor);
	graphicsInfo gInfo = pCrs->getGfxInfo();
	pWind->DrawRectangle(gInfo.x, gInfo.y, gInfo.x + CRS_WIDTH, gInfo.y + CRS_HEIGHT);
	pWind->DrawLine(gInfo.x, gInfo.y + CRS_HEIGHT / 2, gInfo.x + CRS_WIDTH, gInfo.y + CRS_HEIGHT / 2);
	
	//Write the course code and credit hours.
	int Code_x = gInfo.x + CRS_WIDTH * 0.15;
	int Code_y = gInfo.y + CRS_HEIGHT * 0.05;
	pWind->SetFont(CRS_HEIGHT * 0.4, BOLD , BY_NAME, "Gramound");
	pWind->SetPen(CourseCodeColor);

	ostringstream crd;
	crd<< "crd:" << pCrs->getCredits();
	pWind->DrawString(Code_x, Code_y, pCrs->getCode());
	pWind->DrawString(Code_x, Code_y + CRS_HEIGHT/2, crd.str());
}

void GUI::DrawAcademicYear(const AcademicYear* pY) 
{
	graphicsInfo gInfo = pY->getGfxInfo();

	//Draw All years Rectangles
	pWind->SetBrush(WHITE);
	pWind->SetPen(BLACK);

	pWind->DrawRectangle(Year_X1, // x1
		pY->StartingY + (pY->i * pY->YearHeight) + MyFactor, //y1
		Year_X2, // x2
		(pY->StartingY + (pY->i * pY->YearHeight)) + pY->YearHeight, //y2
		FRAME);

	//Titles bar
	pWind->DrawRectangle(0, MenuBarHeight + MyFactor, TitleBarWidth, TitleBarY2);

	// Separators
	pWind->SetPen(BLACK, 9);
	for (int i = 1; i <= NumOfYrs-1; i++)
		pWind->DrawLine(0, //x1
			MenuBarHeight + (i * (Y_div/ NumOfYrs)), //y1
			Year_X2 - MyFactor, //x2
			MenuBarHeight + (i * (Y_div / NumOfYrs))); //y2
	pWind->SetPen(BLACK);

	// Vertical Separator
	pWind->DrawLine(VerticalSeparatorX, MenuBarHeight + MyFactor, VerticalSeparatorX, 
		MenuBarHeight + Y_div - MyFactor);

	//Semester Separations
	for (int i = 1; i <= NumOfYrs ; i++) {
		pWind->DrawLine(VerticalSeparatorX,
			MenuBarHeight + ((i - 1) * (One_Year_Div+MyFactor)) + (1 * One_Semester_Div),
			Year_X2,
			MenuBarHeight + ((i - 1) * (One_Year_Div + MyFactor)) + (1 * One_Semester_Div));
		pWind->DrawLine(VerticalSeparatorX,
			MenuBarHeight + ((i - 1) * (One_Year_Div + MyFactor)) + (2 * One_Semester_Div) + MyFactor,
			Year_X2,
			MenuBarHeight + ((i - 1) * (One_Year_Div + MyFactor)) + (2 * One_Semester_Div) + MyFactor
		);
	}

	//Draw years (Until Finding a Way to be implemented dynamically)
	pWind->DrawImage("GUI\\Images\\Years\\Year_One.jpg",
		1, (1 * One_Year_Div) - YearImgMidSubtractor + (0 * MyFactor), 18, 50);
	pWind->DrawImage("GUI\\Images\\Years\\Year_Two.jpg",
		1, (2 * One_Year_Div) - YearImgMidSubtractor + (1 * MyFactor), 18, 50);
	pWind->DrawImage("GUI\\Images\\Years\\Year_Three.jpg",
		1, (3 * One_Year_Div) - YearImgMidSubtractor + (2 * MyFactor), 18, 50);
	pWind->DrawImage("GUI\\Images\\Years\\Year_Four.jpg",
		1, (4 * One_Year_Div) - YearImgMidSubtractor + (3 * MyFactor), 18, 50);
	pWind->DrawImage("GUI\\Images\\Years\\Year_Five.jpg",
		1, (5 * One_Year_Div) - YearImgMidSubtractor + (4 * MyFactor), 18, 50);

	// Draw Semesters 
	pWind->SetFont(15, BOLD, BY_NAME, "Times New Rome");
	pWind->SetPen(BLACK);
	for (int i = 1; i <= NumOfYrs; i++) {
		pWind->DrawString(29, 
			MenuBarHeight + ((i - 1) * (One_Year_Div + MyFactor)) + SemesterMidFactor,
			"Fall");
		pWind->DrawString(20, 
			MenuBarHeight + ((i -1) * (One_Year_Div + MyFactor)) + SemesterMidFactor + (1 * One_Semester_Div),
			"Spring");
		pWind->DrawString(20, 
			MenuBarHeight + ((i - 1) * (One_Year_Div + MyFactor)) + SemesterMidFactor + (2 * One_Semester_Div),
			"Summ");
	}
	
		/*//To be moved

		//Add notes
		pWind->DrawRectangle(SideBarX1, NotesY1, SideBarX2, NotesY1 + NotesHeight);
		pWind->DrawLine(SideBarX1, NotesY1 + 25, SideBarX2, NotesY1 + 25);
		pWind->DrawString(SideBarX1 + myNotesFactor, NotesY1 + 6, "My Notes");
		//Course Info
		pWind->DrawRectangle(SideBarX1, CourseInfoY1, SideBarX2, CourseInfoY1 + CourseInfoHeight);
		pWind->DrawLine(SideBarX1, CourseInfoY1 + 25, SideBarX2, CourseInfoY1 + 25);
		pWind->DrawString(SideBarX1 + courseInfoFactor, CourseInfoY1 + 6, "Course Information");
	
	*/
	
}


////////////////////////    Input functions    ///////////////////
//This function reads the position where the user clicks to determine the desired action
//If action is done by mouse, actData will be the filled by mouse position
ActionData GUI::GetUserAction(string msg) const
{
	keytype ktInput;
	clicktype ctInput;
	char cKeyData;

	
	// Flush out the input queues before beginning
	pWind->FlushMouseQueue();
	pWind->FlushKeyQueue();
	
	PrintMsg(msg);

	while (true)
	{
		int x, y;
		ctInput = pWind->GetMouseClick(x, y);	//Get the coordinates of the user click
		ktInput = pWind->GetKeyPress(cKeyData);

		if (ktInput == ESCAPE)	//if ESC is pressed,return CANCEL action
		{
			return ActionData{ CANCEL };
		}

		
		if (ctInput == LEFT_CLICK)	//mouse left click
		{
			//[1] If user clicks on the Menu bar
			if (y >= 0 && y < MenuBarHeight)
			{
				//Check whick Menu item was clicked
				//==> This assumes that menu items are lined up horizontally <==
				int ClickedItemOrder = (x / (MenuItemWidth + MenuItemWidthGap));
				//Divide x coord of the point clicked by the menu item width (int division)
				//if division result is 0 ==> first item is clicked, if 1 ==> 2nd item and so on

				switch (ClickedItemOrder)
				{
				case ITM_ADD: return ActionData{ ADD_CRS };	break;//Add course
				case ITM_DELETE: return ActionData{ DEL_CRS }; break; //Delete course
				case ITM_SAVE: return ActionData{ SAVE }; break;
				case ITM_Note: return ActionData{ ADD_Note }; break;
				case ITM_EXIT: return ActionData{ EXIT }; break;		//Exit

				default: return ActionData{ MENU_BAR };	//A click on empty place in menu bar
				}
			}

			//[2] User clicks on the drawing area
			if (y >= MenuBarHeight && y < WindHeight - StatusBarHeight)
			{
				return ActionData{ DRAW_AREA,x,y };	//user want clicks inside drawing area
			}

			//[3] User clicks on the status bar
			return ActionData{ STATUS_BAR };
		}
	}//end while

}

string GUI::GetSrting() const
{
	//Reads a complete string from the user until the user presses "ENTER".
	//If the user presses "ESCAPE". This function should return an empty string.
	//"BACKSPACE" is also supported
	//User should see what he is typing at the status bar

	

	string userInput;
	char Key;
	while (1)
	{
		pWind->WaitKeyPress(Key);

		switch (Key)
		{
		case 27: //ESCAPE key is pressed
			PrintMsg("");
			return ""; //returns nothing as user has cancelled the input

		case 13:		//ENTER key is pressed
			return userInput;

		case 8:		//BackSpace is pressed
			if (userInput.size() > 0)
				userInput.resize(userInput.size() - 1);
			break;

		default:
			userInput += Key;
		};

		PrintMsg(userInput);
	}

}

void GUI::PrintNotes() const
{
	int MsgX = NotesX1;
	int MsgY = NotesY2;
	pWind->SetBrush(WHITE);
	pWind->SetPen(BLACK);
	string msg = Notes;
	int Start = 0, End = 37;
	// Print the Notes
	pWind->SetFont(20, BOLD, BY_NAME, "Times New Rome");
	pWind->SetPen(BLACK);
	if ((size(msg)) > string_Max_Width)
		for (int i = 0; i < ((size(Notes) / 37) + 1); i++)
		{
			pWind->DrawString(MsgX, MsgY + 15 * i, msg.substr(Start + 37 * i, End));
			End = size(msg) - End;
			if (End < 0)
			{
				End = size(msg);
			}
		}
	else
		pWind->DrawString(MsgX, MsgY, msg);
}
void GUI::DrawNoteArea()const
{
	pWind->SetFont(15, BOLD, BY_NAME, "Times New Rome");
	pWind->SetBrush(WHITE);
	pWind->SetPen(BLACK);
	pWind->DrawRectangle(SideBarX1, NotesY1, SideBarX2, NotesY1 + NotesHeight, FRAME);
	pWind->SetBrush(WHITE);
	pWind->DrawLine(SideBarX1, NotesY1 + 25, SideBarX2, NotesY1 + 25);
	pWind->DrawRectangle(SideBarX1, NotesY1, SideBarX2, NotesY1 + 25);
	pWind->DrawString(SideBarX1 + 110, NotesY1 + 6, "My Notes");
}
void GUI::DrawInfoArea()const
{
	pWind->SetFont(15, BOLD, BY_NAME, "Times New Rome");
	pWind->SetBrush(WHITE);
	pWind->SetPen(BLACK);
	pWind->DrawRectangle(SideBarX1, CourseInfoY1, SideBarX2, CourseInfoY1 + CourseInfoHeight, FRAME);
	pWind->DrawRectangle(SideBarX1, CourseInfoY1, SideBarX2, CourseInfoY1 + 25);
	pWind->DrawLine(SideBarX1, CourseInfoY1 + 25, SideBarX2, CourseInfoY1 + 25);
	pWind->DrawString(SideBarX1 + 85, CourseInfoY1 + 6, "Course Information");
}

//Dimention getters
int GUI::getMenuBarHeight() {
	return MenuBarHeight;
}
int GUI::getY_div() {
	return Y_div;
}


int GUI::getYDivStartingPos() {
	return (WindHeight - StatusBarHeight);
}



GUI::~GUI()
{
	delete pWind;
}
