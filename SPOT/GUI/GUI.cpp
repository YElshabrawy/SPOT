#include "GUI.h"
#include "../Courses/Course.h"
#include "../StudyPlan/AcademicYear.h"
#include <sstream>
#include <iostream>
#include"../Registrar.h"
using namespace std;
int GUI::XCoord = 0;
int GUI::YCoord = 0;
clicktype GUI::Last_CLick = RIGHT_CLICK;
bool GUI::Draw_Dependacies_Flag = false;
bool GUI::Draw_Dependacies_For_One_Course = false;
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
	//MenuItemImages[ITM_UNDO] = "GUI\\Images\\Menu\\menu_undo.jpg";
	//MenuItemImages[ITM_REDO] = "GUI\\Images\\Menu\\menu_redo.jpg";
	MenuItemImages[ITM_SAVE] = "GUI\\Images\\Menu\\menu_save.jpg";
	MenuItemImages[ITM_IMPORT] = "GUI\\Images\\Menu\\menu_import.jpg";
	MenuItemImages[ITM_SWAP] = "GUI\\Images\\Menu\\menu_swap.jpg";
	MenuItemImages[ITM_EXCHANGE] = "GUI\\Images\\Menu\\menu_exchange.jpg";
	MenuItemImages[ITM_Note] = "GUI\\Images\\Menu\\menu_notes.jpg";
	MenuItemImages[ITM_ERASE] = "GUI\\Images\\Menu\\menu_erase.jpg";
	MenuItemImages[ITM_MAJOR] = "GUI\\Images\\Menu\\menu_major.jpg";
	MenuItemImages[ITM_CRS_DEP] = "GUI\\Images\\Menu\\menu_crs_dep.jpg";
	MenuItemImages[ITM_PLAN_DEP] = "GUI\\Images\\Menu\\menu_plan_dep.jpg";
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
	DrawReportArea();
	PrintCourseInfo();
	
	pWind->UpdateBuffer();
	pWind->SetBuffering(false);

}
////////////////////////    Drawing functions    ///////////////////
void GUI::DrawCourse(const Course* pCrs)
{
	if (pCrs->isSelected())
		pWind->SetPen(HiColor, 1);
	else
	pWind->SetPen(pCrs->getBorderColor(), 1);
	pWind->SetBrush(pCrs->getColor());
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
void GUI::DrawCourse(const Course* pCrs, int x, int y)
{
	if (pCrs->isSelected())
		pWind->SetPen(HiColor, 1);
	else
		pWind->SetPen(pCrs->getBorderColor(), 1);
	pWind->SetBrush(pCrs->getColor());
	graphicsInfo gInfo = pCrs->getGfxInfo();
	pWind->DrawRectangle(x, y,x + CRS_WIDTH, y + CRS_HEIGHT);
	pWind->DrawLine(x, y + CRS_HEIGHT / 2, x + CRS_WIDTH, y + CRS_HEIGHT / 2);

	//Write the course code and credit hours.
	int Code_x = x + CRS_WIDTH * 0.15;
	int Code_y = y + CRS_HEIGHT * 0.05;
	pWind->SetFont(CRS_HEIGHT * 0.4, BOLD, BY_NAME, "Gramound");
	pWind->SetPen(CourseCodeColor);

	ostringstream crd;
	crd << "crd:" << pCrs->getCredits();
	pWind->DrawString(Code_x, Code_y, pCrs->getCode());
	pWind->DrawString(Code_x, Code_y + CRS_HEIGHT / 2, crd.str());
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
	if (NumOfYrs > 5) {
		pWind->DrawImage("GUI\\Images\\Years\\Year_Six.jpg",
			1, (6 * One_Year_Div) - YearImgMidSubtractor + (5 * MyFactor), 18, 50);
	}

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
				XCoord = x; YCoord = y;
				Last_CLick = ctInput;
				//Check whick Menu item was clicked
				//==> This assumes that menu items are lined up horizontally <==
				int ClickedItemOrder = (x / (MenuItemWidth + MenuItemWidthGap));
				//Divide x coord of the point clicked by the menu item width (int division)
				//if division result is 0 ==> first item is clicked, if 1 ==> 2nd item and so on
				switch (ClickedItemOrder)
				{
				case ITM_ADD: return ActionData{ ADD_CRS };	break;//Add course
				case ITM_DELETE: return ActionData{ DEL_CRS }; break; //Delete course
				case ITM_SAVE: return ActionData{ SAVE }; break; //Save Plan
				case ITM_Note: return ActionData{ ADD_Note }; break; //Add Notes
				case ITM_IMPORT: return ActionData{ IMPORT_PLAN }; break; //Import Plan
				case ITM_ERASE: return ActionData{ ERASE }; break;
				case ITM_SWAP: return ActionData{ SWAP }; break;
				case ITM_EXCHANGE: return ActionData{ CHANGE_CODE }; break;
				case ITM_MAJOR: return ActionData{ DECLARE_MAJOR }; break;
				case ITM_CRS_DEP:
				{
					Draw_Dependacies_For_One_Course = false;
					return ActionData{ CRS_DEP }; break;
				}
				case ITM_PLAN_DEP:
				{
					if (Draw_Dependacies_Flag == true)
						Draw_Dependacies_Flag = false;
					else
						Draw_Dependacies_Flag = true;
					return ActionData{ PLAN_DEP }; break;
				}
				case ITM_EXIT: return ActionData{ EXIT }; break;		//Exit

				default: return ActionData{ MENU_BAR };	//A click on empty place in menu bar
				}
			}

			//[2] User clicks on the drawing area
			if (y >= MenuBarHeight && y < WindHeight - StatusBarHeight)
			{
				XCoord = x; YCoord = y;
				Last_CLick = ctInput;
				return ActionData{ DRAW_AREA,x,y };	//user want clicks inside drawing area
			}
			//[3] User clicks on the status bar
			return ActionData{ STATUS_BAR };
		}
		if (ctInput == RIGHT_CLICK)	//mouse left click
		{
			if (y >= MenuBarHeight && y < WindHeight - StatusBarHeight)
			{
				XCoord = x; YCoord = y;
				Last_CLick = ctInput;
				return ActionData{ DRAW_AREA,x,y };	//user want clicks inside drawing area
			}
		}
	}//end while

}
string GUI::GetSrting() const
{
	//Reads a complete string from the user until the user presses "ENTER".
	//If the user presses "ESCAPE". This function should return an empty string.
	//"BACKSPACE" is also supported
	//User should see what he is typing at the status bar


	string userInput="";
	vector<char>KeyInput{'|'};
	char Cursor = '|';
	int Cursor_Position = 0;
	char Key;
	while (1)
	{
		pWind->WaitKeyPress(Key);

		switch (Key)
		{
		case 27: //ESCAPE key is pressed
			PrintMsg("");
			return " "; //returns nothing as user has cancelled the input
		case 13://ENTER key is pressed
		{
			KeyInput.erase(KeyInput.begin() + Cursor_Position);
			userInput = "";
			for (int i = 0; i < KeyInput.size(); i++)
			{
				userInput += KeyInput[i];
			}
			return userInput;
		}
		case 8:		//BackSpace is pressed
			if ((KeyInput.size() != 0)&&(Cursor_Position != 0))
			{
				KeyInput.erase(KeyInput.begin() + Cursor_Position-1);
				Cursor_Position--;
			}
			break;
		case 6:// arrow right
		{
			if (Cursor_Position == KeyInput.size() - 1)
				break;
			KeyInput.erase(KeyInput.begin() + Cursor_Position);
			KeyInput.insert(KeyInput.begin()+ Cursor_Position+1, Cursor);
			Cursor_Position++;
			break;
		}
		case 2:
		{break; }

		case 4:// arrow left
		{
			if (Cursor_Position ==0)
				break;
			KeyInput.erase(KeyInput.begin() + Cursor_Position);
			KeyInput.insert(KeyInput.begin() + Cursor_Position-1, Cursor);
			Cursor_Position--;
			break;
		}

		default:
		{
			if (Cursor_Position == KeyInput.size()-1)
			{
				KeyInput.erase(KeyInput.begin() + Cursor_Position);
				KeyInput.push_back(Key);
				KeyInput.push_back(Cursor);
				Cursor_Position++;
			}
			else if (Cursor_Position < KeyInput.size() - 1)
			{
				KeyInput.insert(KeyInput.begin() + Cursor_Position, Key);
				Cursor_Position++;
			}
		}
		};
		userInput = "";
		for (int i = 0; i < KeyInput.size(); i++)
		{
			userInput += KeyInput[i];
		}
		PrintMsg(userInput);
	}

}
string GUI::GetSrting( string Text)
{
	//Reads a complete string from the user until the user presses "ENTER".
	//If the user presses "ESCAPE". This function should return an empty string.
	//"BACKSPACE" is also supported
	//User should see what he is typing at the status bar


	string userInput = Text;
	vector<char>KeyInput;
	char Cursor = '|';
	for (int i = 0; i < size(Text); i++)
	{
		KeyInput.push_back(Text[i]);
	}
	KeyInput.push_back(Cursor);
	int Cursor_Position = size(Text);
	char Key;
	PrintMsg(Text);
	while (1)
	{
		pWind->WaitKeyPress(Key);
		switch (Key)
		{
		case 27: //ESCAPE key is pressed
			PrintMsg("");
			return Notes; //returns the same notes
		case 13://ENTER key is pressed
		{
			KeyInput.erase(KeyInput.begin() + Cursor_Position);
			userInput = "";
			for (int i = 0; i < KeyInput.size(); i++)
			{
				userInput += KeyInput[i];
			}
			return userInput;
		}
		case 8:		//BackSpace is pressed
			if ((KeyInput.size() != 0) && (Cursor_Position != 0))
			{
				KeyInput.erase(KeyInput.begin() + Cursor_Position - 1);
				Cursor_Position--;
			}
			break;
		case 6:// arrow right
		{
			if (Cursor_Position == KeyInput.size() - 1)
				break;
			KeyInput.erase(KeyInput.begin() + Cursor_Position);
			KeyInput.insert(KeyInput.begin() + Cursor_Position + 1, Cursor);
			Cursor_Position++;
			break;
		}
		case 2:
		{break; }

		case 4:// arrow left
		{
			if (Cursor_Position == 0)
				break;
			KeyInput.erase(KeyInput.begin() + Cursor_Position);
			KeyInput.insert(KeyInput.begin() + Cursor_Position - 1, Cursor);
			Cursor_Position--;
			break;
		}

		default:
		{
			if (Cursor_Position == KeyInput.size() - 1)
			{
				KeyInput.erase(KeyInput.begin() + Cursor_Position);
				KeyInput.push_back(Key);
				KeyInput.push_back(Cursor);
				Cursor_Position++;
			}
			else if (Cursor_Position < KeyInput.size() - 1)
			{
				KeyInput.insert(KeyInput.begin() + Cursor_Position, Key);
				Cursor_Position++;
			}
		}
		};
		userInput = "";
		for (int i = 0; i < KeyInput.size(); i++)
		{
			userInput += KeyInput[i];
		}
		PrintMsg(userInput);
	}

}
void GUI::PrintNotes() const
{
	int MsgX = NotesX1+5;
	int MsgY = NotesY1+MyFactor*6+5;
	string msg = Notes;
	int Size = size(msg);
	int Test_Msg=0;
	int Start =1, End = string_Max_Width;
	pWind->SetFont(15, BOLD, BY_NAME, "Times New Rome");
	pWind->SetPen(BLACK);
	if (Size-1 > string_Max_Width)
		for (int i = 0; i < ((Size/string_Max_Width)+1); i++)
		{
			if (Test_Msg > Size-1)
			{
				End = Size-i-Start;
			}
				pWind->DrawString(MsgX, MsgY + 15 * i, msg.substr(Start, End));
				Start += string_Max_Width;
				Test_Msg= Start+ string_Max_Width;
					End = string_Max_Width;
		}
	else
		pWind->DrawString(MsgX, MsgY, msg);

}
void GUI::DrawNoteArea()const
{
	pWind->SetFont(15, ITALICIZED, BY_NAME, "Times New Rome");
	pWind->SetBrush(WHITE);
	pWind->SetPen(BLACK);
	pWind->DrawRectangle(SideBarX1, NotesY1, SideBarX2, NotesY1 + NotesHeight, FRAME);
	pWind->SetBrush(WHITE);
	pWind->DrawLine(SideBarX1, NotesY1 + 25, SideBarX2, NotesY1 + 25);
	pWind->DrawRectangle(SideBarX1, NotesY1, SideBarX2, NotesY1 + 25);
	pWind->DrawString(SideBarX1 + myNotesFactor , NotesY1 + 6, "My Notes");
	pWind->DrawImage("GUI\\Images\\Menu\\Edit_Notes.jpeg", SideBarX1+(SideBarX2- SideBarX1)/2-45, 10, 100, 30);
}
void GUI::DrawReportArea() const
{
	pWind->SetFont(15, ITALICIZED, BY_NAME, "Times New Rome");
	pWind->SetBrush(WHITE);
	pWind->SetPen(BLACK);
	pWind->DrawRectangle(SideBarX1, ReportAreaY1, SideBarX2, ReportAreaY1 + ReportAreaHeight, FRAME);
	pWind->SetBrush(WHITE);
	pWind->DrawLine(SideBarX1, ReportAreaY1 + 25, SideBarX2, ReportAreaY1 + 25);
	pWind->DrawRectangle(SideBarX1, ReportAreaY1, SideBarX2, ReportAreaY1 + 25);
	pWind->DrawString(SideBarX1 + myReportFactor, ReportAreaY1 + 6, "Live Report");
	//pWind->DrawImage("GUI\\Images\\Menu\\Edit_Notes.jpeg", SideBarX1 + (SideBarX2 - SideBarX1) / 2 - 45, 10, 100, 30);
}
void GUI::PrintCriticalError(Error Er, int I)const
{
	int MsgX = NotesX1 + 5;
	int MsgY = NotesY1 + MyFactor *10+ 5 +NotesHeight*2;
	string msg = Er.Msg;
	int Size = size(msg);
	int Test_Msg = 0;

	pWind->SetBrush(RED);
	pWind->SetPen(RED);
	pWind->DrawRectangle(NotesX1+5, MsgY + 15 * I, MsgX+ MyFactor * 22, MsgY + 15 * I+14, FILLED);
	pWind->SetPen(WHITE);
	pWind->SetFont(12, BOLD, BY_NAME, "Times New Rome");
	pWind->DrawString(NotesX1 + 5, MsgY + 15 * I, "CRITICAL ERROR:");

	pWind->SetPen(BLACK);
	pWind->SetFont(12, BOLD, BY_NAME, "Times New Rome");
    pWind->DrawString(MsgX, MsgY+15*(I+1), msg);
}
void GUI::PrintPrintModerateError(Error Er, int I, int Sem_Total_Crs,int Min_Crs,int Max_Crs)const
{
	int MsgX = NotesX1 + 5;
	int MsgY = NotesY1 + MyFactor * 10 + 5 + NotesHeight * 2;
	string msg = Er.Msg;
	string Petition;
	int Size = size(msg);

	pWind->SetBrush(GOLD);
	pWind->SetPen(GOLD);
	pWind->DrawRectangle(NotesX1 + 5, MsgY + 15 * I, MsgX + MyFactor * 25, MsgY + 15 * I + 14, FILLED);
	pWind->SetPen(WHITE);
	pWind->SetFont(12, BOLD, BY_NAME, "Times New Rome");
	pWind->DrawString(NotesX1 + 5, MsgY + 15 * I, "MODERATE ERROR:");

	pWind->SetPen(BLACK);
	pWind->SetFont(12, BOLD, BY_NAME, "Times New Rome");
	pWind->DrawString(MsgX, MsgY + 15 * (I + 1), msg);
	if (Sem_Total_Crs < Min_Crs)
	{
		Petition = "You May Need An Underload Petition";
		pWind->DrawString(MsgX, MsgY + 15 * (I + 2), Petition);
	}
	else if ((Sem_Total_Crs > Max_Crs))
	{
		Petition = "You May Need An Overload Petition";
		pWind->DrawString(MsgX, MsgY + 15 * (I + 2), Petition);
	}
}
void GUI::DrawInfoArea()const
{
	pWind->SetFont(15, ITALICIZED, BY_NAME, "Times New Rome");
	pWind->SetBrush(WHITE);
	pWind->SetPen(BLACK);
	pWind->DrawRectangle(SideBarX1, CourseInfoY1, SideBarX2, CourseInfoY1 + CourseInfoHeight, FRAME);
	pWind->DrawRectangle(SideBarX1, CourseInfoY1, SideBarX2, CourseInfoY1 + 25);
	pWind->DrawLine(SideBarX1, CourseInfoY1 + 25, SideBarX2, CourseInfoY1 + 25);
	pWind->DrawString(SideBarX1 + courseInfoFactor, CourseInfoY1 + 6, "Course Information");
}
void GUI::PrintCourseInfo()const
{
	int MsgX = InfoX1+5;
	int MsgY = InfoY1-20;
	string msg1 = CourseTitle;
	int Size = size(msg1);
	int Test_Msg = 0;
	int Start = 0, End = string_Max_Width;
	pWind->SetFont(15, BOLD, BY_NAME, "Times New Rome");
	pWind->SetPen(BLACK);
	if (Size> string_Max_Width)
		for (int i = 0; i < ((Size / (string_Max_Width))+1);i++)
		{
			if (Test_Msg > Size)
			{
				End = Size - Start;
			}
			pWind->DrawString(MsgX, MsgY + 15 * i, msg1.substr(Start, End));
			Start += string_Max_Width;
			Test_Msg = Start + string_Max_Width;
			End = string_Max_Width;
		}

	else 
	{
		pWind->DrawString(MsgX, MsgY, msg1);
	}

	string msg2 = CourseCode;
	pWind->DrawString(MsgX, MsgY + 60, msg2);
	string msg3 = CourseCredit;
	pWind->DrawString(MsgX, MsgY + 90, msg3);
	string msg4 = CourseStatus;
	pWind->DrawString(MsgX, MsgY + 120, msg4);

}
void GUI::DrawCourse_Dependacies(Course* pCr, Course* DpCr) const
{
	pWind->SetBrush(BLACK);
	//pWind->SetPen(BLACK);
	graphicsInfo gInfo_Of_PreOrCo = pCr->getGfxInfo();
	graphicsInfo gInfo_Of_DepCr = DpCr->getGfxInfo();
	/*if (gInfo_Of_DepCr.y == gInfo_Of_PreOrCo.y)
	{

	}*/
	//else
	//{
		if (gInfo_Of_DepCr.x == gInfo_Of_PreOrCo.x)
		{
			pWind->DrawTriangle(gInfo_Of_DepCr.x + CRS_WIDTH, gInfo_Of_DepCr.y + CRS_HEIGHT / 2, gInfo_Of_DepCr.x + CRS_WIDTH + 5, gInfo_Of_DepCr.y + CRS_HEIGHT / 2 - 5, gInfo_Of_DepCr.x + CRS_WIDTH + 5, gInfo_Of_DepCr.y + CRS_HEIGHT / 2 + 5, FILLED);
			//pWind->SetPen(BLACK, 2);
			pWind->DrawBezier(gInfo_Of_DepCr.x + CRS_WIDTH, gInfo_Of_DepCr.y + CRS_HEIGHT / 2, gInfo_Of_DepCr.x + CRS_WIDTH + 100, gInfo_Of_DepCr.y + 20, gInfo_Of_PreOrCo.x + CRS_WIDTH - 10, gInfo_Of_PreOrCo.y + CRS_HEIGHT - 10, gInfo_Of_PreOrCo.x + CRS_WIDTH / 2, gInfo_Of_PreOrCo.y + CRS_HEIGHT);
		}
		else if (gInfo_Of_DepCr.x > gInfo_Of_PreOrCo.x)
		{
			pWind->DrawTriangle(gInfo_Of_DepCr.x, gInfo_Of_DepCr.y + CRS_HEIGHT / 2, gInfo_Of_DepCr.x - 5, gInfo_Of_DepCr.y + CRS_HEIGHT / 2 - 5, gInfo_Of_DepCr.x - 5, gInfo_Of_DepCr.y + CRS_HEIGHT / 2 + 5, FILLED);
			//pWind->SetPen(BLACK, 2);
			pWind->DrawBezier(gInfo_Of_DepCr.x, gInfo_Of_DepCr.y + CRS_HEIGHT / 2, gInfo_Of_DepCr.x - 70, gInfo_Of_DepCr.y, gInfo_Of_PreOrCo.x + CRS_WIDTH / 2, gInfo_Of_PreOrCo.y + 120, gInfo_Of_PreOrCo.x + CRS_WIDTH / 2, gInfo_Of_PreOrCo.y + CRS_HEIGHT);
		}
		else if (gInfo_Of_DepCr.x < gInfo_Of_PreOrCo.x)
		{
			pWind->DrawTriangle(gInfo_Of_DepCr.x + CRS_WIDTH, gInfo_Of_DepCr.y + CRS_HEIGHT / 2, gInfo_Of_DepCr.x + CRS_WIDTH + 5, gInfo_Of_DepCr.y + CRS_HEIGHT / 2 - 5, gInfo_Of_DepCr.x + CRS_WIDTH + 5, gInfo_Of_DepCr.y + CRS_HEIGHT / 2 + 5, FILLED);
			//pWind->SetPen(BLACK, 2);
			pWind->DrawBezier(gInfo_Of_DepCr.x + CRS_WIDTH, gInfo_Of_DepCr.y + CRS_HEIGHT / 2, gInfo_Of_DepCr.x + CRS_WIDTH + 60, gInfo_Of_DepCr.y + 20, gInfo_Of_PreOrCo.x + CRS_WIDTH - 10, gInfo_Of_PreOrCo.y + CRS_HEIGHT - 10, gInfo_Of_PreOrCo.x + CRS_WIDTH / 2, gInfo_Of_PreOrCo.y + CRS_HEIGHT);
		}
	//}
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