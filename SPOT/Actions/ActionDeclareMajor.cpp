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
	cout << "Declaring a major.\n";
	
	while (pMajorWind) {
		if (pMajorWind == nullptr) break;
		UpdateWind();
	}
	

	return true;
}

ActionDeclareMajor::~ActionDeclareMajor()
{
	delete pMajorWind;
	pMajorWind = nullptr;
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

