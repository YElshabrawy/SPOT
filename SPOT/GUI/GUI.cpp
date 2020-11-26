#include "GUI.h"
#include "../Courses/Course.h"
#include "../StudyPlan/AcademicYear.h"
#include <sstream>

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
	MenuItemImages[ITM_ADD] = "GUI\\Images\\Menu\\menu_add2.jpg";
	MenuItemImages[ITM_DELETE] = "GUI\\Images\\Menu\\menu_delete2.jpg";
	MenuItemImages[ITM_UNDO] = "GUI\\Images\\Menu\\menu_undo2.jpg";
	MenuItemImages[ITM_REDO] = "GUI\\Images\\Menu\\menu_redo2.jpg";
	MenuItemImages[ITM_IMPORT] = "GUI\\Images\\Menu\\menu_import2.jpg";
	MenuItemImages[ITM_SAVE] = "GUI\\Images\\Menu\\menu_save2.jpg";
	MenuItemImages[ITM_EXIT] = "GUI\\Images\\Menu\\menu_quit2.jpg";

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
	pWind->UpdateBuffer();
	pWind->SetBuffering(false);

}

////////////////////////    Drawing functions    ///////////////////
void GUI::DrawCourse(const Course* pCrs)
{
	if (pCrs->isSelected())
		pWind->SetPen(HiColor, 2);
	else
	pWind->SetPen(DrawColor, 2);
	pWind->SetBrush(FillColor);
	graphicsInfo gInfo = pCrs->getGfxInfo();
	pWind->DrawRectangle(gInfo.x, gInfo.y, gInfo.x + CRS_WIDTH, gInfo.y + CRS_HEIGHT);
	pWind->DrawLine(gInfo.x, gInfo.y + CRS_HEIGHT / 2, gInfo.x + CRS_WIDTH, gInfo.y + CRS_HEIGHT / 2);
	
	//Write the course code and credit hours.
	int Code_x = gInfo.x + CRS_WIDTH * 0.15;
	int Code_y = gInfo.y + CRS_HEIGHT * 0.05;
	pWind->SetFont(CRS_HEIGHT * 0.4, BOLD , BY_NAME, "Gramound");
	pWind->SetPen(MsgColor);

	ostringstream crd;
	crd<< "crd:" << pCrs->getCredits();
	pWind->DrawString(Code_x, Code_y, pCrs->getCode());
	pWind->DrawString(Code_x, Code_y + CRS_HEIGHT/2, crd.str());
}

void GUI::DrawAcademicYear(const AcademicYear* pY) 
{
	graphicsInfo gInfo = pY->getGfxInfo();

	pWind->SetBrush(WHITE);
	pWind->SetPen(BLACK);

	pWind->DrawRectangle(pY->Year_X1, pY->Year_Y, pY->Year_X2, pY->Year_Height, FRAME);
	pWind->DrawRectangle(0, MenuBarHeight + 4, 60, 690);
	for (int i = 0; i < 4; i++)
		pWind->DrawLine(0, i * 120 + 210, 1000, i * 120 + 210);
	pWind->DrawLine(20, MenuBarHeight, 20, 690);
	int Y1 = 600, Y2 = 480, Y3 = 360, Y4 = 240, Y5 = 115;
	pWind->DrawLine(60, MenuBarHeight, 60, 690);// draw line to separte semesters from the courses slot
	pWind->DrawLine(20, Y1 + 10, 1000, Y1 + 10);
	pWind->DrawLine(20, Y1 + 50, 1000, Y1 + 50);
	pWind->DrawLine(20, Y2 + 10, 1000, Y2 + 10);
	pWind->DrawLine(20, Y2 + 50, 1000, Y2 + 50);
	pWind->DrawLine(20, Y3 + 10, 1000, Y3 + 10);
	pWind->DrawLine(20, Y3 + 50, 1000, Y3 + 50);
	pWind->DrawLine(20, Y4 + 10, 1000, Y4 + 10);
	pWind->DrawLine(20, Y4 + 50, 1000, Y4 + 50);
	pWind->DrawLine(20, Y5 + 10, 1000, Y5 + 10);
	pWind->DrawLine(20, Y5 + 50, 1000, Y5 + 50);
	pWind->SetFont(15, BOLD, BY_NAME, "Times New Rome");
	pWind->SetPen(BLACK);
	pWind->DrawImage("GUI\\Images\\Menu\\Year_Five.jpg", 1, 125, 18, 50);
	pWind->DrawImage("GUI\\Images\\Menu\\Year_Four.jpg", 1, 245, 18, 50);
	pWind->DrawImage("GUI\\Images\\Menu\\Year_Three.jpg", 1, 365, 18, 50);
	pWind->DrawImage("GUI\\Images\\Menu\\Year_Two.jpg", 1, 485, 18, 50);
	pWind->DrawImage("GUI\\Images\\Menu\\Year_One.jpg", 1, 605, 18, 50);
	for (int i = 0; i < 5; i++)
	{
		if (i == 0)
		{
			pWind->DrawString(28, 80, "Fall");
			pWind->DrawString(20, 125, "Spring");
			pWind->DrawString(20, 165, "Summ");
		}
		else
		{
			pWind->DrawString(28, i * 120 + 90, "Fall");
			pWind->DrawString(20, i * 120 + 130, "Spring");
			pWind->DrawString(20, i * 120 + 170, "Summ");
		}
		//to be moved
		pWind->DrawRectangle(1000 + 5, MenuBarHeight, 1280, MenuBarHeight + 250);
		pWind->DrawLine(1000 + 5, MenuBarHeight + 25, 1280, MenuBarHeight + 25);
		pWind->DrawString(1000 + 110, MenuBarHeight + 6, "Add Notes");
		pWind->DrawRectangle(1000 + 5, MenuBarHeight + 260, 1280, MenuBarHeight + 510);
		pWind->DrawLine(1000 + 5, MenuBarHeight + 290, 1280, MenuBarHeight + 290);
		pWind->DrawString(1000 + 95, MenuBarHeight + 270, "Course Information");
	}
	///TODO: compelete this function to:
	//		1- Draw a rectangle for the academic year 
	//		2- Draw a sub-rectangle for each semester
	//Then each course should be drawn inside rect of its year/sem
	
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
				int ClickedItemOrder = (x / MenuItemWidth);
				//Divide x coord of the point clicked by the menu item width (int division)
				//if division result is 0 ==> first item is clicked, if 1 ==> 2nd item and so on

				switch (ClickedItemOrder)
				{
				case ITM_ADD: return ActionData{ ADD_CRS };	//Add course
				case ITM_EXIT: return ActionData{ EXIT };		//Exit

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

int GUI::getYDiv() {
	return Y_div;
}


int GUI::getYDivStartingPos() {
	return (WindHeight - StatusBarHeight);
}



GUI::~GUI()
{
	delete pWind;
}
