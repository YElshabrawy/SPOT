#include"Maestro.h"
window* Maestro::pMaestroWind = NULL;
Maestro::Maestro()
{
	pMaestroWind = new window(MaestroWindWidth, MaestroWindowHeight, StartX, StartY);
	pMaestroWind->ChangeTitle("Spot Master Interface");
	ClearDrawingArea();
}
void Maestro::AddSpot()
{

	pReg = new Registrar;
	pReg->MeineNummer = List_Of_Spots.size()+1;
	pReg->SetMaestroWindowP(this);
	GUI* pGUI = pReg->getGUI();
	List_Of_windows.push_back(pGUI->pWind);
	pGUI->GetVecOfWindows(List_Of_windows);
	List_Of_Spots.push_back(pReg);
	pReg = NULL;
}
void Maestro::DeleteSpot(int Number)
{

}
void Maestro::Run() 
{
	while (true)
	{
		int x, y;
		pMaestroWind->DrawImage("GUI\\Images\\Menu\\menu_add.jpg", 30, 30, 70, 70);
		pMaestroWind->DrawImage("GUI\\Images\\Menu\\menu_delete.jpg", 150, 30, 70, 70);
		pMaestroWind->GetMouseClick(x, y);
		if ((x >= (30)) && (x <= ((100)) && (y >= (30)) && (y <= (100))))
		{
			AddSpot();
		}
		if (List_Of_Spots.size() > 0)
		{
			for (int i = 0; i < List_Of_Spots.size(); i++)
			{
			pReg = List_Of_Spots[i];
			pReg->MeineNummer = i+1;
			GUI* pGUI = pReg->getGUI();
			if ((pGUI->pWind->GetGreen(x, y) == 0.8) && (pGUI->pWind->GetRed(x, y) == 0.8) && (pGUI->pWind->GetBlue(x, y) == 0.8))
			{
				List_Of_Spots.erase(List_Of_Spots.begin()+i);
				List_Of_windows.erase(List_Of_windows.begin()+i);
			}
			if (pGUI->pWind->GetClicked())
			{
				pGUI->pWind->SetClicked(false);
				pReg->Run();
			}
			if (pGUI->pWind->GetMouseClick(x, y))
				pReg->Run();
			}

		}
	}
}
void  Maestro::UpdateWind()
{
	pMaestroWind->SetBuffering(1);
	ClearDrawingArea();
	pMaestroWind->UpdateBuffer();
	pMaestroWind->SetBuffering(0);
}
void  Maestro::ClearDrawingArea()
{
	pMaestroWind->SetBrush(BackgroundColor);
	pMaestroWind->DrawRectangle(0, 0, MaestroWindWidth, MaestroWindowHeight, FILLED);
}
//bool Maestro::ExecuteAction(Action*)
//{
//
//}
Maestro::~Maestro()
{

}