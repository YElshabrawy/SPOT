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
	int	Sci_Flag = 0, Eng_Flag = 0, DoubleMajor_flag = 0, DoubleConcentration_flag = 0; 
	int BMS_Flag = 0, PEU_Flag = 0, MATSCI_Flag = 0, NANSC_Flag = 0,REE_Flag = 0,SPC_Flag = 0,CIE_Flag = 0,ENV_Flag = 0,NAN_Flag=0;
	int Exit_flag = 0;
	int NANO_FABRICATION_Flag = 0, NANOVLSI_flag=0, NANOPHOTONICS_flag=0;
	int second_concentration_flag = 0, second_major_flag = 0, exit_flag = 0;
	int token = 0;
	static string first_major = "", second_major = ""; //to hold the values for the first and the second major
	static string first_concentration = "",second_concentration="";
	StudyPlan* pS = pReg->getStudyPlay();

	cout << "Declaring a major.\n";
	while (token == 0) {
		while ((pMajorWind) && ((Sci_Flag != 1) || (Eng_Flag != 1))) {
			clicktype ctInput;
			int x, y;
			pMajorWind->GetMouseCoord(x, y);
			if ((pMajorWind->GetGreen(x, y) == 0.8) && (pMajorWind->GetRed(x, y) == 0.8) && (pMajorWind->GetBlue(x, y) == 0.8))
			{
				delete pMajorWind;
				pMajorWind = nullptr;
				break;
			}
			pMajorWind->DrawImage("GUI\\Images\\Major_img\\eng_orange.jpg", MajorWindWidth / 8, 0, MajorWindWidth / 4, MajorWindWidth / 8);
			pMajorWind->DrawImage("GUI\\Images\\Major_img\\sci_orange.jpg", MajorWindWidth * (5.0 / 8), 0, MajorWindWidth / 4, MajorWindWidth / 8);
			pMajorWind->DrawImage("GUI\\Images\\Major_img\\dec_eng.jpg", 0, (MajorWindWidth / 8) + 50, MajorWindWidth / 2, (MajorWindWidth / 2) + MajorWindWidth / 16);
			pMajorWind->DrawImage("GUI\\Images\\Major_img\\dec_sci.jpg", MajorWindWidth / 2, (MajorWindWidth / 8) + 50, MajorWindWidth / 2, (MajorWindWidth / 2) + MajorWindWidth / 16);
			pMajorWind->GetMouseClick(x, y);
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
			if (pMajorWind->GetMouseClick(x, y))
				UpdateWind();
		}
		//Science majors
		while ((pMajorWind) && (Sci_Flag == 1))
		{
			clicktype ctInput;
			int x, y;
			pMajorWind->GetMouseCoord(x, y);
			if (BMS_Flag == 0)
			{
				pMajorWind->DrawImage("GUI\\Images\\Major_img\\Major_BMS.jpg", MajorWindWidth / 8, MajorWindWidth / 16, MajorWindWidth / 4, MajorWindWidth / 8 + MajorWindWidth / 16);
			}
			if (PEU_Flag == 0)
			{
				pMajorWind->DrawImage("GUI\\Images\\Major_img\\Major_PEU.jpg", MajorWindWidth * (5.0 / 8), MajorWindWidth / 16, MajorWindWidth / 4, MajorWindWidth / 8 + MajorWindWidth / 16);
			}
			if (MATSCI_Flag == 0)
			{
				pMajorWind->DrawImage("GUI\\Images\\Major_img\\Major_MATSCI.jpg", MajorWindWidth / 8, MajorWindWidth * (3.5 / 8), MajorWindWidth / 4, MajorWindWidth / 8 + MajorWindWidth / 16);
			}
			if (NANSC_Flag == 0)
			{
				pMajorWind->DrawImage("GUI\\Images\\Major_img\\Major_NANSC.jpg", MajorWindWidth * (5.0 / 8), MajorWindWidth * (3.5 / 8), MajorWindWidth / 4, MajorWindWidth / 8 + MajorWindWidth / 16);
			}
			if ((pMajorWind->GetGreen(x, y) == 0.8) && (pMajorWind->GetRed(x, y) == 0.8) && (pMajorWind->GetBlue(x, y) == 0.8))
			{
				delete pMajorWind;
				pMajorWind = nullptr;
				break;
			}
			pMajorWind->GetMouseClick(x, y);
			if (BMS_Flag == 0)
			{
				if ((x >= (MajorWindWidth / 8)) && (x <= (MajorWindWidth / 4 + MajorWindWidth / 8)) && (y >= (MajorWindWidth / 16)) && (y <= (MajorWindWidth / 4 + MajorWindWidth / 8)))
				{
					BMS_Flag = 1;
					token = 1;
					if (DoubleMajor_flag == 0)
					{
						pS->setMajor(BMS);
					}
					if (DoubleMajor_flag == 1)
					{
						pS->setDoubleMajor(BMS);
						delete pMajorWind;
						pMajorWind = nullptr;
						return true;
					}
					UpdateWind();
					break;
				}
			}
			if (PEU_Flag == 0)
			{
				if ((x >= MajorWindWidth * (5.0 / 8)) && (x <= ((MajorWindWidth * (5.0 / 8)) + (MajorWindWidth / 4))) && (y >= (MajorWindWidth / 16)) && (y <= (MajorWindWidth / 4 + MajorWindWidth / 8)))
				{
					PEU_Flag = 1;
					token = 1;
					if (DoubleMajor_flag == 0)
					{
						pS->setMajor(PEU);
					}
					if (DoubleMajor_flag == 1)
					{
						pS->setDoubleMajor(PEU);
						delete pMajorWind;
						pMajorWind = nullptr;
						return true;
					}
					UpdateWind();
					break;
				}
			}
			if (MATSCI_Flag == 0)
			{
				if ((x >= (MajorWindWidth / 8)) && (x <= ((MajorWindWidth / 8) + (MajorWindWidth / 4))) && (y >= (MajorWindWidth * (3.5 / 8))) && (y <= (MajorWindWidth * (3.5 / 8) + ((MajorWindWidth / 4) + (MajorWindWidth / 16)))))
				{
					MATSCI_Flag = 1;
					token = 1;
					if (DoubleMajor_flag == 0)
					{
						pS->setMajor(MATSCI);
					}
					if (DoubleMajor_flag == 1)
					{
						pS->setDoubleMajor(MATSCI);
						delete pMajorWind;
						pMajorWind = nullptr;
						return true;
					}
					UpdateWind();
					break;
				}
			}
			if (NANSC_Flag == 0)
			{
				if ((x >= MajorWindWidth * (5.0 / 8)) && (x <= ((MajorWindWidth * (5.0 / 8)) + (MajorWindWidth / 4))) && (y >= (MajorWindWidth * (3.5 / 8))) && (y <= (MajorWindWidth * (3.5 / 8) + ((MajorWindWidth / 4) + (MajorWindWidth / 16)))))
				{
					NANSC_Flag = 1;
					token = 1;
					if (DoubleMajor_flag == 0)
					{
						pS->setMajor(NANSCI);
					}
					if (DoubleMajor_flag == 1)
					{
						pS->setDoubleMajor(NANSCI);
						delete pMajorWind;
						pMajorWind = nullptr;
						return true;
					}
					UpdateWind();
					break;
				}
			}
			if (pMajorWind->GetMouseClick(x, y))
				UpdateWind();
		}
		/*if (Sci_Flag == 1)
		{
			delete pMajorWind;
			pMajorWind = nullptr;
			return true;
		}*/
		//Eng Majors
		while ((pMajorWind) && (Eng_Flag == 1))
		{
			clicktype ctInput;
			int x, y;
			pMajorWind->GetMouseCoord(x, y);
			if (REE_Flag == 0)
			{
				pMajorWind->DrawImage("GUI\\Images\\Major_img\\Major_REE.jpg", MajorWindWidth / 16, MajorWindWidth / 16, MajorWindWidth / 4, MajorWindWidth / 4);
			}
			if (SPC_Flag == 0)
			{
				pMajorWind->DrawImage("GUI\\Images\\Major_img\\Major_SPC.jpg", MajorWindWidth * (6.0 / 8) - MajorWindWidth / 12, MajorWindWidth / 16, MajorWindWidth / 4, MajorWindWidth / 4);
			}
			if (CIE_Flag == 0)
			{
				pMajorWind->DrawImage("GUI\\Images\\Major_img\\Major_CIE.jpg", MajorWindWidth * (3.0 / 8), MajorWindWidth / 4, MajorWindWidth / 4, MajorWindWidth / 4);
			}
			if (ENV_Flag == 0)
			{
				pMajorWind->DrawImage("GUI\\Images\\Major_img\\Major_ENV.jpg", MajorWindWidth * (6.0 / 8) - MajorWindWidth / 12, MajorWindWidth / 2 - MajorWindWidth / 16, MajorWindWidth / 4, MajorWindWidth / 4);
			}
			if (NAN_Flag == 0)
			{
				pMajorWind->DrawImage("GUI\\Images\\Major_img\\Major_NAN.jpg", MajorWindWidth / 16, MajorWindWidth / 2 - MajorWindWidth / 16, MajorWindWidth / 4, MajorWindWidth / 4);
			}
			if ((pMajorWind->GetGreen(x, y) == 0.8) && (pMajorWind->GetRed(x, y) == 0.8) && (pMajorWind->GetBlue(x, y) == 0.8))
			{
				delete pMajorWind;
				pMajorWind = nullptr;
				break;
			}
			pMajorWind->GetMouseClick(x, y);
			if (REE_Flag == 0)
			{
				if ((x >= (MajorWindWidth / 16)) && (x <= (MajorWindWidth / 4 + MajorWindWidth / 16)) && (y >= (MajorWindWidth / 16)) && (y <= (MajorWindWidth / 4 + MajorWindWidth / 16)))
				{
					REE_Flag = 1;
					token = 1;
					if (DoubleMajor_flag == 0)
					{
						pS->setMajor(REE);
					}
					if (DoubleMajor_flag == 1)
					{
						pS->setDoubleMajor(REE);
						delete pMajorWind;
						pMajorWind = nullptr;
						return true;
					}
					UpdateWind();
					break;
				}
			}
			if (SPC_Flag == 0)
			{
				if ((x >= (MajorWindWidth * (6.0 / 8) - MajorWindWidth / 12)) && (x <= (MajorWindWidth / 4 + MajorWindWidth * (6.0 / 8) - MajorWindWidth / 12)) && (y >= (MajorWindWidth / 16)) && (y <= (MajorWindWidth / 4 + MajorWindWidth / 16)))
				{
					SPC_Flag = 1;
					token = 1;
					if (DoubleMajor_flag == 0)
					{
						pS->setMajor(SPC);
					}
					if (DoubleMajor_flag == 1)
					{
						pS->setDoubleMajor(SPC);
						delete pMajorWind;
						pMajorWind = nullptr;
						return true;
					}
					UpdateWind();
					break;
				}
			}
			if (CIE_Flag == 0)
			{
				if ((x >= (MajorWindWidth * (3.0 / 8))) && (x <= (MajorWindWidth / 4 + MajorWindWidth * (3.0 / 8))) && (y >= (MajorWindWidth / 4)) && (y <= (MajorWindWidth / 2)))
				{
					CIE_Flag = 1;
					token = 1;
					if (DoubleMajor_flag == 0)
					{
						pS->setMajor(CIE);
					}
					if (DoubleMajor_flag == 1)
					{
						pS->setDoubleMajor(CIE);
						delete pMajorWind;
						pMajorWind = nullptr;
						return true;
					}
					UpdateWind();
					break;
				}
			}
			if (ENV_Flag == 0)
			{
				if ((x >= (MajorWindWidth * (6.0 / 8) - MajorWindWidth / 12)) && (x <= (MajorWindWidth / 4 + MajorWindWidth * (6.0 / 8) - MajorWindWidth / 12)) && (y >= (MajorWindWidth / 2 - MajorWindWidth / 16)) && (y <= (MajorWindWidth / 2 - MajorWindWidth / 16 + MajorWindWidth / 4)))
				{
					ENV_Flag = 1;
					token = 1;
					if (DoubleMajor_flag == 0)
					{
						pS->setMajor(ENV);
					}
					if (DoubleMajor_flag == 1)
					{
						pS->setDoubleMajor(ENV);
						delete pMajorWind;
						pMajorWind = nullptr;
						return true;
					}
					UpdateWind();
					break;
				}
			}
			if (NAN_Flag == 0)
			{
				if ((x >= (MajorWindWidth / 16)) && (x <= (MajorWindWidth / 4 + MajorWindWidth / 16)) && (y >= (MajorWindWidth / 2 - MajorWindWidth / 16)) && (y <= (MajorWindWidth / 2 - MajorWindWidth / 16 + MajorWindWidth / 4)))
				{
					NAN_Flag = 1;
					token = 1;
					if (DoubleMajor_flag == 0)
					{
						pS->setMajor(NANENG);
					}
					if (DoubleMajor_flag == 1)
					{
						pS->setDoubleMajor(NANENG);
						delete pMajorWind;
						pMajorWind = nullptr;
						return true;

					}
					UpdateWind();
					break;
				}
			}
			if (pMajorWind->GetMouseClick(x, y))
				UpdateWind();
		}
		
		while ((pMajorWind) && (Eng_Flag == 1) && (NAN_Flag == 1)&&(DoubleMajor_flag==0))//don't allow double concentration with double major
		{
			clicktype ctInput;
			int x, y;
			pMajorWind->GetMouseCoord(x, y);
			//draw images
			pMajorWind->DrawImage("GUI\\Images\\Major_img\\NAMOTECH_Concentrations\\button_nano-fabrication-and-mems.jpg", MajorWindWidth / 5, MajorWindWidth / 16);
			pMajorWind->DrawImage("GUI\\Images\\Major_img\\NAMOTECH_Concentrations\\button_nano-vlsi.jpg", MajorWindWidth / 5, MajorWindowHeight / 4 + MajorWindWidth / 16);
			pMajorWind->DrawImage("GUI\\Images\\Major_img\\NAMOTECH_Concentrations\\button_nano-photonics.jpg", MajorWindWidth / 5, 2 * MajorWindowHeight / 4 + MajorWindWidth / 16);
			pMajorWind->GetMouseClick(x, y);

			if ((pMajorWind->GetGreen(x, y) == 0.8) && (pMajorWind->GetRed(x, y) == 0.8) && (pMajorWind->GetBlue(x, y) == 0.8))
			{
				delete pMajorWind;
				pMajorWind = nullptr;
				break;
			}
			//NanoFabrication
			if ((x >= (MajorWindWidth / 5)) && (x <= (MajorWindWidth / 5 + 279)) && (y >= (MajorWindWidth / 16)) && (y <= (MajorWindWidth / 16 + 66)))
			{
				NANO_FABRICATION_Flag = 1;
				pS->setConcentration(NANOFABRICATION);
				first_concentration = "NANOFABRICATION";
				cout << "This is conc" << pS->getConcentration() << endl;//debug
				UpdateWind();
				break;
			}
			//NANOVLSI
			if ((x >= (MajorWindWidth / 5)) && (x <= (MajorWindWidth / 5 + 279)) && (y >= (MajorWindowHeight / 4 + MajorWindWidth / 16)) && (y <= (MajorWindowHeight / 4 + MajorWindWidth / 16 + 66)))
			{
				NANOVLSI_flag = 1;
				pS->setConcentration(NANOVLSI);
				first_concentration = "NANOVLSI";
				cout << "This is conc" << pS->getConcentration() << endl; //debug
				UpdateWind();
				break;
			}
			//NANOPHOTONICS
			if ((x >= (MajorWindWidth / 5)) && (x <= (MajorWindWidth / 5 + 278)) && (y >= (2 * MajorWindowHeight / 4 + MajorWindWidth / 16)) && (y <= (2 * MajorWindowHeight / 4 + MajorWindWidth / 16 + 66)))
			{
				NANOPHOTONICS_flag = 1;
				pS->setConcentration(NANOPHOTONICS);
				first_concentration = "NANOPHOTONICS";
				cout << "This is conc" << pS->getConcentration() << endl;//debug 
				UpdateWind();
				break;
			}
			if (pMajorWind->GetMouseClick(x, y))
				UpdateWind();

		}

		//double declare major/ concentration/ exit 
		while ((pMajorWind) && ((DoubleMajor_flag != 1) && (DoubleConcentration_flag != 1)) && ((Sci_Flag == 1) || (Eng_Flag == 1)))
		{
			clicktype ctInput;
			int x, y;
			pMajorWind->GetMouseCoord(x, y);
			if ((pMajorWind->GetGreen(x, y) == 0.8) && (pMajorWind->GetRed(x, y) == 0.8) && (pMajorWind->GetBlue(x, y) == 0.8))//if user clicked white area
			{
				delete pMajorWind;
				pMajorWind = nullptr;
				break;
			}
			pMajorWind->DrawImage("GUI\\Images\\Major_img\\DeclareDoubleMajor.jpg", MajorWindWidth / 8, 60.5);
			pMajorWind->DrawImage("GUI\\Images\\Major_img\\DeclareDoubleConcentration.jpg", MajorWindWidth * (5.0 / 8), 60.5);
			pMajorWind->DrawImage("GUI\\Images\\Major_img\\exit.jpg", MajorWindWidth* (0.375), 10);
			/*if (nextgray_flag == 1)
			{
				pMajorWind->DrawImage("GUI\\Images\\Major_img\\nextgray.jpg", MajorWindWidth - 80, MajorWindowHeight / 30);
			}*/
			ctInput = pMajorWind->GetMouseClick(x, y);
			if (ctInput == 1) {
				pMajorWind->GetMouseCoord(x, y);

				if ((x >= (MajorWindWidth / 8)) && (x <= ((MajorWindWidth / 8) + 200)) && (y >= 60.5) && (y <= (60.5) + 481))
				{
					//to edit 
					DoubleMajor_flag = 1;
					if (DoubleMajor_flag == 1)
					{
						//Sci_Flag = 0, Eng_Flag = 0, BMS_Flag = 0, PEU_Flag = 0, MATSCI_Flag = 0, NANSC_Flag = 0, REE_Flag = 0, SPC_Flag = 0, CIE_Flag = 0, ENV_Flag = 0, NAN_Flag = 0;
						token = 0;
					}
					UpdateWind();
					break;
				}
				if ((x >= MajorWindWidth * (5.0 / 8)) && (x <= ((MajorWindWidth * (5.0 / 8)) + (MajorWindWidth / 4))) && (y >= 60.5) && (y <= (60.5) + (MajorWindWidth / 1.5)))
				{
					//to edit 
					DoubleConcentration_flag = 1;
					UpdateWind();
					break;
				}
				if ((x >= MajorWindWidth * (0.375)) && (x <= ((MajorWindWidth * (0.375)) + (MajorWindWidth / 4)))&& (y >= 10) && (y <= (10) + (MajorWindWidth / 10)))
				{
					
					delete pMajorWind;
					pMajorWind = nullptr;
					return true;
				}
				/*if (nextgray_flag==0)
				{
					pMajorWind->DrawImage("GUI\\Images\\Major_img\\nextorange.jpg", MajorWindWidth - 80, MajorWindowHeight / 30);
					UpdateWind();
					if ((x >= MajorWindWidth - 80) && (x <= ((MajorWindWidth - 80) + (50))) && (y >= MajorWindowHeight / 30) && (y <= (MajorWindowHeight / 30) + (50)))
					{
						nextorange_flag = 1;
						UpdateWind();
						break;
					}
				}*/
				if (pMajorWind->GetMouseClick(x, y))
					UpdateWind();

			}
		}
		if (DoubleConcentration_flag == 1 && (NAN_Flag == 1))
		{
			NANO_FABRICATION_Flag = 0, NANOVLSI_flag = 0, NANOPHOTONICS_flag = 0;
			while ((pMajorWind) && (Eng_Flag == 1) && (NAN_Flag == 1) && (second_concentration_flag == 0 || second_major_flag == 0 || exit_flag == 0))
			{
				clicktype ctInput;
				int x, y;
				pMajorWind->GetMouseCoord(x, y);
				//draw images
				if (first_concentration == "NANOFABRICATION")
				{
					pMajorWind->DrawImage("GUI\\Images\\Major_img\\NAMOTECH_Concentrations\\button_nano-vlsi.jpg", MajorWindWidth / 5, MajorWindowHeight / 4 + MajorWindWidth / 16);
					pMajorWind->DrawImage("GUI\\Images\\Major_img\\NAMOTECH_Concentrations\\button_nano-photonics.jpg", MajorWindWidth / 5, 2 * MajorWindowHeight / 4 + MajorWindWidth / 16);
				}
				else if (first_concentration == "NANOVLSI")
				{
					pMajorWind->DrawImage("GUI\\Images\\Major_img\\NAMOTECH_Concentrations\\button_nano-fabrication-and-mems.jpg", MajorWindWidth / 5, MajorWindWidth / 16);
					pMajorWind->DrawImage("GUI\\Images\\Major_img\\NAMOTECH_Concentrations\\button_nano-photonics.jpg", MajorWindWidth / 5, 2 * MajorWindowHeight / 4 + MajorWindWidth / 16);
				}
				else if (first_concentration == "NANOPHOTONICS")
				{
					pMajorWind->DrawImage("GUI\\Images\\Major_img\\NAMOTECH_Concentrations\\button_nano-fabrication-and-mems.jpg", MajorWindWidth / 5, MajorWindWidth / 16);
					pMajorWind->DrawImage("GUI\\Images\\Major_img\\NAMOTECH_Concentrations\\button_nano-vlsi.jpg", MajorWindWidth / 5, MajorWindowHeight / 4 + MajorWindWidth / 16);
				}

				pMajorWind->GetMouseClick(x, y);

				if ((pMajorWind->GetGreen(x, y) == 0.8) && (pMajorWind->GetRed(x, y) == 0.8) && (pMajorWind->GetBlue(x, y) == 0.8))
				{
					delete pMajorWind;
					pMajorWind = nullptr;
					break;
				}
				if (first_concentration == "NANOFABRICATION")
				{
					//NANOVLSI
					if ((x >= (MajorWindWidth / 5)) && (x <= (MajorWindWidth / 5 + 279)) && (y >= (MajorWindowHeight / 4 + MajorWindWidth / 16)) && (y <= (MajorWindowHeight / 4 + MajorWindWidth / 16 + 66)))
					{
						NANOVLSI_flag = 1;
						//pS->setConcentration(NANOVLSI);
						second_concentration = "NANOVLSI";
						second_major_flag = 1;
						cout << "This is conc" << pS->getConcentration() << endl; //debug
						UpdateWind();
						delete pMajorWind;
						pMajorWind = nullptr;
						return true;
					}
					//NANOPHOTONICS
					if ((x >= (MajorWindWidth / 5)) && (x <= (MajorWindWidth / 5 + 278)) && (y >= (2 * MajorWindowHeight / 4 + MajorWindWidth / 16)) && (y <= (2 * MajorWindowHeight / 4 + MajorWindWidth / 16 + 66)))
					{
						NANOPHOTONICS_flag = 1;
						pS->setConcentration(NANOPHOTONICS);
						second_concentration = "NANOPHOTONICS";
						second_major_flag = 1;
						cout << "This is conc" << pS->getConcentration() << endl;//debug 
						UpdateWind();
						delete pMajorWind;
						pMajorWind = nullptr;
						return true;
					}
				}
				else if (first_concentration == "NANOVLSI")
				{
					//NanoFabrication
					if ((x >= (MajorWindWidth / 5)) && (x <= (MajorWindWidth / 5 + 279)) && (y >= (MajorWindWidth / 16)) && (y <= (MajorWindWidth / 16 + 66)))
					{
						NANO_FABRICATION_Flag = 1;
						//->setConcentration(NANOFABRICATION);
						second_concentration = "NANOFABRICATION";
						second_major_flag = 1;
						cout << "This is conc" << pS->getConcentration() << endl;//debug
						UpdateWind();
						delete pMajorWind;
						pMajorWind = nullptr;
						return true;
					}
					//NANOPHOTONICS
					if ((x >= (MajorWindWidth / 5)) && (x <= (MajorWindWidth / 5 + 278)) && (y >= (2 * MajorWindowHeight / 4 + MajorWindWidth / 16)) && (y <= (2 * MajorWindowHeight / 4 + MajorWindWidth / 16 + 66)))
					{
						NANOPHOTONICS_flag = 1;
						pS->setConcentration(NANOPHOTONICS);
						second_concentration = "NANOPHOTONICS";
						second_major_flag = 1;
						cout << "This is conc" << pS->getConcentration() << endl;//debug 
						UpdateWind();
						delete pMajorWind;
						pMajorWind = nullptr;
						return true;
					}
				}
				else if (first_concentration == "NANOPHOTONICS")
				{
					//NanoFabrication
					if ((x >= (MajorWindWidth / 5)) && (x <= (MajorWindWidth / 5 + 279)) && (y >= (MajorWindWidth / 16)) && (y <= (MajorWindWidth / 16 + 66)))
					{
						NANO_FABRICATION_Flag = 1;
						//->setConcentration(NANOFABRICATION);
						second_concentration = "NANOFABRICATION";
						second_major_flag = 1;
						cout << "This is conc" << pS->getConcentration() << endl;//debug
						UpdateWind();
						delete pMajorWind;
						pMajorWind = nullptr;
						return true;
					}
					//NANOVLSI
					if ((x >= (MajorWindWidth / 5)) && (x <= (MajorWindWidth / 5 + 279)) && (y >= (MajorWindowHeight / 4 + MajorWindWidth / 16)) && (y <= (MajorWindowHeight / 4 + MajorWindWidth / 16 + 66)))
					{
						NANOVLSI_flag = 1;
						//pS->setConcentration(NANOVLSI);
						second_concentration = "NANOVLSI";
						second_major_flag = 1;
						cout << "This is conc" << pS->getConcentration() << endl; //debug
						UpdateWind();
						delete pMajorWind;
						pMajorWind = nullptr;
						return true;
					}
				}

				if (pMajorWind->GetMouseClick(x, y))
					UpdateWind();

			}

		}
		if (DoubleMajor_flag == 1 && second_major_flag==1)
		{
			delete pMajorWind;
			pMajorWind = nullptr;
			return true;
		}
	}
	/*else if (DoubleMajor_flag == 1)
	{

	}*/
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

/*if (Eng_Flag == 1)
		{
			delete pMajorWind;
			pMajorWind = nullptr;
			return true;
		}*/










//