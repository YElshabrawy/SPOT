#include "ActionDeclareMajor.h"
#include "..\Registrar.h"
#include <iostream>


ActionDeclareMajor::ActionDeclareMajor(Registrar* p) :Action(p)
{
	pMajorWind = new window(MajorWindWidth, MajorWindowHeight, StartX, StartY);
	pMajorWind->ChangeTitle("Major Declaration");
	ClearDrawingArea();
}

bool ActionDeclareMajor::Execute()
{
int	Sci_Flag = 0, Eng_Flag = 0;
	cout << "Declaring a major.\n";
	while ((pMajorWind)&&((Sci_Flag!=1)||(Eng_Flag!=1))) {
		clicktype ctInput;
		int x, y;
		pMajorWind->GetMouseCoord(x, y);
		if ((pMajorWind->GetGreen(x, y) == 0.8) && (pMajorWind->GetRed(x, y) == 0.8) && (pMajorWind->GetBlue(x, y) == 0.8))
		{
			delete pMajorWind;
			pMajorWind = nullptr;
			break;
		}
		pMajorWind->DrawImage("GUI\\Images\\Major_img\\eng_orange.jpg", MajorWindWidth/8, 0, MajorWindWidth / 4, MajorWindWidth / 8);
		pMajorWind->DrawImage("GUI\\Images\\Major_img\\sci_orange.jpg", MajorWindWidth*(5.0/8), 0, MajorWindWidth / 4, MajorWindWidth / 8);
		pMajorWind->DrawImage("GUI\\Images\\Major_img\\dec_eng.jpg", 0,( MajorWindWidth / 8)+50, MajorWindWidth / 2, (MajorWindWidth / 2) + MajorWindWidth / 16);
		pMajorWind->DrawImage("GUI\\Images\\Major_img\\dec_sci.jpg", MajorWindWidth/2, (MajorWindWidth / 8) + 50, MajorWindWidth / 2, (MajorWindWidth / 2)+ MajorWindWidth / 16);
		pMajorWind->GetMouseClick(x,y);
		if ((x >= (MajorWindWidth / 8)) && (x <= ((MajorWindWidth / 8) + (MajorWindWidth / 4))) && (y >= (0)) && (y <= (MajorWindWidth / 8)))
		{
			Eng_Flag = 1;
			UpdateWind();
			break;
		}
		if ((x >= MajorWindWidth * (5.0 / 8)) && (x <= ((MajorWindWidth * (5.0 / 8)) + (MajorWindWidth / 4))) && (y >= (0)) && (y <= (MajorWindWidth / 8)))
		{
			Sci_Flag = 1;
			UpdateWind();
			break;
		}
		if(pMajorWind->GetMouseClick(x, y))
		UpdateWind();
	}
	while ((pMajorWind)&&(Sci_Flag == 1))
	{
		clicktype ctInput;
		int x, y;
		pMajorWind->GetMouseCoord(x, y);
		pMajorWind->DrawImage("GUI\\Images\\Major_img\\Major_BMS.jpg", MajorWindWidth / 8, MajorWindWidth / 16, MajorWindWidth / 4, MajorWindWidth / 8 + MajorWindWidth / 16);
	    pMajorWind->DrawImage("GUI\\Images\\Major_img\\Major_PEU.jpg", MajorWindWidth * (5.0 / 8), MajorWindWidth / 16, MajorWindWidth / 4, MajorWindWidth / 8 + MajorWindWidth / 16);
		pMajorWind->DrawImage("GUI\\Images\\Major_img\\Major_MATSCI.jpg", MajorWindWidth / 8, MajorWindWidth * (3.5 / 8), MajorWindWidth / 4, MajorWindWidth / 8 + MajorWindWidth / 16);
	    pMajorWind->DrawImage("GUI\\Images\\Major_img\\Major_NANSC.jpg", MajorWindWidth * (5.0 / 8), MajorWindWidth * (3.5 / 8), MajorWindWidth / 4, MajorWindWidth / 8 + MajorWindWidth / 16);
		if ((pMajorWind->GetGreen(x, y) == 0.8) && (pMajorWind->GetRed(x, y) == 0.8) && (pMajorWind->GetBlue(x, y) == 0.8))
		{
			delete pMajorWind;
			pMajorWind = nullptr;
			break;
		}
		if (pMajorWind->GetMouseClick(x, y))
		UpdateWind();
	}
	while ((pMajorWind) && (Eng_Flag == 1))
	{
		clicktype ctInput;
		int x, y;
		pMajorWind->GetMouseCoord(x, y);
		pMajorWind->DrawImage("GUI\\Images\\Major_img\\Major_REE.jpg", MajorWindWidth / 16, MajorWindWidth / 16, MajorWindWidth / 4, MajorWindWidth / 4 );
		pMajorWind->DrawImage("GUI\\Images\\Major_img\\Major_SPC.jpg", MajorWindWidth *(6.0 / 8)- MajorWindWidth / 12, MajorWindWidth / 16, MajorWindWidth / 4, MajorWindWidth / 4);
		pMajorWind->DrawImage("GUI\\Images\\Major_img\\Major_CIE.jpg", MajorWindWidth * (3.0 / 8),MajorWindWidth/4, MajorWindWidth / 4, MajorWindWidth / 4);
		pMajorWind->DrawImage("GUI\\Images\\Major_img\\Major_ENV.jpg", MajorWindWidth * (6.0 / 8)- MajorWindWidth / 12, MajorWindWidth/2- MajorWindWidth / 16, MajorWindWidth / 4, MajorWindWidth / 4);
		pMajorWind->DrawImage("GUI\\Images\\Major_img\\Major_NAN.jpg", MajorWindWidth / 16, MajorWindWidth/2- MajorWindWidth / 16,MajorWindWidth / 4, MajorWindWidth / 4);
		if ((pMajorWind->GetGreen(x, y) == 0.8) && (pMajorWind->GetRed(x, y) == 0.8) && (pMajorWind->GetBlue(x, y) == 0.8))
		{
			delete pMajorWind;
			pMajorWind = nullptr;
			break;
		}
		if (pMajorWind->GetMouseClick(x, y))
			UpdateWind();
	}
	return true;
}

ActionDeclareMajor::~ActionDeclareMajor()
{
}

void ActionDeclareMajor::UpdateWind() const
{
	pMajorWind->SetBuffering(1);
	ClearDrawingArea();
	pMajorWind->UpdateBuffer();
	pMajorWind->SetBuffering(0);
}

void ActionDeclareMajor::ClearDrawingArea() const
{
	pMajorWind->SetBrush(BackgroundColor);
	//pMajorWind->SetPen(BackgroundColor);
	pMajorWind->DrawRectangle(0, 0, MajorWindWidth, MajorWindowHeight, FILLED);
}

