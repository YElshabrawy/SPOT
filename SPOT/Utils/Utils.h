#pragma once
#include<fstream>
#include<iostream>
#include<vector>
#include"../Rules.h"
#include"../GUI/Drawable.h"
#include"../Registrar.h"

using namespace std;

vector<CourseInfo> createAllCoursesVector(string fileName);
void addExtraSpace(Course_Code& code);
Course_Code interrogateCourse(int x, int y, Registrar* pReg);
//graphicsInfo** coursesGridArray();