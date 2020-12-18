#include "Utils.h"
#include"../GUI/GUI.h"
#include"../Registrar.h"
#include<iterator>

vector<string> splitString(string str, string delim)
{
	vector<string> output;
	auto start = 0U;
	auto end = str.find(delim);
	while (end != string::npos) {
		output.push_back(str.substr(start, end - start));
		start = end + delim.length();
		end = str.find(delim, start);
	}
	output.push_back(str.substr(start, end));
	return output;
}
