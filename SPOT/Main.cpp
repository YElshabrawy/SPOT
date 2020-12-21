#include "Registrar.h"
#include<iostream>
#include<Windows.h>
#include"GUI/CMUgraphicsLib/CMUgraphics.h"
#include "GUI/CMUgraphicsLib/auxil.h"	// where Pause is found

using namespace std;
int main()
{
	// WIll be used later to have the same window resolution for each user local machine
	HWND hd = GetDesktopWindow();
	RECT rect;
	GetClientRect(hd, &rect);
	int Client_Width = (rect.right - rect.left);
	//int Client_Height = (rect.bottom - rect.top);
	cout << "Client Width = " << Client_Width << "px\n";

	Registrar Reg;
	Reg.Run();
	//window pWind(800, 600, 400, 140);
	//char x;
	//cin >> x;
	//desperate :(;
	//C++ course is :(;
}