#include "Registrar.h"
#include<iostream>
#include<Windows.h>
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
	//"A7a"
}