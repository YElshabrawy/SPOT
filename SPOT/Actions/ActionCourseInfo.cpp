#include "ActionCourseInfo.h"
#include "..\Registrar.h"
#include <iostream>
#include"../GUI/GUI.h"
#include"../Utils/Utils.h"
#include<sstream>  
#include <algorithm>


ActionCourseInfo::ActionCourseInfo(Registrar* P) :Action(P)
{
}
bool ActionCourseInfo::Execute()
{

	GUI* pGUI = pReg->getGUI();
	int x, y;
	x = pGUI->XCoord;
	y = pGUI->YCoord;
		Course* pCr = pReg->interrogateCourse(x, y);
		if (pCr == nullptr) {
			return true;
		}
		else
		{
			pGUI->CourseStatus = "Course Status: ";
			pReg->UpdateInterface();
			pCr->changeColor(BLACK);
			if((pReg->OldpCr!=nullptr)&&(pReg->OldpCr != pCr))
			pReg->OldpCr->changeColor(MYCYAN);
			string Title ="Course Title: "+pCr->getTitle();
			string code = "Course Code: " + pCr->getCode();
			int credits =pCr->getCredits();
			stringstream ss;
			ss << credits;
			string String_Credits;
			ss >>String_Credits;
			pGUI->CourseTitle = Title;
			pGUI->CourseCode = code;
			pGUI->CourseCredit = "Course Credits: " + String_Credits;
			pReg->UpdateInterface();
			//Course Status
			pGUI->PrintMsg("Input Course Status (Pending/Done/IN Progress)");
			string msg=pGUI->GetSrting();
			for_each(msg.begin(), msg.end(), [](char& c)
				{
					c = ::toupper(c);
				});
			//checks
			if(msg=="TRUE"||msg=="YES" || msg == "DONE" || msg == "D" || msg == "1")
			{
				pCr->setCoursedone(true);
				pCr->setCoursepending(false);
				pCr->setCourseinprogress(false);
			}
			else if (msg == "PENDING" || msg == "P" || msg == "STILL" || msg == "NO" || msg == "PEND")
			{
					pCr->setCoursepending(true);
					pCr->setCoursedone(false);
					pCr->setCourseinprogress(false);

			}
			else if(msg == "I" || msg == "IN" || msg == "IN PROGRESS" || msg == "INPROGRESS" )
			{
					pCr->setCoursedone(false);
					pCr->setCoursepending(false);
					pCr->setCourseinprogress(true);
			}
			else
			{
					/*pGUI->PrintMsg("Wrong input)");*/
			}

			}
			if (pCr->getCoursedone() == true)
			{
				pGUI->CourseStatus = "Course Status: Done";

			}
			else if (pCr->getCourseinprogress() == true)
			{
				pGUI->CourseStatus = "Course Status: Inprogress";
			}
			else if (pCr->getCoursepending() == true)
			{
				pGUI->CourseStatus = "Course Status: Pending ";
			}
			else
			{
				pGUI->CourseStatus = "";
			}
			
		
		if (pReg->OldpCr != pCr)
		pReg->OldpCr = pCr;
		if (pReg->OldpCr == nullptr)
			pReg->OldpCr = pCr;
		pReg->UpdateInterface();
	return true;
}

ActionCourseInfo::~ActionCourseInfo()
{
}