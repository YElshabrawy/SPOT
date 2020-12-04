#include "Utils.h"
#include"../GUI/GUI.h"
#include"../Registrar.h"
#include<iterator>
vector<CourseInfo> createAllCoursesVector(string fileName) {
	vector<CourseInfo> c; // output

	ifstream finput(fileName);
	char* token; // pointer to char (tokens)
	char* context = nullptr; // used in strtok_s function as a context
	const int size = 300; // More than the longest line
	char line[size];
	int i = 0; // for each line we have one course with index i

	while (finput.getline(line, size)) { // gets every line
		vector<string> tokensVector;
		token = strtok_s(line, ",", &context);
		while (token != NULL) {
			tokensVector.push_back(token);
			token = strtok_s(NULL, ",", &context);
		}
		CourseInfo newCourse;
		c.push_back(newCourse);
		c[i].Code = tokensVector[0];
		c[i].Title = tokensVector[1];
		c[i].Credits = stoi(tokensVector[2]); // string to int
		// Handle prereq
		for (string token : tokensVector) {
			bool condition = token[0] == 'P' && token[1] == 'r' && token[2] == 'e';
			if (condition) {
				vector<string> preReq;
				token.erase(0, 7);
				// Parse using string dilimeter " And "
				string delim = " And ";
				auto start = 0U;
				auto end = token.find(delim);
				while (end != string::npos) {
					preReq.push_back(token.substr(start, end - start));
					start = end + delim.length();
					end = token.find(delim, start);
				}
				preReq.push_back(token.substr(start, end));
				c[i].PreReqList = preReq;
			}
		}
		// Handle coreq
		for (string token : tokensVector) {
			bool condition = token[0] == 'C' && token[1] == 'o' && token[2] == 'r';
			if (condition) {
				vector<string> coReq;
				token.erase(0, 6);
				// Parse using string dilimeter " And "
				string delim = " And ";
				auto start = 0U;
				auto end = token.find(delim);
				while (end != string::npos) {
					coReq.push_back(token.substr(start, end - start));
					start = end + delim.length();
					end = token.find(delim, start);
				}
				coReq.push_back(token.substr(start, end));
				c[i].CoReqList = coReq;
			}
		}
		i++;
	}
	finput.close();
	return c;
}

void addExtraSpace(Course_Code& code) {
	if (!(code.find(" ") != string::npos)) {
		// Only if there is no space
		Course_Code output = "";
		for (int i = 0; i < code.length(); i++) {
			bool condition = code[i] == '0' || code[i] == '1' || code[i] == '2' || code[i] == '3' || code[i] == '4' ||
				code[i] == '5' || code[i] == '6' || code[i] == '7' || code[i] == '8' ||
				code[i] == '9';
			if (condition) {
				if (code[i + 1] != NULL && code[i + 2] != NULL) {
				output += ' ';
				output += code[i];
				output += code[i + 1];
				output += code[i + 2];
				// Assuming that all courses have 3 numbers "CODE XXX"
				break;
				}
			}
			else {
				output += code[i];
			}
		}
		code = output;
	}
}

Course* interrogateCourse(int x, int y, Registrar* pReg) {
	Course* output = nullptr;
	StudyPlan* pS = pReg->getStudyPlay(); // pointer on study plan
	vector<AcademicYear*>* pPlan = pS->getStudyPlanVector(); // pointer on the plan vector
	bool flag = 0;
	for (AcademicYear* yr : *pPlan) { 
		list<Course*>* pYr = yr->getListOfYears(); // pointer to the year
		for (int sem = FALL; sem < SEM_CNT; sem++) {
			for (auto it = pYr[sem].begin(); it != pYr[sem].end(); it++) {
				int course_x = (*it)->getGfxInfo().x;
				int course_y = (*it)->getGfxInfo().y;
				bool condition = x > course_x && x < (course_x + CRS_WIDTH) &&
					y > course_y && y < (course_y + CRS_HEIGHT);
				if (condition) {
					flag = 1;
					output = (*it)->getCoursePtr();
					break;
				}
			}
			if (flag) break;
		}
		if (flag) break;
	}

	if (flag) {
		return output;
	}
	else {
		cout << "No course is sellected!" << endl;
		return nullptr;
	}
}

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



/*graphicsInfo* coursesGridArray() {
	auto* arr = new graphicsInfo[GUI::NumOfYrs * 3][GUI::Year_X2 / CRS_WIDTH];

	int factor; // to handle the blach bold lines
	for (int j = 0; j < GUI::Year_X2 / CRS_WIDTH; j++) {
		for (int i = 0; i < GUI::NumOfYrs * 3; i++) {
			factor = j / 3; // 0 between (0 : 2), 1 between (3 : 5),...
			arr[j][i].x = GUI::TitleBarWidth + (CRS_WIDTH * i);
			arr[j][i].y = GUI::MenuBarHeight + GUI::MyFactor + (GUI::MyFactor * factor) +
				(CRS_HEIGHT * j);
		}
	}
	return arr;
}*/